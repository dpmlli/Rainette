# Semaine 17, du lundi 28.04 au dimanche 04.05 2025

## Résumé de la semaine

## Lundi 28.04

## Mardi 29.04

Discussion avec M. Favrat
- utiliser un DC/DC au lieu du LDO car mauvaise performance sur batterie, ajouter une inductance
- 10uF autour du DC/DC, utile pour le reste si éloigné, 100nF seulement autour des IC mais pas du DC/DC, 2 condos en parallèle pour ESR
- Schottky OK pour batterie, diode simple pour High DC. 
- TVS Bidirectionnelles avec seuil adapté et attention ESR, Normes IEC serres !!
- Transistors back to back : c'est un seul composant.
- STM32 : 100nF pour chaque patte sauf 4 et 6 regroupées
- on suppprime la résistance de pull up du capteur de luminosité car il y en a déjà à dispo sur la GPIO.
- pas de protections sur les optocoupleurs
- calculer les pull up de l'I2C avec RC
- SPI et USART limités en usage externes (exemple de la Flash SPI), il faut calculer et limiter le baud rate (1 MHz)
- Pas de protections pour le ST Link
-Définir si pull up pour le SPI

Sur la fin, mise en place des TVS 3V3.

## Mercredi 30.04

## Jeudi 01.05

## Vendredi 02.05

Selon le datasheet du STM32WB55XX, pas besoin de pull up externe sur le signal NSS (Slave Select) car on peut congigurer une pull up interne. Je supprime donc la résistance initialement placée.

Renommez les composants : 
...

Importer les composants du schéma au PCB :
...

Les TVS comportent des erreurs car le symbôle est *off grid*.

Pour le rapport, on met juste le *résultat final* pas les 15 étapes qui précèdent.

Début du placement des composants sur le PCB.

## Samedi 03.05