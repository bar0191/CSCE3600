************ READ ME ***************

Brandon Reid
Ravnit Benipal
Robert Torres

Design Overview: procedural program, no functions

Our design process started by creating a working interactive version first
We used examples from our course material to get a continuous loop taking in input similar to a shell
We started by completing sub-problems, taking in input, parsing the lines (strsep)
Essentially taking this program in a step by step approach
once commands were parsed it we needed to figure out how to fork each of those commands into children
minor6 helped tremendously with this
allowing the use of system call made this trivial

Complete Specifications:

Parsed command lines using strsep
compared strings with strcmp
checked for substrings using strstr
used chdir for cd command
system() for parsed commands
used isspace to check for white spaces in command lines
strcpy to copy strings or cat
fgets to get user input and check for ctr-D also file reading
getenv was used to find USER name from system shell
wait() to allow parent to wait for each child to terminate
used array of process ids for simultaneous children running
fprintf for stderr messages
perror for fork errors

Known bugs or problems:

1. the cd command does not support wildcards
2. cd also does not support spaces

************* READ ME ***************
