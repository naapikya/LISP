DESCRIPTION:
This program interprets a lisp code with c++

FILES:
We have the followong files in this directory
1)tokenise.cpp _this file contains the operations of separating the given text into tokens_
2)token.cpp _this file contains the operation of parenthesizing our tokens(making the tree)_
3)token.h _here are the declarations of the functions implemented in the previous file_
4)func.cpp _here is implemented our class func(it is for the defined functions and their call in the lisp code)
5)func.h _here are the declarations of the functions implemented in the previous file_
6)interpreter.cpp _this file contains the functions which perform the interpretation of the lisp file_
7)functions.h _here are all the declarations of all the functions in these files_
8)gcd_file.txt _this is the file where the gcd function is written in lisp_
9)linear_file.txt _this is the file where the linear gcd function is written in lisp_
10)file.txt _here is written a lisp code, you can write here your lisp code and compile(interpret) it with our program
11)Makefile
12)README.md_

WHAT OUR PROGRAM ACTUALLY DOES:
1)Solves arithmetic expressions (+, -, /, *, remainder)
2)Determines conditional expressions(>, <, <=, >=, equal?, =)
3)interprets lisp functions(display, define(variable, function), set!, begin, if, newline...)

WHAT WE HAVEN'T MANAGED TO FINISH YET:
1) interpretation of define_syntax_
2)interpretation of let_

THE IMPERFECTIONS THAT WE SHOULD CORRECT:
1)our functions can return only a special value(not a structure, for example list)
2)we have used global variables to manage the problem of the variables, and the variables that are inside the function

**if you want to change the file where the lisp code is written, just open the main.cpp file, and in the 5-th line type the filename.txt

HOW TO RUN OUR PROGRAM:
to compile the program type in the command line
  make
to run the program type
 ./lisp.exe
to run again type
 make re
to delete the .o files type
 make clean
to delete the .exe file type
 make fclean
