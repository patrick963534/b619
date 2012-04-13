#include <mz/object.h>

MZ_API mz_vtable_t* mz_get_object_vtable()
{
    static mz_vtable_t vtable;

    if (vtable.size == 0)
    {
        vtable.size = sizeof(vtable);
    }

    return &vtable;
}
