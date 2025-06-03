<p align="center">
  <img src="assets/LOGO.png" alt="SysMon Logo" width="200"/>
</p>
# SysMon-cpp

## Description
## Test Pull Request
updeat READEME

**SysMon-cpp** est un moniteur système développé en **C++** permettant de surveiller en temps réel l'utilisation des ressources du système, telles que le processeur (CPU), la mémoire (RAM), et les processus actifs. Inspiré des outils `top` et `htop`, il fournit une interface en ligne de commande (CLI) pour afficher des informations système critiques en temps réel.

### Fonctionnalités :
- **Surveillance CPU** : Affiche l'utilisation du CPU via `/proc/stat`.
- **Surveillance Mémoire** : Affiche l'utilisation de la mémoire via `/proc/meminfo`.
- **Surveillance des processus** : Liste des processus actifs avec leur consommation de CPU/RAM.
- **Exportation des données** : Possibilité d'exporter les informations sous format texte ou CSV.
- **Architecture modulaire** : Conçu en utilisant la programmation orientée objet (POO).

## Structure du Projet

Voici un aperçu de la structure des répertoires du projet :

```plaintext
SysMon-cpp/
├── src/                    # Code source principal
│   ├── CpuMonitor.cpp      # Module de gestion du CPU
│   ├── MemoryMonitor.cpp   # Module de gestion de la mémoire
│   ├── ProcessMonitor.cpp  # Module de gestion des processus
│   └── main.cpp            # Programme principal
├── include/                # Fichiers d'en-tête
│   ├── CpuMonitor.h
│   ├── MemoryMonitor.h
│   ├── ProcessMonitor.h
│   └── SysMon.h            # Entête commun
├── tests/                  # Tests unitaires
│   └── test_CpuMonitor.cpp
├── .github/                # Configuration des workflows GitHub Actions pour CI/CD
│   └── workflows/
│       └── ci.yml          # Pipeline d'intégration continue
├── .gitignore              # Ignorer les fichiers temporaires et de compilation
├── README.md               # Documentation du projet
└── LICENSE                 # Licence open-source (MIT par défaut)
```
Prérequis
Dépendances :

    C++11 ou version ultérieure

    Linux (utilisation de /proc pour la collecte des données)

    Outils de compilation : g++, make

Installation
1. Cloner le Repository

Clonez le repository avec la commande suivante :

```
git clone https://github.com/essaady/SysMon-cpp.git
cd SysMon-cpp
```

2. Compiler le Projet

Le projet utilise CMake pour la gestion de la compilation. Vous pouvez le compiler en utilisant les commandes suivantes :
```
mkdir build
cd build
cmake ..
make
```

3. Exécuter le Moniteur Système

Une fois le projet compilé, vous pouvez exécuter le moniteur système avec la commande suivante :

```
./SysMon-cpp
```

Utilisation

Le programme affichera les informations suivantes en temps réel :

    CPU : Utilisation du CPU par cœur.
    Mémoire : Utilisation de la RAM et de la mémoire swap.
    Processus : Liste des processus actifs avec leur consommation de ressources (CPU/RAM).

Commandes disponibles :
```
    --help : Affiche l'aide avec la liste des options disponibles.

    --export : Exporte les données sous forme de fichier texte ou CSV.

    --update-interval : Définit l'intervalle de mise à jour des données en secondes.
```
Tests
Exécution des Tests

Pour tester le projet, vous devez avoir un environnement de test configuré. Les tests sont situés dans le dossier tests/ et sont écrits en C++.

Pour exécuter les tests, vous pouvez utiliser CMake :

```
cd build
make tests
./tests/test_CpuMonitor
```
Ajouter des Tests

Pour ajouter des tests unitaires pour un module spécifique (par exemple, CpuMonitor), ajoutez un fichier de test dans le répertoire tests/ et implémentez les tests nécessaires. Assurez-vous que vos tests vérifient bien toutes les fonctionnalités du module (par exemple, calcul des ressources CPU, gestion des erreurs, etc.).
Contribuer

Les contributions sont les bienvenues ! Pour contribuer à ce projet, veuillez suivre ces étapes :

- Fork ce repository
- Créez une branche pour votre fonctionnalité : git checkout -b feature/ma-nouvelle-fonctionnalite
- Effectuez vos modifications
- Committez vos changements : 
```
git commit -am 'Ajout de la fonctionnalité X'
```
- Poussez vos changements : git push origin feature/ma-nouvelle-fonctionnalite
- Ouvrez une pull request pour intégrer vos changements à la branche principale.

Bonnes pratiques de développement

-  Ajoutez des tests unitaires pour chaque fonctionnalité que vous implémentez.
- Documentez votre code et mettez à jour le README si nécessaire.

Licence

Ce projet est sous licence MIT. Vous pouvez consulter les détails de la licence dans le fichier LICENSE
