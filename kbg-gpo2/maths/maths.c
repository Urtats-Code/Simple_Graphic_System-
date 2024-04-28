#include "maths.h"
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

    int linear_index_m1 ,linear_index_m2;
    int row, col, k;
    double res;
    
    for (row = 0; row < 4; row++){

        for (col = 0; col < 4; col++){
            res = 0.0;
            for (k = 0; k < 4; k++){

                linear_index_m1 = 4 * row + k; 
                linear_index_m2 = 4 * k + col; 

                res += m1[  linear_index_m1 ] * m2[ linear_index_m2 ] ;
                helper_matrix [ 4 * row + col ] = res;

            }
        }
    }
    
}


void printM( double *m ){
    
    int i;

    printf("Matrix:\n");

    for( i = 0; i < 16 ; i ++ ) {
        
        printf("%f, ",  m[ i ]);
        if( i == 3 || i == 7 || i == 11 || i == 15 )  printf("\n"); 

    }

} 