#include "../common_structures.h"
#include "maths.h"
#include "math.h"
#include <stdio.h> 

/**
 *  Identity matrix 
    ( 1, 0, 0, 0, ) 
    ( 0, 1, 0, 0, ) 
    ( 0, 0, 1, 0, ) 
    ( 0, 0, 0, 1  ) 
 */

void transform_into_identity_matrix( double *m ) {
    
    int i, j ; 

    for( i = 0 ; i < 16 ; i ++ ){ 
        
        if( i == 0 || i == 5 || i == 10 || i == 15 ) m[ i ] = 1.0;
        else m[ i ] = 0.0;

    }

}

/**
 *  
    Matrix m1:         Matrix m2:         Matrix m1: 
    ( 1  2  3  4 )      ( 5  9  13 17 )   ( 130  238  346  454 )
    ( 5  6  7  8 )      ( 6  10 14 18 )   ( 318  554  790 1026 )
    ( 9 10 11 12 )  X   ( 7  11 15 19 ) = ( 506  870 1234 1598 )
    (13 14 15 16 )      ( 8  12 16 20 )   ( 694 1206 1718 2230 )
 */

void MxM( double *helper_matrix,  double *m1, double *m2 ) {

    double x11, x12, x13, x14;  // Row values
    double x21, x22, x23, x24;  // Row values
    double x31, x32, x33, x34;  // Row values
    double x41, x42, x43, x44;  // Row values

    x11 = (m1[0] * m2[0]) + (m1[1] * m2[4]) + (m1[2] * m2[8]) + (m1[3] * m2[12]);
    x12 = (m1[0] * m2[1]) + (m1[1] * m2[5]) + (m1[2] * m2[9]) + (m1[3] * m2[13]);
    x13 = (m1[0] * m2[2]) + (m1[1] * m2[6]) + (m1[2] * m2[10]) + (m1[3] * m2[14]);
    x14 = (m1[0] * m2[3]) + (m1[1] * m2[7]) + (m1[2] * m2[11]) + (m1[3] * m2[15]);

    x21 = (m1[4] * m2[0]) + (m1[5] * m2[4]) + (m1[6] * m2[8]) + (m1[7] * m2[12]);
    x22 = (m1[4] * m2[1]) + (m1[5] * m2[5]) + (m1[6] * m2[9]) + (m1[7] * m2[13]);
    x23 = (m1[4] * m2[2]) + (m1[5] * m2[6]) + (m1[6] * m2[10]) + (m1[7] * m2[14]);
    x24 = (m1[4] * m2[3]) + (m1[5] * m2[7]) + (m1[6] * m2[11]) + (m1[7] * m2[15]);

    x31 = (m1[8] * m2[0]) + (m1[9] * m2[4]) + (m1[10] * m2[8]) + (m1[11] * m2[12]);
    x32 = (m1[8] * m2[1]) + (m1[9] * m2[5]) + (m1[10] * m2[9]) + (m1[11] * m2[13]);
    x33 = (m1[8] * m2[2]) + (m1[9] * m2[6]) + (m1[10] * m2[10]) + (m1[11] * m2[14]);
    x34 = (m1[8] * m2[3]) + (m1[9] * m2[7]) + (m1[10] * m2[11]) + (m1[11] * m2[15]);

    x41 = (m1[12] * m2[0]) + (m1[13] * m2[4]) + (m1[14] * m2[8]) + (m1[15] * m2[12]);
    x42 = (m1[12] * m2[1]) + (m1[13] * m2[5]) + (m1[14] * m2[9]) + (m1[15] * m2[13]);
    x43 = (m1[12] * m2[2]) + (m1[13] * m2[6]) + (m1[14] * m2[10]) + (m1[15] * m2[14]);
    x44 = (m1[12] * m2[3]) + (m1[13] * m2[7]) + (m1[14] * m2[11]) + (m1[15] * m2[15]);

    helper_matrix[0] = x11;
    helper_matrix[1] = x12;
    helper_matrix[2] = x13;
    helper_matrix[3] = x14;

    helper_matrix[4] = x21;
    helper_matrix[5] = x22;
    helper_matrix[6] = x23;
    helper_matrix[7] = x24;

    helper_matrix[8] = x31;
    helper_matrix[9] = x32;
    helper_matrix[10] = x33;
    helper_matrix[11] = x34;

    helper_matrix[12] = x41;
    helper_matrix[13] = x42;
    helper_matrix[14] = x43;
    helper_matrix[15] = x44;
    
}


