# browniefudge.
## This is a practice project for learning C++, do not expect its code to be optimized!

browniefudge is a brainf compiler that compiles .bf code to .c code.

Features:
1. Compilation of .bf code to .c code
2. Optimization for repeated increments, decrements, pointer movements, and char outputs     (to do)
3. Functions (sort of like #define in c)     (to do)

```
.bf code ===> intermediate code (optimized)
  						  ||
						  \/
executable <========== .c code
```

Usage:
```
$ browniefudge --help ==> shows help message

$ browniefudge file.bf ==> compiles file.bf to file.c

$ browniefudge file.bf -o out.c ==> compiles file.bf to out.c

$ browniefudge --auto file.bf -o out ==> compiles file.bf to executable

$ browniefudge --set-compiler clang ==> sets default compiler to clang

$ brudge ==> alias
```