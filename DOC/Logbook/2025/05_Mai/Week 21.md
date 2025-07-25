# Semaine 21, du lundi 26.05 au dimanche 01.06 2025

## Résumé de la semaine

## Lundi 26.05

## Mardi 27.05

Pour la bibliographie, utiliser ce format :
```latex
@online{1,
	author={Ferme},
	title = {La Ferme Aquaponique du Pays de Gex {\textbar} Versonnex},
	url = {https://www.lafermeaquaponique.com},
	urldate = {2025-03-25},
    shorthand = {FermeAqua2025}
}
```
et citer avec \cite{1}. Ca va afficher "FermeAqua2025" (shorthand)

Modification de la règle sur les freins thermiques : Design -> Rules -> Plane -> Polygon Connect Style

Update d'un footprint modifié : Clique droit -> Component Actions -> Update Selected Components From PCB Lib

En l'absence du prof, je commence à m'intéresser au code.

*bluepy* ou *bleak* pour communiquer en BLE sur le rapsberry.

| Élément   | `Timer`                           | `Ticker`                               |
| --------- | --------------------------------- | -------------------------------------- |
| Rôle      | Mesurer un temps écoulé           | Déclencher une fonction périodiquement |
| Style     | Contrôle manuel (`start`, `stop`) | Interruptions automatiques             |
| Wakeup    | Ne réveille pas                   | Peut réveiller le MCU                  |
| DeepSleep | ❌ (classique)                     | ✅ avec `LowPowerTicker`                |
| Exemple   | Chronométrer une opération        | Réveil périodique pour capteur         |

GATT (Generic Attribute Profile) est un protocole BLE qui définit comment deux appareils échangent des données selon un modèle client-serveur, structuré en services et caractéristiques. Le GattClient initie les requêtes pour lire ou écrire des données, tandis que le GattServer expose ces données et y répond. 

Eurocircuits : importer le zip des gerber. OubJob donné par Yann. 
On a récupéré les règles 7D en faisant un projet 7D Empty. Nouvelles règles -> redéfinir les règles sur les planes et les polygones. Modifier légèrement les vias. 

C/C++ tools..
Task Runner
Copy File Name
Cortex Debug

Il faut encore installer Mercurial, Mbed CLI et les requirements (voir lien Teams)

## Mercredi 28.05

## Jeudi 29.05

## Vendredi 30.05

## Samedi 31.05