//  rainette_event_handlers.cpp
//  project : Rainette
//  author : Damien Pomelli 
//  last modified : 2025-07-24

#include "rainette_event_handlers.h"

events::EventQueue event_queue;
EventFlags event_flags;
std::vector<GattAttribute::Handle_t> handles_vec;
RainetteIO& rainette_io = RainetteIO::getInstance();

RainetteApp app;
Thread app_thread;
Thread handle_thread;

GapEventHandler::GapEventHandler(BLE &arg_ble) :
    ble(arg_ble),

    is_connected(false),
    advertising_in_progress(false),
    communication_in_progress(false),

    adv_buffer{0},
    adv_data_builder(adv_buffer)       
{

}
    
GapEventHandler::~GapEventHandler()
{
    if (ble.hasInitialized()) {
        ble.shutdown();
    }
}

void GapEventHandler::run()
{
    ble.init(this, &GapEventHandler::onInitComplete);
    event_queue.dispatch_forever();
    
    // Shuting down
    is_connected = false;
    advertising_in_progress = false;
    communication_in_progress = false;

    printf("Shutdown\n");
    if (ble.hasInitialized()) {
        ble.shutdown();
    }
}

bool GapEventHandler::isConnected()
{
    return is_connected;
}

void GapEventHandler::onInitComplete(BLE::InitializationCompleteCallbackContext *params)
{
    if (params->error != BLE_ERROR_NONE) {
        printf("Ble initialization failed.");
        return;
    }

    ble.gap().setEventHandler(this);

    startAdvertising();
}

void GapEventHandler::startAdvertising()
{
    ble::AdvertisingParameters adv_parameters(
        ble::advertising_type_t::CONNECTABLE_UNDIRECTED, 
        ble::adv_interval_t(ble::millisecond_t(100))
    );

    adv_data_builder.setFlags();
    adv_data_builder.setName(DEVICE_NAME);

    ble_error_t error = ble.gap().setAdvertisingParameters(
        ble::LEGACY_ADVERTISING_HANDLE,
        adv_parameters
    );

    if (error) {
        printf("ble.gap().setAdvertisingParameters() failed\r\n");
        return;
    }

    error = ble.gap().setAdvertisingPayload(
        ble::LEGACY_ADVERTISING_HANDLE,
        adv_data_builder.getAdvertisingData()
    );

    if (error) {
        printf("ble.gap().setAdvertisingPayload() failed\r\n");
        return;
    }

    /*** Activate the advertising timeout ***/
    // advertising_in_progress = true;
    // event_queue.call_in(45s, callback(this, &GapEventHandler::advertisingTimeout));

    error = ble.gap().startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);

    if (error) {
        printf("ble.gap().startAdvertising() failed\r\n");
        return;
    }

    printf("Advertising, please connect\r\n");
}

void GapEventHandler::advertisingTimeout()
{
    if (advertising_in_progress){
        printf("Timeout advertising atteint\n");
        ble.gap().stopAdvertising(ble::LEGACY_ADVERTISING_HANDLE);
        advertising_in_progress = false;
        event_queue.break_dispatch();
    }
}

void GapEventHandler::communicationTimeout()
{
    if (communication_in_progress){
        printf("Timeout communication atteint\n");
        communication_in_progress = false;
        event_queue.break_dispatch();
    }
}

void GapEventHandler::onConnectionComplete(const ble::ConnectionCompleteEvent &event)
{
    if (event.getStatus() == ble_error_t::BLE_ERROR_NONE) {
        printf("Client connected, you may now subscribe to updates\r\n");
        is_connected = true;
        advertising_in_progress = false;

        /*** Activate the communication timeout ***/
        // communication_in_progress = true;
        // event_queue.call_in(15s, callback(this, &GapEventHandler::communicationTimeout));
    }
}

void GapEventHandler::onDisconnectionComplete(const ble::DisconnectionCompleteEvent &event)
{
    printf("Client disconnected, shuting down BLE\r\n");
    is_connected = false;
    event_queue.break_dispatch();

    if (ble.hasInitialized()) {
        ble.shutdown();
    }
}

GattServerEventHandler::GattServerEventHandler()
{

}

void GattServerEventHandler::onDataRead(const GattReadCallbackParams &params)
{
    printf("Data venit, data vidit, data vicit\n");
}

