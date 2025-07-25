# Semaine 24, du lundi 16.06 au dimanche 22.06 2025

## Résumé de la semaine

## Lundi 16.06

Le GAP gère toujours la connexion, même avec GATT. On a une adresse *MAC* en BLE.

L’**advertising** (publicité) en BLE est le mécanisme par lequel un périphérique annonce sa présence aux autres (généralement les clients).

Le contenu des paquets publiés est le suivant (+- standardisé):
- Nom du périphérique
- Adresse MAC
- UUID de services
- Données optionnelles (apparence, fabricant, etc.)

La **pile BLE** est l'ensemble des couches logicielles qui implémentent le protocole Bluetooth Low Energy, elle comprend notamment GAP, GATT, ATT, HCI, etc.

Le payload est la donnée transmise dans les paquets advertising BLE. C’est ce que voit un client BLE quand il scanne.

`on_init_complete` : Callback appelé une fois l'initialisation de la pile BLE terminée pour configurer l’environnement.

La classe **GattServerProcess** est un utilitaire mbed qui encapsule :
- L'initialisation du BLE (ble.init()),
- La configuration du GAP (nom, mode publicité, etc.),
- Le démarrage de la publicité BLE.

Code *Update Characteristics* :
```
main()
 └── crée GattServerProcess + ClockService
         └── GattServerProcess.on_init() → ClockService::start()
                 ├── Enregistre le service (_clock_service) auprès du GattServer
                 ├── Enregistre ClockService comme EventHandler BLE
                 └── Lance l'incrément périodique via EventQueue

```

Une caractéristique GATT peut avoir plusieurs propriétés combinées par OU binaire (|), comme : `READ | WRITE | NOTIFY | INDICATE`.
Le serveur annonce ce qu’il autorise via les propriétés et le client agit en conséquence.

**Handlers (gestionnaires d’événements)** : spécifiques au serveur GATT. Elles sont appelées automatiquement par la pile BLE quand un événement lié à une caractéristique se produit.

| Handler                       | Événement déclencheur côté client                   | Rôle côté serveur                                                                      |
| ----------------------------- | --------------------------------------------------- | -------------------------------------------------------------------------------------- |
| `onDataSent(...)`             | Une notification ou indication vient d’être envoyée | Confirmation que le serveur a bien transmis                                            |
| `onDataWritten(...)`          | Le client écrit dans une caractéristique            | Permet de réagir (log, mise à jour, validation)                                        |
| `onDataRead(...)`             | Le client lit une caractéristique                   | Permet de tracer ou de vérifier un accès                                               |
| `onUpdatesEnabled(...)`       | Le client s’abonne à `NOTIFY` ou `INDICATE`         | Permet de savoir quel client écoute quoi                                               |
| `onUpdatesDisabled(...)`      | Le client se désabonne                              | Idem, pour le suivi                                                                    |
| `onConfirmationReceived(...)` | Le client a acquitté une `INDICATE`                 | Confirmation de réception (obligatoire pour `INDICATE`)                                |
| `authorize_client_write(...)` | Le client veut écrire une valeur                    | Permet d’accepter ou refuser **dynamiquement** l’écriture (validation, sécurité, etc.) |

`authorize_client_write(...)` n’est pas une surcharge de `GattServer::EventHandler`, mais un callback d'autorisation attaché manuellement ici :
`_hour_char.setWriteAuthorizationCallback(this, &ClockService::authorize_client_write);`

Il permet de bloquer ou valider une écriture avant qu’elle soit appliquée (ici, valeurs numériques valides uniquement).

Les handlers permettent au serveur :
- de réagir à toutes les opérations du client,
- de tracer, valider, ou refuser des actions,
- d'implémenter des règles métier (ex : borne horaire).

Une **surcharge** (override), c’est quand une classe dérivée remplace une méthode virtuelle de la classe de base, pour personnaliser son comportement.

Avec MBed OS, c'est un client à la fois.

**GattServer** : outil pour gérer les interactions GATT réelles (services, caracs, événements).
**GattServerProcess** : classe mbed facilitatrice qui initialise BLE + GAP + GATT, et appelle ton code quand prêt.

