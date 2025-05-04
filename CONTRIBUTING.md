Contribuer à SysMon-cpp
Merci de votre intérêt pour SysMon-cpp ! 
Votre contribution est précieuse pour faire évoluer ce moniteur système en C++.
Voici comment vous pouvez participer :

1. Cloner et configurer le projet
Fork le dépôt.

Clonez votre fork en local :

git clone https://github.com/<votre-utilisateur>/SysMon-cpp.git
cd SysMon-cpp

Ajoutez le dépôt principal comme remote :

git remote add upstream https://github.com/<organisation-ou-projet>/SysMon-cpp.git

Créez une branche dédiée à votre contribution :

git checkout -b feature/nom-de-fonctionnalite

2. Créer une Pull Request (PR)

# Mettez à jour votre branche : 
git fetch upstream
git merge upstream/main

# Poussez vos modifications : 
git push origin feature/nom-de-fonctionnalite

# Ouvrez une PR sur GitHub :
Donnez un titre clair 
Décrivez vos changements 
Reliez toute issue fermée (ex: Closes #42).

3. Convention de nommage

## Branches
Type	                     Format	                                        Exemple
Fonctionnalité	  feature/nom-fonctionnalite	   feature/ajout-affichage-ram
Correction de bug	 fix/nom-bug	                   fix/fuite-memoire-processus
Amélioration	improve/nom-amelioration	improve/interface-affichage
Tests	                test/nom-tests	                test/unitaire-cpumonitor

## Commits
type: brève description

Exemples :
·  feat: ajout du support multi-core
·  fix: correction calcul pourcentage CPU
·  docs: amélioration du README
·  test: ajout des tests pour MemoryMonitor
·  refactor: nettoyage de ProcessMonitor

4. Formatage & Tests avant PR

## Formatage du code
Respectez les conventions suivantes :
Indentation : 4 espaces.
Pas de using namespace std; dans les headers.
Préférez const partout où possible.
Classes et méthodes : PascalCase.
Variables locales : camelCase.

## Compilation & exécution
make clean
make all
./bin/SysMon

## Tests unitaires
Pour exécuter les tests :
g++ -std=c++11 tests/test_CpuMonitor.cpp src/CpuMonitor.cpp -o test_cpu
./test_cpu
Tous les tests doivent passer .

## CI/CD
Une fois votre PR créée, GitHub Actions vérifiera automatiquement :
La compilation
Les tests unitaires

5. Bonnes pratiques C++
Quelques règles à suivre pour maintenir une base propre :
RAII : Utilisez la gestion automatique des ressources.
Evitez les fuites mémoire : Utilisez des smart pointers 
(std::unique_ptr, std::shared_ptr) si nécessaire.
Commentaires utiles uniquement : Le code doit être auto-documenté autant que possible.
Petites PRs : Une PR doit être facile à relire.

6. Besoin d’aide ?
Vous pouvez :
Ouvrir une Issue 
Discuter dans Discussions 
Contacter un mainteneur 

Merci pour votre contribution ! Ensemble, rendons SysMon-cpp encore plus puissant !






