# Assignment 1


## Student details

This assignment is done and submitted by *Varul Srivastava*
Roll Number : 2019111015
Batch : UG-2019 CSD

Course : Operating Systems and Networking
Assignment No. : 1


### Details about the Assignment 

#### Question 1

In this problem, it is required to make a c code that makes a copy of a file passed as argument in a seperate file with the same name and location ./Assignment/{filename}.txt
The copied file should have the contents in reverse -> ABCD becomes DCBA w.r.t the original file passed in the arguments.

The file created with read and write permissions for the USER. No other permissions.

The file is created in the path ./Assignment/{filename} w.r.t the current place of execution. Not w.r.t the position of executable or the position of text file. This is the best option out of all available options, as all the executables create files unless specified in the current directory.


#### Question 2

In this problem, it is required to make check for 2 files to be reverse of each other. Also, inputting 2 files and one directory, it prints the permissions on those files/directory.


### Libraries used

sys/stat.h  - For creating stat struct and other system function calls.
sys/types.h - Same
fcntl.h     - For read/write/open file management

string.h    - Only for string handling for strcat etc.
stdio.h     - Only for perror statements.
stdlib.h    - Only for malloc and free statements.

### Assumptions Made : 

1. The user *RAM* is large enough to allow memory allocation to *200 MB* of array size. However, if needed, lesser memory allocation can be used by changing the buffer size in both codes.
2. For proper working of both portions of *question 2*, it is assumed that the user has *read permissions* for both the files.
3. It is assumed that the *Operating system* on which the code is tested/deployed is *Linux Based* (more specifically Ubuntu).
4. For the printing of the *PROGRESS BAR*  in one line only, it is assumed the code will be opened in *full screen mode* of the terminal that runs it.
5. Basic assumptions understandably made, like a fully installed version of gcc (GCC 9.3.0 ) is available on the PC.
6. The total *length of the path* in all cases will be less than or equal to *1000 characters*.
7. The file created will be in the *path ./Assignment/{filename}* w.r.t. to the *current place of execution*.
8. It is assumed that if file *alpha.txt* is duplicated w.r.t. Q1, there doesnot exist a file in path *./Assignment/alpha.txt* which forbids read/write permissions to the user.
9. In Q2 the arguments passed are ./a.out <file 1> <file 2> <Directory>
