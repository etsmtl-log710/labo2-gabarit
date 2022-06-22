#include "error.h"

#include <assert.h>
#include <stddef.h>

// NOTE: Un mapping du nom de l'erreur à une représentation textuelle de cette
// erreur.
//
// Notez l'utilisation de *ERROR_LAST* pour définir la taille du tableau.  Ce
// patron avec les enumérations en C est très commun.
static const char* error_names[ERROR_LAST] = {
    NULL,
    "UNIMPLEMENTED",
    "OUT_OF_MEMORY",
};

const char* error_to_str(error_t error)
{
    assert(error > OK);
    assert(error < ERROR_LAST);
    return error_names[error];
}
