# Semaine 23, du lundi 09.06 au dimanche 15.06 2025

## Résumé de la semaine

## Lundi 09.06

## Mardi 10.06

```c
I2C i2c(I2C3_SDA, I2C3_SCL);


#define STTS22H_WRITE_ADDR 0x70  
#define STTS22H_READ_ADDR  0x71

#define WHO_AM_I_REG       0x01
#define CTRL_REG1          0x04
#define TEMP_OUT_L         0x06
#define TEMP_OUT_H         0x07

void write_register(uint8_t reg, uint8_t value) {
    char data[2] = { (char)reg, (char)value };
    i2c.write(STTS22H_WRITE_ADDR, data, 2);
}

uint8_t read_register(uint8_t reg) {
    char data = reg;
    char result;
    i2c.write(STTS22H_WRITE_ADDR, &data, 1);
    i2c.read(STTS22H_READ_ADDR, &result, 1);
    return result;
}

float read_temperature() {
    char reg = TEMP_OUT_L;
    char data[2];
    i2c.write(STTS22H_WRITE_ADDR, &reg, 1);
    i2c.read(STTS22H_READ_ADDR, data, 2);
    int16_t raw_temp = (int16_t)((data[1] << 8) | (uint8_t)data[0]); // Little endian
    return raw_temp / 100.0f; // Selon configuration par défaut
}

int main() {
    printf("STTS22H Init\n");

    uint8_t who_am_i = read_register(WHO_AM_I_REG);
    printf("WHO_AM_I: 0x%X\n", who_am_i);

    write_register(CTRL_REG1, 0x0C); // Incrément auto et FREERUN

    while (true) {
        float temp = read_temperature();
        printf("Temperature: %.2f C\n", temp);
        ThisThread::sleep_for(1s);
    }
}

```

## Mercredi 11.06

## Jeudi 12.06

Remarque importante de Yann : Interuption -> **EventFlag**.
Dans le callback on set le flag et dans le while(1) `wait_for`

Il faut mettre une pull-up pour le bouton : `btn0.mode(PullUp);`

Actuellement, la communication I2C ne fonctionne pas (pas d'ACK pour les *read* et *write*)

## Vendredi 13.06

La communication I2C fonctionne. Le circuit ne comporte pas de pull-up, le composant non plus. Il faut donc activer 
une pull-up interne au MCU. Cela se passe dans `PeripheralPins.c`, les `GPIO_NOPULL` du I2C3 sont remplacés par `GPIO_PULLUP`. 

Le GAP dans BLE gère la découverte, l'établissement de la connexion, la gestion des rôles des appareils, et d'autres interactions de base dans la communication sans fil entre les appareils BLE.

## Samedi 14.06