void printM( double *m, char *custom_message ){
    
    int i;

    printf(" %s :\n", custom_message );

    for( i = 0; i < 16 ; i ++ ) {
        
        printf("%f, ",  m[ i ]);
        if( i == 3 || i == 7 || i == 11 || i == 15 )  printf("\n"); 

    }

} 


/**

 ( x1 y1 z1 e1 )        ( p1 )
 ( x2 y2 z2 e2 )   x    ( p2 )
 ( x3 y3 z3 e3 )        ( p3 )
 ( 0  0  0  1  )        ( 1  )

 */

int mxp(punto *pptr, double m[16], punto p) {

    double x1,y1,z1,w;

    // pptr->x = m[0] * p.x + m[1] * p.y + m[2] *  p.z + m[3];
    // pptr->y = m[4] * p.x + m[5] * p.y + m[6] *  p.z + m[7];
    // pptr->z = m[8] * p.x + m[9] * p.y + m[10] * p.z + m[11];

    x1 = m[0]*p.x + m[1]*p.y + m[2]*p.z   + 1 * m[3];
    y1 = m[4]*p.x + m[5]*p.y + m[6]*p.z   + 1 * m[7];
    z1 = m[8]*p.x + m[9]*p.y + m[10]*p.z  + 1 * m[11];
    w = m[12]*p.x + m[13]*p.y + m[14]*p.z + 1 * m[15];

    if( w <= 0 ) return -1; 
 
    x1 = x1/w;
    y1 = y1/w;
    z1 = z1/w;


    pptr->x = x1;
    pptr->y = y1;
    pptr->z = -z1;

    pptr->u = p.u;
    pptr->v = p.v;

    return 0;

}

void mxpunot_2(punto *pptr, double m[16], punto p) {

    double x1,y1,z1,w;

    x1 = m[0]*p.x + m[1]*p.y + m[2]*p.z   + 1 * m[3];
    y1 = m[4]*p.x + m[5]*p.y + m[6]*p.z   + 1 * m[7];
    z1 = m[8]*p.x + m[9]*p.y + m[10]*p.z  + 1 * m[11];


    pptr->x = x1;
    pptr->y = y1;
    pptr->z = z1;

    pptr->u = p.u;
    pptr->v = p.v;

}

int mxpunto(double *pptr, double m[16], double *p ) {

    double x1,y1,z1,w;

    // pptr->x = m[0] * p.x + m[1] * p.y + m[2] *  p.z + m[3];
    // pptr->y = m[4] * p.x + m[5] * p.y + m[6] *  p.z + m[7];
    // pptr->z = m[8] * p.x + m[9] * p.y + m[10] * p.z + m[11];

    x1 = m[0]*p[0] + m[1]*p[ 1 ] + m[2]*p[ 2 ]   + 1 * m[3];
    y1 = m[4]*p[0] + m[5]*p[ 1 ] + m[6]*p[ 2 ]   + 1 * m[7];
    z1 = m[8]*p[0] + m[9]*p[ 1 ] + m[10]*p[ 2 ]  + 1 * m[11];
    w = m[12]*p[0] + m[13]*p[ 1 ] + m[14]*p[ 2 ] + 1 * m[15];

    if( w <= 0 ) return -1; 
 
    x1 = x1/w;
    y1 = y1/w;
    z1 = z1/w;


    pptr[0] = x1;
    pptr[1] = y1;
    pptr[2] = -z1;


    return 0;

}

/**

 ( x1 y1 z1 e1 )        ( v1 )
 ( x2 y2 z2 e2 )   x    ( v2 )
 ( x3 y3 z3 e3 )        ( v3 )
 ( 0  0  0  1  )        ( 0  )

 */

void mxv(punto *pptr, double m[16], punto v)
{

    pptr -> x = ( m[0] * v.x ) + ( m[1] * v.y )  + ( m[2] * v.z );
    pptr -> y = ( m[4] * v.x ) + ( m[5] * v.y )  + ( m[6] * v.z );
    pptr -> z = ( m[8] * v.x ) + ( m[9] * v.y )  + ( m[10] * v.z );

}