`BLE &ble = BLE::Instance();` : Instance centrale pour accéder à la pile BLE.

On ne peut pas passer `event_flags` par référence à une ISR (`btn1_irq`, `btn2_irq`) classique, pour une raison fondamentale : les fonctions ISR sont des callbacks sans argument imposées par *Mbed*.

**BlueNRG_2** est réservé aux cibles sans BLE intégré, le code suivant devrait être ajoutée dans le mbed_app.json si un pépin apparait. Le README.md des exemples BLE mentionne cet ajout pour d'autres cartes mais ça peut être une piste.
```json
"DISCO_WB5MMG": {
    "target.features_add": ["BLE"],
    "target.extra_labels_add": ["CORDIO"]
}
```

Lien pour les utils BLE de MBed OS : https://github.com/ARMmbed/mbed-os-ble-utils/tree/master.
`pretty_printer.h`, `ble_process.h` et `gatt_server_process.h` en ont été téléchargés pour utiliser la classe *GattServerProcess*

`I2C &bus` est une référence vers un objet I2C passé au constructeur. L’initialiseur : `_i2c(bus)` assigne directement cette référence au membre `_i2c` lors de la construction.

| Élément                               | Rôle                                                              |
| ------------------------------------- | ----------------------------------------------------------------- |
| `set(...)`                            | Écrit manuellement une valeur dans le GATT BLE.                   |
| `call_every(..., update_temperature)` | Exécute périodiquement une fonction qui lit + appelle `set(...)`. |

