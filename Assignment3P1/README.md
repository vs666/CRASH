# craSH [CRAZY SHELL] 
This project is a part of Operating Systems and Networking course Assignment.
Features of the shell include :

1. Color based seperation of prompt-line, error-messages, directory, files and executable. 
2. Implementation of `pipes` and `Input-Output redirection`.
3. Implementation of space and time efficient data-structures like `stack` and `linked-list` for commands for efficient execution.
4. The entire code-base is written in `C Language` with libraries used capable of running on any standard `LINUX based OS`.
5. Implementation of  `signals` and its `handling`.
6. A feature that shows Smile `:')` on successful execution of command and frown `:'(` other-wise.
7. Ability to read, write and update `environment-variables` of the base OS / SHELL.
8. Process handling, including running a process in background, bringing it to foreground etc.


## Codebase 

### Programs implementing the commands

1.  *mainP.c*       -> contains the main program that runs the infinite loop taking prompt (also contains code for commands like echo and pwd
2.  *ls.c*          -> code for the ls command that lists out all the files of a specified directory. Implemented -a and -l flags and color coded names.
3.  *ls.h*          -> header file for ls.c
4.  *forktest.c*    -> contains code for running foreground and background child processes
5.  *forktest.h*    -> header file for forktest.c
6.  *cd.c*          -> contains code for changing directory (Not needed anymore)
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

### Makefiles and Scripts to compile the code

18. *init(.sh)*       -> creates initial logfiles and compiles code for the first time
19. *compile(.sh)*    -> script for compiling the shell
20. *makefile*        -> to implement `make` command and create an executable named `crash`

### Include-files and Constants for code modularity 

21. *includefiles.h*  -> includes #include command required in various scripts
22. *constants.h* -> includes constants used in code e.g. LIST_LEN is used to allocate memory to arrays, strings etc.

## Compilation and usage - makefile compilation

Go to the folder with code and compile

``` bash
$ make
$ ./crash
```

## Compilation and usage - SHELL Compilation 

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
3. Implementation of both `newborn` and `interrupt` in nightswatch.
4. To quit nightswatch we have to prompt by pressing `q` followed by `return`.
5. As a feature, it seemed wise to implement that echo doesnot take `" "` into account but does take `"\t"` into account (feature, not a bug).
6. All background processes need to be exitted and history saved, so shell will be exitted using `exit` command only.
7. The pinfo command is more compatible with `Linux 5.4.0.45-generic` and OS `Ubuntu 20.04.1 LTS (Focal Fossa)`. This is just a precautionary assumption, and is not necessarily required for the proper working of the crash shell entirely.
8. Maximum number of parellal instruction that can be run is 1000 but can be changed by changing value of a constant which is `LIST-LEN`.

## Starting the Shell

To start the shell, for the first time : 

``` bash
$ ./init.sh
$ ./crash
```

For subsequent changes and compilation 

``` bash
$ ./compile.sh
$ ./crash.sh
```

## Created by 

`VARUL SRIVASTAVA`

- Roll Number : *2019111015*
- E-Mail : *varul.srivastava@research.iiit.ac.in*
- Website : *researchweb.iiit.ac.in/~varul.srivastava*
- Github : *https://github.com/vs666*
