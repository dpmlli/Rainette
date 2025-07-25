# Semaine 15, du lundi 14.04 au dimanche 20.04 2025

## Résumé de la semaine

## Lundi 14.04

## Mardi 15.04

Calcul de la consommation (p.86) 1 réveil chaque 10 minutes
- BLE : 10 sec @ 1mA -> moyenne = 16.6 uA (10/(10*60))
- Icore Sleep : 740 uA (à vérifier/valider...)
- Sondes ADC : 10 sec @ 3mA -> moyenne = 50 uA
- Load Switch : 3.6 uA * nbr_de_switchs (4) = 14.4 uA -> dépend de l'utilisation
- Battery tester : négligeable ?
- Leds Duty Cycle 20% 10sec à chaque 10 minutes : 2* 10mA *0.2 * 10/(10*60) = 66.6 uA
- Lumi sensor : 45 uA * 10/(10*60) < 1uA
- Temp sensor : 320 uA * 10/(10*60) = 4 uA
- LDO 34 uA

- GPIO pull ups : 40kOhm (p.133)

LDO choisi (300 mA pour être large)

Travail sur le routage vers le MCU. L'absence du prof m'a fait débuté le rapport sur MarkDown.

REFLECHIR SUR LES GND ANA ET NUM !!!!!

## Mercredi 16.04

## Jeudi 17.04

## Vendredi 18.04

## Samedi 19.04