```css
[INFO][BLCO]: WSF Cordio tracing enabled
[INFO][BLE ]: Allocated 2250 bytes for Cordio
[INFO][BLE ]: BLE features enabled: EXTENDED_ADVERTISING GATT_CLIENT GATT_SERVER PERIODIC_ADVERTISING PHY_MANAGEMENT PRIVACY SECURE_CONNECTIONS SECURITY SIGNING WHITELIST
[INFO][BLE ]: BLE roles enabled: PERIPHERAL CENTRAL
[INFO][BLE ]: Initialising Cordio host stack
[DBG ][BLE ]: HCI handler id: 0
[DBG ][BLE ]: DM handler id: 1
[DBG ][BLE ]: L2C slave handler id: 2
[DBG ][BLE ]: ATT handler id: 3
[DBG ][BLE ]: SMP handler id: 4
[DBG ][BLE ]: stack handler id: 5
[INFO][BLPR]: Initialize privacy PAL
Ble process started.
[INFO][BLE ]: Initialising BLE instance
[INFO][BLHC]: CordioHCIDriver initializing
[INFO][BLWB]: init_debug ENABLED
[DBG ][BLWB]: syscmd_status_not, status:1
[DBG ][BLWB]: syscmd_status_not, status:0
[DBG ][BLWB]: syscmd_status_not, status:1
[INFO][BLWB]: WIRELESS COPROCESSOR FW VERSION ID = 1.13.2
[INFO][BLWB]: WIRELESS COPROCESSOR FW STACK TYPE = 1 (ROM size 0xca000)
[DBG ][BLE ]: Cordio HCI reset started
[DBG ][BLWB]: mbox_write type:1, len:3
[INFO][BLWB]: TX>> BLE CMD
[DBG ][BLWB]:   Type 1
[DBG ][BLWB]:    Cmd c03
[DBG ][BLWB]:    Len 0
[DBG ][BLE ]: Reset sequence command complete: 0c03
[DBG ][BLWB]: Command Complete Event Command
[DBG ][BLWB]:   HCI_EVT_HDR_LEN=2
[DBG ][BLWB]:   opcode = 000c03
[INFO][BLWB]: WB Reset Received
[INFO][BLWB]: Found Unique Device Number: 6f00a3
[DBG ][BLWB]: mbox_write type:1, len:11
[INFO][BLWB]: TX>> BLE CMD
[DBG ][BLWB]:   Type 1
[DBG ][BLWB]:    Cmd fc0c
[DBG ][BLWB]:    Len 8
[INFO][BLWB]: Setting Bdaddr: a3:00:6f:27:e1:80
[DBG ][BLE ]: Reset sequence command complete: fc0c
[DBG ][BLWB]: Command Complete Event Command
[DBG ][BLWB]:   HCI_EVT_HDR_LEN=2
[DBG ][BLWB]:   opcode = 00fc0c
[DBG ][BLWB]: Bluetooth Device address set
[DBG ][BLWB]: mbox_write type:1, len:5
[INFO][BLWB]: TX>> BLE CMD
[DBG ][BLWB]:   Type 1
[DBG ][BLWB]:    Cmd fc0f
[DBG ][BLWB]:    Len 2
[DBG ][BLE ]: Reset sequence command complete: fc0f
[DBG ][BLWB]: Command Complete Event Command
[DBG ][BLWB]:   HCI_EVT_HDR_LEN=2
[DBG ][BLWB]:   opcode = 00fc0f
[DBG ][BLWB]: Tx Power Level set
[DBG ][BLWB]: mbox_write type:1, len:11
[INFO][BLWB]: TX>> BLE CMD
[DBG ][BLWB]:   Type 1
[DBG ][BLWB]:    Cmd c01
[DBG ][BLWB]:    Len 8
[DBG ][BLE ]: Reset sequence command complete: 0c01
[DBG ][BLWB]: Command Complete Event Command
[DBG ][BLWB]:   HCI_EVT_HDR_LEN=2
[DBG ][BLWB]:   opcode = 000c01
[DBG ][BLWB]: mbox_write type:1, len:11
[INFO][BLWB]: TX>> BLE CMD
[DBG ][BLWB]:   Type 1
[DBG ][BLWB]:    Cmd 2001
[DBG ][BLWB]:    Len 8
[DBG ][BLE ]: Reset sequence command complete: 2001
[DBG ][BLWB]: Command Complete Event Command
[DBG ][BLWB]:   HCI_EVT_HDR_LEN=2
[DBG ][BLWB]:   opcode = 002001
[DBG ][BLWB]: mbox_write type:1, len:3
[INFO][BLWB]: TX>> BLE CMD
[DBG ][BLWB]:   Type 1
[DBG ][BLWB]:    Cmd 1009
[DBG ][BLWB]:    Len 0
[DBG ][BLE ]: Reset sequence command complete: 1009
[DBG ][BLWB]: Command Complete Event Command
[DBG ][BLWB]:   HCI_EVT_HDR_LEN=2
[DBG ][BLWB]:   opcode = 001009
[DBG ][BLWB]: mbox_write type:1, len:3
[INFO][BLWB]: TX>> BLE CMD
[DBG ][BLWB]:   Type 1
[DBG ][BLWB]:    Cmd 2002
[DBG ][BLWB]:    Len 0
[INFO][BLWB]: Command Status event, status:1, opcode=0x2002
```

Il va surement falloir passer en mode debug pour voir où on bloque.

## Mardi 17.06

Le mode debug n'aide pas.

```css
[INFO][BLWB]: Command Status event, status:1, opcode=0x2002
```
indique une erreur **HCI_LE_READ_BUFFER_SIZE** selon l'AN https://www.st.com/resource/en/application_note/an5270-stm32wb-bluetooth-low-energy-wireless-interface-stmicroelectronics.pdf

Le **HCI** (Host Controller Interface) est une interface de communication entre le contrôleur Bluetooth (souvent un module ou une puce Bluetooth) et le hôte (généralement un microcontrôleur ou un processeur principal qui gère la pile Bluetooth et les applications associées).

**Analyse des logs**:
- HCI_RESET -> ok
- ACI_HAL_WRITE_CONFIG_DATA -> ok
- ACI_HAL_SET_TX_POWER_LEVEL -> ok
- HCI_SET_EVENT_MASK -> ok
- HCI_LE_SET_EVENT_MASK -> ok
- HCI_READ_BD_ADDR -> ok
- HCI_LE_READ_BUFFER_SIZE -> **PAS OK**

https://wiki.st.com/stm32mcu/wiki/Connectivity:STM32WB_BLE_Wireless_Stack

