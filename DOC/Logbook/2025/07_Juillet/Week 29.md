# Semaine 29, du lundi 21.07 au dimanche 27.07 2025

## Résumé de la semaine

## Lundi 21.07

Il fallait aussi activer le capteur de température lorsqu'on lit celui de luminosité. À présent on voit bien des trames I2C mais le résultat n'est toujours pas incroyable.

**I2C**
| Phase                  | Émetteur des données | Émetteur de l’ACK/NACK       |
| ---------------------- | -------------------- | -----------------------------|
| Adresse (R/W = 0 ou 1) | Maître               | Esclave                      |
| Écriture (write)       | Maître               | Esclave                      |
| Lecture (read)         | Esclave              | Maître (NACK -> fin de comm) |

Tous les acknowledges sont ok côté capteur mais il envoit systématiquement 0000, ce qui n'est pas cohérent. Le premier bit du registre de config devrait être par défaut à 1, hors ce n'est pas le cas ici. Du coup, pas possible d'activer quoique ce soit, l'ID est aussi à 0000..

**M0+**
- Pas d’extinction/réactivation directe du M0+ depuis le code principal.
- Seulement des modes globaux (Standby/Shutdown) ou vérification d’état via les flags.

**Lib SparkFun capteur lumi** : https://github.com/sparkfun/SparkFun_Ambient_Light_Sensor_Arduino_Library/tree/master

## Mardi 22.07

La consommation sur pile est en fait bien inférieur, il fallait retirer le port série, qui consommait beaucoup.

Si on a encore un peu de temps en  fin de semaine, il faudrait faire ces tests **brute force** pour le capteur de température.

## Mercredi 23.07

## Jeudi 24.07

## Vendredi 25.07

## Samedi 26.07