// Translations

void translate( double *m, char axis, int direction);
void translateX( double *m, int direction); 
void translateY( double *m, int direction); 
void translateZ( double *m, int direction); 


// Rotations 

void rotate(  double *m, char axis, int direction, int transform_camera );
void rotateX( double *m , int direction );
void rotateY( double *m , int direction );
void rotateZ( double *m , int direction );


// Scaling 

void scale( double *m, int direction );