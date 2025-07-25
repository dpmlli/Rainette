# Semaine 25, du lundi 23.06 au dimanche 29.06 2025

## Résumé de la semaine

## Lundi 23.06

Début du travail sur le *DeepSleep*.

Un *Ticker* c'est comme un *Timeout* mais qui se répète périodiquement.

Dans le json, j'ai ajouté la macro `"MBED_TICKLESS"` et mis à `false` le paramètre `platform.stdio-buffered-serial`. À présent, le DeepSleep est lancé lorsqu'on dort `ThisThread::sleep_for(1s);`. Le fait de *locker* ce mode empêche le MCU d'y rentrer, cela ne nous contraint pas à y rester. Le snippet suivant montre bien cet effet :

```c
printf("Deep sleep allowed: %i\r\n", sleep_manager_can_deep_sleep()); // 1
ThisThread::sleep_for(1000);

// Lock deep sleep
printf("Locking deep sleep\r\n");
sleep_manager_lock_deep_sleep();

printf("Deep sleep allowed: %i\r\n", sleep_manager_can_deep_sleep()); // 0
ThisThread::sleep_for(1000);
```

Dans notre cas, faire `ThisThread::sleep_for(10min);` peut parfaitement faire l'affaire, pas besoin d'un ticker car lors du sommeil on attend aucun événement.

On peut essayer `constexpr auto SLEEP_TIME = 10min;`

#### Découpage des fichiers

* main.cpp

* services.h - services.cpp

* ble.h - ble.cpp

* utils.h - utils.cpp

#### Services et caractéristiques

**General**
* BattDetect
* StartMeas
* EndOfComm
* PingPong

**Analog**
* Probe0
* Probe1
* Lumi
* Temp
* Humi
* Batt

**Digital**
* Float
* MosRel0
* MosRel1
* Gpio0
* Gpio1
* Gpio2
* Gpio3
* Gpio4
* Button

**Cas ReadOnly**
- Le client lit manuellement la température chaque seconde.
- Tu déclares la caractéristique avec seulement GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ.

**Cas Read + Notify**
- Le client s’abonne aux notifications.
- Le serveur appelle ble.gattServer().notify(...) à chaque mise à jour de température.
- Le client reçoit les nouvelles valeurs en temps réel, sans polling.

Création des fichiers `rainette_services.cpp` et `rainette_services.h`, il faut encore corriger les incohérences de types (bool <-> uint8_t).

## Mardi 24.06

Rédaction du rapport.

## Mercredi 25.06

Rédaction du rapport et amélioration des classes de services en ajoutant notamment des setter.

| Terme       | Nature      | Rôle principal                           | Exemple typique           |
| ----------- | ----------- | ---------------------------------------- | ------------------------- |
| **handle**  | Identifiant | Référence indirecte à une ressource      | `GattAttribute::Handle_t` |
| **handler** | Fonction    | Réagit à un événement, traite une action | `onConnectionHandler()`   |

Création de `rainette_event_handlers.cpp` et `rainette_event_handlers.h`. Maintenant on peut voir chaque caractéristique.

## Jeudi 26.06

## Vendredi 27.06

Attention faut pas stocker l'état des I/O dans la stack BLE car en cas de problème ou de shutdown pour perd tout.

Problème avec le ping-pong test !

Vérifier les erreurs pour la GUI.

## Samedi 28.06