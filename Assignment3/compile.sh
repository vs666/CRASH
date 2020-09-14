#!/bin/bash

# error check for header files

gcc ./stack.h ./cd.h ./ls.h ./profork.h ./pinfor.h ./list.h ./hist.h ./nightwatch.h
rm ./stack.h.gch ./cd.h.gch ./ls.h.gch ./profork.h.gch ./pinfor.h.gch ./hist.h.gch ./list.h.gch ./nightwatch.h.gch

# compilation of c code

gcc -o crash ./mainP.c ./cd.c ./stack.c ./ls.c  ./profork.c ./pinfor.c ./hist.c ./list.c ./nightwatch.c

printf "\033[1;35m Code compilation complete \033[0m\n"
