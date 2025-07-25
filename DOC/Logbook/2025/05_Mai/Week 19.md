# Semaine 19, du lundi 12.05 au dimanche 18.05 2025

## Résumé de la semaine

## Lundi 12.05

## Mardi 13.05

À priori, seuls le MCU et le DC/DC converter 3V3 possèdent une recommandation de layout.

Aïe, le datasheet du STM32 recommande de laisser une large surface de plan de masse à droite et de passer les signaux en bas.
Ca impliquerait de tout recommencer et d'utiliser d'autres vias.

Après discussion avec M. Favrat, on va juste passer les pistes de droite sur le bottom pour laisser un plan de masse sur le top. On peut garder 
le ST-Link à commander.

Le convertisseur DC/DC oscille autour des 1.5 MHz donc on ne risque pas de perturber l'ADC pour mesurera à des fréquences inférieures. On peut en revanche
élaborer des filtres RC pour couper les hautes fréquences (faut juste estimer la fréquence de l'ADC). Pour ça, on rajoute une résistance pour bien définir la valeur de tau.

Routage des signaux, c'est pas mal mais il faut continuer en disposant les composants et la sériegraphie.

## Mercredi 14.05

## Jeudi 15.05

## Vendredi 16.05

Gamme min de tension analogique à lire : 0-420mV (DO) et 1.058-1.3 (temp)
L'ADC possède 12 bits de résolution -> pas de 805uV donc suffisant selon moi sans devoir ajuster Vref+.


## Samedi 17.05