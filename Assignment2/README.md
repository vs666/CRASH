# craSH 
ASSIGNMENT 2 - Operating Systems and Networking


## Codebase 

### Programs implementing the commands

1.  *mainP.c*       -> contains the main program that runs the infinite loop taking prompt (also contains code for commands like echo and pwd

2.  *ls.c*          -> code for the ls command that lists out all the files of a specified directory. Implemented -a and -l flags and color coded names.
3.  *ls.h*          -> header file for ls.c

4.  *forktest.c*    -> contains code for running foreground and background child processes
5.  *forktest.h*    -> header file for forktest.c

6.  *cd.c*          -> contains code for changing directory ( more specifically a check to see cd doesnot go below ~ of our terminal)
7.  *cd.h*          -> header file for cd.c

8.  *hist.c*        -> contains code for saving/loading/displaying history.
9.  *hist.h*        -> header file for hist.c
*logfiles/history.log* contains history from previous sessions stored. (atmost 20 lines)

10. *pinfor.c*      -> contains code for displaying information of a process whose pid is given as arguments (pinfo command)
11. *pinfor.h*      -> header file for pinfor.c

12. *nightswatch.c* -> contains code for nightswatch command
13. *nightswatch.h* -> header file for nightswatch.c

### Data Structures implemented to enhance complexity

14. *list.c*        -> implementation of linked list for history command
15. *list.h*        -> header file for list.c
16. *stack.c*       -> implementation of stack data structure for ls command
17. *stack.h*       -> header file for stack.c

### BASH script to compile the code

18. *compile.sh*    -> script for compiling the shell


## Compilation and usage

```bash
$ ./compile.sh
$ ./crash
```

## Assumptions and Important Details

1. The length of all commands, arrays and stacks are assumed to be 1000 in length. This is a reasonable assumption but can be increased if needed.
2. `exit` command should be used for exit so that history can be saved.To exit type :
``` bash
<username@devicename:~/path> exit
```
3. It is assumed that since 
``` bash
<username@devicename:~> cd ..
```
doesnot take to previous directory, thus the terminal in general has prevented access to directories before ~ in the shell. It is imposed using code written in cd.c file with cd.h as the header.

4. Implementation of both `nweborn` and `interrupt` in nightswatch.
5. To quit nightswatch we have to prompt by pressing `q` followed by `return`.

