//  disco_event_handlers.cpp
//  project : Rainette
//  author : Damien Pomelli 
//  last modified : 2025-07-24

#include "disco_event_handlers.h"

events::EventQueue event_queue;
EventFlags event_flags;
std::vector<GattAttribute::Handle_t> handles_vec;
DiscoIO& disco_io = DiscoIO::getInstance();

DiscoInfo disco_info;

DiscoApp app;
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

    advertising_in_progress = true;
    event_queue.call_in(45s, callback(this, &GapEventHandler::advertisingTimeout));

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
        printf(" Timeout advertising atteint\n");
        ble.gap().stopAdvertising(ble::LEGACY_ADVERTISING_HANDLE);
        advertising_in_progress = false;
        event_queue.break_dispatch();
    }
}

void GapEventHandler::communicationTimeout()
{
    if (communication_in_progress){
        printf(" Timeout communication atteint\n");
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
    } else if (params.handle == handles_vec[LED_R_CHAR_IDX]) {
        event_flags.set(LED_R_FLAG);
    } else if (params.handle == handles_vec[LED_G_CHAR_IDX]) {
        event_flags.set(LED_G_FLAG);
    } else if (params.handle == handles_vec[LED_B_CHAR_IDX]) {
        event_flags.set(LED_B_FLAG);
    } else {
        // Handle inconnu
    }
}

DiscoApp::DiscoApp() :

    ble(BLE::Instance()),
    gap_handler(ble),
    gatt_handler(),

    ping_pong_in_progress(false),

    general_service(ble),
    analog_service(ble),
    digital_service(ble)
{

}

void DiscoApp::init()
{
    mbed_trace_init();

    disco_io.btn1.mode(PullUp);
    disco_io.btn1.fall(callback(this, &DiscoApp::btn1Irq));
    
    disco_io.btn2.mode(PullUp);
    disco_io.btn2.fall(callback(this, &DiscoApp::btn2Irq));

    write_sensor_register(CTRL_REG1, 0x0C);
    
    handle_thread.start(callback(this, &DiscoApp::handleDataWritten));

    event_queue.call_every(
        1min,
        [this] {
            pingPongTest();
        }
    );

    ble.onEventsToProcess(schedule_ble_events);
    ble.gattServer().setEventHandler(&gatt_handler);

    printf("Initialisation terminée\n");
}

void DiscoApp::run()
{
    while (1) {
        uint32_t flags_read = event_flags.wait_any(BLE_ON_FLAG);
        if (flags_read & BLE_ON_FLAG) {
            printf("Initialisation de la stack BLE \n");
            
            clearAndFillServices();

            gap_handler.run();
            
            printf("Fin de communication\n");
            event_flags.clear(BLE_ON_FLAG);
        }
    }
}

void DiscoApp::clearAndFillServices()
{
    general_service.resetAndAddService();
    analog_service.resetAndAddService();
    digital_service.resetAndAddService();

    handles_vec.clear();
    fillHandlesVector();

    digital_service.writeLedR(disco_info.led_r);
    digital_service.writeLedG(disco_info.led_g);
    digital_service.writeLedB(disco_info.led_b);
    digital_service.writeButton(disco_info.button);
    startMeasurement();

    ping_pong_in_progress = false;
}

void DiscoApp::fillHandlesVector()
{
    handles_vec.push_back(general_service.getStartMeasHandle());
    handles_vec.push_back(general_service.getEndOfCommHandle());
    handles_vec.push_back(general_service.getPingPongHandle());
    handles_vec.push_back(analog_service.getTempHandle());
    handles_vec.push_back(analog_service.getAdcTestHandle());
    handles_vec.push_back(digital_service.getLedRHandle());
    handles_vec.push_back(digital_service.getLedGHandle());
    handles_vec.push_back(digital_service.getLedBHandle());
    handles_vec.push_back(digital_service.getButtonHandle());
}

void DiscoApp::handleDataWritten()
{
    while (1) {
        uint32_t flags_read = event_flags.wait_any(
            START_MEAS_FLAG | END_OF_COMM_FLAG | 
            PING_PONG_FLAG | LED_R_FLAG | 
            LED_G_FLAG | LED_B_FLAG
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
                event_queue.break_dispatch();
            }
            event_flags.clear(END_OF_COMM_FLAG);
        } else if (flags_read & PING_PONG_FLAG) {
            if (!general_service.readPingPong()) {
                if (ping_pong_in_progress) {
                    printf("Test Ping-Pong OK\n");
                    ping_pong_in_progress = false;
                    general_service.writePingPong(false);
                }
            }
            event_flags.clear(PING_PONG_FLAG);
        } else if (flags_read & LED_R_FLAG) {
            disco_info.led_r = digital_service.readLedR();
            digital_service.writeLedR(disco_info.led_r);
            event_flags.clear(LED_R_FLAG);
        } else if (flags_read & LED_G_FLAG) {
            disco_info.led_g = digital_service.readLedG();
            digital_service.writeLedG(disco_info.led_g);
            event_flags.clear(LED_G_FLAG);
        } else if (flags_read & LED_B_FLAG) {
            disco_info.led_b = digital_service.readLedB();
            digital_service.writeLedB(disco_info.led_b);
            event_flags.clear(LED_B_FLAG);
        } else {
            // Handle inconnu
        }
    }
}

void DiscoApp::startMeasurement()
{
    float temp = read_temperature();
    if (temp == -50.0f) {
        printf("Erreur lecture température\n");
        return;
    }
    
    analog_service.writeTemp(static_cast<int16_t>(temp * 100.0f));

    uint16_t adc_val = static_cast<int16_t>(100*3.3*disco_io.adc_test.read());
    analog_service.writeAdcTest(adc_val);
}

void DiscoApp::pingPongTest()
{
    if (gap_handler.isConnected()){
        printf("Start Ping Pong test\n");
        general_service.writePingPong(true);

        event_queue.call_in(15s, callback(this, &DiscoApp::pingPongTimeout));
        ping_pong_in_progress = true;
    }
}

void DiscoApp::pingPongTimeout()
{
    if (ping_pong_in_progress) {
        printf("Timeout atteint lors du test Ping-Pong\n");
        ping_pong_in_progress = false;
        event_queue.break_dispatch();
    }
}

void DiscoApp::btn1Irq() 
{
    event_flags.set(BLE_ON_FLAG);
}

void DiscoApp::btn2Irq() 
{
    event_queue.call(callback(this, &DiscoApp::toggleButtonChar));
}

void DiscoApp::toggleButtonChar()
{
    disco_info.button = !disco_info.button;
    digital_service.writeButton(disco_info.button);
}

void schedule_ble_events(BLE::OnEventsToProcessCallbackContext *context)
{
    event_queue.call(Callback<void()>(&context->ble, &BLE::processEvents));
}

void start_disco()
{
    printf("Début du programme\n");
    app.init();
    app_thread.start(callback(&app, &DiscoApp::run));

    while (1) {
        if (disco_info.led_r) {
            rgb_led_red();
            ThisThread::sleep_for(500ms);
        }
        
        if (disco_info.led_g) {
            rgb_led_green();
            ThisThread::sleep_for(500ms);
        }
        
        if (disco_info.led_b) {
            rgb_led_blue();
            ThisThread::sleep_for(500ms);
        }
        
        rgb_led_off();
        ThisThread::sleep_for(500ms);
    }
}