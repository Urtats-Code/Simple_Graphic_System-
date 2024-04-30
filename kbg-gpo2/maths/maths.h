#define ROW_SZ 4; 

void transform_into_identity_matrix( double *m ); 
void MxM( double *helper_matrix, double *m1, double *m2 ); 
void printM( double *m, char *custom_message ); 
void mxp(punto *pptr, double m[16], punto p);
void mxv(punto *pptr, double m[16], punto v);

