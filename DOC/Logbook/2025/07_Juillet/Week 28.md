# Semaine 28, du lundi 14.07 au dimanche 20.07 2025

## Résumé de la semaine

## Lundi 14.07

Ca refonctionne pour la carte `disco` en appuyant sur le bouton *Start Wireless Stack* dans le FUS de STMCubeProgrammer.
On peut par la suite reflasher depuis *OpenOCD*.

Il faudrait recréer une autre config pour custom target pour la disco afin d'y déposer les codes pour ne pas devoir couper-coller les nouveaux codes à chaque fois.

La programmation fonctionne en respectant les étapes suivantes :
1. Connect
2. Upgrade firmware
3. Read FUS info et Start Wireless Stack
4. Flasher le binaire avec CubeProgrammer

Il faut encore débugger le capteur de lumi et l'ADC batterie (facteur correctif ?). On peut essayer d'ajouter le pull-up dans le MCU pour voir.

## Mardi 15.07

Pour la mesure de batterie, les valeurs des résistances sont bonnes, la Zener est dans le bon sens. 

**Mesure *battMeas***
1V -> 0V
1.5V -> 0.712V
2V -> 1.33V
2.5V -> 1.5V
3V -> 1.62V
3.5 -> 1.7V
4V -> 1.77V
4.5V -> 1.83V

**Mesure sortie de transistor**
1V -> 0V
1.5V -> 0.9V
2V -> 1.87V
2.5V -> 2.36V
3V -> 2.84V
3.5 -> 3.3V
4V -> 3.82V
4.5V -> 4.32V

Ces mesures ont permis la création du fichier `sim_fact_corr.ipynb` qui permet de passer des valeurs mesurées aux valeurs attendues en appliquant un facteur correctif.

Coefficients du polynôme (x = a*y² + b*y + c):
a = 5.187502, b = -11.389900, c = 7.911509 

Sondes OK
GPIO OK
Lumi toujours nul malgré les pull ups ajoutées
ADC ajusté donc OK

La connexion GAP est drôlement plus merdique avec la carte Rainette, il faut inspecter les différences pour trouver la cause.

Il faut regarder le flotteur encore et les connexions GAP sont encore bien trop aléatoires..

## Mercredi 16.07

Sur la 2e carte montée sans diode Zener pour la mesure de la batterie, le pont diviseur fonctionne parfaitement, c'est donc la diode qui introduit ce problème.

Bon vraiment je pige pas le problème avec lumi..

Dans l'état, on a une conso moyenne de 7mA, même au repos, ce qui est largement trop. Faut trouver comment activer ce foutu mode *DeepSleep*. Il y a un pic à 680mA lors de la mesure !!

`Deep sleep allowed: No`

Bon faut contrôler ce ***** de port série afin de checker le mode deepsleep et vérifier pourquoi on a ce pic de courant de con. 

## Jeudi 17.07

## Vendredi 18.07

**Yann** : 
- Le comportement peut être attendu, il faudrait diminuer les valeurs de résistances du pont diviseur pour que l'impédance de la diode soit vraiment supérieure au pont diviseur.
- Il faudrait gicler le condo 10uF du capteur de luminosité car il est surement responsable du maxi pic et c'est inutile car il est activé pendant un court instant. Il faudrait également vérifier les signaux SDA et SCL à l'oscillo.
- Bastien si connait un peu en mode *deepsleep* et **il faut utiliser le mode RELEASE**.

Si on mesure avec le PPK2 sur la place du jumper, on obtient aussi 7mA donc le problème ne vient pas de ce qu'il y a avant.

L'ajout de `"MBED_SLEEP_TRACING_ENABLED"` dans mbed_app.json permet de voir ce qui lock le deepsleep

Ex :
`deepsleep locked by: [lp_ticker.c x 1] [i2c_api.c x 1] [SysTimer.cpp x 1]`
`deepsleep unlocked`

Le fait d'instancier un bus i2c peut bloquer le mode deepsleep, il faudrait uniquement le créer lorsque l'on s'apprête à faire des mesures.

```c++
I2C* myi2c = new I2C(SDA,SCL);

myi2c->read();
myi2c->write();

delete myi2c;
```

