# Semaine 20, du lundi 19.05 au dimanche 25.05 2025

## Résumé de la semaine

## Lundi 19.05

## Mardi 20.05

#### Dimensionnement des résistances de pull-up

Longueur max de SCL : 43 mm
Longueur max de SDA : 44 mm


Demander
- Trous de fixation
- I2C pull up
- Plan de masse (polygon)
- Design rules

Connecter pattes à la masse
FR4 epoxy
0201 100nF non ok

Mettre les pattes en ANALOG pour passer avec la masse.
10k OK pour pull up

## Mercredi 21.05

## Jeudi 22.05

## Vendredi 23.05

M2 -> M2.5 min (idéalement M3)
Plan de masse sur tout le *Top*
Pas de GND sur GPIO (si vrm, VCC mais pas GND)
Connecter les condos sur schéma
Pas de freins thermiques sur les vias de stitching
On a réduit la taille des pads du module.
On peut laisser les pistes des GPIO sur le *Bottom*

Revoir l'estimation du courant, avec les différents modes du MCU
*Icore* plus grand en vitesse de croisière

Faut regarder sur GitHub/VSCode ce que font les modes SLEEP, DEEP SLEEP, .. (ouvrir avec CodeSpace)

Longueur de piste/plan < lambda/4 ou moins. Plus c'est petit et moins on atteint le sommet de la sinusoïde (explication Yann).

Il faut encore mettre des vias de stitching

## Samedi 24.05