# Semaine 27, du lundi 07.07 au dimanche 13.07 2025

## Résumé de la semaine

## Lundi 07.07

Avancement sur le code *Rainette*. Utilisation d'un singleton pour RainetteIO.

Il faut continuer le code *Rainette* en créant les 2 modes de fonctionnement.

Attention résoudre le problème de réf croisées avec `GattServerEventHandler::onDataWritten` et `RainetteApp::handleDataWritten`.

## Mardi 08.07

Implémentation de timeout et autres. Division par des données sur client.

Logique à vérifier.

## Mercredi 09.07

Code `rainette` presque fini, il faut encore terminé un coup. Les cartes et les composants sont arrivés, une 1e carte a été montée.
Pour le moment par de problèmes au niveau de l'alimentation.

Le connecteur *tag-connect* ne matchait pas avec le *ST-Link* donc liaisons recomposées avec des fils.

## Jeudi 10.07

L'USB fonctionne aussi. Début des modifications du code *disco* pour reprendre la forme du code *rainette* propre.

Peut-être un problème d'accès aux membres privés pour `handleDataWritten`, à creuser.. EN effet on ne peut pas accéder aux membres.

Ok, en utilisant un `event_flags` on peut contrer ce problème. Il faut encore résoudre le problème du singleton.

Vérifier la cohérence du code `rainette`.

## Vendredi 11.07

Code rainette compilé, `PB_0` et `PB_1` ne sont pas présents dans le fichier `PinNames.h` donc à voir lors de l'adaptation sur la nouvelle target.

FW de la carte *Rainette* programmé correctement avec *CubeProgrammer*, du premier coup ! 🎉🎉

Créer les fichiers de configs avec le script Python : https://github.com/ARMmbed/mbed-os/tree/master/targets/TARGET_STM#board-specific-files-pinmap

```bash
damien@LAPTOP-L2M9U4N6:/mnt/c/Users/Damien/Dropbox/PC/Documents/Code Rainette/Rainette/mbed-os$ python3 targets/TARGET_STM/tools/STM32_gen_PeripheralPins.py -m "STM32WB5MMGHx.xml"

Script version 1.20.5

Checking STM32_open_pin_data repo...
        Already up to date.

STM32_open_pin_data DB version STM32CubeMX-DB.6.0.90

 * Output directory: /mnt/c/Users/Damien/Dropbox/PC/Documents/Code Rainette/Rainette/mbed-os/TARGET_CUSTOM/TARGET_STM/TARGET_STM32WB/TARGET_STM32WB5MxG/TARGET_STM32WB5MMGH
 * Generating PeripheralPins.c and PinNames.h with 'STM32_open_pin_data/mcu/STM32WB5MMGHx.xml'
 * GPIO file: STM32_open_pin_data/mcu/IP/GPIO-STM32WB5Mx_gpio_v1_0_Modes.xml
 * I/O pins found: 73 connected: 1
```

Dans `PinNames.h`, on modifie les lignes suivantes pour indiquer les lignes RX et TX de la console, reliées ensuite au PC via l'USART.

```c
    // STDIO for console print
#ifdef MBED_CONF_TARGET_STDIO_UART_TX
    CONSOLE_TX = MBED_CONF_TARGET_STDIO_UART_TX,
#else
    CONSOLE_TX = PB_6,
#endif
#ifdef MBED_CONF_TARGET_STDIO_UART_RX
    CONSOLE_RX = MBED_CONF_TARGET_STDIO_UART_RX,
#else
    CONSOLE_RX = PB_7,
#endif
```

On arrive pas à flasher le programme avec OpenOCD mais on y arrive avec CubeProgrammer, en chargeant `Rainette.bin`.

Pour décompiler le code, on utilise la commande 

`arm-none-eabi-objdump -d Rainette_application.elf --start-address 0x801F800 --stop-address 0x801F8FF`

car l'erreur est la suivante et on on vise la gamme mémoire autour.

++ MbedOS Error Info ++
Error Status: 0x80FF0100 Code: 256 Module: 255
Error Message: Fatal Run-time error
Location: 0x801F84F
Error Value: 0x0
Current Thread: AppThread Id: 0x200047C0 Entry: 0x8024719 StackSize: 0x1000 StackMem: 0x20008020 SP: 0x20008E4C 
For more info, visit: https://mbed.com/s/error?error=0x80FF0100&tgt=RAINETTE
-- MbedOS Error Info --
ERROR booting WB controler

Il semblerait qu'il s'agisse d'un problème d'ordonnanceur (hypothèse seulement).

Symptômes à décrire à GPT (enchainement de breackpoint juste avant la lecture du sémaphore, mais pas le seul problème..).


Reset -> connect -> lâcher reset
1. FW FUS
2. Flasher avec CubeProgrammer (hypothèse)

## Samedi 12.07