C'était en effet la bonne chose à faire pour débloquer le *deepsleep*, maintenant l'i2c est présent uniquement au moment de la mesure.

Il faut tester la conso avec un code minimal. On peut aussi créer une fonction dans le main qu'on appelle en `extern`
dans `wb_sleep.c` pour toggle une led lorsqu'on rentre dans le mode deepsleep.

Dans `targets`->`TARGET_STM`->`TARGET_STM32WB`, le dossier `STMCube_FW` contient la **HAL ST**, elle n'est pas modifiable. Les fichiers c/h sont les fichiers écrits par Mbed pour employer la HAL sur la target, on peut les 
modifier. Le fichier `wb_sleep.c` concerne le deepsleep, on doit voir quel mode est activé (StopMode,..).

C'est mieux, la haute consommation était due au câble relié au ST-Link, il faut le retirer et faire un *reset*.

| Mode HAL              | Économie        | Horloge système | RAM sauvegardée | Consommation typique |
|-----------------------|-----------------|-----------------|-----------------|----------------------|
|`PWR_EnterSleepMode()` | Faible          | Oui             | Oui             | \~1 mA typiquement   |
|`PWR_EnterStopMode()`  | Moyen           | Non             | Oui             | \~0.3–1 mA           |
|`PWR_EnterOffMode()`   | Élevé (Standby) | Non             | Non             | \~1 µA typiquement   |

**Constatations** :
- init : DeepSleep dispo -> conso 5mA
- 1e attente : DeepSleep dispo -> conso 5mA
- 2e attente : DeepSleep plus dispo

La stack BLE pourrait être responsable, peut-être faudrait-il regarder le coprocesseur M0.

Liste non-exaustive des périphériques qui bloquent le mode *deep sleep* : 
    [timer, ticker, timeout, SPI, I2C, CAN, PWM, Serial, USB]

* Par défaut (sans `MBED_TICKLESS`)
    - Le systicker (SysTick, fréquence 1 kHz en général) génère une interruption toutes les millisecondes.
    - Cela empêche l’entrée en deep sleep (Stop Mode), car le timer système réveille constamment le CPU.
    - Consommation énergétique relativement élevée, même en veille.

* Avec `MBED_TICKLESS`
    - Le RTOS désactive le systicker périodique.
    - À la place, il programme un wakeup unique avec un Low Power Timer pour le prochain événement planifié.
    - Cela permet d’entrer automatiquement en deep sleep (Stop Mode) dès que possible.
    - Réduit significativement la consommation énergétique si le système passe souvent en veille.

Selon GPT :

- Faire `delete` sur une instance BLE allouée dynamiquement ne garantit pas l'arrêt du coprocesseur M0+.
- Avec `stm32wb5x_BLE_HCILayer_fw.bin`, on gère la pile BLE en Mbed/Cordio côté *M4*, et le M0+ joue le rôle de
  contrôleur HCI BLE pur. Il n’interprète aucun appel SHCI car le firmware minimal HCI ne contient pas le support SHCI (System HCI).

**SHCI**: *System Host Controller Interface*. Protocole d’extension propriétaire de ST qui permet au M4 de :
- Commander des transitions d’alimentation du M0+.
- Initialiser/désinitialiser les stacks sans fil (BLE, Thread, Zigbee).
- Reconfigurer le firmware radio sans redémarrer.

| Terme                       | Signification                    | Rôle principal               |
| --------------------------- | -------------------------------- | ---------------------------- |
| RCC                         | Reset and Clock Control          | Gérer les horloges et resets |
| IPCC                        | Inter-Processor Comm. Controller | Comm M4 ↔ M0+                |
| HSEM                        | Hardware Semaphore               | Mutex entre M4 et M0+        |
| `__HAL_RCC_*_CLK_DISABLE()` | Macro HAL                        | Coupe l’horloge d’un domaine |

Solutions pour éteindre le M0+ : 
1. Couper explicitement le domaine réseau en passant par les HAL STM32Cube du projet.
2. Changer de firmware pour bénéficier de SHCI en utilisant une version *full*.

## Samedi 19.07