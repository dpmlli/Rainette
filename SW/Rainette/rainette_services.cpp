//  rainette_services.cpp
//  project : Rainette
//  author : Damien Pomelli 
//  last modified : 2025-07-24

#include "rainette_services.h"

GeneralService::GeneralService(BLE &arg_ble) : 
    ble(arg_ble),
    batt_detect_val(false),
    start_meas_val(false),
    end_of_comm_val(false),
    ping_pong_val(false),

    batt_detect_char(
        BATT_DETECT_CHAR_UUID,
        &batt_detect_val,
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
    ),
    start_meas_char(
        START_MEAS_CHAR_UUID,
        &start_meas_val,
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
    ),
    end_of_comm_char(
        END_OF_COMM_CHAR_UUID,
        &end_of_comm_val,
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
    ),
    ping_pong_char(
        PING_PONG_CHAR_UUID,
        &ping_pong_val,
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
    ),

    charTable{  &batt_detect_char, 
                &start_meas_char, 
                &end_of_comm_char, 
                &ping_pong_char 
    },
    service(GENERAL_SERVICES_UUID, charTable, sizeof(charTable) / sizeof(charTable[0]))
{

}

void GeneralService::resetAndAddService()
{
    batt_detect_val = false,
    start_meas_val = false;
    end_of_comm_val = false;
    ping_pong_val = false;
    ble.gattServer().addService(service);
}

void GeneralService::writeBattDetect(bool new_batt_detect)
{
    batt_detect_val = new_batt_detect;
    ble.gattServer().write(
        batt_detect_char.getValueHandle(),
        (uint8_t *)&batt_detect_val,
        sizeof(batt_detect_val)
    );
}

bool GeneralService::readStartMeas()
{
    uint8_t start_meas_resp = 0;
    uint16_t length = sizeof(start_meas_resp);
    ble.gattServer().read(
        start_meas_char.getValueHandle(),
        (uint8_t *)&start_meas_resp,
        &length
    );
    return (start_meas_resp != 0);
}

void GeneralService::writeStartMeas(bool new_start_meas)
{
    start_meas_val = new_start_meas;
    ble.gattServer().write(
        start_meas_char.getValueHandle(),
        (uint8_t *)&start_meas_val,
        sizeof(start_meas_val)
    );
}

bool GeneralService::readEndOfComm()
{
    uint8_t end_of_comm_resp = 0;
    uint16_t length = sizeof(end_of_comm_resp);
    ble.gattServer().read(
        end_of_comm_char.getValueHandle(),
        (uint8_t *)&end_of_comm_resp,
        &length
    );
    return (end_of_comm_resp != 0);
}

void GeneralService::writeEndOfComm(bool new_end_of_comm)
{
    end_of_comm_val = new_end_of_comm;
    ble.gattServer().write(
        end_of_comm_char.getValueHandle(),
        (uint8_t *)&end_of_comm_val,
        sizeof(end_of_comm_val)
    );
}

bool GeneralService::readPingPong()
{
    uint8_t ping_pong_resp = 0;
    uint16_t length = sizeof(ping_pong_resp);
    ble.gattServer().read(
        ping_pong_char.getValueHandle(),
        (uint8_t *)&ping_pong_resp,
        &length
    );
    return (ping_pong_resp != 0);
}

void GeneralService::writePingPong(bool new_ping_pong)
{
    ping_pong_val = new_ping_pong;
    ble.gattServer().write(
        ping_pong_char.getValueHandle(),
        (uint8_t *)&ping_pong_val,
        sizeof(ping_pong_val)
    );
}

GattAttribute::Handle_t GeneralService::getBattDetectHandle()
{
    return batt_detect_char.getValueHandle();
}

GattAttribute::Handle_t GeneralService::getStartMeasHandle()
{
    return start_meas_char.getValueHandle();
}

