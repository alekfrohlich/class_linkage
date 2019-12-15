// @FILE: file1.cc

int a = 5; // definition
int b; // definition
extern int c; // declaration

int foo(void) { return 1; } // definition
int bar(void); // declaration
extern int baz(void); // declaration

class C; // forward declaration
struct S; // forward declaration
struct Point { int x, y; }; // definition
class IntPair { int x1, x2; }; // definition

// sizeof can't be used here since the compiler does not know what
// the size of S is supposed to be.
// struct  S;
// int a =  sizeof(S); // compilation error: incomplete type is not allowed

// only definitions appear in the object file
// classes and structures can only be instantiated if defined somewhere
// SYMBOL TABLE (objdump -tC file1.o ):
// 0000000000000000 l    df *ABS*  0000000000000000 file1.cc
// 0000000000000000 l    d  .text  0000000000000000 .text
// 0000000000000000 l    d  .data  0000000000000000 .data
// 0000000000000000 l    d  .bss   0000000000000000 .bss
// 0000000000000000 l    d  .note.GNU-stack        0000000000000000 .note.GNU-stack
// 0000000000000000 l    d  .eh_frame      0000000000000000 .eh_frame
// 0000000000000000 l    d  .comment       0000000000000000 .comment
// 0000000000000000 g     O .data  0000000000000004 a
// 0000000000000000 g     O .bss   0000000000000004 b
// 0000000000000000 g     F .text  000000000000000b foo()
