# Semaine 26, du lundi 30.06 au dimanche 06.07 2025

## Résumé de la semaine

## Lundi 30.06

Pas de pull-up/down sur les ADC mais pour la carte Rainette ça devrait le faire grâce au diviseur résistif.

On a droit à des spécialités avec la communication BLE, le client se connecte mais pas de nouveaux logs sur le serveur.

Création des fichiers `Disco` pour développer une app faite pour la carte Disco dans le cas où on aurait un pépin avec Rainette
pour la démo.

Il faut demander à Yann comment faire pour les event_flags et pour les `sleep_for` asynchrones. Comment faire pour envoyer une dernière trame depuis le serveur avant de couper la communication ?

## Mardi 01.07

Discussion avec Yann : classe BLEManagment pour gérer le tout.

Raspberry Pi OS installé sur une carte SD. Problème de formatage reglé avec diskpart :

```
select disk X     (remplacer X par le numéro de la carte SD)
clean
create partition primary
format fs=fat32 quick
assign
exit
```
Le code GUI posait problème car plusieurs boucles tournaient en même temps (toléré sur Windows mais pas sur Linux).
La nouvelle version corrige cela est le code fonctionne bien sur le Raspberry.

Il faut une partie `Mise en service et tests` dans le rapport.

Problème avec `schedule_ble_events` : soit membre de la classe mais il faut une fonction libre, soit event_queue hors de portée. Peut être faut il redéclarer event_queue dehors.. À voir avec Yann.

## Mercredi 02.07

Visite YOTAVIS

## Jeudi 03.07

Discussion avec Yann : 

* Rapport :
    - Pas trop d'exemples inutiles
    - Schémas d'architecture
    - Expliquer les points compliqués avec du code
    - GitHub pour le code, pas en dur dans les annexes
    - Expliquer les points à améliorer (découpage pas incr)

* Code :
    - Pas mélanger BLE avec applicatif
    - *Singleton* pour `BLEManagement` (mais un poussé, je préfère déclarer `event_queue` dehors)
    - Instance de `I2C` dans le main, puis `extern` dans `utils.h` pour créer les *callbacks* à cet endroit

On a des problèmes avec les resets, il faut encore explorer mais la méthode crée `reset()` pose problème dès la première communication si elle est évoquée avant `run()`.

## Vendredi 04.07

## Samedi 05.07