Le Wireless Coprocessor FW est le “cerveau radio” du STM32WB, c'est le microprogramme (“firmware”) qui s’exécute sur le cœur Cortex-M0+ (“CPU2”) des microcontrôleurs STM32WB.

Le fichier `stm32wb5x_BLE_Stack_full_fw.bin` est un firmware radio BLE “tout-en-un” à flasher sur CPU2 pour activer toutes les fonctions Bluetooth LE.

https://github.com/STMicroelectronics/STM32CubeWB/blob/master/Projects/STM32WB_Copro_Wireless_Binaries/STM32WB5x/Release_Notes.html 

https://forums.mbed.com/t/mbed-os-ble-stopped-working-on-stm32wb55-after-update-fus-and-ble-stack-to-version-1-14-1/18387

https://community.st.com/t5/stm32-mcus-wireless/stm32wb55rg-hci-driver-communication-problems-after-ble-stack/td-p/107056

Le FUS (Firmware Upgrade Service) est un composant logiciel intégré dans les microcontrôleurs STM32WB (notamment sur la puce CPU2). Son rôle principal est de gérer les opérations de mise à jour et de sécurité du firmware sans fil (Bluetooth, Thread, Zigbee, etc.) sur les STM32WB.

https://htmlpreview.github.io/?https://github.com/STMicroelectronics/STM32CubeWB/blob/master/Projects/STM32WB_Copro_Wireless_Binaries/STM32WB5x/Release_Notes.html

**ACI** (Application Communication Interface) : Interface entre l'application (logique de haut niveau) et le firmware BLE, simplifiant l'accès aux fonctionnalités BLE.

**HCI** (Host Controller Interface) : Interface entre le host (processeur principal) et le contrôleur Bluetooth, permettant de gérer la communication sans fil.

**HAL** (Hardware Abstraction Layer) : Couche d'abstraction matérielle qui permet au stack Bluetooth de fonctionner indépendamment du matériel sous-jacent.

| Abréviation | Signification complète  | Description                                                                |
| ----------- | ----------------------- | -------------------------------------------------------------------------- |
| **LO**      | **Link Layer Only**     | Implémentation avec uniquement la couche liaison (LL), sans pile complète. |
| **PO**      | **Peripheral Only**     | Implémentation BLE agissant uniquement comme périphérique (slave).         |
| **BO**      | **Beacon Only**         | Implémentation très minimale, souvent utilisée pour les balises (beacons). |
| **BF**      | **Basic Functionality** | Configuration de base avec un ensemble limité de fonctionnalités BLE.      |
| **LB**      | **Link Layer Basic**    | Version basique de la couche liaison, encore plus restreinte que LO.       |

MEMORY
{ 
  FLASH (rx) : ORIGIN = MBED_APP_START, LENGTH = MBED_APP_SIZE
  RAM (rwx)   : ORIGIN = MBED_RAM_START + VECTORS_SIZE, LENGTH = 0x30000 - VECTORS_SIZE
  RAM2a (rw)  : ORIGIN = 0x20030000, LENGTH = 10K
  RAM2b (rw)  : ORIGIN = 0x20038000, LENGTH = 20K
}

    .ble_stby_mem (NOLOAD) :
    {
      *(MAPPING_TABLE);
      *(MB_MEM1);
    } >RAM2a

https://github.com/ARMmbed/mbed-os/commit/ca28a86d1b2120b0c2816b99d40b36fe1f968fb0

https://wiki.st.com/stm32mcu/wiki/Connectivity:STM32WB_BLE_Wireless_Stack

Fonctionne avec une adresse MAC aléatoire. `stm32wb5x_BLE_HCILayer_fw.bin` v1.12.0 pour le FUS.

https://htmlpreview.github.io/?https://github.com/STMicroelectronics/STM32CubeWB/blob/master/Projects/STM32WB_Copro_Wireless_Binaries/STM32WB5x/Release_Notes.html

Chercher la version du firmware dans les commits GitHub.

