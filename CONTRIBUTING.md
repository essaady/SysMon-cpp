# **Contribuer à SysMon-cpp**

Merci de votre intérêt pour contribuer à SysMon-cpp !

Voici les étapes et règles à suivre pour participer efficacement au projet.

## Cloner et configurer le projet

- **Fork** le dépôt GitHub.
- **Clone** votre fork localement :

git clone <https://github.com/&lt;votre-utilisateur&gt;/SysMon-cpp.git>

cd SysMon-cpp

- Configurer le dépôt distant officiel :

git remote add upstream <https://github.com/&lt;organisation-ou-projet&gt;/SysMon-cpp.git>

- Créer votre branche de travail :

git checkout -b feature/nom-fonctionnalite

## Créer une Pull Request (PR)

- Assurez-vous que votre branche est à jour :

git fetch upstream

git merge upstream/main

- Pousser votre branche :

git push origin feature/nom-fonctionnalite

- Ouvrir une Pull Request via GitHub :

· Décrivez clairement vos modifications.

· Liez tout problème (issue) résolu avec

Closes #issue_number

## Convention de nommage des branches

Type	                                    Format	                                     Exemple
Nouvelle fonctionnalité	feature/nom-fonctionnalite	feature/ajout-monitor-cpu
Correction de bug	                 fix/nom-bug	                fix/memoire-leak-processus
Amélioration	                 improve/nom-amélioration    improve/interface-cli
Tests	                                 test/nom-tests	            test/memory-monitor-tests

## Convention des commits

- Utiliser un format clair et concis :

type: sujet du commit

Exemple :

- feat: ajouter l'affichage de l'utilisation RAM
- fix: corriger une fuite mémoire dans ProcessMonitor
- docs: mise à jour du README
- test: ajout des tests unitaires pour CpuMonitor
- refactor: simplification de la classe MemoryMonitor

## Formatage et tests avant PR

Avant de soumettre une PR :

1. Formater le code pour respecter un style cohérent :

- Indentation : 4 espaces.
- Nommage des classes et méthodes : en PascalCase.
- Nommage des variables locales : en camelCase.

1. Compiler et tester :

make clean

make all

./bin/SysMon

1. Exécuter les tests unitaires (situés dans tests/) :

g++ -std=c++11 tests/test_CpuMonitor.cpp src/CpuMonitor.cpp -o test_cpu

./test_cpu

1. Vérifiez que GitHub Actions (CI) passe sans erreur après votre push.