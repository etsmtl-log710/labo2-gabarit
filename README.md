# LOG710 - Laboratoire 2

Ce projet contient le code source du laboratoire 2 du cours LOG710.

## Dépendances

Les dépendances externes suivantes sont nécessaire sur Ubuntu pour compiler et
utiliser ce projet:

  - build-essential
  - pkg-config
  - check

Ces dernières sont disponibles par défaut sur la machine virtuelle qui vous est
fournie ou sont installable par cette commande sur Ubuntu:

```
$ sudo apt install build-essential pkg-config check
```

## Construction

Afin de construire l'exécutable final dans le cadre de ce laboratoire, il ne
suffit que d'utiliser cette commande à la racine du code du laboratoire
(celui contenant le fichier *Makefile*):

```sh
$ make
```

## Exécution

L'exécutable généré s'appele `Log710Schedulers` et se trouve dans le dossier
racine du code du laboratoire.

En l'exécutant sans arguments, vous serez présenté avec un message expliquant
son utilisation:

```
UTILISATION:

	./Log710Schedulers --file <file> [--scheduler fcfs|priority|hybrid] [--quantum <ms>] [--help]

DESCRIPTION:

	Lance la simulation d'une stratégie d'ordonnancement.

OPTIONS:

	--file <file>
		Indique à la simulation quel fichier d'entrées de processus à utiliser

	--scheduler fcfs|priority|hybrid
		Indique à la simulation le type d'ordonnanceur à utiliser. La valeur par défaut est "fcfs".

	--quantum <ms>
		Définit la longueur du quantum.  N'a pas d'impact sur l'ordonnancement, seulement sur la vitesse de la simulation.  La valeur par défaut est 1000, et la valeur 0 indique de ne pas attendre.

	--help
		Affiche ce message d'erreur.
```

## Tests

Le laboratoire comporte aussi des tests automatiser pour vous aider à
implémenter les fonctionnalités connexes demandées, tel les files de processus,
le calcul des statistiques, etc:

```
$ make check
```

Ces tests sont divisés en 2 suites de tests:

  - `queue` pour les tests reliés à la file de processus;
  - `resources` pour les tests reliés aux resources;

Il est possible d'utiliser la variable d'environnement *CK_RUN_SUITE* afin de
choisir la suite de tests à exécuter:

```sh
CK_RUN_SUITE=resources make check
```

## Suggestions

Faîtes une recherche pour les `TODO(Chargé):` dans le code.  Cela indique les boûts de
code que vous avez à implémenter.

### 1. Fonctionnalités utilitaires

Afin d'être en mesure d'implémenter correctement les stratégies d'ordonnancement
demandées, nous vous conseillons fortement de commencer avec les
fonctionnalitées utilisataires dans ces fichiers:

  1. `src/queue.c`
  2. `src/resources.c`

Ces fonctionnalités utilitaires peuvent être développé en parallèle.  Vous
pouvez valider leur fonctionnement à l'aide des tests automatisés fournis.

### 2. Prendre connaissance des entêtes

Les énumérations, structures de données, et fonctions des fichiers suivant vous
permettront d'implémenter les stratégies d'ordonnancement:

  - `src/queue.h`
  - `src/ressources.h`
  - `src/process.h`

#### Déterminer si un processus s'est arrêté

Afin de déterminer si un processus s'est arrêté, vous n'avez qu'à utiliser son
champ `status`, la fonction `process_tick` va automatiquement changer ce status
d'un tick à l'autre:

```c
process->status == TERMINATED
```

### 3. Conception et implémentation des ordonnanceurs

Il vous est maintenant possible d'implémenter les stratégies d'ordonnancement.
Il vous est possible de créer autant de fichier que vous le voulez, les fichiers
créés devraient automatiquement être reconnus par le *Makefile*.

La qualité de l'abstraction que vous utilisez pour les ordonnanceurs vaut pour
une portion considérable de la partie pratique.

## Outils intéressants

### *valgrind*

*valgrind* est un programme pour Linux permettant de valider l'exécution d'un
programme.  *valgrind* est votre ami pour valider la qualité de votre
implémentation du laboratoire, et il vous est fortement recommandé de souvent
exécuter votre code sous ce dernier:

```sh
$ valgrind --leak-check=full ./Log710Schedulers [...]
```

*valgrind* vous aidera à trouver des problèmes tels:

  - Fuites de mémoire;
  - Double libération de mémoire;
  - Lecture de mémoire non-initialisée;
  - Lecture ou écrire de mémoire invalide;
  - Exécution de comportement non-définit;
  - etc.

Comme tout programme Linux, la documentation est disponible avec `man valgrind`.

### *strace*

*strace* est une programme interceptant et affichant les appels système d'un
programme.  Ce dernier affiche le nom de l'appel système, les arguments de
l'appel système, ainsi que le code de retour (et l'erreur si nécessaire):

```sh
$ strace ./Log710Schedulers [...]
```

Comme tout programme Linux, la documentation est disponible avec `man strace`.

## Problèmes

### Git - Detected Dubious Ownership

Ce problème survient lorsque le dossier dans lequel vous travailler avec Git
n'appartient pas à votre utilisateur.  Cela risque de survenir avec des dossiers
partagées, car ces derniers appartiennent à `root:vboxsf`.  Votre utilisateur
fait parti du groupe `vboxsf`, et peut donc lire, modifier et exécuter, mais Git
ne comprend pas cela.  Utilisez la commande suivante afin de désactiver cette
fonctionnalitée de Git:

```sh
$ git config --global --add safe.directory '*'
```

### Git - Old Mode et New Mode dans le Diff

Par défaut, Git sauvegarde aussi les permissions des fichiers et dossiers du
projet, cependant les permissions de vos fichiers et dossiers peuvent grandement
changer entre votre machine hôte et votre machine invitée.

```
diff --git a/.editorconfig b/.editorconfig
old mode 100644
new mode 100755
```

Pour empêcher Git de prendre en compte les permissions de fichiers et dossiers,
assurez vous que le fichier `.git/config` ne comporte pas d'option
`core.filemode`, et exécutez cette commande:

```sh
$ git config --global core.filemode false
```
