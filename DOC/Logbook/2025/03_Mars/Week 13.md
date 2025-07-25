# Semaine 13, du lundi 31.03 au dimanche 06.04 2025

## Résumé de la semaine

## Lundi 31.03

## Mardi 01.04

Mesure de la tension de la pile : on pourrait envisager un montage à ampli-op pour mesurer la tension dans la bonne plage, cela éviterait de tirer trop de courant sur la pile avec un pont diviseur. Selon Wikipédia et GPT, pas de montage "classique" pour un gain positif et inférieur à 1. Il faudrait un pont diviseur avec un suiveur. Euh ⚠️attention⚠️ la tension d'entrée du suiveur doit être comprise dans la plage de l'alimentation ! Donc ça ne fonctionnerait pas si on met direct la tension de la pile en entrée.. 

Tout comme les disco-kit, on pourrait aussi simplement utiliser un jumper pour sélectionner la source d'alimentation (pile ou filaire) car les deux options ne sont pas amenées à se présenter en même temps.

Pour les relais, je ne trouve pas pertinent d'utiliser un gros relais pour driver du 230V, on va rester sur des **relais MOSFET** avec des tensions de charge "raisonnables".

Pour les régulateurs de tensions, on pourrait imaginer 3 LDO : 1 (3V3?) pour les éléments activés en permance (mode sleep), 1 3V3 pour les éléments activés pour les mesures ponctuels et 1 5V pour applications alimentées par fil. Les 2 derniers seraient (dés)activables pour les périodes de sommeil.

**Discussion avec M. Favrat** :
- Logic level shifter pas nécessaire, y'a pas vraiment d'interface 5V.
- Au lieu d'utiliser le régulateur 3V3 activable, utiliser des power switchs (schéma fourni) pour contrôler chaque élément.
- Régulateur 5V pas nécessaire à priori, pas d'usage.
- Mesure de la pile : pas besoin de suiveur, schéma fourni avec 2 MOSFETs et un pont diviseur.

⚠️ Important ⚠️ Il faut quand même vérifier que l'interface de chaque capteur ADC ne demande pas de 5V mais fonctionne avec du 3V3.

Toutes les interfaces de Atlas Scientific fonctionnent en 3V3. Les flotteurs sont de simples switchs, donc à priori pas de problème avec du 3V3.

Prévoir des protections de polarité.

Lire *Applied Embedded Electronics* pour trouver des points importants de la conception de carte.

Pourquoi pas ajouter un capteur de luminosité sur la carte (réf. intéressante trouvée).

Planification mise à jour.

Création d'une checklist pour robustesse du design électronique.

Module STM32 : pas besoin de quartz externe.

Travail et prise de notes sur Altium (librairies, sheet symbols,..)

## Mercredi 02.04

## Jeudi 03.04

## Vendredi 04.04

Est-ce qu'on met un step-down pour l'alim filaire ? Oui avec un autre connecteur que le 5V. Guide optique pour capteur de luminosité et leds.

Footprint du capteur de temp. modifié.

Trouver un nom, genre Frogy ou Rainette.

## Samedi 05.04