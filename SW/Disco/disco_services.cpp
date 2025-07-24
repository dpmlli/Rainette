//  disco_services.cpp
//  project : Rainette
//  author : Damien Pomelli 
//  last modified : 2025-07-24

#include "disco_services.h"

GeneralService::GeneralService(BLE &arg_ble) : 
    ble(arg_ble),
    start_meas_val(false),
    end_of_comm_val(false),
    ping_pong_val(false),

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

    charTable{ &start_meas_char, &end_of_comm_char, &ping_pong_char },
    service(GENERAL_SERVICES_UUID, charTable, sizeof(charTable) / sizeof(charTable[0]))
{

}

void GeneralService::resetAndAddService()
{
    start_meas_val = false;
    end_of_comm_val = false;
    ping_pong_val = false;
    ble.gattServer().addService(service);
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
    temp_val(0),
    adc_test_val(0),

    temp_char(
        TEMP_CHAR_UUID,
        &temp_val,
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
    ),
    adc_test_char(
        ADC_TEST_CHAR_UUID,
        &adc_test_val,
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
    ),
    charTable{ &temp_char, &adc_test_char },
    service(ANALOG_SERVICES_UUID, charTable, sizeof(charTable) / sizeof(charTable[0]))
{
    
}

void AnalogService::resetAndAddService()
{
    temp_val = 0;
    adc_test_val = 0;
    ble.gattServer().addService(service);
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

void AnalogService::writeAdcTest(uint16_t new_adc_test)
{
    adc_test_val = new_adc_test;
    ble.gattServer().write(
        adc_test_char.getValueHandle(),
        (uint8_t *)&adc_test_val,
        sizeof(adc_test_val)
    );
}

GattAttribute::Handle_t AnalogService::getTempHandle()
{
    return temp_char.getValueHandle();
}

GattAttribute::Handle_t AnalogService::getAdcTestHandle()
{
    return adc_test_char.getValueHandle();
}

DigitalService::DigitalService(BLE &arg_ble) :
    ble(arg_ble),
    led_r_val(false),
    led_g_val(false),
    led_b_val(false),
    button_val(false),

    led_r_char(
        LED_R_CHAR_UUID,
        &led_r_val,
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
    ),
    led_g_char(
        LED_G_CHAR_UUID,
        &led_g_val,
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE |
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
    ),
    led_b_char(
        LED_B_CHAR_UUID,
        &led_b_val,
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
    charTable{ &led_r_char, &led_g_char, &led_b_char, &button_char },
    service(DIGITAL_SERVICES_UUID, charTable, sizeof(charTable) / sizeof(charTable[0]))
{

}

void DigitalService::resetAndAddService()
{
    led_r_val = false;
    led_g_val = false;
    led_b_val = false;
    button_val = false;
    ble.gattServer().addService(service);
}

bool DigitalService::readLedR()
{
    uint8_t led_r_resp = 0;
    uint16_t length = sizeof(led_r_resp);
    ble.gattServer().read(
        led_r_char.getValueHandle(),
        (uint8_t *)&led_r_resp,
        &length
    );
    return (led_r_resp != 0);
}

void DigitalService::writeLedR(bool new_led_r)
{
    led_r_val = new_led_r;
    ble.gattServer().write(
        led_r_char.getValueHandle(),
        (uint8_t *)&led_r_val,
        sizeof(led_r_val)
    );
}

bool DigitalService::readLedG()
{
    uint8_t led_g_resp = 0;
    uint16_t length = sizeof(led_g_resp);
    ble.gattServer().read(
        led_g_char.getValueHandle(),
        (uint8_t *)&led_g_resp,
        &length
    );
    return (led_g_resp != 0);
}

void DigitalService::writeLedG(bool new_led_g)
{
    led_g_val = new_led_g;
    ble.gattServer().write(
        led_g_char.getValueHandle(),
        (uint8_t *)&led_g_val,
        sizeof(led_g_val)
    );
}

bool DigitalService::readLedB()
{
    uint8_t led_b_resp = 0;
    uint16_t length = sizeof(led_b_resp);
    ble.gattServer().read(
        led_b_char.getValueHandle(),
        (uint8_t *)&led_b_resp,
        &length
    );
    return (led_b_resp != 0);
}

void DigitalService::writeLedB(bool new_led_b)
{
    led_b_val = new_led_b;
    ble.gattServer().write(
        led_b_char.getValueHandle(),
        (uint8_t *)&led_b_val,
        sizeof(led_b_val)
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

GattAttribute::Handle_t DigitalService::getLedRHandle()
{
    return led_r_char.getValueHandle();
}

GattAttribute::Handle_t DigitalService::getLedGHandle()
{
    return led_g_char.getValueHandle();
}

GattAttribute::Handle_t DigitalService::getLedBHandle()
{
    return led_b_char.getValueHandle();
}

GattAttribute::Handle_t DigitalService::getButtonHandle()
{
    return button_char.getValueHandle();
}