void GattServerEventHandler::onDataWritten(const GattWriteCallbackParams &params)
{
    if (params.handle == handles_vec[START_MEAS_CHAR_IDX]) {
        event_flags.set(START_MEAS_FLAG);
    } else if (params.handle == handles_vec[END_OF_COMM_CHAR_IDX]) {
        event_flags.set(END_OF_COMM_FLAG);
    } else if (params.handle == handles_vec[PING_PONG_CHAR_IDX]) {
        event_flags.set(PING_PONG_FLAG);
    } else if (params.handle == handles_vec[MOS_REL0_CHAR_IDX]) {
        event_flags.set(MOS_REL0_FLAG);
    } else if (params.handle == handles_vec[MOS_REL1_CHAR_IDX]) {
        event_flags.set(MOS_REL1_FLAG);
    } else if (params.handle == handles_vec[GPIO0_CHAR_IDX]) {
        event_flags.set(GPIO0_FLAG);
    } else if (params.handle == handles_vec[GPIO1_CHAR_IDX]) {
        event_flags.set(GPIO1_FLAG);
    } else if (params.handle == handles_vec[GPIO2_CHAR_IDX]) {
        event_flags.set(GPIO2_FLAG);
    } else if (params.handle == handles_vec[GPIO3_CHAR_IDX]) {
        event_flags.set(GPIO3_FLAG);
    } else if (params.handle == handles_vec[GPIO4_CHAR_IDX]) {
        event_flags.set(GPIO4_FLAG);
    } else {
        printf("Handle inconnu\n");
    }
}

RainetteApp::RainetteApp() :

    ble(BLE::Instance()),
    gap_handler(ble),
    gatt_handler(),

    rainette_info(),
    ping_pong_in_progress(false),

    general_service(ble),
    analog_service(ble),
    digital_service(ble)
{

}

void RainetteApp::init()
{
    printf("Deep sleep allowed: %s\n", sleep_manager_can_deep_sleep() ? "Yes" : "No");
    mbed_trace_init();

    battDetection();
    

    rainette_io.usr_button.mode(PullUp);
    rainette_io.usr_button.fall(callback(this, &RainetteApp::btnIrq));

    rainette_io.float_in.mode(PullDown);
    rainette_io.float_in.rise(callback(this, &RainetteApp::floatIrq));
    rainette_io.float_in.fall(callback(this, &RainetteApp::floatIrq));

    handle_thread.start(callback(this, &RainetteApp::handleDataWritten));
    
    if (!rainette_info.batt_detect) {
        printf("\nMode filaire activé\n");
        rainette_io.led1.write(1);
        rainette_io.float_out.write(1);

        // event_queue.call_every(
        //     1min,
        //     [this] {
        //         pingPongTest();
        //     }
        // );
        // event_queue.call_every(
        //     1min,
        //     [this] {
        //         startMeasurement();
        //     }
        // );

        event_queue.call_every(
            500ms,
            [this] {
                toggleLed0();
            }
        );

        
    }
    else {
        printf("Mode sur pile activé\n");
        rainette_io.led1.write(0);
        rainette_io.float_out.write(0);

        // event_queue.call_every(
        //     30s,
        //     [this] {
        //         sendMeas();
        //     }
        // );
    }

    ble.onEventsToProcess(schedule_ble_events);
    ble.gattServer().setEventHandler(&gatt_handler);

    if (ble.hasInitialized()) {
        ble.shutdown();
    }

    printf("Initialisation terminée\n");
}

void RainetteApp::run()
{
    while (1) {
        if (rainette_info.batt_detect) {
            printf("Attente\n");
            printf("Deep sleep allowed: %s\n", sleep_manager_can_deep_sleep() ? "Yes" : "No");
            ThisThread::sleep_for(30s);
        }
        clearAndFillServices();
        gap_handler.run();
    }
}

void RainetteApp::clearAndFillServices()
{
    general_service.resetAndAddService();
    analog_service.resetAndAddService();
    digital_service.resetAndAddService();

    handles_vec.clear();
    fillHandlesVector();

    general_service.writeBattDetect(rainette_info.batt_detect);
    digital_service.writeFloat(rainette_io.float_in.read());   // rainette_info.float_in
    digital_service.writeMosRel0(rainette_io.mos_rel0_out.read());
    digital_service.writeMosRel1(rainette_io.mos_rel1_out.read());
    digital_service.writeGPIO0(rainette_io.gpio0_out.read());
    digital_service.writeGPIO1(rainette_io.gpio1_out.read());
    digital_service.writeGPIO2(rainette_io.gpio2_out.read());
    digital_service.writeGPIO3(rainette_io.gpio3_out.read());
    digital_service.writeGPIO4(rainette_io.gpio4_out.read());
    digital_service.writeButton(rainette_info.usr_btn);

    ping_pong_in_progress = false;
}

