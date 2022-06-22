#ifndef LOG710__SCHEDULER
#define LOG710__SCHEDULER

/**
 * @brief Une énumération contenant les types d'ordonnanceurs à implémenter dans
 * le cadre du laboratoire.
 */
typedef enum scheduler_type {
    FCFS_SCHEDULER,
    PRIORITY_SCHEDULER,
    HYBRID_SCHEDULER,
} scheduler_type_t;

// TODO: N'oubliez pas de bien concevoir vos ordonnanceurs. Libre à vous de
// créer de nouveaux fichiers pour isoler la logique des ordonnanceurs.
//
// Je vous conseille fortement d'isoler la logique de chaque ordonnanceur dans
// leurs propres fichiers (ex.: `fcfs_scheduler.{c,h}`,
// `priority_scheduler.{c,h}`, et `hybrid_scheduler.{c,h}`).

#endif
