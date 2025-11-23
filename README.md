# MarsRover


Ce dépôt contient une application client/serveur ("Rover" / "Mission Control") écrite en C++. Le client (rover) simule un robot qui explore une grille, le serveur (mission_control) reçoit les positions découvertes et commande le rover.

## Prise en charge

- Linux
- Sur Windows, utilisez WSL (Windows Subsystem for Linux) ou une VM Linux pour une compatibilité correcte.

## Prérequis

- make (GNU make)
- g++ compatible C++20

## Compilation

Depuis la racine du projet, lancez :
```bash
# compile serveur et client
make all

# ou compiler uniquement le rover
make rover

# ou compiler uniquement la mission control
make mission
```

Les exécutables produits seront :
- `rover/rover`
- `mission_control/mission_control`

## Exécution

1. Démarrez d'abord le serveur `mission_control` en renseignant un port :
   
```bash
./mission_control/mission_control [PORT]
```

2. Dans un autre terminal, lancez le rover en fournissant : port, adresse du serveur, largeur et hauteur de la planète :

```bash
./rover/rover [PORT] [ADRESSE IP] [LONGUEUR] [LARGEUR]
```

3. Interaction :
- Le serveur affiche la carte inconnue initiale et demande des commandes via le prompt.
- Entrez une suite de commandes composée des caractères : `F` (Forward), `B` (Backward), `L` (Left), `R` (Right).
- Le serveur envoie la commande au rover, reçoit les cases découvertes et met à jour l'affichage.
- Pour quitter proprement, saisir `exit` au prompt du serveur.

## Architecture
<img width="1448" height="1164" alt="image" src="https://github.com/user-attachments/assets/e79e4681-2037-46f1-8e5e-834e11570654" />

## Argumentation révisée

Après simplification des interfaces de communication, les modules Rover et MissionControl ne conservent chacun qu’un point d’entrée unique via leur méthode Main(). Le module Map regroupe désormais l’ensemble des éléments liés à la cartographie, Tile et Orientation. Cette réorganisation entraîne l’ajout d’une dépendance du module Communication vers Map, notamment pour la manipulation des données nécessaires à la sérialisation des paquets.

Dépendances
Rover et MissionControl dépendent du module Communication.
Pour pouvoir lancer les programmes en tant que serveur et client, les deux modules utilisent les fonctionnalités fournies par UnixSocket, qui implémente l'interface ISocket. Cette dernière a été conservée malgré la suppression de l’ancienne implémentation WinSocket, pour préserver une structure extensible.
Les paquets échangés transitent via le module Packet, qui fournit les méthodes de sérialisation et désérialisation utilisées directement dans Send() et Receive() de UnixSocket.

Rover et MissionControl dépendent aussi du module Map.
Dans leurs méthodes Main(), chacun crée une instance de Planet :
- côté Rover, elle contient la carte réelle ;
- côté MissionControl, elle est initialisée avec des valeurs UNKNOWN (X) et se met à jour au fur et à mesure des déplacements du rover.
La progression du rover est une synchronisation de la carte côté MissionControl en fonction des informations reçues.

Contraintes

Rover et MissionControl doivent rester compatibles en fonction des évolutions futures des modules Communication et Map. Toute modification dans la structure des paquets, l’API des sockets ou la représentation de la carte sera intégrée dans leurs logiques respectives donc.
