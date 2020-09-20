crash: \
			ls.o \
			list.o \
			stack.o \
			pinfor.o \
			profork.o \
			cd.o \
		    hist.o \
			nightwatch.o \
			mainP.o \
			
	gcc -o crash $^
	rm -f *.o 


list.o:  list.h list.c
	gcc -g -c list.c

stack.o:  stack.h stack.c
	gcc -g -c stack.c

ls.o:  ls.h ls.c  
	gcc -g -c ls.c

pinfor.o:  pinfor.h pinfor.c  
	gcc -g -c pinfor.c

profork.o:  profork.h profork.c  
	gcc -g -c profork.c


cd.o:  cd.h cd.c stack.c stack.h
	gcc -g -c  cd.c


hist.o:  hist.h hist.c list.h list.c  
	gcc -g -c  hist.c


nightwatch.o:  nightwatch.h nightwatch.c  
	gcc -g -c  nightwatch.c

jobs.o: jobs.h jobs.c
	gcc -g -c jobs.c

mainP.o: mainP.c 
	gcc -g -c mainP.c

