## Playing with arena allocator

1. `gcc -o malloc_bug malloc_bug.c -fsanitize=address`
2. Check that `./malloc_bug` produces "SUMMARY: AddressSanitizer: heap-use-after-free (malloc_bug:arm64+0x100003b0c) in main+0xd0" error
3. `gcc -o arena arena.c`
4. `./arena`
