// @FILE: file3.cc

#include  "file2.h"

int  main(void) { foo(); }

// SYMBOL TABLE:
// 0000000000000000 l    df *ABS*  0000000000000000 file3.cc
// 0000000000000000 l    d  .text  0000000000000000 .text
// 0000000000000000 l    d  .data  0000000000000000 .data
// 0000000000000000 l    d  .bss   0000000000000000 .bss
// 0000000000000000 l    d  .note.GNU-stack        0000000000000000 .note.GNU-stack
// 0000000000000000 l    d  .eh_frame      0000000000000000 .eh_frame
// 0000000000000000 l    d  .comment       0000000000000000 .comment
// 0000000000000000 g     F .text  000000000000000b foo()
// 000000000000000b g     F .text  0000000000000010 main
