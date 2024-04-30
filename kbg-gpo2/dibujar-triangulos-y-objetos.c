//	Program developed by

//	Informatika Fakultatea
//	Euskal Herriko Unibertsitatea
//	http://www.ehu.eus/if

//  Author: Urtats Berrocal

// Version: Caamara v0

// Execute project using ./execute.sh

# ifdef __APPLE__
# include <OpenGL/gl.h>
# include <stdlib.h>
# else
# include <GL/gl.h>
# endif
# ifdef __APPLE__
# include <GLUT/glut.h>
# else
# include <GL/glut.h>
# endif

#include <stdio.h>
#include <string.h>

// Geometry loading
#include "common_structures.h"

// Global and helper variables 
#include "helper_variables.h"

// Mathematical functions 
#include "maths/maths.h"
#include "maths/object_mutations.h"

// Transformations 
#include "./transformation_functions/transformations.h"




// testuraren informazioa
// información de textura

extern int load_ppm(char *file, unsigned char **bufferptr, int *dimxptr, int *dimyptr);
unsigned char *bufferra;
int dimx, dimy;
int indexx;

char fitxiz[100];


/**
 ************************************************************************ 
 *                           LOAD AND RENDER ITEMS
 ************************************************************************  
 */


unsigned char *color_textura(float u, float v)
{
    int desplazamendua;
    char *lag;
    int textura_x, textura_y;

    textura_x = u * (dimx - 1);
    textura_x = textura_x % dimx;

    textura_y = (1.0 - v) * (dimy - 1);
    textura_y = textura_y % dimy;

    desplazamendua = textura_y * dimx + textura_x;
    lag = (unsigned char *)bufferra; // pixel on the left and top
    return (lag + 3 * desplazamendua);
}

void dibujar_linea_z(int linea, float c1x, float c1z, float c1u, float c1v, float c2x, float c2z, float c2u, float c2v)
{
    float xkoord, zkoord;
    float u, v;
    float t, deltat;
    unsigned char r, g, b;
    unsigned char *colorv;

    glBegin(GL_POINTS);
    
    deltat = 1.0/(c2x-c1x);
    
    for (t = 1.0; t >= 0; t -= deltat)
    {
        xkoord = c1x * t + (1 - t) * c2x;
        zkoord = c1z * t + (1 - t) * c2z;

        u = c1u * t + (1 - t) * c2u;
        v = c1v * t + (1 - t) * c2v;
        colorv = color_textura(u, v);
        r = colorv[0];
        g = colorv[1];
        b = colorv[2];
        glColor3ub(r, g, b);
        glVertex3f(xkoord, linea, zkoord);
        // TODO
        // zkoord, u eta v berriak kalkulatu eskuineko puntuarentzat
        // calcular zkoord, u y v del siguiente pixel
    }
    glEnd();
}

void punto_de_corte(punto *p_arriba_ptr, punto *p_abajo_ptr, int altura, punto *p_centro_prt)
{
    float resta_y = (p_arriba_ptr->y - p_abajo_ptr->y);

    if (resta_y == 0)
    {
        *p_centro_prt = *p_arriba_ptr;
        return;
    }

    float resta_y_altura = p_arriba_ptr->y - altura;
    p_centro_prt->y = altura;

    float resta_x = (p_arriba_ptr->x - p_abajo_ptr->x);
    p_centro_prt->x = -(resta_x * resta_y_altura) / resta_y + p_arriba_ptr->x;

    float resta_u = (p_arriba_ptr->u - p_abajo_ptr->u);
    p_centro_prt->u = -(resta_u * resta_y_altura) / resta_y + p_arriba_ptr->u;

    float resta_z = (p_arriba_ptr->z - p_abajo_ptr->z);
    p_centro_prt->z = -(resta_z * resta_y_altura) / resta_y + p_arriba_ptr->z;

    float resta_v = (p_arriba_ptr->v - p_abajo_ptr->v);
    p_centro_prt->v = -(resta_v * resta_y_altura) / resta_y + p_arriba_ptr->v;
}

