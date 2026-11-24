# mypreprocessor

A simple C preprocessor built in C. It takes a `.c` file as input, removes comments, expands `#include` files, replaces `#define` macros, and outputs a `.i` file.

## Files in this project

- `main.c` - handles command line arguments and runs the 3 preprocessing steps
- `preprocessor.h` - header file with function declarations
- `comment_removal.c` - removes `//` and `/* */` comments
- `file_inclusion.c` - replaces `#include` lines with the actual file content
- `macro_handler.c` - handles `#define` constants and macros with arguments
- `Makefile` - builds the `mypreprocessor` executable
- `add.c`, `add.h`, `add.i` - sample addition program and its generated output

## How to compile ???

Using make:
```bash

make
```

Or using gcc directly:
```bash

gcc main.c comment_removal.c file_inclusion.c macro_handler.c -o mypreprocessor
```

## How to run

```bash

./mypreprocessor add.c
```

On Windows:
```powershell

.\mypreprocessor.exe add.c
```

This creates `add.i` in the same folder.
