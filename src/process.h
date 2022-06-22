#ifndef LOG710__PROCESS
#define LOG710__PROCESS

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#include "resources.h"

/**
 * @brief Une énumération de tous les états de processus possibles dans ce
 * laboratoire.
 */
typedef enum process_status {
    /**
     * @brief Le processus est entrain d’être créé.
     * @note Le processus n'est pas arrivé.
     */
    NEW,
    /**
     * @brief Le processus est en attente d'être assigné au processeur.
     */
    READY,
    /**
     * @brief Les instructions du programme associé sont en cours d'exécution.
     */
    RUNNING,
    /**
     * @brief Le processus a terminé sont exécution.
     */
    TERMINATED,
} process_status_t;

/**
 * @brief Le type de processus.
 */
typedef enum process_type {
    /**
     * @brief Un processus temps-réel.
     */
    REAL = 0,
    /**
     * @brief Un processus utilisateur.
     */
    USER = 1,
} process_type_t;

/**
 * @brief La structure de données contenant l'information d'un processus dans
 * cette simulation.
 */
typedef struct process {
    /**
     * @brief L'identifier unique du processus.
     * @note Cet identifiant est la ligne dans le fichier.
     */
    size_t pid;
    /**
     * @brief La status du processus.
     * @note La processus est @e NEW par défaut.
     */
    process_status_t status;
    /**
     * @brief Le type de processus.
     * @note Cette valeur provient du fichier.
     */
    process_type_t type;
    /**
     * @brief Le temps d'exécution du processus.
     * @note Cette valeur provient du fichier.
     */
    size_t execution_time;
    /**
     * @brief Le temps d'arrivé du processus.
     * @note Cette valeur provient du fichier.
     */
    size_t arrival_time;
    /**
     * @brief Les ressources nécessaire pour exécuter le processus.
     * @note Ces valeurs proviennent du fichier.
     */
    resources_t resources;

    /**
     * @brief Le temps que le processus a passé à s'exécuter.
     * @note Utiliser le champ @e status pour déterminer si le processus s'est
     * arrêté.
     */
    size_t executed_time;
    /**
     * @brief Le temps d'attente du processus.
     * @note Utiliser cette valeur pour le calcul des temps d'attente moyens.
     */
    size_t waited_time;
} process_t;

// Vous n'avez pas à utiliser cette fonction.
void process_init_from_line(process_t* process, const char* line, size_t line_number);

/**
 * @brief Indique si le processus à commencé à être exécuté.
 * @note Le processus peut être suspendu ou terminé.
 *
 * @param process Le processus.
 * @return Vrait si le processus a commencé à être exécuté
 */
bool process_has_started(const process_t* process);

/**
 * @brief Change le status du processus à @e READY.
 * @note Le processus doit être @e NEW.
 *
 * @param process Le processus
 */
void process_schedule(process_t* process);

/**
 * @brief Change le status du processus à @e RUNNING.
 * @note Le processus doit être @e READY.
 * @note Il ne doit pas y avoir de processus en cours d'exécution.
 *
 * @param process Le processus
 */
void process_execute(process_t* process);

/**
 * @brief Change le status du processus à @e READY.
 * @note Le processus doit être @e RUNNING.
 * @note Il ne doit pas y avoir de processus en cours d'exécution.
 *
 * @param process Le processus
 */
void process_suspend(process_t* process);

/**
 * @brief Avance l'état du processus de 1 quanta.
 * @note Cette fonction doit être appelée pour tous les processus, car elle
 * accumule de l'information pour les processus en suspendus, terminées, en
 * attente, etc.
 *
 * @param process Le processus
 */
void process_tick(process_t* process);

#endif
