 #!/bin/bash

gcc *.c  -Wno-deprecated-declarations maths/*.c transformation_functions/*.c -framework OpenGL -framework GLUT -lm -o   program  

./program