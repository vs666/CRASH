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

#### Question 2

In this problem, it is required to make check for 2 files to be reverse of each other. Also, inputting 2 files and one directory, it prints the permissions on those files/directory.

### Libraries used

sys/stat.h  - For creating stat struct and other system functions.
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


