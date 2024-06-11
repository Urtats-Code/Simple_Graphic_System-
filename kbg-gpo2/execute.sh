 #!/bin/bash

gcc *.c maths/*.c transformation_functions/*.c -framework OpenGL -framework GLUT -lm -o  -Wno-deprecated-declarations program 

./program