GattAttribute::Handle_t GeneralService::getEndOfCommHandle()
{
    return end_of_comm_char.getValueHandle();
}

GattAttribute::Handle_t GeneralService::getPingPongHandle()
{
    return ping_pong_char.getValueHandle();
}

AnalogService::AnalogService(BLE &arg_ble) :
    ble(arg_ble),
    probe0_val(0),
    probe1_val(0),
    lumi_val(0),
    temp_val(0),
    humi_val(0),
    batt_val(0),

    probe0_char(
        PROBE0_CHAR_UUID,
        &probe0_val,
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
    ),
    probe1_char(
        PROBE1_CHAR_UUID,
        &probe1_val,
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
    ),
    lumi_char(
        LUMI_CHAR_UUID,
        &lumi_val,
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
    ),
    temp_char(
        TEMP_CHAR_UUID,
        &temp_val,
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
    ),
    humi_char(
        HUMI_CHAR_UUID,
        &humi_val,
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
    ),
    batt_char(
        BATT_CHAR_UUID,
        &batt_val,
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
    ),

    charTable{  &probe0_char,
                &probe1_char,
                &lumi_char,
                &temp_char,
                &humi_char,
                &batt_char
    },
    service(ANALOG_SERVICES_UUID, charTable, sizeof(charTable) / sizeof(charTable[0]))
{

}

void AnalogService::resetAndAddService()
{
    probe0_val = 0,
    probe1_val = 0,
    lumi_val = 0,
    temp_val = 0,
    humi_val = 0,
    batt_val = 0,
    ble.gattServer().addService(service);
}

void AnalogService::writeProbe0(uint16_t new_probe0)
{
    probe0_val = new_probe0;
    ble.gattServer().write(
        probe0_char.getValueHandle(),
        (uint8_t *)&probe0_val,
        sizeof(probe0_val)
    );
}

void AnalogService::writeProbe1(uint16_t new_probe1)
{
    probe1_val = new_probe1;
    ble.gattServer().write(
        probe1_char.getValueHandle(),
        (uint8_t *)&probe1_val,
        sizeof(probe1_val)
    );
}

void AnalogService::writeLumi(uint16_t new_lumi)
{
    lumi_val = new_lumi;
    ble.gattServer().write(
        lumi_char.getValueHandle(),
        (uint8_t *)&lumi_val,
        sizeof(lumi_val)
    );
}

void AnalogService::writeTemp(uint16_t new_temp)
{
    temp_val = new_temp;
    ble.gattServer().write(
        temp_char.getValueHandle(),
        (uint8_t *)&temp_val,
        sizeof(temp_val)
    );
}

void AnalogService::writeHumi(uint16_t new_humi)
{
    humi_val = new_humi;
    ble.gattServer().write(
        humi_char.getValueHandle(),
        (uint8_t *)&humi_val,
        sizeof(humi_val)
    );
}

void AnalogService::writeBatt(uint16_t new_batt)
{
    batt_val = new_batt;
    ble.gattServer().write(
        batt_char.getValueHandle(),
        (uint8_t *)&batt_val,
        sizeof(batt_val)
    );
}

GattAttribute::Handle_t AnalogService::getProbe0Handle()
{
    return probe0_char.getValueHandle();
}

GattAttribute::Handle_t AnalogService::getProbe1Handle()
{
    return probe1_char.getValueHandle();
}

GattAttribute::Handle_t AnalogService::getLumiHandle()
{
    return lumi_char.getValueHandle();
}

GattAttribute::Handle_t AnalogService::getTempHandle()
{
    return temp_char.getValueHandle();
}

GattAttribute::Handle_t AnalogService::getHumiHandle()
{
    return humi_char.getValueHandle();
}

GattAttribute::Handle_t AnalogService::getBattHandle()
{
    return batt_char.getValueHandle();
}

