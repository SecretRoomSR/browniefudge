# browniefudge.

## This is a practice project for learning C++, do not expect its code to be optimized!

## Description

browniefudge is a brainf compiler that compiles .bf code to .c code.

## Features
1. Compilation of .bf code to .c code
2. Optimization for repeated increments, decrements, pointer movements, and char outputs     (to do)
3. Macros (sort of like #define in c)     (to do)

```
.bf code ===> intermediate code (optimized) ===> .c code ===> executable
```

## Usage
```
$ browniefudge --help ==> shows help message

$ browniefudge file.bf ==> compiles file.bf to out.c (default)

$ browniefudge file.bf -o name.c ==> compiles file.bf to name.c

$ (not implemented yet) browniefudge --auto file.bf -o out ==> compiles file.bf to executable

$ browniefudge --set-compiler clang ==> sets default compiler to clang

$ brudge ==> alias
```

## Building

Prerequisites:
1. C and C++ compiler
2. Make
3. Windows SDK (on windows)

```
$ make
$ make rebuild ==> cleans before building
```

## License

This project is licensed under the terms of the GNU General Public License v3.0.

You are free to use, modify, and distribute this software under the same license.

See the [LICENSE](LICENSE) file for details.

## Contributing

Pull requests and contributions are welcome! If you change any core logic, please document it in the changelog and ensure your code retains GPLv3 compliance.

## Author

Jason Christian (@secretroomsr)