Comme on a pas la version *extended*, pas de modif du .ld à faire. CubeIDE c'est GCC. On aurait pu demandé à GPT de modifier le .ld pour nous.

## Mercredi 18.06

**Différents binaires** : https://github.com/STMicroelectronics/STM32CubeWB/tree/master/Projects/STM32WB_Copro_Wireless_Binaries/STM32WB5x

**Explications FW M0** : https://github.com/ARMmbed/mbed-os/tree/master/targets/TARGET_STM/TARGET_STM32WB

Ca ne fonctionnait plus mais ça a été résolu en reflashant le FW en suivant rigoureusement les instructions du README.md des targets 

```
* connect the board with ST-LINK
* In the left column, go to "Firmware Upgrade Services"
* "Start FUS"
* "Read FUS infos" => version v1.2.0 is expected
* Firmware Upgrade / "Browse" : select the chosen FW (see above)
* Firmware Upgrade / Start address : depends on the chosen FW (see above)
* Firmware Upgrade / "Firmware Upgrade"
* In the left column, go to "Option bytes"
* User Configuration => "Read"
* User Configuration / enable nSWBOOT0 => "Apply"
```
J'ai également rajouté, conformément à ce README.md, la ligne suivante dans le `mbed_app.json`.

```json
"DISCO_WB5MMG": {
    "target.mbed_rom_size": "0xE1000"
}
```

Ce qui nous a fait passer de :

```css
[INFO][BLWB]: WIRELESS COPROCESSOR FW VERSION ID = 1.12.0
[INFO][BLWB]: WIRELESS COPROCESSOR FW STACK TYPE = 0 (ROM size 0xca000)
```
à :

```css
[INFO][BLWB]: WIRELESS COPROCESSOR FW VERSION ID = 1.12.0
[INFO][BLWB]: WIRELESS COPROCESSOR FW STACK TYPE = 0 (ROM size 0xe1000)
```
En essayant la version *update* ce message apprait après une longue série de logs.

```
++ MbedOS Error Info ++
Error Status: 0x8001012F Code: 303 Module: 1
Error Message: Error - writing to a file in an ISR or critical section

Location: 0x802492B
Error Value: 0x1
Current Thread: rtx_idle Id: 0x20000DC4 Entry: 0x8003A79 StackSize: 0x380 StackMem: 0x20000E08 SP: 0x2002FDAC 
For more info, visit: https://mbed.com/s/error?error=0x8001012F&tgt=DISCO_WB5MMG
-- MbedOS Error Info --
```

Le problème semble venir des classes *process* car ça fonctionne pour l'exemple *GAP* ou l'*Advertising*.

Le test *HeartRate* fonctionne ! C'est l'exemple `BLE_GattServer_AddService`, renommé `main_add_serv.cpp`.
Le repo est le suivant : https://github.com/ARMmbed/mbed-os-example-ble/tree/mbed-os-6.7.0/BLE_GattServer_AddService
Attention, le **service HeartRate** est déjà existant et comporte des méthodes, il faudrait peut-être s'en inspirer pour 
mettre à jour les données sans lever d'erreurs. https://github.com/ARMmbed/ble/blob/master/ble/services/HeartRateService.h

Il existe déjà un *ReadOnlyGattCharacteristic* dans `GattCharacteristic.h`. Attention il y a de légères diff entre le GitHub et le `.h` !

Maintenant, il faut adapter ce code pour transmettre une température.

## Jeudi 19.06

La norme Bluetooth standardise le format de nombreuses caractéristiques pour permettre à chaque appareil de les lire. Exemple pour le Heart Rate : https://www.bluetooth.com/specifications/specs/heart-rate-service-1-0/.

Exemple de service/caractéristiques 

