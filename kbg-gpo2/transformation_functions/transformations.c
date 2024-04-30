#include "../helper_variables.h"
#include "transformations.h"
#include <stdio.h>
#include <math.h>

/**
 *  Transaltion matrix, the values tx, ty and tz can be modified 
    ( 1, 0, 0, tx, )
    ( 0, 1, 0, ty, )
    ( 0, 0, 1, tz, )
    ( 0, 0, 0, 1   )
 */

void translate( double *m, char axis, int direction ){

    if( axis == X_AXIS) translateX( m, direction ); 
    if( axis == Y_AXIS) translateY( m, direction ); 
    if( axis == Z_AXIS) translateZ( m, direction ); 

}

void translateX( double *m, int direction ){

    if( direction == POSITIVE_DIRECTION ) m[ TX ] += MOVING_PIXELS; 
    else m[ TX ] -= MOVING_PIXELS;

}

void translateY( double *m, int direction ){

    if( direction == POSITIVE_DIRECTION ) m[ TY ] += MOVING_PIXELS; 
    else m[ TY ] -= MOVING_PIXELS;

}   

void translateZ( double *m, int direction ){

    if( direction == POSITIVE_DIRECTION ) m[ TZ ] += MOVING_PIXELS; 
    else m[ TZ ] -= MOVING_PIXELS;

}


/**
    Rotation matrix
    
    You multiply by cos( alpha ) and sen( alpha ) the vectors that you want to rotate. In other words, you force an object
    to rotate through the axis that that is not being multiplied. 

    Rx

    ( 1,  0,          0,           0 )
    ( 0,  cos(alpha), -sen(alpha), 0 )
    ( 0,  sen(alpha), cos(alpha),  0 )
    ( 0,  0,          0,           1 )

    Ry
    
    ( cos(alpha),  0,  sen(alpha),  0 )
    ( 0,           1,  0,           0 )
    ( -sen(alpha), 1,  cos(alpha),  0 )
    ( 0,           0,  0,           1 )

    Rz

    ( cos(alpha), -sen(alpha), 0,   0 )
    ( sen(alpha),  cos(alpha), 0,   0 )
    ( 0,           0,          1,   0 )
    ( 0,           0,          0,   1 )


    NOTE: 

    cos( alpha ) ==  -cos( alpha )
    sin( alpha ) != -sin( alpha )

 */

void rotate(  double *m, char axis, int direction ){

    if( axis == X_AXIS) rotateX( m, direction ); 
    if( axis == Y_AXIS) rotateY( m, direction ); 
    if( axis == Z_AXIS) rotateZ( m, direction ); 

}

void rotateX( double *rx , int direction ){


    if( direction == POSITIVE_DIRECTION ){

        rx[ 5 ]  =  cos( ROTATION_ALPHA );
        rx[ 6 ]  = -sin( ROTATION_ALPHA );
        rx[ 9 ]  =  sin( ROTATION_ALPHA );
        rx[ 10 ] =  cos( ROTATION_ALPHA );

    } else {

        rx[ 5 ]  =  cos( ROTATION_ALPHA );
        rx[ 6 ]  =  sin( ROTATION_ALPHA );
        rx[ 9 ]  = -sin( ROTATION_ALPHA );
        rx[ 10 ] =  cos( ROTATION_ALPHA );

    }

}

void rotateY( double *ry , int direction ){

    if( direction == POSITIVE_DIRECTION ){

        ry[ 0 ]  =  cos( ROTATION_ALPHA );
        ry[ 2 ]  =  sin( ROTATION_ALPHA );
        ry[ 8 ]  = -sin( ROTATION_ALPHA );
        ry[ 10 ] =  cos( ROTATION_ALPHA );

    } else {

        ry[ 0 ]  =  cos( ROTATION_ALPHA );
        ry[ 2 ]  = -sin( ROTATION_ALPHA );
        ry[ 8 ]  =  sin( ROTATION_ALPHA );
        ry[ 10 ] =  cos( ROTATION_ALPHA );

    }

}

void rotateZ( double *rz , int direction ){

    if( direction == POSITIVE_DIRECTION ){

        rz[ 0 ]  =  cos( ROTATION_ALPHA );
        rz[ 1 ]  = -sin( ROTATION_ALPHA );
        rz[ 4 ]  =  sin( ROTATION_ALPHA );
        rz[ 5 ]  =  cos( ROTATION_ALPHA );

    } else {

        rz[ 0 ]  =  cos( ROTATION_ALPHA );
        rz[ 1 ]  =  sin( ROTATION_ALPHA );
        rz[ 4 ]  = -sin( ROTATION_ALPHA );
        rz[ 5 ]  =  cos( ROTATION_ALPHA );

    }

}


void scale( double *m, int direction ) {

    double scale = SCALING_VALUE; 

    if( direction == NEGATIVE_DIRECTION ) scale = 1/scale;

    m[ DIAGONAL_X ] = scale;
    m[ DIAGONAL_Y ] = scale;
    m[ DIAGONAL_Z ] = scale;

}