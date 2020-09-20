#ifndef STACK__
#define STACK__

typedef char * String;

// Error codes for different errors and some constants.

# define STACK_SIZE 1000
# define STACK_OVERFLOW 3
# define EMPTY_STACK 2



String ar__stack[1000];
void push(String n1);
String pop();
int size();

#endif