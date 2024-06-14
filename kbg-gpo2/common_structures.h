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
triobj *fcamprt; 
triobj *sel_ptr;
triobj *cam_ptr; 

// Modes 

int denak;
int lineak;
int objektuak;
char aldaketa;
int ald_lokala;

// Helper vars
 
char adding_file_type[100];



// States

int perspective_mode; 
int transform_camera; 
int camera_view; 
int analisis_mode; 
int backface_culling;
int draw_normals; 

// Projection matrix

double MProjection[16]; 
double MCsr[16]; 
double Modelview[16]; 
double test_variable[16];