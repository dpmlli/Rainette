//  disco_event_handlers.h
//  project : Rainette
//  author : Damien Pomelli 
//  last modified : 2025-07-24

#ifndef __EVENT_HANDLERS_H__
#define __EVENT_HANDLERS_H__

#include <events/mbed_events.h>
#include <vector>

#include "disco_utils.h"
#include "disco_services.h"

const static char DEVICE_NAME[] = "DISCO";

struct DiscoInfo {
    bool led_r = false;
    bool led_g = false;
    bool led_b = false;
    bool button = false;
};

enum event_flags {
    START_MEAS_FLAG  = (1UL << 0),
    END_OF_COMM_FLAG = (1UL << 1),
    PING_PONG_FLAG   = (1UL << 2),

    TEMP_FLAG        = (1UL << 3),
    ADC_TEST_FLAG    = (1UL << 4),

    LED_R_FLAG       = (1UL << 5),
    LED_G_FLAG       = (1UL << 6),
    LED_B_FLAG       = (1UL << 7),
    BUTTON_FLAG      = (1UL << 8),

    BLE_ON_FLAG      = (1UL << 9)
};

enum idx_vector_handles {
    START_MEAS_CHAR_IDX,
    END_OF_COMM_CHAR_IDX,
    PING_PONG_CHAR_IDX,      
    TEMP_CHAR_IDX,             
    ADC_TEST_CHAR_IDX,            
    LED_R_CHAR_IDX,      
    LED_G_CHAR_IDX,      
    LED_B_CHAR_IDX,      
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

class DiscoApp {
public:
    DiscoApp();
    
    void init();
    void run();

private:
    void clearAndFillServices();
    void handleDataWritten();
    void fillHandlesVector();
    void startMeasurement();
    void pingPongTest();
    void pingPongTimeout();
    void btn1Irq();
    void btn2Irq();
    void toggleButtonChar();

    BLE &ble;
    GapEventHandler gap_handler;
    GattServerEventHandler gatt_handler;

    bool ping_pong_in_progress;

    GeneralService general_service;
    AnalogService analog_service;
    DigitalService digital_service;
};

void schedule_ble_events(BLE::OnEventsToProcessCallbackContext *context);

void start_disco();
#endif