DigitalService::DigitalService(BLE &arg_ble) :
    ble(arg_ble),
    float_val(false),
    mos_rel0_val(false),
    mos_rel1_val(false),
    gpio0_val(false),
    gpio1_val(false),
    gpio2_val(false),
    gpio3_val(false),
    gpio4_val(false),
    button_val(false),

    float_char(
        FLOAT_CHAR_UUID,
        &float_val,
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
    ),
    mos_rel0_char(
        MOS_REL0_CHAR_UUID,
        &mos_rel0_val,
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
    ),
    mos_rel1_char(
        MOS_REL1_CHAR_UUID,
        &mos_rel1_val,
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
    ),
    gpio0_char(
        GPIO0_CHAR_UUID,
        &gpio0_val,
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
    ),
    gpio1_char(
        GPIO1_CHAR_UUID,
        &gpio1_val,
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
    ),
    gpio2_char(
        GPIO2_CHAR_UUID,
        &gpio2_val,
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
    ),
    gpio3_char(
        GPIO3_CHAR_UUID,
        &gpio3_val,
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
    ),
    gpio4_char(
        GPIO4_CHAR_UUID,
        &gpio4_val,
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
    ),
    button_char(
        BUTTON_CHAR_UUID,
        &button_val,
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
    ),

    charTable{  &float_char,
                &mos_rel0_char,
                &mos_rel1_char,
                &gpio0_char,
                &gpio1_char,
                &gpio2_char,
                &gpio3_char,
                &gpio4_char,
                &button_char
    },
    service(DIGITAL_SERVICES_UUID, charTable, sizeof(charTable) / sizeof(charTable[0]))
{

}

void DigitalService::resetAndAddService()
{
    float_val = false,
    mos_rel0_val = false,
    mos_rel1_val = false,
    gpio0_val = false,
    gpio1_val = false,
    gpio2_val = false,
    gpio3_val = false,
    gpio4_val = false,
    button_val = false,
    ble.gattServer().addService(service);
}

void DigitalService::writeFloat(bool new_float)
{
    float_val = new_float;
    ble.gattServer().write(
        float_char.getValueHandle(),
        (uint8_t *)&float_val,
        sizeof(float_val)
    );
}

bool DigitalService::readMosRel0()
{
    uint8_t mos_rel0_resp = 0;
    uint16_t length = sizeof(mos_rel0_resp);
    ble.gattServer().read(
        mos_rel0_char.getValueHandle(),
        (uint8_t *)&mos_rel0_resp,
        &length
    );
    return (mos_rel0_resp != 0);
}

void DigitalService::writeMosRel0(bool new_mos_rel0)
{
    mos_rel0_val = new_mos_rel0;
    ble.gattServer().write(
        mos_rel0_char.getValueHandle(),
        (uint8_t *)&mos_rel0_val,
        sizeof(mos_rel0_val)
    );
}

bool DigitalService::readMosRel1()
{
    uint8_t mos_rel1_resp = 0;
    uint16_t length = sizeof(mos_rel1_resp);
    ble.gattServer().read(
        mos_rel1_char.getValueHandle(),
        (uint8_t *)&mos_rel1_resp,
        &length
    );
    return (mos_rel1_resp != 0);
}

void DigitalService::writeMosRel1(bool new_mos_rel1)
{
    mos_rel1_val = new_mos_rel1;
    ble.gattServer().write(
        mos_rel1_char.getValueHandle(),
        (uint8_t *)&mos_rel1_val,
        sizeof(mos_rel1_val)
    );
}

bool DigitalService::readGPIO0()
{
    uint8_t gpio0_resp = 0;
    uint16_t length = sizeof(gpio0_resp);
    ble.gattServer().read(
        gpio0_char.getValueHandle(),
        (uint8_t *)&gpio0_resp,
        &length
    );
    return (gpio0_resp != 0);
}