void dibujar_triangulo(triobj *optr, int i)
{
    hiruki *tptr;

    punto *pgoiptr, *pbeheptr, *perdiptr, corte1, corte2;
    float x1, h1, z1, u1, v1, x2, h2, z2, u2, v2, x3, h3, z3, u3, v3;
    float c1x, c1z, c1u, c1v, c2x, c2z, c2u, c2v;
    int linea;
    float cambio1, cambio1z, cambio1u, cambio1v, cambio2, cambio2z, cambio2u, cambio2v;
    punto p1, p2, p3;

    if (i >= optr->num_triangles)
        return;
    
    tptr = optr->triptr + i;

    // Multiply points by the transformation matrix

    mxp(&p1, optr->mptr->m, tptr->p1);
    mxp(&p2, optr->mptr->m, tptr->p2);
    mxp(&p3, optr->mptr->m, tptr->p3);

    if (lineak == 1)
    {
        glBegin(GL_POLYGON);
        glVertex3d(p1.x, p1.y, p1.z);
        glVertex3d(p2.x, p2.y, p2.z);
        glVertex3d(p3.x, p3.y, p3.z);
        glEnd();
        return;
    }

    if (p1.y > p2.y)
    {
        pgoiptr = &(p1);
        pbeheptr = &(p2);
    }
    else
    {
        pgoiptr = &(p2);
        pbeheptr = &(p1);
    }
    if (p3.y > pgoiptr->y)
    {
        perdiptr = pgoiptr;
        pgoiptr = &(p3);
    }
    else
    {
        if (p3.y < pbeheptr->y)
        {
            perdiptr = pbeheptr;
            pbeheptr = &(p3);
        }
        else
        {
            perdiptr = &(p3);
        }
    }


    for (i = pgoiptr->y; i > perdiptr->y; i = i - 1)
    {

        punto_de_corte(pgoiptr, perdiptr, i, &corte1);
        punto_de_corte(pgoiptr, pbeheptr, i, &corte2);

        if (corte1.x > corte2.x)
        {
            dibujar_linea_z(i, corte2.x, corte2.z, corte2.u, corte2.v, corte1.x, corte1.z, corte1.u, corte1.v);
        }
        else
        {
            dibujar_linea_z(i, corte1.x, corte1.z, corte1.u, corte1.v, corte2.x, corte2.z, corte2.u, corte2.v);
        }

    }


    for (i = perdiptr->y; i > pbeheptr->y; i = i - 1)
    {
        punto_de_corte(pgoiptr, pbeheptr, i, &corte1);
        punto_de_corte(perdiptr, pbeheptr, i, &corte2);

        if (corte1.x > corte2.x)
        {

            dibujar_linea_z(i, corte2.x, corte2.z, corte2.u, corte2.v, corte1.x, corte1.z, corte1.u, corte1.v);

        }
        else
        {

            dibujar_linea_z(i, corte1.x, corte1.z, corte1.u, corte1.v, corte2.x, corte2.z, corte2.u, corte2.v);

        }

    }
}

static void marraztu(void)
{
    float u, v;
    int i, j;
    triobj *auxptr;
    /*
    unsigned char* colorv;
    unsigned char r,g,b;
    */

    // marrazteko objektuak behar dira
    // no se puede dibujar sin objetos
    if (foptr == 0) return;

    // clear viewport...
    if (objektuak == 1) 
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    else
    {
        if (denak == 0)
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-500.0, 500.0, -500.0, 500.0, -500.0, 500.0);


    triangulosptr = sel_ptr->triptr;

    if (objektuak == 1)
    {
        if (denak == 1)
        {
            for (auxptr = foptr; auxptr != 0; auxptr = auxptr->hptr)
            {
                for (i = 0; i < auxptr->num_triangles; i++)
                {
                    dibujar_triangulo(auxptr, i);
                }
            }
        }
        else
        {
            for (i = 0; i < sel_ptr->num_triangles; i++)
            {
                dibujar_triangulo(sel_ptr, i);
            }
        }
    }
    else
    {
        dibujar_triangulo(sel_ptr, indexx);
    }
    glFlush();
}

void read_from_file( char *fitx, triobj **list_ptr )
{
    int i, retval;
    triobj *optr;

    // printf("%s fitxategitik datuak hartzera\n",fitx);
    optr = (triobj *) malloc(sizeof(triobj));
    
    retval = cargar_triangulos(fitx, &(optr -> num_triangles), &(optr->triptr));
    if (retval != 1)
    {
        printf("%s fitxategitik datuak hartzerakoan arazoak izan ditut\n    Problemas al leer\n", fitxiz);
        free(optr);
    }
    else
    {
        triangulosptr = optr->triptr;

        // printf("objektuaren matrizea...\n");

        optr->mptr = (mlist *) malloc( sizeof( mlist ) );

        transform_into_identity_matrix( &( optr -> mptr -> m[0] ) ); 


        // Null head pointer
        optr -> mptr -> hptr = 0;

        // printf("objektu zerrendara doa informazioa...\n");

        // Asignt head pointer to the list pointer 
        optr->hptr = *list_ptr;
        // Set optr as the first item on the list
        *list_ptr = optr;

    }

    printf("datuak irakurrita\nLecura finalizada\n");
}


