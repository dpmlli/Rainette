//  rainette_event_handlers.h
//  project : Rainette
//  author : Damien Pomelli 
//  last modified : 2025-07-24

#ifndef __EVENT_HANDLERS_H__
#define __EVENT_HANDLERS_H__

#include <events/mbed_events.h>
#include "ble/BLE.h"
#include "mbed.h"
#include <vector>

#include "rainette_utils.h"
#include "rainette_services.h"

using namespace std::literals::chrono_literals;

const static char DEVICE_NAME[] = "RAINETTE";

struct RainetteInfo {
    bool batt_detect = false;
    bool usr_btn = false;
    bool float_in = false;
};

enum event_flags {
    START_MEAS_FLAG  = (1UL << 0),
    END_OF_COMM_FLAG = (1UL << 1),
    PING_PONG_FLAG   = (1UL << 2),

    MOS_REL0_FLAG    = (1UL << 3),
    MOS_REL1_FLAG    = (1UL << 4),
    GPIO0_FLAG       = (1UL << 5),
    GPIO1_FLAG       = (1UL << 6),
    GPIO2_FLAG       = (1UL << 7),
    GPIO3_FLAG       = (1UL << 8),
    GPIO4_FLAG       = (1UL << 9)
};

enum idx_vector_handles {
    BATT_DETECT_CHAR_IDX,
    START_MEAS_CHAR_IDX,
    END_OF_COMM_CHAR_IDX,
    PING_PONG_CHAR_IDX,
    PROBE0_CHAR_IDX,     
    PROBE1_CHAR_IDX,    
    LUMI_CHAR_IDX,       
    TEMP_CHAR_IDX,       
    HUMI_CHAR_IDX,       
    BATT_CHAR_IDX,       
    FLOAT_CHAR_IDX,      
    MOS_REL0_CHAR_IDX,   
    MOS_REL1_CHAR_IDX,   
    GPIO0_CHAR_IDX,      
    GPIO1_CHAR_IDX,      
    GPIO2_CHAR_IDX,      
    GPIO3_CHAR_IDX,      
    GPIO4_CHAR_IDX,      
    BUTTON_CHAR_IDX    
};

class GapEventHandler : ble::Gap::EventHandler {
public:
    GapEventHandler(BLE &arg_ble);

    ~GapEventHandler();

    void run();
    
    bool isConnected();

private:
    void onInitComplete(BLE::InitializationCompleteCallbackContext *params);    
    void startAdvertising();
    void advertisingTimeout();
    void communicationTimeout();

    virtual void onConnectionComplete(const ble::ConnectionCompleteEvent &event);
    virtual void onDisconnectionComplete(const ble::DisconnectionCompleteEvent &event);

    BLE &ble;

    bool is_connected;
    bool advertising_in_progress;
    bool communication_in_progress;

    uint8_t adv_buffer[ble::LEGACY_ADVERTISING_MAX_SIZE];
    ble::AdvertisingDataBuilder adv_data_builder;
};

class GattServerEventHandler : public ble::GattServer::EventHandler {
public:
    GattServerEventHandler(); 

private:
    void onDataRead(const GattReadCallbackParams &params) override;
    void onDataWritten(const GattWriteCallbackParams &params) override;

};    

class RainetteApp {
public:
    RainetteApp();
    
    void init();
    void run();
    
private:
    void clearAndFillServices();
    void fillHandlesVector();
    void handleDataWritten();
    void startMeasurement();
    void pingPongTest();
    void pingPongTimeout();
    void btnIrq();
    void toggleButtonChar();
    void floatIrq();
    void toggleFloatChar();
    void battDetection();
    void toggleLed0();

    BLE &ble;
    GapEventHandler gap_handler;
    GattServerEventHandler gatt_handler;

    RainetteInfo rainette_info;

    bool ping_pong_in_progress;

    GeneralService general_service;
    AnalogService analog_service;
    DigitalService digital_service;
};

void schedule_ble_events(BLE::OnEventsToProcessCallbackContext *context);

void start_rainette();

#endif