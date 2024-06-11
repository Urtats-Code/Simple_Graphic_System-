#include "../common_structures.h"
#include "maths.h"
#include <stdlib.h>
#include <stdio.h>

/*

    Transforms the object itself relative to the world coordinate system.

    Example: If M is a rotation matrix and T is your object's transformation matrix:
    M * T rotates the object around the world's origin.
    Right Multiplication (T * M):

    Transforms the object's local coordinate system.

    Example: If M is a rotation matrix and T is your object's transformation matrix:
    T * M rotates the object around its own local axis.

*/

void M_Left( double *m ) {
    
    double *multiplying_matrix;
    mlist *new_matrix;

    triobj *using_ptr; 

    if( transform_camera ) using_ptr = cam_ptr; 
    else using_ptr = sel_ptr; 

    new_matrix = (mlist *)malloc (sizeof(mlist));

    multiplying_matrix = &(using_ptr->mptr->m[0]); 

    MxM(&(new_matrix->m[0]), multiplying_matrix, m);

    new_matrix->hptr = using_ptr->mptr;
    using_ptr->mptr = new_matrix;

}

void M_Right( double *m ){

    double *multiplying_matrix;
    mlist *new_matrix;

    triobj *using_ptr; 

    if( transform_camera ) using_ptr = cam_ptr; 
    else using_ptr = sel_ptr; 

    new_matrix = (mlist *)malloc (sizeof(mlist));

    multiplying_matrix = (double * ) &(using_ptr -> mptr -> m)  ;

    MxM( ( double *) &(new_matrix-> m) , m, multiplying_matrix);

    new_matrix->hptr = using_ptr->mptr;
    using_ptr->mptr = new_matrix;

}