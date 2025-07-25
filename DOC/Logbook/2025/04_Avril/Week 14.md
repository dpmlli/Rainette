# Semaine 14, du lundi 07.04 au dimanche 13.04 2025

## Résumé de la semaine

## Lundi 07.04

## Mardi 08.04

Réception des Disco-kit pour le STM32WB5MMG. Le module est vraiment compact, il ne se brase pas à la main. 

Nom trouvé pour le projet : Rai**net**te

Justification de la diversité des éléments présents sur la carte : pour faire des essais et déterminer ce qui est pertinent en vrai et ce qui ne l'est pas.

Connecteurs 1729128 de Phoenix contact.

**REGULATEUR 1V8 ???** -> non pas besoin, c'est géré en interne.

Calcul de la consommation (p.86) 1 réveil chaque 10 minutes
- BLE : 10 sec @ 1mA -> moyenne = 16.6 uA (10/(10*60))
- Sondes ADC : 10 sec @ 3mA -> moyenne = 50 uA
- 

Installation du SamacSys Loader pour les footprints Altium.

`Ctrl + M` pour les mesures sur `.pcblib`

Footprint du connecteur 1729128 vérifié. Pareil pour le TSR_1-2450E.

Je choisis le G3VM-61VY3 pour piloter de grandes tensions car moins cher et 60V @ 700mA.

Il faut discuter des leds avec M. Favrat car grosse conso..

Tous les composants ajoutés aujourd'hui (y compris capteur de luminosité) ont été vérifiés.

## Mercredi 09.04

## Jeudi 10.04

## Vendredi 11.04

On continue, il faudrait pouvoir acheminer toutes les connexions requises vers le module aujourd'hui.

Pins LPUART et SPI ? -> plutôt USART

Connector ST-Link : Tag-connect "Connect the TC2050-IDC(-NL)-050-STDC14 to the ST-LINK V3 SET via the STDC14 connector."
Version "legged" : https://www.tag-connect.com/product/tc2050-idc-050-stdc14. 
Le STDC14 est le connecteur du ST-Link V3set. Info sur Datasheet St-Link tableau 8.1.2.

Sur CubeMX on trouve pas direct le SWD, il faut aller dans GPIO -> debug et sélectionner comme on veut. Le datasheet du STM32WB55xx indique que les pattes SW sont PA13 et PA14, elles sont déjà inclues avec le JTAG 5 pins.

Avant on avait les leds :
```
BLUE : Vf = 3V, If = 20mA.
Rb = (3.3-Vf)/If = 15 -> 18 Ohm

RED : Vf = 2V, If = 20mA.
Rr = (3.3-Vf)/If = 65 -> 68 Ohm
```
*If* trop important, on regarde le schéma du DK et on va abaisser *If*. 

Je retire la led bleue car la rouge consomme moins. On met *If* = 10mA => R = 120 Ohm

Les pull-up sont à mettre au centre, sans switchs pour l'alim.

Il faut terminer l'estimation de consommation pour le LDO.

## Samedi 12.04