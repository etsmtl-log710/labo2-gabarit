#ifndef LOG710__ERROR
#define LOG710__ERROR

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Indique qu'un variable n'est pas utilisée.
 * @param _X Un nom de variable
 */
#define UNUSED(_X) (void)_X

/**
 * @brief Imprime un avertissement avec un nom de fichier et numéro de ligne.
 * Cette macro accepte une chaîne de formattage et un nombre variadique
 * d'arguments dans la même style que *printf*.
 */
#define WARN(fmt, ...)                                                                 \
    do {                                                                               \
        (void)fprintf(stderr, "[%s:%u] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (false)

/**
 * @brief Imprime un message d'erreur, et arrête l'exécution du processus par
 * l'utilisation du signal SIGABRT, indiquant que le processus est en état
 * érroné.
 * Cette macro accepte une chaîne de formattage et un nombre variadique
 * d'arguments dans la même style que *printf*.
 */
#define ERROR(fmt, ...)           \
    do {                          \
        WARN(fmt, ##__VA_ARGS__); \
        abort();                  \
    } while (false)

/**
 * @brief Une énumération de toutes les erreurs qui peuvent survenir dans la
 * simulation.
 * @note En ajoutant un élément dans cette énumération, il faut changer
 * @e ERROR_NAMES dans @e error.c
 */
typedef enum error {
    /**
     * @brief L'absence d'erreur.
     * @note Cette valeur devrait toujours être zéro.
     */
    OK = 0,
    /**
     * @brief Une fonction n'est pas implémentée.
     */
    UNIMPLEMENTED,
    /**
     * @brief Une allocation de mémoire a échouée.
     */
    OUT_OF_MEMORY,
    /**
     * @private
     * @note Ne rien rajouter après cette entrée.  Cette entrée représente le
     * nombre d'éléments dans cette énumération.
     * @note Cette valeur est connue à la compilation, et peut donc être
     * utilisée pour généré des tableaux de grandeur approriée.
     */
    ERROR_LAST
} error_t;

/**
 * @brief Donne le nom textuelle d'une erreur à partir du code de l'erreur
 * faisant partit de l'énumération @e error_t.
 *
 * @param error Le code de l'erreur
 * @return Le nom de l'erreur
 */
const char* error_to_str(error_t error);

#endif
