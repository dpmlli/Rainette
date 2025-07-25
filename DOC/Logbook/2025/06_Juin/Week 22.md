# Semaine 22, du lundi 02.06 au dimanche 08.06 2025

## Résumé de la semaine

## Lundi 02.06

## Mardi 03.06

Pour étendre le solder mask de 0,02mm -> 0.786mil je passe la couche solder mask en rectangular pour passer des dimensions 13.78mil à 12.2mil
et de 11.81mil à 10.23mil

### Mode pile
Chaque 10 minutes :
- Réveil
- Mesure temp sensor
- Mesure lum sensor
- (Mesure pile)
- Mesure sondes
- Envoi BLE
- Dodo

### Mode fil
Chaque 10 minutes :
- Mesure temp sensor
- Mesure lum sensor
- (Mesure pile)
- Mesure sondes
- Envoi BLE

En permanence :
- Écoute BLE -> exécution
- Lecture/écriture GPIO

### Étapes programmation
1. Pouvoir debug la carte, installation de Mbed OS
2. Allumer les leds, lire les GPIO
3. ADC
4. I2C
5. BLE

BOM modifiée à la main

**Prompt BLE**
Tu es un expert en BLE. Je dois programmer une petite carte qui fonctionne soit sur batterie, auquel cas et ne fait qu'émettre ses mesures à un raspberry pi, soit sur fil et là en plus des mesures chaque x minutes elle peut aussi recevoir des ordres de la part du raspberry. Si on est sur batterie, on s'éveille juste pour envoyer les mesures et on se rendort, dans l'autre cas la consommation n'est pas un problème dans un peut rester attentif pour recevoir des infos. Je vais utiliser Mbed OS sur la petite carte. Que me proposes-tu pour mettre en place cette configuration ?

🔹 Périphérique BLE (ta carte) = GATT Server
Fournit un Service Mesures (e.g., température, humidité, etc.).
Une caractéristique de lecture/notification : le Raspberry lit ou s’abonne.
En mode filaire : ajoute une caractéristique de commande en écriture.

🔸 Raspberry Pi = GATT Client
Se connecte.
Lit ou reçoit les notifications.
Envoie des commandes en mode filaire via écriture.

| Élément             | Description                                               |
| ------------------- | --------------------------------------------------------- |
| **Service**         | Regroupe plusieurs caractéristiques liées à une fonction  |
| **Caractéristique** | Contient une donnée (valeur), avec des propriétés d’accès |

Une caractéristique peut permettre différentes opérations, selon les propriétés qui lui sont attribuées. Les principales sont :

| Propriété                  | Sens de communication          | Description                                             |
| -------------------------- | ------------------------------ | ------------------------------------------------------- |
| **Read**                   | Périphérique → Centrale        | La centrale (ex: smartphone) peut lire la valeur.       |
| **Write**                  | Centrale → Périphérique        | La centrale peut écrire une nouvelle valeur.            |
| **Notify**                 | Périphérique → Centrale (push) | Le périphérique envoie automatiquement une mise à jour. |
| **Indicate**               | Périphérique → Centrale (ack)  | Comme Notify, mais avec accusé de réception.            |
| **Write Without Response** | Centrale → Périphérique        | Écriture rapide, sans réponse nécessaire.               |

| Rôle             | Description                                                                                                        |
| ---------------- | ------------------------------------------------------------------------------------------------------------------ |
| **Centrale**     | L’appareil qui **initie** la connexion. Il **scanne** les périphériques BLE (ex : smartphone).                     |
| **Périphérique** | L’appareil qui **annonce** sa présence (advertising) et **attend** une connexion (ex : capteur, montre connectée). |

| Rôle        | Description                                                                            |
| ----------- | -------------------------------------------------------------------------------------- |
| **Client**  | Il **interroge** ou **écrit** des données : il demande l’accès à des services/valeurs. |
| **Serveur** | Il **fournit** les données (services/caractéristiques BLE).                            |

| Aspect         | Centrale/Périphérique  | Client/Serveur           |
| -------------- | ---------------------- | ------------------------ |
| Niveau         | Connexion physique BLE | Accès aux données (GATT) |
| Centrale       | Initie la connexion    | En général client        |
| Périphérique   | Attend la connexion    | En général serveur       |
| Indépendants ? | ✅ Oui                  | ✅ Oui                    |


### Exemple pour SHT41 I2C sur Mbed
```c++
#include "mbed.h"

#define SHT41_ADDR (0x44 << 1) // Adresse I2C 7 bits décalée pour mbed

I2C i2c(I2C_SDA, I2C_SCL);
Serial pc(USBTX, USBRX, 115200); // Pour affichage série

char cmd_measure_high_prec[] = { 0xFD };
char data[6];

float convert_temperature(uint16_t raw) {
    return -45.0f + 175.0f * ((float)raw / 65535.0f);
}

float convert_humidity(uint16_t raw) {
    float rh = -6.0f + 125.0f * ((float)raw / 65535.0f);
    if (rh > 100.0f) rh = 100.0f;
    if (rh < 0.0f) rh = 0.0f;
    return rh;
}

int main() {
    while (true) {
        i2c.write(SHT41_ADDR, cmd_measure_high_prec, 1);
        wait_us(10000); // 10 ms
        // Je préfère ThisThread::sleep_for(10)

        if (i2c.read(SHT41_ADDR, data, 6) == 0) {
            uint16_t temp_raw = (data[0] << 8) | data[1];
            uint16_t hum_raw  = (data[3] << 8) | data[4];

            float temperature = convert_temperature(temp_raw);
            float humidity = convert_humidity(hum_raw);

            pc.printf("Temp: %.2f °C, RH: %.2f %%\r\n", temperature, humidity);
        } else {
            pc.printf("Erreur de lecture I2C.\r\n");
        }

        thread_sleep_for(2000); // 2 sec entre chaque lecture
    }
}
```

