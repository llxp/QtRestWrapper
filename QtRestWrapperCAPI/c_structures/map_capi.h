#ifndef MAP_CAPI_H
#define MAP_CAPI_H

#include <string.h>
#include <search.h>

struct mapNodeCAPI
{
    char *index;
    char *value;
};

struct strMapNode {
    struct mapNodeCAPI *nodes;
};

/*int compar(const void *l, const void *r)
{
    const intStrMap *lm = l;
    const intStrMap *lr = r;
    return lm->key - lr->key;
}*/

#endif // MAP_CAPI_H
