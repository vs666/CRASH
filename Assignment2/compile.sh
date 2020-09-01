#!/bin/bash

# error check for header files

gcc ./stack.h ./cd.h ./ls.h ./profork.h
rm ./stack.h.gch ./cd.h.gch ./ls.h.gch ./profork.h.gch

# compilation of c code

gcc -o term ./mainP.c ./cd.c ./stack.c ./ls.c  ./profork.c

printf "\033[1;35m Code compilation complete \033[0m\n"