/**
 ************************************************************************ 
 *                           ITEM TRANSFORMATIONS
 ************************************************************************  
 */

void scale_object( int direction ) {

    double herlper_matrix[ 16 ] = { 0 }; 

    transform_into_identity_matrix( &herlper_matrix[ 0 ] ); 

    scale( ( double * ) &( herlper_matrix ), direction );

    if( ald_lokala ) M_Right( ( double * ) &herlper_matrix ); 
    else  M_Left( ( double * ) &herlper_matrix ); 

}

void x_aldaketa(int dir)
{

    double herlper_matrix[ 16 ] = { 0 }; 

    transform_into_identity_matrix( &herlper_matrix[ 0 ] ); 

    if( aldaketa == TRANSLATE ) translate( ( double * ) &herlper_matrix, X_AXIS, dir ); 
    if( aldaketa == ROTATE )    rotate(    ( double * ) &herlper_matrix, X_AXIS, dir );
    
    if( ald_lokala ) M_Right( ( double * ) &herlper_matrix ); 
    else  M_Left( ( double * ) &herlper_matrix ); 

}

void y_aldaketa(int dir)
{

    double herlper_matrix[ 16 ] = { 0 }; 

    transform_into_identity_matrix( &herlper_matrix[ 0 ] ); 

    if( aldaketa == TRANSLATE ) translate( ( double * ) &herlper_matrix, Y_AXIS, dir ); 
    if( aldaketa == ROTATE )    rotate(    ( double * ) &herlper_matrix, Y_AXIS, dir );

    if( ald_lokala ) M_Right( ( double * ) &herlper_matrix ); 
    else  M_Left( ( double * ) &herlper_matrix ); 


}

void z_aldaketa(int dir)
{


    double herlper_matrix[ 16 ] = { 0 }; 

    transform_into_identity_matrix( &herlper_matrix[ 0 ] ); 

    if( aldaketa == TRANSLATE ) translate( ( double * ) &herlper_matrix, Z_AXIS, dir ); 
    if( aldaketa == ROTATE )    rotate(    ( double * ) &herlper_matrix, Z_AXIS, dir );

    if( ald_lokala ) M_Right( ( double * ) &herlper_matrix ); 
    else  M_Left( ( double * ) &herlper_matrix ); 


}

void undo()
{

    int not_initial_position = sel_ptr -> mptr -> hptr != 0;

    if( not_initial_position ){
        mlist* current_matrix = sel_ptr -> mptr;
        sel_ptr -> mptr = current_matrix -> hptr;
    }

}



