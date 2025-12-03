# MarsRover


Ce dépôt contient une application client/serveur ("Rover" / "Mission Control") écrite en C++. Le client (rover) simule un robot qui explore une grille, le serveur (mission_control) reçoit les positions découvertes et commande le rover.

## Prise en charge

- Linux
- Sur Windows, utilisez WSL (Windows Subsystem for Linux) ou une VM Linux pour une compatibilité correcte.

## Prérequis

- make (GNU make)
- g++ compatible C++20

Pour faire les tests :
-sudo apt update
-sudo apt install libgtest-dev
## Compilation

Depuis la racine du projet, lancez :
```bash
# compile serveur et client
make all

# ou compiler uniquement le rover
make rover

# ou compiler uniquement la mission control
make mission


# compile les Test de Planet
make planet_tests

# compile les Test de Mission_Control
make Mission_control_tests

# compile les Test de Rover
make rover_test

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
