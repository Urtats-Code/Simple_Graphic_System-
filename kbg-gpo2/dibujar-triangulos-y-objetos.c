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
#include <math.h>

// Geometry loading
#include "common_structures.h"

// Global and helper variables 
#include "helper_variables.h"

// Mathematical functions 
#include "maths/maths.h"
#include "maths/object_mutations.h"

// Transformations 
#include "./transformation_functions/transformations.h"




void calculate_triangle_centroid( double *centroid, hiruki *triangle_ptr ){

    double sumX, sumY, sumZ; 

    sumX = triangle_ptr->p1.x + triangle_ptr->p2.x + triangle_ptr->p3.x;
    sumY = triangle_ptr->p1.y + triangle_ptr->p2.y + triangle_ptr->p3.y;
    sumZ = triangle_ptr->p1.z + triangle_ptr->p2.z + triangle_ptr->p3.z;

    centroid[0] = sumX / 3;
    centroid[1] = sumY / 3;
    centroid[2] = sumZ / 3;

}

void printV( double *m, char *custom_message ){
    
    int i;

    printf(" %s :\n", custom_message );

    printf( "( ");
    for( i = 0; i < 3 ; i ++ ){
        printf( "  %f ", m[i] ); 
    }
    printf( " ) \n");
}


int is_visible( triobj *view_ptr, triobj *looking_ptr, int triangle_index ){

    hiruki *triangle; 
    punto projected_point; 
    double angle; 
    double normal[3];
    double view_vector[ 3 ] = { 0 };
    double E_cam[ 3 ] = { 0 }; 

    triangle = ( looking_ptr -> triptr  ) + triangle_index;

    mxvector( ( double * ) &normal , ( double * ) &Modelview, ( double * ) &( triangle -> N ) );

    if( perspective_mode ) {

        mxpunot_2( &projected_point,  &Modelview,  triangle -> p1 ); 

        view_vector[ 0 ] =  -( projected_point.x ) ;
        view_vector[ 1 ] =  -( projected_point.y ) ;
        view_vector[ 2 ] =  -( projected_point.z ) ;

        angle = dot( ( double * ) &normal  , ( double * ) &view_vector ); 

            glColor3ub( 255, 255 , 255 );  
        if( angle < 0 ){
            glColor3ub( 255, 0 , 0 );  
        } 
        
        if( !backface_culling ) return 1; 

        return ( angle >= 0 ); 


    } else { 

        glColor3ub( 255, 255 , 255 );  
        
        if ( normal[ 2 ] < 0 ) {
            glColor3ub( 255, 0 , 0 );  
        } 

        if( !backface_culling ) return 1; 

        return (  normal[ 2 ] >= 0); 
    }
    

} 

