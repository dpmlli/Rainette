# Semaine 18, du lundi 05.05 au dimanche 11.05 2025

## Résumé de la semaine

## Lundi 05.05

## Mardi 06.05

On peut modifier les règles (Error, Warning,..) dans Project -> Projet Options -> Error reporting.

J'ai passé les Off grid restantes en No report après avoir modifié le symbole des TVS.
J'ai également retiré l'erreur qui indique les multiples noms, mais ça ne permet pas pour autant de régler le problème du GND qui n'apparait pas sur le PCB.

Problème des GND à priori résolu. Il fallait aller dans Project -> Project options -> Options et dans `Net Identifier Scope` passer de `Strict Hierarchical` à `Hierarchical`. 

| Mode                    | Ports & Sheet Entries obligatoires | Power Ports globaux | Net Labels globaux | 
| ----------------------- | ---------------------------------- | ------------------- | ------------------ | 
| **Hierarchical**        | ✅ Oui                             | ✅ Oui              | ❌ Non            | 
| **Strict Hierarchical** | ✅ Oui                             | ❌ Non              | ❌ Non            | 
| **Global**              | ❌ Non                             | ✅ Oui              | ✅ Oui            | 

Un premier Design Rules Check relève plusieures erreurs, notamment la hauteur du 3D body du connecteur STLink. Il y a surtout des erreurs pour le `Minimum Solder Mask Sliver` ce qui implique de vérifier avec Eurocircuits. 

Finalement, après discussion avec le prof, ce n'est pas à faire directement. Il faut en revanche remplacer le footprint du connecteur USB pour prendre du SMD. 

On peut concevoir de nouveaux vias dans `Design` -> `Layer Stack Manager` -> `Via Types`, mais ce n'est pas comme ça qu'on va le faire.
On crée une bibliothèque de via dans File -> New -> Library -> Via. On peut ensuite faire clique droit sur la fenêtre et ajouter un via, puis le dimensionner.

On a récupéré le projet Altium du Disco Kit, les vias du côté du module sont des blind1:2 -> classe de prix plus élevée. On essaie de faire sans.

Il faut mettre les signaux sur la périphérie du module pour éviter ces vias.

Pas de blind dispo pour le prototypage Eurocircuits : https://www.eurocircuits.com/services/pcb-proto/.

Pour mettre le maximum de signaux sur les bords, `EN_LUMI_SENS` et `EN_TEMP_SENS` sont déplacés des pattes PC8 et PC9 aux pattes PB0 et PB1. Les signaux `GPIO_FLOAT_OUT` et `GPIO_FLOAT_IN` sont déplacés des pattes PD2 et PD3 aux pattes PB10 et PB11. Il ne reste plus que le GND patte 84.

Les pattes de communication ont été validées avec STMCubeMX, les pattes SPI ne sont pas les premières proposées, il faut modifier le routage manuellement.

Changement de connecteur USB-C pour utiliser du SMD. 

Simulation de Vadc(Vbatt)

Ajout de diode Zener 3.6V pour protéger l'ADC. 

Dimensionnement des résistances pour la mesure de la batterie.

## Mercredi 07.05

## Jeudi 08.05

Architecture proposée par GPT (pas oublier la gestion du Sleep et des différents paramétrages )

```
/root_project/
│
├── main.cpp                        <-- Point d'entrée principal
├── mbed_app.json                  <-- Configuration spécifique (ex: BLE, logs, traces, RTOS)
│
├── /include/
│   ├── I2CDeviceManager.h         <-- Interface pour gérer des périphériques I2C
│   ├── BLEManager.h               <-- Interface BLE (GATT, pub, sub)
│   ├── GPIOManager.h              <-- Contrôle GPIO (entrée/sortie)
│   ├── ADCReader.h                <-- Gestion des entrées analogiques
│
├── /src/
│   ├── I2CDeviceManager.cpp       <-- Implémentation des capteurs I2C (ex: TMP102, MPU6050)
│   ├── BLEManager.cpp             <-- Implémentation BLE (serveur/client GATT)
│   ├── GPIOManager.cpp            <-- Implémentation GPIO
│   ├── ADCReader.cpp              <-- Implémentation ADC
│
├── /services/
│   └── AppController.cpp          <-- Logique métier : coordonne les modules ci-dessus
│
├── /events/
│   └── EventQueueHandler.cpp      <-- EventQueue/Thread (pour décharger le main)
│
└── /utils/
    └── Logger.h/.cpp              <-- Utilitaire de debug/log simple (printf ou BLE logs)
```

## Vendredi 09.05

Discussion avec M. Favrat, Hochet et Charbon :
- Pas de cuivre sous l'antenne ! Il faut donc remanier les pistes.
- Ajouter au moins l'UART pour afficher des éléments sur la console.
- On peut éditer le via dans le PvLib en décochant *From Rule* -> *Relief* à *Direct* pour éviter les freins thermiques.
- PCBway : Instant Quote, même prix avec ces choix 
    - Piste/espacement minimum : 4/4 mil = 0,1016 mm / 0,1016 mm
    - Taille de trou minimum : 0,2 mm
- Tag Connect : Utiliser la référence du laboratoire pour ne pas devoir racheter un nouveau connecteur.
- Dans le *Layer Stack Manager*, on peut définir les couches : *Signal* -> le cuivre est en couleur, *Plane* -> le cuivre est invisible.
- Shift + S : isoler la couche sélectionnée.
- Placer des headers GND et VCC pour dépanner.

Il faut vérifier les positionnements décrits dans les datasheets (MCU, IC,..)

## Samedi 10.05

À mettre dans le glossaire :
- PCB = objet physique.
- Layout = fichier de conception (ou étape dans la conception).