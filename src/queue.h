#ifndef LOG710__QUEUE
#define LOG710__QUEUE

#include <stdbool.h>
#include <stddef.h>

#include "error.h"

/// FWD: process.h
typedef struct process process_t;

/// FWD: queue.c
/**
 * @brief La structure de données contenant l'information d'un noeud de la file
 * de processus @e queue.
 */
typedef struct queue_node queue_node_t;

/**
 * @brief La structure de données contenant l'information d'une file de
 * processus.
 * @note Ne pas accéder aux champs directement, mais utiliser les fonctions de
 * ce fichier pour bien encapsuler le fonctionnement de votre file d'attente.
 */
typedef struct queue {
    /**
     * @brief Le premier noeud de la file de processus.
     */
    queue_node_t* begin;
    /**
     * @brief Le dernier noeud de la file de processus.
     */
    queue_node_t* end;
} queue_t;

/**
 * @brief Initialises la file de processus.
 * @note La file doit être vide après l'initialisation.
 *
 * @param queue La file à initialiser.
 */
void queue_init(queue_t* queue);

/**
 * @brief Libère la mémoire utilisée par la file de processus.
 *
 * @param queue La file a libérer
 */
void queue_deinit(queue_t* queue);

/**
 * @brief Indique si la file de processus est vide.
 *
 * @param queue La file de processus
 * @return true Si la file est vide
 * @return false Si la file n'est pas vide
 */
bool queue_is_empty(const queue_t* queue);

/**
 * @brief Ajoute un processus à la fin de la file de processus.
 *
 * @param queue La file de processus
 * @param process Le processus à ajouter
 * @return Un code d'erreur
 */
error_t queue_enqueue(queue_t* queue, process_t* process);

/**
 * @brief Supprimer le premier processus de la file de processus.
 *
 * @param queue La file de processus
 * @return Le premier processus ou @e NULL
 */
process_t* queue_dequeue(queue_t* queue);

/**
 * @brief Un prédicat à utiliser avec la file de processus.
 *
 * @param process Un processus
 * @param userdata Les données contextuelles du prédicat
 */
typedef bool(queue_predicate_t)(process_t* process, void* userdata);

/**
 * @brief Supprimer le premier processus de la file de processus pour lequel
 * le prédicat est vrai.
 *
 * @param queue La file de processus
 * @param predicate Un prédicat
 * @param userdata Les données contextuelles à passer au prédicat
 * @return Le processus supprimé, ou @e NULL
 */
process_t* queue_dequeue_if(queue_t* queue, queue_predicate_t* predicate, void* userdata);

/**
 * @brief Un @e callback à utiliser avec la file de processus.
 *
 * @param process Un processus
 * @param userdata Les données contextuelles de ce @e callback
 */
typedef void(queue_each_t)(process_t* process, void* userdata);

/**
 * @brief Appèle le @e callback pour tous les processus de la file de processus.
 *
 * @param queue La file de processus
 * @param callback Le @e callback
 * @param userdata Les données contextuelles à passer au @e callback
 */
void queue_each(const queue_t* queue, queue_each_t* callback, void* userdata);

#endif