void RainetteApp::fillHandlesVector()
{
    handles_vec.push_back(general_service.getBattDetectHandle());
    handles_vec.push_back(general_service.getStartMeasHandle());
    handles_vec.push_back(general_service.getEndOfCommHandle());
    handles_vec.push_back(general_service.getPingPongHandle());

    handles_vec.push_back(analog_service.getProbe0Handle());
    handles_vec.push_back(analog_service.getProbe1Handle());
    handles_vec.push_back(analog_service.getLumiHandle());
    handles_vec.push_back(analog_service.getTempHandle());
    handles_vec.push_back(analog_service.getHumiHandle());
    handles_vec.push_back(analog_service.getBattHandle());

    handles_vec.push_back(digital_service.getFloatHandle());
    handles_vec.push_back(digital_service.getMosRel0Handle());
    handles_vec.push_back(digital_service.getMosRel1Handle());
    handles_vec.push_back(digital_service.getGpio0Handle());
    handles_vec.push_back(digital_service.getGpio1Handle());
    handles_vec.push_back(digital_service.getGpio2Handle());
    handles_vec.push_back(digital_service.getGpio3Handle());
    handles_vec.push_back(digital_service.getGpio4Handle());
    handles_vec.push_back(digital_service.getButtonHandle());
}

void RainetteApp::handleDataWritten()
{
    while (1) {
        uint32_t flags_read = event_flags.wait_any(
            START_MEAS_FLAG | END_OF_COMM_FLAG | PING_PONG_FLAG | 
            MOS_REL0_FLAG | MOS_REL1_FLAG | GPIO0_FLAG | 
            GPIO1_FLAG | GPIO2_FLAG | GPIO3_FLAG | GPIO4_FLAG
        );
        if (flags_read & START_MEAS_FLAG) {
            if (general_service.readStartMeas()) {
                general_service.writeStartMeas(true);
                startMeasurement();
                general_service.writeStartMeas(false);
            }
            event_flags.clear(START_MEAS_FLAG);
        } else if (flags_read & END_OF_COMM_FLAG) {
            if (general_service.readEndOfComm()) {
                general_service.writeEndOfComm(true);
                ThisThread::sleep_for(1s);
                event_queue.break_dispatch();
            }
            event_flags.clear(END_OF_COMM_FLAG);
        } 
        else if (!rainette_info.batt_detect) {    
            if (flags_read & PING_PONG_FLAG) {
                if (!general_service.readPingPong()) {
                    if (ping_pong_in_progress) {
                        printf("Test Ping-Pong OK\n");
                        ping_pong_in_progress = false;
                        general_service.writePingPong(false);
                    }
                }
                event_flags.clear(PING_PONG_FLAG);
            } else if (flags_read & MOS_REL0_FLAG) {
                rainette_io.mos_rel0_out = digital_service.readMosRel0();
                digital_service.writeMosRel0(rainette_io.mos_rel0_out.read());
                event_flags.clear(MOS_REL0_FLAG);
            } else if (flags_read & MOS_REL1_FLAG) {
                rainette_io.mos_rel1_out = digital_service.readMosRel1();
                digital_service.writeMosRel1(rainette_io.mos_rel1_out.read());
                event_flags.clear(MOS_REL1_FLAG);
            } else if (flags_read & GPIO0_FLAG) {
                rainette_io.gpio0_out = digital_service.readGPIO0();
                digital_service.writeGPIO0(rainette_io.gpio0_out.read());
                event_flags.clear(GPIO0_FLAG);
            } else if (flags_read & GPIO1_FLAG) {
                rainette_io.gpio1_out = digital_service.readGPIO1();
                digital_service.writeGPIO1(rainette_io.gpio1_out.read());
                event_flags.clear(GPIO1_FLAG);
            } else if (flags_read & GPIO2_FLAG) {
                rainette_io.gpio2_out = digital_service.readGPIO2();
                digital_service.writeGPIO2(rainette_io.gpio2_out.read());
                event_flags.clear(GPIO2_FLAG);
            } else if (flags_read & GPIO3_FLAG) {
                rainette_io.gpio3_out = digital_service.readGPIO3();
                digital_service.writeGPIO3(rainette_io.gpio3_out.read());
                event_flags.clear(GPIO3_FLAG);
            } else if (flags_read & GPIO4_FLAG) {
                rainette_io.gpio4_out = digital_service.readGPIO4();
                digital_service.writeGPIO4(rainette_io.gpio4_out.read());
                event_flags.clear(GPIO4_FLAG);
            } else {
                printf("Aucune action entrainée\n");
            }
        }
    }
}

