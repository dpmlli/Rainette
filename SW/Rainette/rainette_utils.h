//  rainette_utils.h
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

#define BATT_DET_THRESH    1.5f

#define SDA_PIN            PB_9
#define SCL_PIN            PB_8

#define LED0_PIN           PB_12
#define LED1_PIN           PB_13
#define LUMI_INT_PIN       PC_12

#define EN_PROBE0_PIN      PC_10
#define EN_PROBE1_PIN      PC_11
#define EN_TEMP_PIN        PB_1
#define EN_LUMI_PIN        PB_0
#define EN_BATT_MEAS_PIN   PC_4

#define USR_BUTTON_PIN     PB_14
#define FLOAT_IN_PIN       PA_12
#define FLOAT_OUT_PIN      PA_10
#define MOS_REL0_OUT_PIN   PD_1
#define MOS_REL1_OUT_PIN   PD_0

#define GPIO0_OUT_PIN      PA_2
#define GPIO1_OUT_PIN      PA_3
#define GPIO2_OUT_PIN      PA_4
#define GPIO3_OUT_PIN      PA_5
#define GPIO4_OUT_PIN      PA_6

#define PROBE0_PIN         PC_2
#define PROBE1_PIN         PC_1
#define BATMEAS_PIN        PC_0     

#define SHT41_ADDR (0x44 << 1)
#define VEML6030_ADDR (0x10 << 1)

class RainetteIO {
public:
    static RainetteIO& getInstance();

    DigitalOut led0;
    DigitalOut led1;
    DigitalIn lumi_int;

    DigitalOut en_probe0;
    DigitalOut en_probe1;
    DigitalOut en_temp;
    DigitalOut en_lumi;
    DigitalOut en_batt_meas;

    InterruptIn usr_button;
    InterruptIn float_in;
    DigitalOut float_out;
    DigitalOut mos_rel0_out;
    DigitalOut mos_rel1_out;

    DigitalOut gpio0_out;
    DigitalOut gpio1_out;
    DigitalOut gpio2_out;
    DigitalOut gpio3_out;
    DigitalOut gpio4_out;

    AnalogIn probe0;
    AnalogIn probe1;
    AnalogIn batMeas;

private:
    RainetteIO();
    RainetteIO(const RainetteIO&) = delete;
    RainetteIO& operator=(const RainetteIO&) = delete;
};

void i2c_init(I2C* i2c_instance);
void i2c_deinit();

float convertTemperature(uint16_t raw);
float convertHumidity(uint16_t raw);
bool readTempHumi(float &temperature, float &humidity);
float convertLux(uint16_t raw);
bool readLumi(float &lumi);
float convertTempSensor(float adc_val);
float correctBattMeas(float meas);

#endif