```c
// Déclaration des UUID pour les services et caractéristiques
static const ble::UUID temperature_service_uuid(0x181A); // UUID pour le service de température (selon Bluetooth SIG)
static const ble::UUID humidity_service_uuid(0x181A);    // UUID pour le service d'humidité
static const ble::UUID luminosity_service_uuid(0x1234);  // Exemple de UUID pour le service de luminosité

static const ble::UUID temperature_char_uuid(0x2A6E); // UUID pour la caractéristique de température
static const ble::UUID humidity_char_uuid(0x2A6F);    // UUID pour la caractéristique d'humidité
static const ble::UUID luminosity_char_uuid(0x1235);   // Exemple de UUID pour la caractéristique de luminosité

// Définition des services et des caractéristiques
ble::GattService temperature_service(temperature_service_uuid);
ble::GattService humidity_service(humidity_service_uuid);
ble::GattService luminosity_service(luminosity_service_uuid);

// Ajouter les caractéristiques aux services respectifs
temperature_service.addCharacteristic(temperature_char_uuid, ble::GattCharacteristic::PROPERTY_READ);
humidity_service.addCharacteristic(humidity_char_uuid, ble::GattCharacteristic::PROPERTY_READ);
luminosity_service.addCharacteristic(luminosity_char_uuid, ble::GattCharacteristic::PROPERTY_READ);

// Enregistrement des services sur le périphérique BLE
_ble.gattServer().addService(temperature_service);
_ble.gattServer().addService(humidity_service);
_ble.gattServer().addService(luminosity_service);

```

Excellent, on parvient à publier les valeurs de température !! C'est avec le `main_temp_service.cpp`, dérivé de `main_add_serv.cpp`, et avec le service perso de `ble_temperature_service.h`. Les valeurs envoyées sont en Big Endian donc il faut les lire à l'envers. On reçoit le message *[INFO][BLGS]: Write attribute 13 - value=08:0c, local_only=false* chaque seconde, indépendamment de la présence d'un client.

À présent, je vais développer le code Python pour être capable de lire ces infos.

Ca fonctionne. Avec Bleak, l'UUID doit avoir ce format 128 bits : `0000xxxx-0000-1000-8000-00805f9b34fb` avec l'UUID 16 bits utilisés côté MCU qui remplace les *xxxx*. Ex : 0x181A -> 0000181a-0000-1000-8000-00805f9b34fb. Contrairement au format des données durant le transport, on récupère les valeurs en *Little Endian*, il faut donc cette extraction :

```python
def notification_handler(sender, data):
    value = struct.unpack("<h", data)[0]  # int16, little-endian
    temperature = value / 100.0
    print(f"Température: {temperature} °C")
```

Fonctionnement d'une event_queue avec exécution infinie :

```c
int main() {
    // Planifier une tâche répétitive toutes les 1000 ms
    _event_queue.call_every(1000ms, update_sensor_value);

    // Planifier une tâche immédiate
    _event_queue.call(immediate_task);

    // Exécuter la boucle d'événements
    _event_queue.dispatch_forever();
}
```

Pour stopper la pile BLE :

```c
void stop()
    {
        if (_ble.hasInitialized()) {
            _ble.shutdown();
            printf("Ble process stopped.");
        }
    }
```

Un **handle** dans le contexte du Bluetooth Low Energy (BLE) et de l'API Mbed OS fait référence à un identifiant ou un pointeur utilisé pour désigner un objet ou une ressource particulière au sein du stack BLE. Ici, il est associé à un processus de publicité particulier.

```c
// démarre le processus de publicité (ce n'est pas une boucle)
error = _ble.gap().startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);

// suspend le processus
error = _ble.gap().stopAdvertising(ble::LEGACY_ADVERTISING_HANDLE);
```

L'**EventQueue** dans Mbed OS est une structure utilisée pour gérer l'exécution asynchrone de fonctions ou d'événements. Elle permet de programmer l'exécution différée de certaines tâches ou de répéter certaines actions à intervalles réguliers.

Le **Legacy Advertising** est le mode de publicité traditionnel dans BLE, basique mais plus ancien et limité que le **Extended Advertising**.

`ble::Gap::EventHandler` est une classe dans le stack Bluetooth Low Energy (BLE) de Mbed OS qui permet de gérer les événements associés à la **GAP** (Generic Access Profile) de Bluetooth.

La communication **PHY 2M** fait référence à l'utilisation d'une vitesse de transmission de données de 2 Mbps (Mégabits par seconde) en BLE, spécifiquement avec la PHY (Physical Layer) 2M introduite avec la spécification Bluetooth 5.0.