// This function will be called whenever the user pushes one key
static void teklatua(unsigned char key, int x, int y)
{

    int retval;
    int i;
    FILE *obj_file;

    switch (key)
    {
    case 13:
        if (foptr != 0) // objekturik ez badago ezer ez du egin behar
                        // si no hay objeto que no haga nada
        {
            indexx++; // azkena bada lehenengoa bihurtu
                      // pero si es el último? hay que controlarlo!
            if (indexx == sel_ptr->num_triangles)
            {
                indexx = 0;
                if ((denak == 1) && (objektuak == 0))
                {
                    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
                    glFlush();
                }
            }
        }
        break;

    case 'd':

        if (denak == 1) denak = 0;
        else denak = 1;
        break;

    case 'o':

        if (objektuak == 1) objektuak = 0;
        else objektuak = 1;  
        break;

    case 'l':

        if (lineak == 1) lineak = 0;
        else lineak = 1;
        break;


    case TRANSLATE:

        printf(" You are now in TRANSLATING MODE \n");
        aldaketa = TRANSLATE;
        break;

    case ROTATE:

        printf(" You are now in ROTATING MODE \n");
        aldaketa = ROTATE;
        break;

    case EXPAND: 

        printf(" You are now in EXPANDING ITEM \n");
        scale_object( 1 );
        break;

    case RETRACT: 

        printf(" You are now in RETRACTING ITEM \n");
        scale_object( 0 );
        break;

    case 'g':

        if (ald_lokala == 1) ald_lokala = 0;
        else ald_lokala = 1;
        break;

    case 'x':
        x_aldaketa(1);
        break;

    case 'y':
        y_aldaketa(1);
        break;

    case 'z':
        z_aldaketa(1);
        break;

    case 'X':
        x_aldaketa(0);
        break;

    case 'Y':
        y_aldaketa(0);
        break;

    case 'Z':
        z_aldaketa(0);
        break;

    case 'u':
        undo();
        break;
    
    case 'f':
        /*Ask for file*/
        
        printf("Introduce file name: \n");
        scanf("%s", &(fitxiz[0]));

        printf("Introduce object type:  \n  1 - 'object'  \n 2 - 'camera' ");
        scanf("%s", &(adding_file_type[0]) );

        // Ads the item to the object items

        if( strcmp(adding_file_type, ADDING_OBJECT)  ) read_from_file( fitxiz, &foptr );
        else read_from_file( fitxiz, &fcamprt );

        indexx = 0;

        break;
        /* case 'S':  // save to file
             printf("idatzi fitxategi izena\n");
             scanf("%s", &(fitxiz[0]));
                 if ((obj_file = fopen(fitxiz, "w")) == NULL)
                          {
                          printf("ezin fitxategia ireki\n");
                          }
                      else
                          {
                          for (i =0; i < sel_ptr->num_triangles; i++)
                             {
                             fprintf(obj_file,"t %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
                                  sel_ptr->triptr[i].p1.x-250, sel_ptr->triptr[i].p1.y-250, sel_ptr->triptr[i].p1.z,
                                  sel_ptr->triptr[i].p1.u, sel_ptr->triptr[i].p1.v,
                                  sel_ptr->triptr[i].p2.x-250, sel_ptr->triptr[i].p2.y-250, sel_ptr->triptr[i].p2.z,
                                  sel_ptr->triptr[i].p2.u, sel_ptr->triptr[i].p2.v,
                                  sel_ptr->triptr[i].p3.x-250, sel_ptr->triptr[i].p3.y-250, sel_ptr->triptr[i].p3.z,
                                  sel_ptr->triptr[i].p3.u, sel_ptr->triptr[i].p3.v );
                             }
                          fclose(obj_file);
                          }
                 break; */
    case 9:             
    
        /* <TAB> */

        // If theres no item does nothing

        if (foptr != 0)                         
        {
            sel_ptr = sel_ptr->hptr;
            /*The selection is circular, thus if we move out of the list we go back to the first element*/
            if (sel_ptr == 0)
                sel_ptr = foptr;
            indexx = 0; // the selected polygon is the first one
        }
        break;
    case 27: // <ESC>
        exit(0);
        break;
    default:
        printf("%d %c\n", key, key);
    }

    // The screen must be drawn to show the new triangle

    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    int retval;

    printf(" Triangeluak: barneko puntuak eta testura\n Triángulos con puntos internos y textura \n");
    printf("Press <ESC> to finish\n");
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("KBG/GO praktika");

    glutDisplayFunc(marraztu);
    glutKeyboardFunc(teklatua);
    /* we put the information of the texture in the buffer pointed by bufferra. The dimensions of the texture are loaded into dimx and dimy */
    retval = load_ppm("foto_urtats.ppm", &bufferra, &dimx, &dimy);
    if (retval != 1)
    {
        printf("Ez dago texturaren fitxategia (foto_urtats.ppm)\n");
        exit(-1);
    }

    glClearColor(0.0f, 0.0f, 0.7f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST); // activar el test de profundidad (Z-buffer)

    // Custom options
    denak = 0;
    lineak = 1;
    ald_lokala = 1;
    objektuak = 1;

    // Object list
    foptr = 0;
    sel_ptr = 0;

    // Camera list
    fcamprt = 0;
    cam_ptr = 0;

    // Starting transformation 
    aldaketa = ROTATE;



    if (argc > 1) 
    { 
        // Cameras 
        read_from_file( "camara.txt", &fcamprt);

        // Object
        read_from_file( argv[1],      &foptr );
    }
    else 
    {
        // Cameras 
        read_from_file( "camara.txt", &fcamprt);

        // Objects 
        read_from_file( "z.txt",      &foptr );
        read_from_file( "k.txt",      &foptr );

        foptr -> mptr -> m[ TX ] = 250; 

    }

    glutMainLoop();

    return 0;
}
