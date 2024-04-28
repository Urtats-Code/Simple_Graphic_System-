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

    printf(" You are updating the matrix LEFT \n"); 

    new_matrix = (mlist *)malloc (sizeof(mlist));

    multiplying_matrix = &(sel_ptr->mptr->m[0]);

    MxM(&(new_matrix->m[0]), multiplying_matrix, m);

    new_matrix->hptr = sel_ptr->mptr;
    sel_ptr->mptr = new_matrix;

}

void M_Right( double *m ){

    double *multiplying_matrix;
    mlist *new_matrix;

    new_matrix = (mlist *)malloc (sizeof(mlist));

    multiplying_matrix = &(sel_ptr->mptr->m[0]);

    MxM(&(new_matrix->m[0]), m, multiplying_matrix);

    new_matrix->hptr = sel_ptr->mptr;
    sel_ptr->mptr = new_matrix;

    printM( ( double * ) &(sel_ptr -> mptr -> m) );

}