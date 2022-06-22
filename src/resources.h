#ifndef LOG710__RESOURCES
#define LOG710__RESOURCES

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Indique les ressources par défauts disponible dans un ordonanceur à
 * son départ.
 */
#define INITIAL_RESOURCES ((resources_t) { \
    .printers = 2,                         \
    .scanners = 1,                         \
    .modems = 1,                           \
    .drives = 2,                           \
})

/**
 * @brief Une structure de donnée contenant toutes les ressources disponible
 * dans un ordonnanceur et demandées par un processus.
 */
typedef struct resources {
    /**
     * @brief Un nombre d'imprimantes.
     */
    size_t printers;
    /**
     * @brief Un nombre de scanneurs.
     */
    size_t scanners;
    /**
     * @brief Un nombre de modems.
     */
    size_t modems;
    /**
     * @brief Un nombre de lecteurs CDs.
     */
    size_t drives;
} resources_t;

/**
 * @brief Indique s'il y a assez de ressources dans @p available pour comblez la
 * demande dans @p needed.
 *
 * @param available Les ressources disponibles
 * @param needed Les ressources nécessaires
 * @return Vrai s'il y a assez de ressources, faux autrement
 */
bool resources_check(const resources_t* available, const resources_t* needed);

/**
 * @brief Vérouille les ressources nécessaires des ressources disponibles.
 *
 * @note @p available doit contenir assez de ressources.
 *
 * @param available Les ressources disponibles
 * @param needed Les ressources nécessaires
 */
void resources_lock(resources_t* available, const resources_t* needed);

/**
 * @brief Dévérouille les ressources utilisées par un processus.
 *
 * @param available Les ressources disponibles
 * @param locked Les ressource vérouillées
 */
void resources_unlock(resources_t* available, const resources_t* locked);

#endif