void DigitalService::writeGPIO0(bool new_gpio0)
{
    gpio0_val = new_gpio0;
    ble.gattServer().write(
        gpio0_char.getValueHandle(),
        (uint8_t *)&gpio0_val,
        sizeof(gpio0_val)
    );
}

bool DigitalService::readGPIO1()
{
    uint8_t gpio1_resp = 0;
    uint16_t length = sizeof(gpio1_resp);
    ble.gattServer().read(
        gpio1_char.getValueHandle(),
        (uint8_t *)&gpio1_resp,
        &length
    );
    return (gpio1_resp != 0);
}

void DigitalService::writeGPIO1(bool new_gpio1)
{
    gpio1_val = new_gpio1;
    ble.gattServer().write(
        gpio1_char.getValueHandle(),
        (uint8_t *)&gpio1_val,
        sizeof(gpio1_val)
    );
}

bool DigitalService::readGPIO2()
{
    uint8_t gpio2_resp = 0;
    uint16_t length = sizeof(gpio2_resp);
    ble.gattServer().read(
        gpio2_char.getValueHandle(),
        (uint8_t *)&gpio2_resp,
        &length
    );
    return (gpio2_resp != 0);
}

void DigitalService::writeGPIO2(bool new_gpio2)
{
    gpio2_val = new_gpio2;
    ble.gattServer().write(
        gpio2_char.getValueHandle(),
        (uint8_t *)&gpio2_val,
        sizeof(gpio2_val)
    );
}

bool DigitalService::readGPIO3()
{
    uint8_t gpio3_resp = 0;
    uint16_t length = sizeof(gpio3_resp);
    ble.gattServer().read(
        gpio3_char.getValueHandle(),
        (uint8_t *)&gpio3_resp,
        &length
    );
    return (gpio3_resp != 0);
}

void DigitalService::writeGPIO3(bool new_gpio3)
{
    gpio3_val = new_gpio3;
    ble.gattServer().write(
        gpio3_char.getValueHandle(),
        (uint8_t *)&gpio3_val,
        sizeof(gpio3_val)
    );
}

bool DigitalService::readGPIO4()
{
    uint8_t gpio4_resp = 0;
    uint16_t length = sizeof(gpio4_resp);
    ble.gattServer().read(
        gpio4_char.getValueHandle(),
        (uint8_t *)&gpio4_resp,
        &length
    );
    return (gpio4_resp != 0);
}

void DigitalService::writeGPIO4(bool new_gpio4)
{
    gpio4_val = new_gpio4;
    ble.gattServer().write(
        gpio4_char.getValueHandle(),
        (uint8_t *)&gpio4_val,
        sizeof(gpio4_val)
    );
}

void DigitalService::writeButton(bool new_button)
{
    button_val = new_button;
    ble.gattServer().write(
        button_char.getValueHandle(),
        (uint8_t *)&button_val,
        sizeof(button_val)
    );
}

GattAttribute::Handle_t DigitalService::getFloatHandle()
{
    return float_char.getValueHandle();
}

GattAttribute::Handle_t DigitalService::getMosRel0Handle()
{
    return mos_rel0_char.getValueHandle();
}

GattAttribute::Handle_t DigitalService::getMosRel1Handle()
{
    return mos_rel1_char.getValueHandle();
}

GattAttribute::Handle_t DigitalService::getGpio0Handle()
{
    return gpio0_char.getValueHandle();
}

GattAttribute::Handle_t DigitalService::getGpio1Handle()
{
    return gpio1_char.getValueHandle();
}

GattAttribute::Handle_t DigitalService::getGpio2Handle()
{
    return gpio2_char.getValueHandle();
}

GattAttribute::Handle_t DigitalService::getGpio3Handle()
{
    return gpio3_char.getValueHandle();
}

GattAttribute::Handle_t DigitalService::getGpio4Handle()
{
    return gpio4_char.getValueHandle();
}

GattAttribute::Handle_t DigitalService::getButtonHandle()
{
    return button_char.getValueHandle();
}