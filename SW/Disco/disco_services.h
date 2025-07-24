//  disco_services.h
//  project : Rainette
//  author : Damien Pomelli 
//  last modified : 2025-07-24

#ifndef __SERVICE_H__
#define __SERVICE_H__

#include "ble/BLE.h"

#define GENERAL_SERVICES_UUID   0x0010
#define ANALOG_SERVICES_UUID    0x0011
#define DIGITAL_SERVICES_UUID   0x0012

#define START_MEAS_CHAR_UUID    0x0021
#define END_OF_COMM_CHAR_UUID   0x0022
#define PING_PONG_CHAR_UUID     0x0023

#define TEMP_CHAR_UUID          0x0033
#define ADC_TEST_CHAR_UUID      0x0035

#define LED_R_CHAR_UUID         0x0045
#define LED_G_CHAR_UUID         0x0046
#define LED_B_CHAR_UUID         0x0047
#define BUTTON_CHAR_UUID        0x0048

class GeneralService {
public :
    GeneralService(BLE &arg_ble);
    
    void resetAndAddService();

    bool readStartMeas();
    void writeStartMeas(bool new_start_meas);
    bool readEndOfComm();
    void writeEndOfComm(bool new_end_of_comm);
    bool readPingPong();
    void writePingPong(bool new_ping_pong);

    GattAttribute::Handle_t getStartMeasHandle();
    GattAttribute::Handle_t getEndOfCommHandle();
    GattAttribute::Handle_t getPingPongHandle();

private :
    BLE &ble;

    bool start_meas_val;
    bool end_of_comm_val;
    bool ping_pong_val;
    ReadWriteGattCharacteristic<bool> start_meas_char;
    ReadWriteGattCharacteristic<bool> end_of_comm_char;
    ReadWriteGattCharacteristic<bool> ping_pong_char;
    
    GattCharacteristic* charTable[3];
    GattService service;
};

class AnalogService {
public :
    AnalogService(BLE &arg_ble);

    void resetAndAddService();

    void writeTemp(uint16_t new_temp);
    void writeAdcTest(uint16_t new_adc_test);

    GattAttribute::Handle_t getTempHandle();
    GattAttribute::Handle_t getAdcTestHandle();

private :
    BLE &ble;

    uint16_t temp_val;
    uint16_t adc_test_val;
    ReadOnlyGattCharacteristic<uint16_t> temp_char;
    ReadOnlyGattCharacteristic<uint16_t> adc_test_char;
        
    GattCharacteristic* charTable[2];
    GattService service;
};

class DigitalService {
public :
    DigitalService(BLE &arg_ble);

    void resetAndAddService();

    bool readLedR();
    void writeLedR(bool new_led_r);
    bool readLedG();
    void writeLedG(bool new_led_g);
    bool readLedB();
    void writeLedB(bool new_led_b);
    void writeButton(bool new_button);

    GattAttribute::Handle_t getLedRHandle();
    GattAttribute::Handle_t getLedGHandle();
    GattAttribute::Handle_t getLedBHandle();
    GattAttribute::Handle_t getButtonHandle();

private :
    BLE &ble;

    bool led_r_val;
    bool led_g_val;
    bool led_b_val;
    bool button_val;
    ReadWriteGattCharacteristic<bool> led_r_char;
    ReadWriteGattCharacteristic<bool> led_g_char;
    ReadWriteGattCharacteristic<bool> led_b_char;
    ReadOnlyGattCharacteristic<bool> button_char;

    GattCharacteristic* charTable[4];
    GattService service;
};

#endif