//  rainette_services.h
//  project : Rainette
//  author : Damien Pomelli 
//  last modified : 2025-07-24

#ifndef __SERVICE_H__
#define __SERVICE_H__

#include "ble/BLE.h"

#define GENERAL_SERVICES_UUID   0x0010
#define ANALOG_SERVICES_UUID    0x0011
#define DIGITAL_SERVICES_UUID   0x0012

#define BATT_DETECT_CHAR_UUID   0x0020
#define START_MEAS_CHAR_UUID    0x0021
#define END_OF_COMM_CHAR_UUID   0x0022
#define PING_PONG_CHAR_UUID     0x0023

#define PROBE0_CHAR_UUID        0x0030
#define PROBE1_CHAR_UUID        0x0031
#define LUMI_CHAR_UUID          0x0032
#define TEMP_CHAR_UUID          0x0033
#define HUMI_CHAR_UUID          0x0034
#define BATT_CHAR_UUID          0x0035

#define FLOAT_CHAR_UUID         0x0040
#define MOS_REL0_CHAR_UUID      0x0041
#define MOS_REL1_CHAR_UUID      0x0042
#define GPIO0_CHAR_UUID         0x0043
#define GPIO1_CHAR_UUID         0x0044
#define GPIO2_CHAR_UUID         0x0045
#define GPIO3_CHAR_UUID         0x0046
#define GPIO4_CHAR_UUID         0x0047
#define BUTTON_CHAR_UUID        0x0048

class GeneralService {
public :
    GeneralService(BLE &arg_ble);
    
    void resetAndAddService();

    void writeBattDetect(bool new_batt_detect);
    bool readStartMeas();
    void writeStartMeas(bool new_start_meas);
    bool readEndOfComm();
    void writeEndOfComm(bool new_end_of_comm);
    bool readPingPong();
    void writePingPong(bool new_ping_pong);

    GattAttribute::Handle_t getBattDetectHandle();
    GattAttribute::Handle_t getStartMeasHandle();
    GattAttribute::Handle_t getEndOfCommHandle();
    GattAttribute::Handle_t getPingPongHandle();

private :
    BLE &ble;

    bool batt_detect_val;
    bool start_meas_val;
    bool end_of_comm_val;
    bool ping_pong_val;
    ReadOnlyGattCharacteristic<bool> batt_detect_char;
    ReadWriteGattCharacteristic<bool> start_meas_char;
    ReadWriteGattCharacteristic<bool> end_of_comm_char;
    ReadWriteGattCharacteristic<bool> ping_pong_char;

    GattCharacteristic* charTable[4];
    GattService service;
};

class AnalogService {
public :
    AnalogService(BLE &arg_ble);

    void resetAndAddService();

    void writeProbe0(uint16_t new_probe0);
    void writeProbe1(uint16_t new_probe1);
    void writeLumi(uint16_t new_lumi);
    void writeTemp(uint16_t new_temp);
    void writeHumi(uint16_t new_humi);
    void writeBatt(uint16_t new_batt);

    GattAttribute::Handle_t getProbe0Handle();
    GattAttribute::Handle_t getProbe1Handle();
    GattAttribute::Handle_t getLumiHandle();
    GattAttribute::Handle_t getTempHandle();
    GattAttribute::Handle_t getHumiHandle();
    GattAttribute::Handle_t getBattHandle();

private :
    BLE &ble;

    uint16_t probe0_val;
    uint16_t probe1_val;
    uint16_t lumi_val;
    uint16_t temp_val;
    uint16_t humi_val;
    uint16_t batt_val;
    ReadOnlyGattCharacteristic<uint16_t> probe0_char;
    ReadOnlyGattCharacteristic<uint16_t> probe1_char;
    ReadOnlyGattCharacteristic<uint16_t> lumi_char;
    ReadOnlyGattCharacteristic<uint16_t> temp_char;
    ReadOnlyGattCharacteristic<uint16_t> humi_char;
    ReadOnlyGattCharacteristic<uint16_t> batt_char;

    GattCharacteristic* charTable[6];
    GattService service;
};

class DigitalService {
public :
    DigitalService(BLE &arg_ble);

    void resetAndAddService();

    void writeFloat(bool new_float);
    bool readMosRel0();
    void writeMosRel0(bool new_mos_rel0);
    bool readMosRel1();
    void writeMosRel1(bool new_mos_rel1);
    bool readGPIO0();
    void writeGPIO0(bool new_gpio0);
    bool readGPIO1();
    void writeGPIO1(bool new_gpio1);
    bool readGPIO2();
    void writeGPIO2(bool new_gpio2);
    bool readGPIO3();
    void writeGPIO3(bool new_gpio3);
    bool readGPIO4();
    void writeGPIO4(bool new_gpio4);
    void writeButton(bool new_button);

    GattAttribute::Handle_t getFloatHandle();
    GattAttribute::Handle_t getMosRel0Handle();
    GattAttribute::Handle_t getMosRel1Handle();
    GattAttribute::Handle_t getGpio0Handle();
    GattAttribute::Handle_t getGpio1Handle();
    GattAttribute::Handle_t getGpio2Handle();
    GattAttribute::Handle_t getGpio3Handle();
    GattAttribute::Handle_t getGpio4Handle();
    GattAttribute::Handle_t getButtonHandle();

private :
    BLE &ble;

    bool float_val;
    bool mos_rel0_val;
    bool mos_rel1_val;
    bool gpio0_val;
    bool gpio1_val;
    bool gpio2_val;
    bool gpio3_val;
    bool gpio4_val;
    bool button_val;
    ReadOnlyGattCharacteristic<bool> float_char;
    ReadWriteGattCharacteristic<bool> mos_rel0_char;
    ReadWriteGattCharacteristic<bool> mos_rel1_char;
    ReadWriteGattCharacteristic<bool> gpio0_char;
    ReadWriteGattCharacteristic<bool> gpio1_char;
    ReadWriteGattCharacteristic<bool> gpio2_char;
    ReadWriteGattCharacteristic<bool> gpio3_char;
    ReadWriteGattCharacteristic<bool> gpio4_char;
    ReadOnlyGattCharacteristic<bool> button_char;

    GattCharacteristic* charTable[9];
    GattService service;
};

#endif