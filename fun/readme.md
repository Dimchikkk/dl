1. Compile shared libs:

`gcc -fPIC -shared libfoo.c -o libfoo.so`
`gcc -fPIC -shared libbar.c -o libbar.so`

2. Compile and link main program:

`gcc -c main.c -o main.o`
`gcc main.o -o main -L. -lfoo -lbar`

3. ./main

Depending on linking order of libraries it will print message from different shared libs.
