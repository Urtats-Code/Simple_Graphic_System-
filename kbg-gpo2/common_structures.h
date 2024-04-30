#include "cargar-triangulo.h"



typedef struct mlist
{
    double m[16];
    struct mlist *hptr;
} mlist;

typedef struct triobj
{
    hiruki *triptr; 
    int num_triangles;
    mlist *mptr;
    struct triobj *hptr;
} triobj;

// Used objects

hiruki *triangulosptr;
triobj *foptr;
triobj *sel_ptr;
