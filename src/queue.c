#include "queue.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

/**
 * @brief La structure de données contenant l'information des noeuds de la file
 * de processus.
 */
struct queue_node {
    /**
     * @brief Le prochain noeud dans la file de processus.
     */
    queue_node_t* next;
    /**
     * @brief Le processus associé à ce noeud.
     */
    process_t* process;
};

#define FOR_EACH_NODE(node, queue) \
    for (queue_node_t* node = (queue)->begin; node != NULL; node = node->next)

void queue_init(queue_t* queue)
{
    assert(queue != NULL);

    // TODO: Initialisez la file de processus.
    //
    // La file gère la mémoire des `queue_node_t` qu'elle contient.
    //
    // Une file initialisée est vide.
}

void queue_deinit(queue_t* queue)
{
    assert(queue != NULL);

    // TODO: Libérez la mémoire de la file.
    //
    // Que faire si la liste a encore des noeuds ?

    // NOTE: Le code suivant:
    //
    // ```c
    // free(node);
    // node->next;
    // ```
    //
    // Invoque du comportement non-définit, car il n'est pas légal d'accéder à
    // un champ de la structure de donnée après l'avoir libérée.
}

bool queue_is_empty(const queue_t* queue)
{
    assert(queue != NULL);

    // TODO: Indiquez si la liste est vide.

    return true;
}

error_t queue_enqueue(queue_t* queue, process_t* process)
{
    assert(queue != NULL);
    assert(process != NULL);

    // TODO: Enfiler le processus donnée dans la file de processus.
    //
    // Vous ne gérez pas la mémoire de `process`, le système d'exploitation
    // (la simulation) la gère pour vous.

    return UNIMPLEMENTED;
}

process_t* queue_dequeue(queue_t* queue)
{
    assert(queue != NULL);

    // TODO: Défiler le processus au début de la file.
    //
    // Que faire si la file est vide ?

    return NULL;
}

process_t* queue_dequeue_if(queue_t* queue, queue_predicate_t* predicate, void* userdata)
{
    assert(queue != NULL);
    assert(predicate != NULL);

    // TODO: Défilez le premier élément de la file pour lequel le prédicat
    // retourne vrai.
    //
    // Inspirez vous de l'implémentation fournie pour `queue_each`.
    ((void)userdata);

    return NULL;
}

void queue_each(const queue_t* queue, queue_each_t* callback, void* userdata)
{
    assert(queue != NULL);
    assert(callback != NULL);

    for (queue_node_t* node = queue->begin; node != NULL; node = node->next) {
        callback(node->process, userdata);
    }
}
