# CSL (C Standard Library)

`csl.h` follows the **stb single-file library** design pattern, popularized by the [nothings/stb](https://github.com/nothings/stb) project. Its core features are:

- **Single-file**: Uses only one `.h` file that contains both declarations and implementations.
- **Opt-in implementation**: By default, the file acts only as a header, generating no actual code. You must define a specific macro (usually `CSL_IMPLEMENTATION`) **in exactly one C/C++ source file** **before including `csl.h`** to compile the implementation into your project.
- **License**: Typically released under the Public Domain or MIT license, allowing very free usage.

For more details, please refer to the [stb documentation](https://github.com/nothings/stb).

## Quick Start

1. Copy the `csl.h` file into your project directory.
2. In any source file where you want to use the library's interface, simply `#include "csl.h"`. At this point, only function declarations are available.
3. Choose **exactly one** `.c` or `.cpp` file in your project and include the header as shown below to generate the implementation code:

    ```c
    /* In one specific source file */
    #define CSL_IMPLEMENTATION
    #include "csl.h"
    ```

4. You can now use the macros, functions, or data structures declared in `csl.h` anywhere in your project.

> **Note**: Make sure `CSL_IMPLEMENTATION` is defined in only one source file; otherwise, you will encounter linker errors due to multiple definitions.

## Usage Example

```c
/* main.c */
#define CSL_IMPLEMENTATION
#include "csl.h"

int main(void) 
{
    printf("Hello, CSL!\n");

    return 0;
}
```

## Contributing

Feedback, suggestions, and improvements are always welcome!

If you have any ideas on how to make this library better, encounter any issues, or would like to propose new features, please feel free to open an `Issue` or submit a `Pull Request`. Your input is highly appreciated.

Happy coding! **:-)**
