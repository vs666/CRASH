crash: \
			includefiles.h \
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


list.o:	includefiles.h  list.h list.c
	gcc -g -c list.c

stack.o:	includefiles.h  stack.h stack.c
	gcc -g -c stack.c

ls.o:	includefiles.h  ls.h ls.c 
	gcc -g -c ls.c

pinfor.o:	includefiles.h  pinfor.h pinfor.c  
	gcc -g -c pinfor.c

profork.o:	includefiles.h  profork.h profork.c  
	gcc -g -c profork.c


cd.o:	includefiles.h  cd.h cd.c stack.c stack.h
	gcc -g -c  cd.c


hist.o:	includefiles.h  hist.h hist.c list.h list.c  
	gcc -g -c  hist.c


nightwatch.o:	includefiles.h  nightwatch.h nightwatch.c  
	gcc -g -c  nightwatch.c


mainP.o:	includefiles.h mainP.c 
	gcc -g -c mainP.c