void RainetteApp::startMeasurement()
{
    rainette_io.en_probe0.write(1);
    rainette_io.en_probe1.write(1);
    rainette_io.en_batt_meas.write(1);
    if (rainette_info.batt_detect) {
        rainette_io.float_out.write(1);
    }
    ThisThread::sleep_for(500ms); 

    uint16_t probe0_meas = static_cast<uint16_t>(100*convertTempSensor(3.3*rainette_io.probe0.read()));
    analog_service.writeProbe0(probe0_meas);
    uint16_t probe1_meas = static_cast<uint16_t>(100*3.3*rainette_io.probe1.read());
    analog_service.writeProbe1(probe1_meas);
    uint16_t batt_meas = static_cast<uint16_t>(100*correctBattMeas(3.3*rainette_io.batMeas.read()));
    analog_service.writeBatt(batt_meas);

    digital_service.writeFloat(rainette_io.float_in.read());

    rainette_io.en_probe0.write(0);
    rainette_io.en_probe1.write(0);
    rainette_io.en_batt_meas.write(0);
    if (rainette_info.batt_detect) {
        rainette_io.float_out.write(0);
    }

    I2C* i2c = new I2C(SDA_PIN, SCL_PIN);

    i2c_init(i2c);

    float temp_meas(0);
    float humi_meas(0);
    readTempHumi(temp_meas,humi_meas);

    float lumi_meas(0);
    readLumi(lumi_meas);

    analog_service.writeTemp(static_cast<uint16_t>(temp_meas * 100.0f));
    analog_service.writeHumi(static_cast<uint16_t>(humi_meas * 100.0f));
    analog_service.writeLumi(static_cast<uint16_t>(lumi_meas * 100.0f));

    ThisThread::sleep_for(500ms); 

    i2c_deinit();
    delete i2c; 

    
}

void RainetteApp::pingPongTest()
{
    if (gap_handler.isConnected()){
        printf("Start Ping Pong test\n");
        general_service.writePingPong(true);

        event_queue.call_in(10s, callback(this, &RainetteApp::pingPongTimeout));
        ping_pong_in_progress = true;
    }   
}

void RainetteApp::pingPongTimeout()
{
    if (ping_pong_in_progress) {
        printf("Timeout atteint lors du test Ping-Pong\n");
        ping_pong_in_progress = false;
        event_queue.break_dispatch();
    }
}

void RainetteApp::btnIrq() 
{
    event_queue.call(callback(this, &RainetteApp::toggleButtonChar));
}

void RainetteApp::toggleButtonChar()
{
    rainette_info.usr_btn = !rainette_info.usr_btn;
    digital_service.writeButton(rainette_info.usr_btn);
}

void RainetteApp::floatIrq() 
{
    event_queue.call(callback(this, &RainetteApp::toggleFloatChar));
}

void RainetteApp::toggleFloatChar()
{
    if (!rainette_info.batt_detect) {
        digital_service.writeFloat(rainette_io.float_in.read());
    }
}

void RainetteApp::battDetection()
{
    rainette_io.en_batt_meas.write(1);
    ThisThread::sleep_for(10ms);
    rainette_info.batt_detect = (correctBattMeas(3.3*rainette_io.batMeas.read()) >= BATT_DET_THRESH);

    rainette_io.en_batt_meas.write(0);
}

void RainetteApp::toggleLed0()
{
    if (gap_handler.isConnected()) {
        rainette_io.led0 = 1;
    }
    else {
        rainette_io.led0 = (!rainette_io.led0.read());
    }
}

void schedule_ble_events(BLE::OnEventsToProcessCallbackContext *context)
{
    event_queue.call(Callback<void()>(&context->ble, &BLE::processEvents));
}

void start_rainette()
{
    printf("Début du programme\n");
    app.init();
    app_thread.start(callback(&app, &RainetteApp::run));
}