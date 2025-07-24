//  rainette_utils.cpp
//  project : Rainette
//  author : Damien Pomelli 
//  last modified : 2025-07-24

#include "rainette_utils.h"

RainetteIO& RainetteIO::getInstance() 
{
    static RainetteIO instance;
    return instance;
}

RainetteIO::RainetteIO() :
    led0(LED0_PIN), led1(LED1_PIN), lumi_int(LUMI_INT_PIN),
    en_probe0(EN_PROBE0_PIN), en_probe1(EN_PROBE1_PIN), en_temp(EN_TEMP_PIN), 
    en_lumi(EN_LUMI_PIN), en_batt_meas(EN_BATT_MEAS_PIN), usr_button(USR_BUTTON_PIN), 
    float_in(FLOAT_IN_PIN), float_out(FLOAT_OUT_PIN), mos_rel0_out(MOS_REL0_OUT_PIN), 
    mos_rel1_out(MOS_REL1_OUT_PIN), gpio0_out(GPIO0_OUT_PIN), gpio1_out(GPIO1_OUT_PIN), 
    gpio2_out(GPIO2_OUT_PIN), gpio3_out(GPIO3_OUT_PIN), gpio4_out(GPIO4_OUT_PIN),
    probe0(PROBE0_PIN), probe1(PROBE1_PIN), batMeas(BATMEAS_PIN)
{

}

static I2C* _i2c = nullptr;

void i2c_init(I2C* i2c_instance) {
    _i2c = i2c_instance;
}

void i2c_deinit() {
    _i2c = nullptr; 
}

float convertTemperature(uint16_t raw) 
{
    return -45.0f + 175.0f * ((float)raw / 65535.0f);
}

float convertHumidity(uint16_t raw) 
{
    float rh = -6.0f + 125.0f * ((float)raw / 65535.0f);
    if (rh > 100.0f) rh = 100.0f;
    if (rh < 0.0f) rh = 0.0f;
    return rh;
}

bool readTempHumi(float &temperature, float &humidity) 
{
    if (!_i2c) return false;    
    RainetteIO::getInstance().en_temp.write(1);
    ThisThread::sleep_for(10ms);

    char cmd_measure_high_prec[] = { 0xFD };
    char data[6];

    if (_i2c->write(SHT41_ADDR, cmd_measure_high_prec, 1) != 0) { 
        RainetteIO::getInstance().en_temp.write(0);
        printf("Erreur d'écriture - commande temp humi\n");
        return false;
    }

    ThisThread::sleep_for(10ms);

    if (_i2c->read(SHT41_ADDR, data, 6) != 0) { 
        RainetteIO::getInstance().en_temp.write(0);
        printf("Erreur de lecture - données temp humi\n");
        return false;
    }

    uint16_t temp_raw = (data[0] << 8) | data[1];
    uint16_t hum_raw  = (data[3] << 8) | data[4];

    temperature = convertTemperature(temp_raw);
    humidity = convertHumidity(hum_raw);

    RainetteIO::getInstance().en_temp.write(0);
    return true;
}

float convertLux(uint16_t raw) 
{
    // Gain x2, IT = 800 ms => 0.0042 lux/bit
    return raw * 0.0042f;
}

bool readLumi(float &lumi) // Not working yet
{
    if (!_i2c) return false;   
    RainetteIO::getInstance().en_lumi.write(1);
    RainetteIO::getInstance().en_temp.write(1);
    ThisThread::sleep_for(500ms);

    char reg = 0x00;
    char reg_val[2];

    _i2c->write(VEML6030_ADDR, &reg, 1);
    _i2c->read(VEML6030_ADDR, reg_val, 2);

    uint16_t value = reg_val[1] << 8 | reg_val[0];

    printf("Registre avant mise à 0 de ALS_SD : 0x%02X%02X\n",reg_val[1],reg_val[0]);
    value &= 0xFFFE;

    char wake_up[3] = {
        0x00,                   // Register address
        static_cast<char>(value & 0xFF),       // LSB
        static_cast<char>((value >> 8) & 0xFF) // MSB
    };

    _i2c->write(VEML6030_ADDR, wake_up, 3);

    ThisThread::sleep_for(10ms);

    // Check
    char reg_ver = 0x00;
    char reg_val_ver[2];

    _i2c->write(VEML6030_ADDR, &reg_ver, 1);
    _i2c->read(VEML6030_ADDR, reg_val_ver, 2);

    uint16_t value_ver = reg_val_ver[1] << 8 | reg_val_ver[0];

    printf("Registre après mise à 0 de ALS_SD : 0x%02X%02X\n",reg_val_ver[1],reg_val_ver[0]);

    // ID Check
    char reg_id[] = { 0x07 };
    char id_data[2];
    _i2c->write(VEML6030_ADDR, reg_id, 1);
    _i2c->read(VEML6030_ADDR, id_data, 2);
    printf("ID : 0x%02X%02X\n", id_data[1], id_data[0]);

    // Configuration : Gain x2 (01), IT = 800 ms (0011), no interrupt, ALS ON
    // b0000 1000 1100 0000 => 0x08C0
    char config[] = { 0x00, 0xC0, 0x08 }; // Registre 0x00, LSB puis MSB
    if (_i2c->write(VEML6030_ADDR, config, 3) != 0) {  
        printf("Erreur I2C lors de l'écriture de la configuration\n");
        RainetteIO::getInstance().en_lumi.write(0);
        return false;
    }

    printf("Configuration envoyée avec succès\n");

    ThisThread::sleep_for(1200ms);

    char reg_als[] = { 0x04 };
    char data[2];

    if (_i2c->write(VEML6030_ADDR, reg_als, 1) != 0) { 
        printf("Erreur I2C lors de l'envoi de l'adresse du registre ALS\n");
        RainetteIO::getInstance().en_lumi.write(0);
        return false;
    }

    if (_i2c->read(VEML6030_ADDR, data, 2) != 0) {
        printf("Erreur I2C lors de la lecture du registre ALS\n");
        RainetteIO::getInstance().en_lumi.write(0);
        return false;
    }

    

    uint16_t als_raw = (data[1] << 8) | data[0]; // LSB first
    printf("Valeur brute ALS : %u (0x%04X)\n", als_raw, als_raw);
    lumi = convertLux(als_raw);
    printf("Éclairement mesuré : %.3f lux\n", lumi);

    RainetteIO::getInstance().en_lumi.write(0);
    RainetteIO::getInstance().en_temp.write(0);
    return true;
}

float convertTempSensor(float adc_val) 
{
    return ((adc_val - 1.058) / 0.009);
}

float correctBattMeas(float meas) {
    const float a = 5.187502f;
    const float b = -11.3899f;
    const float c = 7.911509f;

    if (meas < 1.1) {
        printf("Mesure inférieure à 1.1V : %f\n",meas);
        return meas;
    }

    return a * meas * meas + b * meas + c;
}