void rotate_analisis( double *rotation_M, double *at, int direction, char axis ) {

    double cosine, sine; 
    double x, y, z; 
    double M_helper[ 16 ]   = { 0 }; 
    double M_At[ 16 ]       = { 0 }; 
    double minus_M_At[ 16 ] = { 0 }; 
    double M_rotate[ 16 ]   = { 0 };

    if( direction == POSITIVE_DIRECTION ){
        cosine = cos( ROTATION_ALPHA );
        sine   = sin( ROTATION_ALPHA );
    } else { 
        cosine = cos( - ROTATION_ALPHA );
        sine   = sin( - ROTATION_ALPHA );
    }

    transform_into_identity_matrix( ( double * ) &M_At );
    transform_into_identity_matrix( ( double * ) &minus_M_At );
    transform_into_identity_matrix( ( double * ) &M_helper );

    M_At[ 3 ] = sel_ptr -> mptr -> m[ 3 ];
    M_At[ 7 ] = sel_ptr -> mptr -> m[ 7 ];
    M_At[ 11 ] = sel_ptr -> mptr -> m[ 11 ];

    minus_M_At[ 3 ] =  -M_At[ 3 ];
    minus_M_At[ 7 ] =  -M_At[ 7 ];
    minus_M_At[ 11 ] =  -M_At[ 11 ];

    if( axis  ==  X_AXIS ) {
        x = cam_ptr -> mptr -> m[ 0 ]; 
        y = cam_ptr -> mptr -> m[ 4 ]; 
        z = cam_ptr -> mptr -> m[ 8 ]; 
    }

    if( axis  ==  Y_AXIS ) {
        x = cam_ptr -> mptr -> m[ 1 ]; 
        y = cam_ptr -> mptr -> m[ 5 ]; 
        z = cam_ptr -> mptr -> m[ 9 ]; 
    }

    if( axis  ==  Z_AXIS ) {
        x = cam_ptr -> mptr -> m[ 2 ]; 
        y = cam_ptr -> mptr -> m[ 6 ]; 
        z = cam_ptr -> mptr -> m[ 10 ]; 
    }

    M_helper[ 0 ] = cosine + ( 1 - cosine ) * ( x * x );
    M_helper[ 1 ] = ( 1 - cosine ) * x * y  - z * sine;
    M_helper[ 2 ] = ( 1 - cosine  ) * x * z + y * sine;


    M_helper[ 4 ] = ( 1 - cosine ) * x * y + z * sine; 
    M_helper[ 5 ] = cosine + ( 1 - cosine ) *  ( y * y );
    M_helper[ 6 ] = ( 1 - cosine ) * y * z - x * sine;


    M_helper[ 8 ] = ( 1 - cosine ) * x * z - y * sine;
    M_helper[ 9 ] = ( 1 - cosine ) * y * z + x * sine;
    M_helper[ 10 ] = cosine + ( 1 - cosine ) * ( z * z );

    
    MxM( ( double * ) &M_rotate,   ( double * ) &M_helper , ( double * ) &minus_M_At);
    MxM( rotation_M, ( double * ) &M_At, ( double * ) &M_rotate);


} 


/**
 * @brief Calculates the system refencen change matrix based on the selected object
 * 
 * ( xx xy xz ( -e*x ) )
 * ( yx yy yz ( -e*y ) )
 * ( zx zy zz ( -e*z ) )
 * ( 0  0  0     1     ) 
 * 
 * @param saving_matrix Where the matrix is goign to be saved 
 * @param selected_camera The selected object ptr
 */

void calculate_MCSR ( double *saving_matrix, triobj *selected_camera ){

    transform_into_identity_matrix( saving_matrix ); 

    double *camera_matrix = ( double *) ( selected_camera -> mptr -> m ); 

    double E[ 4 ] = { camera_matrix[3], camera_matrix[7], camera_matrix[11], 0 }; 
    double X[ 4 ] = { camera_matrix[ 0 ], camera_matrix[ 4 ], camera_matrix[ 8 ],  0 };
    double Y[ 4 ] = { camera_matrix[ 1 ], camera_matrix[ 5 ], camera_matrix[ 9 ],  0 };
    double Z[ 4 ] = { camera_matrix[ 2 ], camera_matrix[ 6 ], camera_matrix[ 10 ], 0 };

    double Ex = -1 * dot( ( double * ) X,  ( double * ) E );
    double Ey = -1 * dot( ( double * ) Y,  ( double * ) E );
    double Ez = -1 * dot( ( double * ) Z,  ( double * ) E );

    // Move camera to the origin 
    // Transpose the matrix

    saving_matrix[ 0 ] = camera_matrix[ 0 ];
    saving_matrix[ 1 ] = camera_matrix[ 4 ];
    saving_matrix[ 2 ] = camera_matrix[ 8 ];

    saving_matrix[ 4 ] = camera_matrix[ 1 ];
    saving_matrix[ 5 ] = camera_matrix[ 5 ];
    saving_matrix[ 6 ] = camera_matrix[ 9 ];

    saving_matrix[ 8 ]  = camera_matrix[ 2 ];
    saving_matrix[ 9 ]  = camera_matrix[ 6 ];
    saving_matrix[ 10 ] = camera_matrix[ 10 ];

    // Set the camera as the origin

    saving_matrix[ 3 ] = Ex;
    saving_matrix[ 7 ] = Ey;
    saving_matrix[ 11 ] = Ez;

}


/**
 * @brief This funtion saves the perspective matrix in the saving matrix 
 * 
 * Perspective camera formula: https://www.songho.ca/opengl/gl_projectionmatrix.html
 * 
 * @param saving_matrix 
 */

