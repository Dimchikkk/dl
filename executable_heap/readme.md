## Playing with executable code on heap

To obtain proper machine code: 

1. `as -o return_42.o return_42.s`
2. `objdump -d return_42.o`

Copy instructions to `exec_heap.c` and then:

1. `gcc -o exec_heap exec_heap.c`
2. `./exec_heap`