**Legacy Advertising vs Extended Advertising** :

| **Caractéristique**                  | **Legacy Advertising**                              | **Extended Advertising**                                               |
| ------------------------------------ | --------------------------------------------------- | ---------------------------------------------------------------------- |
| **Introduit dans**                   | Bluetooth 4.0/4.2                                   | Bluetooth 5.0                                                          |
| **Vitesse de transmission**          | 1 Mbps (PHY 1M)                                     | 1 Mbps (PHY 1M), 2 Mbps (PHY 2M), etc.                                 |
| **Taille des paquets publicitaires** | 31 octets max                                       | Jusqu'à 255 octets                                                     |
| **Canaux de publicité**              | 3 canaux (37, 38, 39)                               | Jusqu'à 37 canaux                                                      |
| **Mode de publicité**                | Simple, peu flexible                                | Flexible, configurable                                                 |
| **Compatibilité**                    | Large compatibilité avec tous les périphériques BLE | Nécessite Bluetooth 5.0 ou supérieur                                   |
| **Idéal pour**                       | Applications simples avec peu de données            | Applications complexes avec plus de données ou des besoins spécifiques |

Dans notre cas on peut rester en *Legacy*.

**EventQueue vs Thread** :

| **Caractéristique**    | **`EventQueue`**                                                        | **`Thread`**                                                                |
| ---------------------- | ----------------------------------------------------------------------- | --------------------------------------------------------------------------- |
| **Nature**             | Gestion d'événements asynchrones, exécution différée                    | Exécution concurrente de tâches (multitâche)                                |
| **Multitâche réel**    | Non (utilise une seule boucle d'événements)                             | Oui (exécution de plusieurs threads en parallèle)                           |
| **Blocage**            | Non-bloquant, gère les événements sans bloquer le programme principal   | Peut être bloqué ou suspendu                                                |
| **Utilisation**        | Exécution d'événements ou de tâches à des moments précis ou périodiques | Tâches qui nécessitent un traitement parallèle, intensif ou de longue durée |
| **Simplicité**         | Facile à utiliser pour des tâches légères et asynchrones                | Plus complexe, nécessite la gestion de la synchronisation entre threads     |
| **Coût en ressources** | Moins coûteux en termes de mémoire et de ressources                     | Plus coûteux en ressources, nécessite plus de mémoire pour chaque thread    |

**`OK_main_temp_service.cpp`** contient le code fonctionne pour l'envoi de température en continu, maintenant je repars de ça pour créer un thread BLE et l'interrompre avec un bouton.

On arrive à lancer et couper la communication BLE avec le bouton. Maintenant le temps de `dispatch` doit être suffiamment long (2-3s), idéalement il faudrait pouvoir recevoir l'info que le client a lu les données pour faire le `event_queue.break_dispatch()` mais pour cela il faut modifier le callback `onDataRead` de `ble.gattServer()`, ce qui ne marche pas.

## Vendredi 20.06

Yann le GOAT a trouvé comment faire fonctionner le callback (il fallait un pointeur et non une référence comme argument). Cependant, cette technique n'est plus à jour et il est conseillé d'utiliser un `GattServer::EventHandler`.

Parfois la stack BLE se relance direct après l'avoir shutdown. Il faut vérifier comment vraiment bien la détruire et ajouter peut-être une variable `activateBLE` ou vérifier qu'on a pas eu de rebond avec le bouton pour ne pas reconstruire une instance par erreur.

Compléter la liste de UUID pour la publicité :

```c
UUID service_uuids[] = { _temperature_uuid, _ack_service.getUUID() }; // getUUID() doit retourner un UUID
_adv_data_builder.setLocalServiceList(mbed::make_Span(service_uuids, 2));
```

Il faut essayer de mettre en place ce `GattServer::EventHandler`, ça permettrait notamment d'être notifié lorsque le raspberry envoit une info (au lieu de lire en boucle comme un plouc).

Mettre au propre onDataWritten et les handlers.

## Samedi 21.06