#include "resources.h"

#include <assert.h>
#include <stddef.h>

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
bool resources_check(const resources_t* available, const resources_t* needed)
{
    assert(available != NULL);
    assert(needed != NULL);

    // TODO: Indique si `available` contient assez de ressources pour comblez
    // la demande dans `needed`.

    return false;
}

void resources_lock(resources_t* available, const resources_t* needed)
{
    assert(available != NULL);
    assert(needed != NULL);
    assert(resources_check(available, needed));

    // TODO: Vérouillez les ressources pour utilisation.
}

void resources_unlock(resources_t* available, const resources_t* locked)
{
    assert(available != NULL);
    assert(locked != NULL);

    // TODO: Déverouillez les ressources pour utilisation.
}
