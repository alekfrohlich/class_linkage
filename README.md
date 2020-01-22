# Internal vs External Linkage: a peek into the static and extern keywords of C++

The goal of this article is that, after you read it, you'll be able to tell the linkage attributes of your C++ variables and functions. This knowledge should help you in writing better header files.

## Declaration vs definition

**_Declarations_**  introduce (or re-introduce) an identifier and describes its type, be it a type, an object, a function or a variable. Each kind of entity is declared differently.  It's what  **the compiler  needs** to accept a reference to an identifier.

A **_definition_** actually instantiates/implements this identifier. It's **_what the linker needs_** in order to link references to those entities. Here is were the memory is allocated.

According to [cppreference: Declarations](https://en.cppreference.com/w/cpp/language/declarations), a declaration can appear in many different forms. We won't go much into details since that will be the topic of next week's class.

Here are some examples of such declarations and definitions in the C++ language:

```
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
class IntPair { int x1, x2; } // definition

// sizeof can't be used here since the compiler does not know what
// the size of S is supposed to be.
struct  S;
int a =  sizeof(S); // compilation error: incomplete type is not allowed
```

## Internal vs external linkage

A  **_translation unit_** (TU)  refers to an implementation (`.c/.cc`) file and all header (`.h/.hpp`) files it includes. If an object or function inside such a translation unit has  **_internal linkage_**, then that specific symbol is only visible to the linker within that translation unit. If an object or function has **_external linkage_** the linker can also see it when processing other translation units. The  `static`  keyword, when used in the global namespace, forces a symbol to have internal linkage. The  `extern`  keyword results in a symbol having external linkage.

It's important to note that `extern` and `static` are link-time features, whilst access modifiers such as public and protected are compile-time features.

`static` and `extern` are **_linkage specifiers_** (interest of this article),  **_however_** they are also **_storage specifiers_**, meaning that they determine the lifetime of things. According to [cppreference: Storage class specifiers](https://en.cppreference.com/w/cpp/language/storage_duration), `static` and `extern` imply the following:

-   **static**:  **_static_** or  **_thread_**  storage duration and  **_internal  linkage_**.
-   **extern**: **_static_**  or  **_thread_**  storage duration and  **_external  linkage_**.

We won't go further in the details.

The compiler defaults the linkage of symbols such that:

-   **_Non-const_**  global variables have **_external_**  linkage by default.
-   **_Const_**  global variables have  **_internal_**  linkage by default.
-   **_Functions_** have **_external_** linkage by default.

The rationale between the second point is that `const` variables are used in the same manner as value macros in the C programming language:

```
// the C way
#DEFINE QUANTUM 50

// the C++ way
const unsigned QUANTUM = 50;
```

It's better to have statically typed code.

## Use include guards to avoid multiple definitions of a symbol in the same TU

Include guards prevent redefinition of definitions in the same translation unit (TU). Without it, the following code snippets would result in a linkage error.

```
// @FILE: file2.hpp
int foo(void) { return  1; }

// @FILE: file3.cc
#include  "h1.hpp"
int  main(void) { foo(); }

// @FILE: file4.cc
#include  "file2.h"
int  bar() { return  foo(); }
```

error:

```
$ gcc -c cc1.cc cc2.cc
$ ld cc1.o cc2.o
/tmp/ccdH0phP.o: In function `foo()':
cc2.cc:(.text+0x0): multiple definition of `foo()'
/tmp/ccM6Btul.o:cc1.cc:(.text+0x0): first defined here
collect2: error: ld returned 1 exit status
```

With an include guard, the header would look like:

```
// @FILE: file2.5.h
#ifndef FOO_H
#define FOO_H
int foo(void) { return  1; }
#endif // FOO_H
```

## Global definitions should be extern

One would be tempted to think that include guards solve all problems. That sure isn't the case. The include guard technique solves all problems whose root cause is repeating a definition inside a TU. If code is compiled separately the technique cannot help us:

```
// @FILE: global.h
#ifndef GLOBAL_H
#define GLOBAL_H
int a = 5; // definition
#endif // GLOBAL_H

// @FILE: first.cc
#include "global.h"
... use 'a' for something

// @FILE: second.cc
... use 'a' for another thing
```

Each of `first.c` and `second.c` would still receive it's own copy of `a`. Thus, linking the two object files together would result in the following error:

```
$ gcc -c first.cc second.cc
$ ld first.o second.o
second.o:(.bss+0x0): multiple definition of `a'
first.o:(.bss+0x0): first defined here
```

This is one of the reasons why many people advocate putting implementation code in .c/.cc files instead of header files. We shall soon see that this is not an unbreakable rule. When such external entity is needed, it should be defined as `extern` by `global.h`. This way, each TU can reference `a` in the way it sees fit, while just one is responsible for defining `a`.

## Internal linkage with the static keyword: An expected result (gag)

As `static` implies **_internal linkage_**, it becomes clear as to why `static` variables in header files are odd: Each implementation file (`.c/.cc`) that includes the said header, get's it's own copy of the `static` variable:

```
// @FILE: file5.h
static int variable = 42;

// @FILE: file6.h
void function1();

// @FILE: file7.h
void function2();

// @FILE: file8.cc
#include "file5.h"
void function1() { variable = 10; }

// @FILE: file9.cc
#include "file5.h"
void function2() { variable = 123; }

// @FILE: file10.cc
#include "file5.h"
#include "file6.h"
#include "file7.h"
#include <iostream>
int main() {
	function1();
	function2();
	std::cout << variable << std::endl;
}
```

Because  `variable`  has internal linkage, each translation unit that includes  `header.hpp`  gets its own unique copy of  `variable`. Here, there are three translation units:

1.  file1.cc
2.  file2.cc
3.  main.cc

When  `function1`  is called,  `file1.cc`’s copy of  `variable`  is set to 10. When  `function2`  is called,  `file2.cc`’s copy of  `variable`  is set to 123. However, the value printed out in  `main.cc`  is  `variable`, unchanged: 42.

## Forward declarations: a misnomer

A forward declaration is just a declaration that is not also a definition. The term is barely useful, but just to be clear:

```
// this is a declaration and is not a definition, thus it is a forward declaration
void foo(void);

// this is a declaration and a definition, therefore is not a forward declaration
void bar(void) {}
```

## extern "C" and Language linkage

`extern "C"` makes functions and objects be C-style linked. That is very helpful when writing portable library code. This means that the symbol can be referenced by C programs.

From cppreference:
```
extern "C" {
    int open(const char *pathname, int flags); // C function declaration
}

int main() {
    int fd = open("test.txt", 0); // calls a C function from a C++ program
}

// This C++ function can be called from C code
extern "C" void handler(int) {
    std::cout << "Callback invoked\n"; // It can use C++
}
```

One of the consequences of declaring functions to be `extern "C"`, is that they'll lose their full qualified name. I.g: two variables C-linked are considered to be the same if they have the same name, independent of the namespace in which they appear:

```
// @FILE: test_extern_c.cc
namespace  namespace1 {
extern  "C"  void  foo() {}
}

namespace  namespace2 {
extern  "C"  void  foo() {}
}
```

Compiling this gives the error:

```
/tmp/ccXtHp8y.s: Assembler messages:
/tmp/ccXtHp8y.s:22: Error: symbol `foo' is already defined

```

## Static meta-programming: a world were sometimes it makes sense to put implementation code in header files

One thing to consider when writing header files is that the more you limit the view the compiler has of an TU, the less it can make to optimize your code. Such optimizations are the basis for static meta-programming. By writing meta-programs, you're able to provide application specific optimizations/requirements whilst avoiding code duplication.

One clean example would be the following model of a x86-32 CPU:

```
// @FILE: cpu.h
#define ASM __asm__ __volatile__

class CPU_Good {
public:
	static int_enable(void) { ASM("sti"); }
};

class CPU_Dumb {
public:
	static int_enable(void);
};

// @FILE: cpu.cc
CPU_Dumb::int_enable(void) { ASM("sti"); }

// @FILE: test_cpu.cc
#include "cpu.h"

int main(void) {
	CPU_Good::int_enable();
	CPU_Dumb::int_enable();
}
```

As one can see, `gcc -O2` generated a function call for `CPU_Dumb::int_enable()`, while `CPU_Good::int_enable()`, on the other hand, did not. Less execution-time overhead!

```
Disassembly of section .text:

00000000000004f0 <main>:
 4f0:   48 83 ec 08             sub    $0x8,%rsp
 4f4:   fb                      sti
 4f5:   e8 26 01 00 00          callq  620 <CPU_Dumb::int_enable()>
 4fa:   31 c0                   xor    %eax,%eax
 4fc:   48 83 c4 08             add    $0x8,%rsp
 500:   c3                      retq
 501:   66 2e 0f 1f 84 00 00    nopw   %cs:0x0(%rax,%rax,1)
 508:   00 00 00
 50b:   0f 1f 44 00 00          nopl   0x0(%rax,%rax,1)
```

From another point of view, the more code that you make available in header files, the longer the compilation time (as it's common for a header to be included multiple times).

I should emphasize that in-lining is just one of the capabilities made possible by giving the C++ compiler extra information about the code.

## Summary
We've covered:

- The differences between declaration and definition.
- The effect of `static` and `extern` as **_linkage specifiers_**.
- Practical applications of linkage knowledge: **_Include guards_**, **_global definitions_**, **_internal linkage in header files_** and **_language linkage_**.
- The misnomer that are **_forward declarations_**.

We also scratched the surface on  **_static meta-programming_**.

## What's next?

This week we covered the basic principles concerning internal and external linkage. In the next week we shall come back to the C++ specifics related to declarations, definitions and the **_ODR_** (One Definition Rule).
