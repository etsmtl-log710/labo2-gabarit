# Structure du projet
SOURCE_DIR         := src
HEADER_DIR         := src
TEST_DIR           := test
TARGET_DIR         := target

# Définition de variables qui seront réutilisées plus tard.
CC                 ?= gcc
CPPFLAGS           += -I$(SOURCE_DIR)
CFLAGS             += -Wall -Wextra -Werror=vla -Werror=alloca -Werror=main -std=gnu11 -ggdb
LDFLAGS            +=

# Utilitaires
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

# Fichiers communs
source_files       := $(call rwildcard, $(SOURCE_DIR), *.c)
source_objects     := $(source_files:%.c=$(TARGET_DIR)/%.o)
test_files         := $(call rwildcard, $(TEST_DIR), *.c)
test_objects       := $(test_files:%.c=$(TARGET_DIR)/%.o) $(filter-out $(TARGET_DIR)/$(SOURCE_DIR)/main.o, $(source_objects))
header_files       := $(call rwildcard, $(SOURCE_DIR), *.h)
dependencies       := $(call rwildcard, $(TARGET_DIR), *.d)

# En utilisant:
#
#     $ make clean all PEDANTIC=1
#
# D'autres avertissements seront activés dans GCC.
ifdef PEDANTIC
CFLAGS             += -Wpedantic
endif

# En utilisant:
#
#     $ make clean all IMPORTANT=1
#
# Certains avertissements seront convertit en erreurs, ce qui peut vous aider à
# trouver des problèmes dans votre code.
ifdef IMPORTANT
CFLAGS             += -Werror=uninitialized -Werror=format \
                      -Werror=implicit-function-declaration -Werror=format-overflow \
                      -Werror=misleading-indentation -Werror=return-type \
                      -Werror=maybe-uninitialized -Werror=conversion \
                      -Werror=sign-compare -Werror=address
endif

# Ne pas utiliser si vous ne savez pas ce que vous faites.
#
#     $ make clean all ASAN=1
#
# Active les outils *asan*, et *ubsan* de Google:
#     https://github.com/google/sanitizers
ifdef ASAN
CFLAGS             += -fsanitize=address -fsanitize=undefined
endif

# Ne pas utiliser si vous ne savez pas ce que vous faites.
#
#     $ make clean all LEAKSAN=1
#
# Active l'outil *leaksan* de Google:
#     https://github.com/google/sanitizers
ifdef LEAKSAN
LDFLAGS            += -fsanitize=leak
endif

# La première règle apparaissant dans le GNUMakefile est la règle par défaut
# lorsque le programme `make` est appelé sans arguments.
#
# Par convention cette règle s'appele *all* et sert à créer tous les artéfacts
# consommable du projet.
.PHONY: all
all: Log710Schedulers target/Log710Tests

# Par convention, les fichiers Makefile devraient fournir une règle *clean*
# permettant de supprimer tous les artéfacts générés, intermédiaires ou finaux.
.PHONY: clean
.SILENT: clean
clean:
	rm -rf $(TARGET_DIR)
	rm -f Log710Schedulers

# La règle *check* permet d'exécuter les tests unitaires.
.PHONY: check
.SILENT: check
check: $(TARGET_DIR)/Log710Tests
	@./$<

# Les étapes de compilation pour les fichiers .c dans le dossier `src`.
$(TARGET_DIR)/$(SOURCE_DIR)/%.o: $(SOURCE_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c -o $@ $<

# Les étapes de compilation pour les fichiers .c dans le dossier `test`.
$(TARGET_DIR)/$(TEST_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(shell pkg-config --cflags check) -MMD -MP -c -o $@ $<

# Indique comment construire la commande `Log710Schedulers`.
Log710Schedulers: $(source_objects)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Indique comment construire la commande `target/Log710Tests`
$(TARGET_DIR)/Log710Tests: $(test_objects)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(shell pkg-config --libs check)

# Avec les options -MMD et -MP de GCC, nous générons des fichiers Makefile à
# côté des fichiers .o contenant l'information des entêtes utilisées par les
# fichier .c équivalent.
#
# Cette information est réutilisée et mise-à-jour à chaque compilation du code
# afin de déterminer les dépendances directes et transitives des entêtes.
#
# La ligne commence par un `-` pour indiquer à Make d'ignorer les erreurs.  Si
# ces fichiers n'existes pas, c'est que nous compilons pour la première fois.
-include $(dependencies)
