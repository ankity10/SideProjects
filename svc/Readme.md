# PROBLEM STATEMENT:

Create a simple version control (svc) program called "svc".

# DETAILS:
We have a text file that needs version control i.e., ability to revert back
to any previous version.  
- The text that is composed of one or more lines.
- Each line has a maximum character width of 10 characters (including newline).
- The total number of lines is 20.

The following operations are permitted:
1. Appending a new line at the end of the file.
2. Deleting any existing line.

Only one of the above operations can be done at a given time i.e., the user
can either append a line -or- delete a line. After each operation, the file
is commited using the svc. 

The usage of svc is the following
svc filename   : To commit
svc N          : Output Nth version of the file.

A sample flow is as follows:
1. Create a file test.txt
2. test.txt has the following line:
hello
3. Commit "svc test.txt" /* Version 0 */
4. Add another line:
world
5. Commit "svc test.txt" /* Version 1 */
6. Display version 1 "svc 1"
hello
world
7. Display version 0 "svc 0"
hello
8. Delete the line hello  and then run "svc test.txt"
9. Disp


# REQUIREMENTS:

- gcc compiler

# EXECUTION STEPS:
1. make
2. ./svc


# USAGE

1. TO COMMIT FILE
svc filename

2. TO OUTPUT Nth VERSION OF THE FILE
svc filename Nth

(Note: Version starts from 0)