### 🛠️ Choix de configuration du capteur VEML6030

Dans ce code, le capteur VEML6030 est configuré en écrivant la valeur `0x4000` dans le registre de configuration `0x00` (`ALS_CONF`). Cette configuration active le capteur en mode **normal** (bit `ALS_SD = 0`), **sans interruption** (bit `ALS_INT_EN = 0`). Le gain est fixé à **x2** (`ALS_GAIN = 01`) pour une meilleure sensibilité en conditions de faible luminosité. Le **temps d’intégration** est réglé à **800 ms** (`ALS_IT = 0011`), ce qui permet une mesure plus précise et moins bruitée de la lumière ambiante. Ces paramètres donnent une résolution de **0.0042 lux/bit**, comme spécifié dans la documentation. Le registre `0x04` est ensuite lu pour obtenir la mesure brute (16 bits), qui est convertie en lux par un simple facteur multiplicatif.

```c++
#include "mbed.h"

#define VEML6030_ADDR (0x10 << 1) // ADDR connecté à GND -> adresse 0x20 write, 0x21 read

I2C i2c(I2C_SDA, I2C_SCL);
Serial pc(USBTX, USBRX, 115200); // Console série

char reg_als[] = { 0x04 };
char data[2];

float convert_lux(uint16_t raw) {
    // Gain x2, IT = 800 ms => 0.0042 lux/bit
    return raw * 0.0042f;
}

void init_veml6030() {
    // Configuration : Gain x2 (01), IT = 800 ms (0011), no interrupt, ALS ON
    // b0000 1000 1100 0000 => 0x08C0
    char config[] = { 0x00, 0xC0, 0x08 }; // Registre 0x00, LSB puis MSB
    i2c.write(VEML6030_ADDR, config, 3);
}

int main() {
    pc.printf("Initialisation VEML6030...\r\n");
    init_veml6030();

    while (true) {
        i2c.write(VEML6030_ADDR, reg_als, 1); // Demande de lecture registre 0x04
        if (i2c.read(VEML6030_ADDR, data, 2) == 0) {
            uint16_t als_raw = (data[1] << 8) | data[0]; // LSB first
            float lux = convert_lux(als_raw);

            pc.printf("Lumière ambiante: %u (%.3f lux)\r\n", als_raw, lux);
        } else {
            pc.printf("Erreur de lecture I2C\r\n");
        }

        thread_sleep_for(1000); // Attendre 1s avant nouvelle lecture
    }
}

```

| Bit(s) | Champ        | Valeur binaire | Signification                |
| ------ | ------------ | -------------- | ---------------------------- |
| 15:13  | Réservé      | `000`          | (doit être 0)                |
| 12:11  | `ALS_GAIN`   | `01`           | Gain x2                      |
| 10     | Réservé      | `0`            | -                            |
| 9:6    | `ALS_IT`     | `0011`         | Temps d'intégration = 800 ms |
| 5:4    | `ALS_PERS`   | `00`           | Persistance = 1 lecture      |
| 3:2    | Réservé      | `00`           | -                            |
| 1      | `ALS_INT_EN` | `0`            | Pas d’interruption           |
| 0      | `ALS_SD`     | `0`            | Capteur activé (non éteint)  |

```
bit:  15-13 | 12-11 | 10 | 9-6   | 5-4 | 3-2 | 1 | 0
val:    000 |   01  | 0  | 0011  | 00  | 00  | 0 | 0    -> 0x08C0
```

Dans la datasheet, page 9 (tableau “REFRESH TIME, IDD, AND RESOLUTION RELATION”), on peut lire :

| ALS\_GAIN | PSM Mode | ALS\_IT (ms) | Résolution (lx/bit) |
| --------- | -------- | ------------ | ------------------- |
| x2        | 1        | 800          | **0.0042**          |

### Déclarations des entrées-sorties

```c++
// ATTENTION, ASSIGNIATION À DÉFINIR !!

// ADC
AnalogIn probe0(PC2);
AnalogIn probe1(PC1);
AnalogIn batMeas(PC0);

// Button
InterruptIn button(BUTTON1);

// Leds
DigitalOut led0(LED0);
DigitalOut led1(LED1);

// Float
DigitalIn  floatIn(FLOAT_IN);
DigitalOut floatOut(FLOAT_OUT);

// MOSFET Relays
DigitalOut relay0(RELAYS0);
DigitalOut relay1(RELAYS1);

// Load Switch Enable
DigitalOut pr0En(PC10);
DigitalOut pr1En(PC11);
DigitalOut lumiEn(PB0);
DigitalOut tempEn(PB1);

DigitalOut batMeasEn(PC4);
```

## Mercredi 04.06

## Jeudi 05.06

## Vendredi 06.06

Un thread par évènement asynchrone (ex réceptioniste - administration)

Une event_queue possède déjà un thread

Il faut lister les différents travaux, s'il sont indépendants on peut créer deux thread

On a copié le dossier hors du drive.

Setup software : bash -> powershell

Arguments :
- BLE et 802.15.4 2.4 GHz -> même portée
- grand public donc petite portée ok
- BLE mieux documenté MBed 

## Samedi 07.06