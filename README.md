# MarsRover

Ce dépôt contient une application client/serveur ("Rover" / "Mission Control") écrite en C++. Le client (rover) simule un robot qui explore une grille, le serveur (mission_control) reçoit les positions découvertes et commande le rover.


## Prise en charge

- Linux
- Sur Windows, utilisez WSL (Windows Subsystem for Linux) ou une VM Linux pour une compatibilité correcte.


## Prérequis

- make (GNU make)
- g++ compatible C++20

Pour lancer les différents tests, il faudra également GoogleTest. Pour l'installer, utilisez les commandes suivantes dans un environnement Linus : 
```bash
sudo apt update
sudo apt install libgtest-dev
```
Important : les tests du Rover s'effectue sur le port 8000 et l'adresse 127.0.0.1. Il se peut donc que ceux-ci ne fonctionnent pas si le port est occupé. 


## Compilation

Depuis la racine du projet, lancez :
```bash
# Pour compiler les parties serveur et client :
make all

# Pour compiler uniquement le rover (client) : 
make rover

# Pour compiler uniquement la mission control (serveur) :
make mission

# Pour les tests :
# Pour compiler tous les tests :
make all_tests

# Pour compiler les tests de Planet : 
make planet_tests

# Pour compiler les tests de Mission Control : 
make mission_control_tests

# Pour compiler les tests de Rover : 
make rover_test

```

Les exécutables produits seront alors dans un dossier `bin/`. De plus, lorsque les tests sont compilés, ils se lancent automatiquement.


## Exécution

Important : les commandes suivantes sont à lancer depuis la racine du projet.

1. Démarrez d'abord le serveur `mission_control` en renseignant un port :
   
```bash
./bin/mission_control [PORT]
```

2. Dans un autre terminal, lancez le rover en fournissant : port, adresse du serveur, largeur et hauteur de la planète :

```bash
./bin/rover [PORT] [ADRESSE IP] [LONGUEUR] [LARGEUR]
```

3. Interaction :
- Le serveur affiche la carte inconnue initiale et demande des commandes via le prompt.
- Entrez une suite de commandes composée des caractères : `F` (Forward), `B` (Backward), `L` (Left), `R` (Right).
- Le serveur envoie la commande au rover, reçoit les cases découvertes et met à jour l'affichage.
- Lorsque toutes les cases de la carte sont découvertes, le programme s'arrête.


## Architecture

La structure du projet est la suivante : 
```
.
├── Makefile
├── README.md
├── bin
│   ├── mission_control
│   └── rover
├── communication
│   ├── include
│   │   ├── ipacket_serializer.hpp
│   │   ├── isocket.hpp
│   │   ├── packet_serializer.hpp
│   │   └── unix_socket.hpp
│   └── src
│       ├── packet_serializer.cpp
│       └── unix_socket.cpp
├── mission_control
│   ├── include
│   │   └── mission_control.hpp
│   ├── src
│   │   ├── mission_control.cpp
│   │   └── mission_control_main.cpp
│   └── tests
│       └── mission_control_test.cpp
├── planet
│   ├── include
│   │   ├── iplanet.hpp
│   │   └── planet.hpp
│   ├── src
│   │   └── planet.cpp
│   └── tests
│       └── planet_test.cpp
└── rover
    ├── include
    │   └── rover.hpp
    ├── src
    │   ├── rover.cpp
    │   └── rover_main.cpp
    └── tests
        └── rover_test.cpp
```

L'architecture de celui-ci se traduit par le schéma suivant : 

<img width="1434" height="1266" alt="image" src="https://github.com/user-attachments/assets/e02687f4-d7d6-4e2a-af92-1d948aca6abc" />


## Argumentation 

#### Modules
Ce projet est structuré en quatre modules :  
- **Communication** : il s'agit de la couche réseau de l'application. On y retrouve les méthodes nécessaires aux client et serveur pour communiquer et s'échanger des données. Ce module comprend deux interfaces `ISocket` et `IPacketSerializer` et des structures comme `MissionControlPacket` et `RoverPacket` qui servent à échanger les données de manière structurée et normalisée. On retrouve également une classe `UnixSocket` qui implémente l'interface `ISocket`. Cette distinction permet l'extensiblité de notre structure. En effet, on pourrait par la suite créer une classe `WinSocket`, implémentant l'interface `ISocket`, afin de rendre le programme compatible avec un environnement Windows. 
- **Rover** : il s'agit du point d'entrée "client" du programme. Une fonction `Main()` est présente au sein de la classe `Rover` afin de pouvoir lancer dans le point d'entrée `main()` l'enchaînement des différentes fonctions, tout en exposant publiquement le moins de fonction possible. Les fonctions assesseurs sont présentes uniquement pour le besoin des tests, tout comme la séparation du point d'entrée `main()` dans un autre fichier "rover_main.cpp".
- **MissionControl** : il s'agit du point d'entrée "serveur" du programme. Une fonction `Main()` est présente au sein de la classe `MissionControl` afin de pouvoir lancer dans le point d'entrée `main()` l'enchaînement des différentes fonctions, tout en exposant publiquement le moins de fonction possible. Les fonctions assesseurs sont présentes uniquement pour le besoin des tests, tout comme la séparation du point d'entrée `main()` dans un autre fichier "mission_control_main.cpp".  
- **Planet** : il comprend regroupe l'ensemble des éléments liés à la cartographie de la planète. L'interface `IPlanet` ne comprend qu'une seule méthode `IsFreeTile()`, nécessaire pour vérifier le déplacement du Rover sur des cases valides. On retrouve également dans ce module des outils comme `Tile`, `ObjectType`, `Orientation` et `Command`. 

#### Dépendances
Rover et MissionControl dépendent du module Communication. Pour pouvoir lancer les programmes en tant que serveur et client, les deux modules utilisent les fonctionnalités fournies par `UnixSocket`, qui implémente l'interface `ISocket`. Les paquets échangés transitent via `PacketSerializer`, qui fournit les méthodes de sérialisation et désérialisation utilisées directement dans `Send()` et `Receive()` de `UnixSocket`.

Rover et MissionControl dépendent aussi du module Map.
Dans leurs méthodes `Main()`, chacun crée une instance de Planet :
- côté Rover, elle contient la carte réelle et complète.
- côté MissionControl, elle est initialisée avec des valeurs "UNKNOWN" (représentée par un "X" dans la console) et se met à jour au fur et à mesure des déplacements du rover. La progression du rover est une synchronisation de la carte côté MissionControl en fonction des informations reçues.

#### Structures des modules
Chaque module est représenté dans le projet par un dossier à la racine, comprenant alors plusieurs sous-dossiers : `include/`, `src/` et `tests/`. Cette structure nous permet une grande lisibilité, affichant une séparation claire des responsabilité. Chaque module peut également disposer de ses propres tests.
Toutefois, il est important de noter que cette structure entraîne de la complexité au sein du fichier Makefile. Comme tous les .cpp sont compilés dans celui-ci, la compilation générale du projet peut être longue. Il aurait été possible de faire des sous-makefile, soit un par module, mais nous avons pris parti de conserver un seul Makefile commun, le projet étant petit.

#### Contraintes
La dépendance de Rover et MissionControl envers le module de Communication entraîne des changements importants si le code de ce dernier module est amené à changer. Par exemple, si l'on modifie la structure de MissionControlPacket, il sera nécessaire de modifier une grande partie de la construction du packet au sein du module MissionControl. Il en va de même pour le Rover. On retouve aussi des contraintes similaires avec le module Planet.
