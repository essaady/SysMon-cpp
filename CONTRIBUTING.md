# Guide de Contribution à SysMon-cpp

<p align="center">
    <a href="https://github.com/FPT-Essaady/SysMon-cpp" target="_blank">
        <img src="https://exemple_de_link_pour_le_logo_a_ajouter_apres" 
             alt="Logo SysMon-cpp" 
             width="30%">
    </a>
</p>

Merci de votre intérêt pour contribuer au projet SysMon-cpp ! Ce document vous guidera à travers les étapes nécessaires pour apporter votre contribution de manière efficace et conforme aux standards du projet.

## Table des matières

1. [Configuration du projet](#configuration-du-projet)
2. [Workflow de contribution](#workflow-de-contribution)
3. [Conventions de nommage](#conventions-de-nommage)
4. [Style de code et formatage](#style-de-code-et-formatage)
5. [Tests](#tests)
6. [Création d'une Pull Request](#création-dune-pull-request)

## Configuration du projet

### Clonage et installation

1. Créez un fork du dépôt sur GitHub

```bash
# Visitez https://github.com/FPT-Essaady/SysMon-cpp
# Cliquez sur le bouton "Fork" en haut à droite
```

2. Clonez votre fork localement

```bash
git clone https://github.com/VOTRE-NOM-UTILISATEUR/SysMon-cpp.git
cd SysMon-cpp
```

3. Ajoutez le dépôt original comme remote upstream

```bash
git remote add upstream https://github.com/essaady/SysMon-cpp.git
```

4. Compilez le projet

```bash
mkdir build
cd build
cmake ..
make
```

5. Vérifiez que tout fonctionne correctement

```bash
./SysMon-cpp --help
```

## Workflow de contribution

### 1. Synchronisez votre fork avant de commencer

```bash
git checkout main
git pull upstream main
git push origin main
```

### 2. Créez une branche pour votre travail

```bash
git checkout -b TYPE/nom-descriptif
```

Voir [Conventions de nommage](#conventions-de-nommage) pour les types de branches.

### 3. Développez et committez vos changements

```bash
# Après avoir effectué vos modifications
git add .
git commit -m "TYPE: description détaillée du changement"
```

### 4. Poussez vos changements

```bash
git push origin TYPE/nom-descriptif
```

### 5. Créez une Pull Request (voir section dédiée)

## Conventions de nommage

### Branches

Format: `TYPE/nom-descriptif`

Types de branches:
- `feature/` - Pour ajouter une nouvelle fonctionnalité
- `fix/` - Pour corriger un bug
- `refactor/` - Pour du code qui n'ajoute pas de fonctionnalité ni ne corrige un bug
- `docs/` - Pour les modifications de documentation
- `test/` - Pour ajouter ou modifier des tests
- `perf/` - Pour les améliorations de performance

Exemples:
- `feature/export-csv`
- `fix/memory-leak`
- `docs/update-readme`

### Commits

Format: `TYPE: description détaillée`

Types de commits:
- `FEAT` - Nouvelle fonctionnalité
- `FIX` - Correction de bug
- `REFACTOR` - Réorganisation du code sans changement de comportement
- `DOCS` - Modification de documentation
- `TEST` - Ajout ou modification de tests
- `PERF` - Amélioration des performances
- `CHORE` - Changements de configuration, build, etc.

Exemples:
- `FEAT: ajout de l'exportation en format CSV`
- `FIX: correction d'une fuite mémoire dans ProcessMonitor`
- `DOCS: mise à jour des instructions d'installation`

## Style de code et formatage

Nous utilisons un style de code cohérent dans tout le projet:

- Indentation: 4 espaces (pas de tabulations)
- Noms de classes: PascalCase (ex: `ProcessMonitor`)
- Noms de fonctions: camelCase (ex: `calculateCpuUsage`)
- Variables: camelCase (ex: `totalMemory`)
- Constantes et macros: SNAKE_CASE_MAJUSCULE (ex: `MAX_PROCESSES`)
- Accolades: sur une nouvelle ligne

### Formatage automatique

Pour garantir la cohérence du code, utilisez `clang-format` avant de soumettre votre contribution:

```bash
# Installez clang-format si nécessaire
sudo apt-get install clang-format

# Formatez un fichier spécifique
clang-format -i src/votre-fichier.cpp

# Ou formatez tous les fichiers .cpp et .h
find src include -iname "*.cpp" -o -iname "*.h" | xargs clang-format -i
```

## Tests

Tous les nouveaux code doit être accompagné de tests unitaires. Les tests sont dans le répertoire `tests/`.

### Exécution des tests existants

```bash
cd build
make tests
./tests/test_CpuMonitor
# Exécutez les autres tests disponibles
```

### Ajout de nouveaux tests

1. Créez un nouveau fichier de test dans `tests/`, par exemple `test_MaNouvelleFonctionnalite.cpp`
2. Incluez les en-têtes nécessaires et écrivez vos tests
3. Mettez à jour le CMakeLists.txt pour inclure votre nouveau test
4. Compilez et exécutez pour vérifier que vos tests passent

## Création d'une Pull Request

1. Assurez-vous que votre code:
   - Respecte les conventions de style
   - Inclut des tests appropriés
   - Est accompagné d'une documentation si nécessaire

2. Poussez votre branche vers votre fork:
   ```bash
   git push origin TYPE/nom-descriptif
   ```

3. Visitez GitHub et créez une Pull Request:
   - Allez sur votre fork
   - Cliquez sur "Compare & pull request"
   - Sélectionnez la branche `main` du dépôt original comme cible

4. Remplissez le template de Pull Request:
   - Titre clair et descriptif
   - Description détaillée des changements
   - Référence aux issues que la PR résout (ex: "Fixes #123")
   - Liste des modifications apportées

5. Attendez la revue de code et les tests automatiques

6. Apportez les modifications demandées si nécessaire:
   ```bash
   # Effectuez vos modifications
   git add .
   git commit -m "FIX: changements demandés lors de la revue"
   git push origin TYPE/nom-descriptif
   ```

7. Une fois approuvée, votre PR sera fusionnée dans la branche principale!

---

Merci de contribuer à SysMon-cpp! Nous apprécions votre aide pour améliorer ce projet.