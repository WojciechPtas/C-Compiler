The c4lib is a set of c4-compatible wrappers around standard library functions
that are helpful to write c4-compatible programs.

## Using the c4lib

Almost every non-trivial C program uses the C standard library (which contains
non-c4-compatible things like vararg functions and inline assembly) and
preprocessor features like includes and macros (which are also not supported by
c4).

We can work around this by using the standard library wrappers in c4lib instead
of functions from the real standard library. E.g.

```
#include <stdio.h>
int main(void) {
    printf("Hello %d\n", 42);
}
```
becomes
```
#include "c4lib.h"
int main(void) {
    c4_print_str("Hello ");
    c4_print_int(42);
    c4_print_str("\n");
}
```

In a next step, we can use the gcc/clang preprocessor to get rid of the
unsupported preprocessor commands:
```
gcc -E -C -P -o foo_prep.c foo.c
clang -E -C -P -o foo_prep.c foo.c
```
The resulting file `foo_prep.c` will not contain any macros or includes anymore.
We can therefore use it as input to a c4 implementation to produce LLVM IR.
To make the code actually runnable, we need to link it to the c4lib:
```
clang -o foo c4lib.c foo_prep.ll
```
This works since clang can handle both, LLVM IR and C source code as input and
link the resulting binaries together.

