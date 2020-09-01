#!/bin/bash

# error check for header files

gcc ./stack.h ./cd.h ./ls.h
rm ./stack.h.gch ./cd.h.gch ./ls.h.gch

# compilation of c code

gcc -o term ./mainP.c ./cd.c ./stack.c ./ls.c  

printf "\033[1;35m Code compilation complete \033[0m\n"
