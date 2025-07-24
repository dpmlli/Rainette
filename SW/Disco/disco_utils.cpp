//  disco_utils.cpp
//  project : Rainette
//  author : Damien Pomelli 
//  last modified : 2025-07-24

#include "disco_utils.h"

DiscoIO& DiscoIO::getInstance() 
{
    static DiscoIO instance;
    return instance;
}

DiscoIO::DiscoIO() :
    btn1(BUTTON1), btn2(BUTTON2),
    adc_test(ADC_TEST_PIN), i2c(SDA_PIN, SCL_PIN) 
{

}

void write_sensor_register(uint8_t reg, uint8_t value) {
    char data[2] = { (char)reg, (char)value };
    DiscoIO::getInstance().i2c.write(STTS22H_WRITE_ADDR, data, 2);
}

uint8_t read_sensor_register(uint8_t reg) {
    char data = reg;
    char result;
    DiscoIO::getInstance().i2c.write(STTS22H_WRITE_ADDR, &data, 1);
    DiscoIO::getInstance().i2c.read(STTS22H_READ_ADDR, &result, 1);
    return result;
}

float read_temperature() {
    char reg = TEMP_OUT_L;
    char data[2];

    if (DiscoIO::getInstance().i2c.write(STTS22H_WRITE_ADDR, &reg, 1) != 0) {
        printf("Erreur d'écriture I2C\n");
        return -50.0f;  
    }

    if (DiscoIO::getInstance().i2c.read(STTS22H_READ_ADDR, data, 2) != 0) {
        printf("Erreur de lecture I2C\n");
        return -50.0f;
    }
    int16_t raw_temp = (int16_t)((data[1] << 8) | (uint8_t)data[0]); // Little endian
    return raw_temp / 100.0f; // Selon configuration par défaut
}
