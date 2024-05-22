## Playing with dynamic library

1. `make all`
2. `./host` // should print "Hello, Version 1"
3. Open another terminal and do `make hello2` // should print "42"

## Playing with executable code on heap

To obtain proper machine code: 

1. `as -o return_42.o return_42.s`
2. `objdump -d return_42.o`

Copy instructions to `exec_heap.c` and then:

1. `gcc -o exec_heap exec_heap.c`
2. `./exec_heap`

## Playing with zero-copy between processes

1. `gcc -o producer producer.c`
2. `gcc -o consumer consumer.c`
3. `./producer`
4. `./consumer`
