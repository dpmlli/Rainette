# Semaine 12, du lundi 24.03 au dimanche 30.03 2025

## Résumé de la semaine

## Lundi 24.03

Semaine passée : CRUNCH. Visite d'une ferme aquaponique https://www.lafermeaquaponique.com/. Il en ressort que la plupart des capteurs (pH,DO,..) ne sont pas utiles sur le module, même pour les grandes installations. Peut-être faut il se contenter d'une sonde de température (PT1000 - https://files.atlas-scientific.com/PT-1000-probe.pdf). 

Réflexions sur la manière de lire une PT1000, contraintes de faible conso et de dynamique. Il faut étudier les montages différentiels étudiés en cours de Capteurs. Finalement, pas besoin de développer l'interface pour les ADC, on s'était mal compris sur le cahier des charges et le but du projet.

Commande de kits STM32WB5MM-DK pour tester la communication. Ce même kit est compatible ARM Mbed (https://os.mbed.com/platforms/DISCO-WB5MMG/). Recommandation de M. Favrat : partir du fichier Altium du Dev kit pour ne pas perdre trop de temps.

**Puissance sur la carte** Tension de fonctionnement du STM32 : entre 1V8 et 3V3

**LTC4412** Encore disponible mais cher.. 

Découverte de **SnapMagicSearch** : Librairie de composants gratuite.

Schémas ESD et fonctionnel sur draw.io

**STM32WB5MMG** Tableau 84 : Tension Vin analogique {0-VDDA} donc il faut prévoir des protections sur l'ADC (<3V3). Les interfaces Atlas Scientific fournissent une tension analogique de max 3V donc c'est bon pour cette usage mais attention aux autres capteurs !

## Mardi 25.03

## Mercredi 26.03

## Jeudi 27.03

Version 0 du cahier des charges transmis au professeur. Il faut ajouter le Gantt mais j'ai pas réussi à l'intégrer dans Latex.

**LTC4412** validé par un assistant. Faut pas s'emmerder pour un TB au niveau du prix.
Lien MOUSER (https://www.mouser.ch/ProductDetail/Analog-Devices/LTC4412IS6TRPBF?qs=sGAEpiMZZMvYc73DDAmzVmF2fNtS0fN6z4ClOUbNujH5nRULcINweA%3D%3D)
Autrement, un MOSFET mais c'est moins solide. 

Il faut se poser la question si c'est pertinent d'avoir un fuel gauge avec une pile. Après discussion avec un assistant, ce n'est pas le cas. On peut
relier la pile à l'ADC (attention à la plage min et max, idée : pont diviseur avec MOSFET). Les piles en série sont sensées se décharger en même temps.
Attention aussi à la référence de tension de l'ADC, il faut checker qu'il y ait un réf interne. Pour l'ADC, on utilisera le mode "single" car le mode
"différentiel" est utile contre le bruit mais là c'est en ordre.

Essais avec Altium, début des différentes sheets du projet.

Il faut remplir un coup le rapport.

Finalement, a-t-on vraiment besoin du LTC4412 ? C'est soit l'un soit l'autre donc on peut imaginer plus simple.

Comment merge plusieurs pdf avec LaTex : https://tex.stackexchange.com/questions/8662/merge-two-pdf-files-output-by-latex

## Vendredi 28.03

## Samedi 29.03