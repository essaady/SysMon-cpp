# SysMon - System Monitor en C++

**SysMon** est un mini-projet éducatif développé en C++ qui permet de surveiller les ressources système sous Linux.  
Il affiche des informations essentielles comme :

- Utilisation du CPU
- Mémoire utilisée
- Uptime du système
- (à venir) Liste des processus, utilisation disque...

## Compilation

Pour compiler le projet :

```bash
g++ -std=c++11 -Iinclude -o sysmon src/*.cpp
```

> Assurez-vous d'avoir un compilateur C++ compatible (comme `g++`, `clang++`).

## Utilisation

Lancer le programme :

```bash
./sysmon
```

Sortie typique :

```
CPU Usage: 32%
Memory: 2048 MB / 8192 MB
Uptime: 1h 12m
```

---

## Architecture Modulaire

Le projet est construit selon une **architecture orientée objet (POO)**.  
Chaque composant du système est encapsulé dans une classe spécifique, ce qui rend le code facile à maintenir et à étendre.

---

## Structure du Projet

```plaintext
SysMon-cpp/
├── src/                    # Code source principal
│   ├── CpuMonitor.cpp      # Module CPU
│   ├── MemoryMonitor.cpp   # Module Mémoire
│   ├── ProcessMonitor.cpp  # Module Processus
│   └── main.cpp            # Point d'entrée
├── include/                # Fichiers d'en-tête
│   ├── CpuMonitor.h
│   ├── MemoryMonitor.h
│   ├── ProcessMonitor.h
│   └── SysMon.h
├── tests/                  # Tests unitaires
│   └── test_CpuMonitor.cpp
├── .github/                # Workflows GitHub Actions
│   └── workflows/
│       └── ci.yml
├── .gitignore              # Fichiers ignorés par Git
├── README.md               # Documentation (ce fichier)
└── LICENSE                 # Licence (MIT par défaut)
```

## Prérequis

- Système Linux (Ubuntu, Debian, etc.)
- g++ (ou tout compilateur C++11+)
- make (optionnel)

## Dépendances

Aucune dépendance externe pour l'instant.  
Des bibliothèques futures peuvent être envisagées :

- `nlohmann/json` – pour support JSON
- `ncurses` – pour interface terminale interactive

## Améliorations futures possibles

- Option `--json` pour sortie JSON
- Affichage graphique dans le terminal (barres de progression)
- Traduction multi-langue avec `--lang`
- Export vers fichier log `.txt`
- Interface interactive avec raccourcis clavier

## 👨Contributeurs

- **sofyanehb** – Ajout et amélioration de la documentation
- **FPT-Essaady** – Base du projet

## Licence

Ce projet est à but pédagogique. Licence open source MIT.
