#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <claro/base.h>

static void _iter_cb(const void * key, const void * value, void * unused)
{
    printf("%s = %d\n", key, value);
}

int main(int argc, char ** argv)
{
    claro_hashtable_t * hash; 
    claro_list_t * list;
    int i, len;

    claro_base_init();

    hash = claro_hashtable_str_create(FALSE, NULL);

    claro_hashtable_insert(hash, "one", (void *)1, TRUE);
    claro_hashtable_insert(hash, "two", (void *)2, TRUE);
    claro_hashtable_insert(hash, "three", (void *)3, TRUE);

    claro_hashtable_iter_keys(hash, _iter_cb, NULL);

    list = claro_hashtable_get_keys(hash);

    len = claro_list_count(list);

    for(i = 0; i < len; i++)
    {
        void * key = claro_list_get_item(list, i);
        printf("%s = %d\n", key, claro_hashtable_lookup(hash, key)); 
    }

    claro_list_destroy(list);
    claro_hashtable_unref(hash);

    return 0;
}
