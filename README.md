# Overview
Mitobrevno is a library for debugging multithreaded programs and a program for viewing the resulting logs. The GUI program needs Qt5.

The name is a neuter noun in the second declension, composed of Greek *μίτος* (thread) and Russian *бревно* (log). *Бревно* means a tree trunk, but the "record" sense of *log* developed from the "trunk" sense.

Mitobrevno compiles on Ubuntu Focal and Eoan and DragonFly 5.9. It should work on any Unix system that has Qt5 and pthreads.

To compile, if you're not developing the program:

1. Create a subdirectory build/ inside the directory where you untarred the source code.
2. cd build
3. cmake ..
4. make
5. make install

If you are developing the program:

1. Create a directory build/mitobrevno outside the directory where you cloned the source code.
2. cd build/mitobrevno
3. cmake \<directory where the source code is\>
4. make
5. make install

To use in your program:

1. If you're using CMake, copy the FindMitobrevno.cmake file to your cmake/Modules/ directory.
2. Check whether the library exists. In CMake, the command `find_package(Mitobrevno)` does this.
3. Put the result in config.h.
4. Add code which calls Mitobrevno only if the library was found.
