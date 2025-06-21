# Data Structures and Algorithms in C

The README for each data structure or algorithm is listed in the following sections.

## Compilation

Makefile is provided, run `make` to compile and run `make help` for makefile options.
See my [cross compile script for NCAP](https://github.com/M-Y-Sun/NCAP/blob/main/app/src/main/c/deps/cdsa_install.sh) for an additional example.

---

## Singly Linked List

Singly linked list, modeled after `std::list` in C++. See this [archived repository](https://github.com/s7a19t4r/extdlib-cvector) for the changelog of sll.

#### Usage

1. Create a directory to store the source files (ex. `~/.extdlib/c/`)
2. `cd` to the file (ex `cd ~/.extdlib/c/`)
3. Clone this repository into the directory:
    ```
    $ git clone https://github.com/s7a19t4r/c-dsa
    ```
4. Place the header file named `sll.h` in a directory (ex. ~/.extdlib/c/sll.h).
5. Add `-L[filepath]` to your compile flags (ex. `-L~/.extdlib/c/dsa/src/`).
6. Compile your source file as normal (ex. `gcc -L~/.extdlib/c/dsa/src/ main.c -o main`)

See `GUIDELINES.md` for correct usage of project

#### Important

- Currently is only able to store integer data
- Maximum size is `SIZE_MAX`
- Some functions may be slower than a C++ sll
- Reverse iteration will be especially slow because of the singly linked list format
- Vector setup and cleanup are NOT implicitly called; they must be manually implemented.
- Because C is a procedural language, the raw data of the sll struct can be accessed. Please refrain from directly accessing members and only use the get methods

#### features

- [x] Memory usage scales with the array size
- [x] Supports most features of a C++ sll, excluding iterators and time complexity
- [x] Quality of life features that C++ slls do not have, such as printing the list, sum, product, etc...
- [ ] Ability to store data types other than integers using void pointers (May cause significant changes to code structure, function return values, function behavior, and function parameters)
