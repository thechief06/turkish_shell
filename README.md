You are required to write a complete C program that implements TURKSHELL with following features:
 When invoked, the program will start a new shell over your current Linux shell.
 The user will be allowed to enter one of 6 different shell commands in Turkish.
 5 of these commands should perform same as any of current shell command. One command must be
original to you. Whatever you want :)
 Each command must allow at least one command line parameter. One command (any) must allow at
least two command line arguments.
 Each command must be executed by a new forked process.
 The use of exec() and system() family of functions are not allowed. Each command must be
written at lowest level.