void  set_perspective_matrix( double *saving_matrix ) {

    // Values to calculate the perspective matrix ( 91,5 deg )

    double t = 5.13264357; 
    double b = -5.13264357; 
    double l = 5.13264357; 
    double r = -5.13264357; 
    double n = 5.0; 
    double f = 2000.0; 

    transform_into_identity_matrix( saving_matrix ); 

    // First row 

    saving_matrix[ 0 ] = ( 2 * n ) / (  r - l ); 
    saving_matrix[ 2 ] = ( r + l ) / (  r - l );

    // Second row 

    saving_matrix[ 5 ] = ( 2 * n ) / (  t - b );
    saving_matrix[ 6 ] = ( t + b ) / (  t - b );

    // Third row

    saving_matrix[ 10 ]  = - ( f + n ) / ( f - n ); 
    saving_matrix[ 11 ]  = - ( 2 * f * n ) / ( f - n ); 

    // Fourth row 

    saving_matrix[ 14 ]  = -1; 
    saving_matrix[ 15 ]  = 0;    

}


/**
 * @brief  Sets the camera looking to the selected object
 * 
 */
void look_at () {

    double MLookAt[ 16 ] = { 0 }; 
    
    double E [ 3 ]  = { cam_ptr -> mptr -> m[ 3 ], cam_ptr -> mptr -> m[ 7 ], cam_ptr -> mptr -> m[ 11 ]  }; 
    double At[ 3 ]  = { sel_ptr -> mptr -> m[ 3 ], sel_ptr -> mptr -> m[ 7 ], sel_ptr -> mptr -> m[ 11 ]  }; 
    double Up[ 3 ]  = { cam_ptr -> mptr -> m[ 1 ], cam_ptr -> mptr -> m[ 5 ], cam_ptr -> mptr -> m[ 9 ]   }; 

    double Zc[ 3 ]  = { 0 };
    double Xc [ 3 ]  = { 0 };
    double Yc [ 3 ]  = { 0 };
    
    Zc[ 0 ] = - ( E[ 0 ] - At[ 0 ] );
    Zc[ 1 ] = - ( E[ 1 ] - At[ 1 ] );
    Zc[ 2 ] = - ( E[ 2 ] - At[ 2 ] );
    
    normalize( ( double * ) &Zc ); 

    vector_product( ( double * ) &Xc, ( double * ) &Up, ( double * ) &Zc );

    normalize( ( double * ) &Xc ); 

    vector_product( ( double * ) &Yc, ( double * ) &Zc, ( double * ) &Xc );

    cam_ptr -> mptr -> m[ 0 ] = Xc[ 0 ]; 
    cam_ptr -> mptr -> m[ 4 ] = Xc[ 1 ]; 
    cam_ptr -> mptr -> m[ 8 ] = Xc[ 2 ]; 

    cam_ptr -> mptr -> m[ 1 ] = Yc[ 0 ]; 
    cam_ptr -> mptr -> m[ 5 ] = Yc[ 1 ]; 
    cam_ptr -> mptr -> m[ 9 ] = Yc[ 2 ]; 

    cam_ptr -> mptr -> m[ 2 ]  = Zc[ 0 ]; 
    cam_ptr -> mptr -> m[ 6 ]  = Zc[ 1 ]; 
    cam_ptr -> mptr -> m[ 10 ] = Zc[ 2 ]; 

}


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

        // Recibe la posicion de la textura
        colorv = color_textura(u, v);
        r = colorv[0];
        g = colorv[1];
        b = colorv[2];
        glColor3ub(r, g, b);
        glVertex3f(xkoord, linea, zkoord);

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
    int linea, a, b, c;
    float cambio1, cambio1z, cambio1u, cambio1v, cambio2, cambio2z, cambio2u, cambio2v;
    punto p1, p2, p3;

    if (i >= optr->num_triangles)
        return;
    
    tptr = optr -> triptr + i;

    double *n = tptr -> N; 
    
    // Multiply points by the transformation matrix


    a = mxp(&p1, ( double * ) &Modelview, tptr->p1);
    b = mxp(&p2, ( double * ) &Modelview, tptr->p2);
    c = mxp(&p3, ( double * ) &Modelview, tptr->p3);

    if( a == -1 | b == -1 | c == -1 ) return ; 

    if (lineak == 1)
    {

        if (optr != cam_ptr && draw_normals ){

            double normal[ 3 ]; 
            double *tri_normal = &( tptr -> N ); 
            double normal_point[ 3 ] = { tptr -> p1.x + 50*tptr -> N[ 0 ],  tptr -> p1.y + 50*tptr -> N[ 1 ],  tptr -> p1.z + 50*tptr -> N[ 2 ] };
            double transpose_point[ 3 ];

            mxpunto( &transpose_point, &Modelview , &normal_point );

            // Normals working
            glBegin(GL_LINES);
                glVertex3d(p1.x, p1.y, p1.z);
                glVertex3d( transpose_point[ 0 ], transpose_point[ 1 ], transpose_point[ 2 ] );
            glEnd();

        }

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

static void marraztu( void )
{
    float u, v;
    int i, j;
    triobj *auxptr, *view_ptr;

    

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
    
    // Applying logic: MProjection * MCsr * MObject * point

    double *MObject; 
    double M_Helper[ 16 ] = { 0 }; 
    transform_into_identity_matrix( ( double * ) &M_Helper );

    // Set Mprojection  
    transform_into_identity_matrix( ( double * )  &MProjection );

    if ( perspective_mode ) { 

        double t = 5.13264357; 
        double b = -5.13264357; 
        double l = 5.13264357; 
        double r = -5.13264357; 
        double n = 5.0; 
        double f = 2000.0; 
        // set_perspective_matrix( ( double * ) &MProjection ); 
        glFrustum( l, r, b, t, n, f);
    } else { 
        glOrtho(-500.0, 500.0, -500.0, 500.0, -500.0, 500.0);
    } 

    // Set ModelView
    
    transform_into_identity_matrix( ( double * ) &Modelview ); 

    // Set MCSR

    if( camera_view ) {
        MObject = cam_ptr -> mptr -> m; 
        calculate_MCSR( ( double * ) &MCsr, cam_ptr );
    } else { 
        MObject = sel_ptr -> mptr -> m; 
        calculate_MCSR( ( double * ) &MCsr, sel_ptr );
    }



    triangulosptr = sel_ptr -> triptr;

    if( camera_view ) view_ptr = cam_ptr;
    else view_ptr = sel_ptr;  

    if (objektuak == 1)
    {
        if (denak == 1)
        {

            for (auxptr = foptr; auxptr != 0; auxptr = auxptr->hptr)
            {

                MxM( ( double * ) &Modelview, ( double * ) &MCsr, auxptr -> mptr -> m );
                
                for (i = 0; i < auxptr -> num_triangles; i++)
                {
                    if( is_visible( view_ptr, auxptr, i ) ) {
                        dibujar_triangulo(auxptr, i);
                    }
                }
            }

            for (auxptr = fcamprt; auxptr != 0; auxptr = auxptr -> hptr)
            {
                
                MxM( ( double * ) &Modelview, ( double * ) &MCsr, auxptr -> mptr -> m );
                

                for (i = 0; i < auxptr->num_triangles; i++)
                {
                    if( is_visible( view_ptr, auxptr, i ) ) {
                        dibujar_triangulo(auxptr, i);
                    }
                }

            }

        } else {
            
            for (i = 0; i < sel_ptr->num_triangles; i++)
            {
                dibujar_triangulo(sel_ptr, i);
            }

        }
    } else {

        dibujar_triangulo(sel_ptr, indexx);

    }


    glFlush();
}

void read_from_file( char *fitx, triobj **list_ptr, int is_camera )
{
    int i, retval, index;
    triobj *optr;

    // printf("%s fitxategitik datuak hartzera\n",fitx);
    optr = (triobj *) malloc(sizeof(triobj));
    unsigned char *color; 
    color = "c255255255";
    retval = cargar_triangulos_color(fitx, &(optr -> num_triangles), &(optr->triptr), &(color));
    if (retval == 1)
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
        // optr -> mptr -> hptr = 0;

        // Calculate all triangle normals 
        for( index = 0; index < optr -> num_triangles ; index ++  ){
            hiruki *triangle = ( optr -> triptr ) + index; 
            calculate_triangle_normal( ( double * )  triangle -> N , triangle );
        }

        // printf("objektu zerrendara doa informazioa...\n");

        if ( is_camera ){
            optr->hptr = fcamprt;
            fcamprt = optr;
            cam_ptr = *list_ptr;
        } else {
            optr->hptr = foptr;
            foptr = optr;
            sel_ptr = *list_ptr;
        }

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

    if( !ald_lokala && transform_camera ){

        if( aldaketa == ROTATE ) {
            double at[ 3 ] = { sel_ptr -> mptr -> m[ 3 ], sel_ptr -> mptr -> m[ 7 ], sel_ptr -> mptr -> m[ 11 ] };
            rotate_analisis( ( double * ) &herlper_matrix, ( double * ) &at, dir, Y_AXIS );
        }


    } else { 

        if( aldaketa == TRANSLATE ) translate( ( double * ) &herlper_matrix, X_AXIS, dir ); 
        if( aldaketa == ROTATE )    rotate(    ( double * ) &herlper_matrix, X_AXIS, dir, transform_camera );    

    }
    

    if( ald_lokala ) M_Right( ( double * ) &herlper_matrix ); 
    else  M_Left( ( double * ) &herlper_matrix ); 

}

void y_aldaketa(int dir)
{

    double herlper_matrix[ 16 ] = { 0 }; 

    transform_into_identity_matrix( &herlper_matrix[ 0 ] ); 

    if( !ald_lokala && transform_camera ){

        if( aldaketa == ROTATE ){
            double at[ 3 ] = { sel_ptr -> mptr -> m[ 3 ], sel_ptr -> mptr -> m[ 7 ], sel_ptr -> mptr -> m[ 11 ] };
            rotate_analisis( ( double * ) &herlper_matrix, ( double * ) &at, dir, X_AXIS );
        }

    } else { 

        if( aldaketa == TRANSLATE ) translate( ( double * ) &herlper_matrix, Y_AXIS, dir ); 
        if( aldaketa == ROTATE )    rotate(    ( double * ) &herlper_matrix, Y_AXIS, dir, transform_camera );    

    }

    if( ald_lokala ) M_Right( ( double * ) &herlper_matrix ); 
    else  M_Left( ( double * ) &herlper_matrix ); 


}

void z_aldaketa(int dir) {


    double herlper_matrix[ 16 ] = { 0 }; 

    transform_into_identity_matrix( &herlper_matrix[ 0 ] ); 


    if( !ald_lokala && transform_camera ){


        if( aldaketa == ROTATE ){
            double at[ 3 ] = { sel_ptr -> mptr -> m[ 3 ], sel_ptr -> mptr -> m[ 7 ], sel_ptr -> mptr -> m[ 11 ] };
            rotate_analisis( ( double * ) &herlper_matrix, ( double * ) &at, dir, Z_AXIS );
        }

        if( aldaketa == TRANSLATE ) {
            translate( ( double * ) &herlper_matrix, Z_AXIS, dir    ); 
            M_Right( ( double * ) &herlper_matrix ); 
            return ; 
        }

    } else { 

        if( aldaketa == TRANSLATE ) translate( ( double * ) &herlper_matrix, Z_AXIS, dir    ); 
        if( aldaketa == ROTATE )    rotate(    ( double * ) &herlper_matrix, Z_AXIS, dir, transform_camera );

    }

    if( ald_lokala ) M_Right( ( double * ) &herlper_matrix ); 
    else  M_Left( ( double * ) &herlper_matrix ); 


}

void undo() {

    triobj *aux_ptr; 

    if( camera_view ) aux_ptr = cam_ptr; 
    else aux_ptr = sel_ptr;

    int not_initial_position = sel_ptr -> mptr -> hptr != 0;   
    int not_intial_position_camera = cam_ptr -> mptr -> hptr != 0;

    if( not_initial_position ) {

        mlist* current_matrix = sel_ptr -> mptr;
        sel_ptr -> mptr = current_matrix -> hptr;

    } else { 

        if( not_intial_position_camera ) {
            mlist* current_matrix = cam_ptr -> mptr;
            cam_ptr -> mptr = current_matrix -> hptr;
        }

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

        if( ald_lokala ){

            ald_lokala = 0; 
            printf("You are in GLOBAL mode \n");
            if( transform_camera ) {
                look_at(); 
                printf( "You are in ANALISIS mode \n");
            }

        } else {

            ald_lokala = 1;
            printf( "You are in LOCAL mode \n");
            if( transform_camera ) printf( "You are in FLIGHT mode \n" );

        }


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

    case SET_PERSPECTIVE_MDOE: 

        if( perspective_mode ) {

            perspective_mode = 0; 
            printf( "You are in PARALLALEL mode \n");

        } else {

            perspective_mode = 1; 
            printf( "You are in PERSPECTIVE mode \n");
            
        }



        break; 
    
    case SELECT_CAMERA:
        
        if( transform_camera ){
            transform_camera = 0; 
            printf( "You are transforming the OBJECT \n ");
        } else {

            transform_camera = 1; 
            printf( "You are transforming the CAMERA \n ");

            if( !ald_lokala ) look_at();

        }

        break;

    case VIEW_FROM_CAMERA_PERSPECTIVE: 

        if( camera_view ) {

            camera_view = 0; 
            transform_camera = 0; 

            printf( "You are viewing from  the OBJECT PERSPECTIVE  \n ");
            printf( "You are transforming the OBJECT \n ");

        } else {

            camera_view = 1; 
            transform_camera = 1;

            printf( "You are viewing from  the OBJECT CAMERA  \n ");
            printf( "You are transforming the CAMERA \n ");

        }

        break; 
    
    case SET_BAKCFACE_CULLING:

        if( backface_culling ) {

            backface_culling = 0; 
            printf( "BACKFACE CULLING OFF \n" );

        } else {

            backface_culling = 1; 
            printf( "BACKFACE CULLING ON \n" );
        }

        break; 
    case 'm':

        look_at(); 

        break; 
    case 'n':

        if( draw_normals ) {

            draw_normals = 0; 
            printf( "NORMALS OFF \n" );

        } else {

             draw_normals = 1; 
            printf( "NOMALS ON \n" );

        }

        break;

    case 'f':

        /* Ask for file and type */
        
        printf("Introduce file name: \n");
        scanf("%s", &(fitxiz[0]));

        printf("Introduce object type:  \n  1 - 'object'  \n 2 - 'camera' ");
        scanf("%s", &(adding_file_type[0]) );

        // Ads the item to the object items

        if( strcmp( adding_file_type, ADDING_OBJECT )) read_from_file( fitxiz, &foptr, 0 );
        else read_from_file( fitxiz, &fcamprt, 1 );

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

        if( transform_camera ) {

            cam_ptr = cam_ptr -> hptr;

            if ( cam_ptr == 0 ) cam_ptr = fcamprt;

            if( !ald_lokala ) look_at();

            indexx = 0;

        }   else { 
        
            sel_ptr = sel_ptr->hptr;

            if ( sel_ptr == 0 ) sel_ptr = foptr;
            indexx = 0;

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
    glutInitWindowPosition(500, 300);
    glutCreateWindow(" GC Practica ");

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
    denak = 1;
    lineak = 1;
    ald_lokala = 1;
    objektuak = 1;
    draw_normals = 0; 

    // Object list
    foptr = 0;
    sel_ptr = 0;

    // Camera list
    fcamprt = 0;
    cam_ptr = 0;

    // Starting transformation 
    aldaketa = ROTATE;

    // Camera States
    analisis_mode = 0; 
    transform_camera = 1; 
    camera_view = 1; 
    backface_culling = 1;

    // Projections 
    perspective_mode = 1;  // 1 Perspective mode,  0 Parallel Mode

    if (argc > 1) 
    { 

                // Cameras 
        read_from_file( "camara.txt", &fcamprt, 1 );

        fcamprt -> mptr -> m[ TZ ] = -250; 

        for (int i = 1; i < argc; i++) {
            read_from_file( argv[i], &foptr, 0  );
        }


    } else {

        // Cameras 
        read_from_file( "camara.txt", &fcamprt, 1 );

        fcamprt -> mptr -> m[ TZ ] = -250; 

        // Object
        read_from_file( "z.txt",      &foptr,   0  );
        read_from_file( "k.txt",      &foptr,   0  );
        
        foptr -> mptr -> m[ TX ] = 250; 
    }

    glutMainLoop();

    return 0;
}