void mxvector(double *saving, double *m, double *v) {

    saving[ 0 ] = ( m[ 0 ] * v[ 0 ] ) + ( m[1] * v[ 1 ] )  + ( m[2] * v[ 2 ] ) ;
    saving[ 1 ] = ( m[ 4 ] * v[ 0 ] ) + ( m[5] * v[ 1 ] )  + ( m[6] * v[ 2 ] ) ;
    saving[ 2 ] = ( m[ 8 ] * v[ 0 ] ) + ( m[9] * v[ 1 ] )  + ( m[10] * v[ 2 ] );


}

void mxvector2(double *saving, double *m, double *v)
{

    saving[ 0 ] = ( m[ 0 ] * v[ 0 ] ) + ( m[1] * v[ 1 ] )  + ( m[2] * v[ 2 ] )  + ( m[3] * v[3]);
    saving[ 1 ] = ( m[ 4 ] * v[ 0 ] ) + ( m[5] * v[ 1 ] )  + ( m[6] * v[ 2 ] )  + ( m[7] * v[3]);
    saving[ 2 ] = ( m[ 8 ] * v[ 0 ] ) + ( m[9] * v[ 1 ] )  + ( m[10] * v[ 2 ] ) + ( m[11] * v[3]);
    saving[ 3 ] = ( m[ 12 ] * v[ 0 ] ) + ( m[13] * v[ 1 ] )  + ( m[14] * v[ 2 ] ) + ( m[15 ] * v[3]);

}

/**

 ( x1 y1 z1 e1 )        ( v1 )
 ( x2 y2 z2 e2 )   x    ( v2 )
 ( x3 y3 z3 e3 )        ( v3 )
 ( 0  0  0  1  )        ( 0  )

 */

void MxV( double *pptr, double *m, double  *v )
{
    pptr[ 0 ] = ( m[0] * v[ 0 ] ) + ( m[1] * v[ 1 ] )  + ( m[2] * v[ 2 ] );
    pptr[ 1 ] = ( m[4] * v[ 0 ] ) + ( m[5] * v[ 1 ] )  + ( m[6] * v[ 2 ] );
    pptr[ 2 ] = ( m[8] * v[ 0 ] ) + ( m[9] * v[ 1 ] )  + ( m[10] * v[ 2 ] );

}

double  dot( double *v1,  double *v2 ){

    return ( v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2] );

}

void normalize( double *v1 ){

    double sum = ( v1[ 0 ] * v1[ 0 ] ) + ( v1[ 1 ] * v1[ 1 ] ) + ( v1[ 2 ] * v1[ 2 ] );
    double root = sqrt( sum );

    if( root > 0 ){
        v1[ 0 ] = v1[ 0 ] / root; 
        v1[ 1 ] = v1[ 1 ] / root; 
        v1[ 2 ] = v1[ 2 ] / root; 
    }


}


void vector_product( double *saving_v, double *v1, double *v2 ){

    saving_v[ 0 ] = ( v1[ 1 ] * v2[ 2 ] ) - ( v1[ 2 ] * v2[ 1 ] ); 
    saving_v[ 1 ] = ( v1[ 2 ] * v2[ 0 ] ) - ( v1[ 0 ] * v2[ 2 ] ); 
    saving_v[ 2 ] = ( v1[ 0 ] * v2[ 1 ] ) - ( v1[ 1 ] * v2[ 0 ] ); 

}

void calculate_triangle_normal( double *saving_vector, hiruki *triangle ) {

    double v1 [ 3 ] = {  ( triangle -> p2.x - triangle -> p1.x ) , ( triangle -> p2.y - triangle -> p1.y ) ,  ( triangle -> p2.z - triangle -> p1.z )} ; 
    double v2 [ 3 ] = {  ( triangle -> p3.x - triangle -> p1.x ) , ( triangle -> p3.y - triangle -> p1.y ) ,  ( triangle -> p3.z - triangle -> p1.z )} ; 
    
    vector_product( saving_vector, &v1, &v2 );

    normalize( saving_vector );

}