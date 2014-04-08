SystemsProgramming
==================

Milestone 4

Simple terminal program created, it will read from stdin, exit if exit is entered and run commands using system()

Terminal file improved to use execvp and fork, the main different between execvp and system is that execvp overwrites the process that calls it, this means that if you do not fork and use it in a child process, then whatever process is running execvp will be ended and therefore you cannot run more than one command before having to restart terminal without it being run in the child.


Milestone 5

For cd, calling the process using system or execvp will not work as they will not change the directory of the current process, i.


Milestone 5

For cd, calling the process using system or execvp will not work as they will not change the directory of the current process, i.e the terminal application itself, instead they will change the directory of the process created to call them, which will leave the terminal application in the same directory it was before.
