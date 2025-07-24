//  disco_utils.h
//  project : Rainette
//  author : Damien Pomelli 
//  last modified : 2025-07-24

#ifndef UTILS_H
#define UTILS_H

#include "mbed.h"

#include "mbed_trace.h"
#define TRACE_GROUP  "main"

#include "PinNames.h"

#include "platform/Callback.h"
#include "events/EventQueue.h"
#include "ble/BLE.h"
#include "mbed-trace/mbed_trace.h"

using namespace std::literals::chrono_literals;

#define SDA_PIN         PB_11
#define SCL_PIN         PB_13

#define ADC_TEST_PIN    PC_3

#define STTS22H_WRITE_ADDR 0x70  
#define STTS22H_READ_ADDR  0x71

#define WHO_AM_I_REG       0x01
#define CTRL_REG1          0x04
#define TEMP_OUT_L         0x06
#define TEMP_OUT_H         0x07

class DiscoIO {
public:
    static DiscoIO& getInstance();

    InterruptIn btn1;
    InterruptIn btn2;

    AnalogIn adc_test;

    I2C i2c;

private:
    DiscoIO();
    DiscoIO(const DiscoIO&) = delete;
    DiscoIO& operator=(const DiscoIO&) = delete;
};

void write_sensor_register(uint8_t reg, uint8_t value);
uint8_t read_sensor_register(uint8_t reg);
float read_temperature();

#endif