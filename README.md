# SuperFastPrint
A very light weight, very basic, but very fast C++ print library. It can print to stdout or to a character buffer.

Usage:

    Print(stdout, "Task: ", 54, "ms\n");


    Print(stderr, "Task: ", 54, "ms\n");


    char buffer[100];
    Print(stderr, "Task: ", 54, "ms\n");


# Warning!
When writing to character buffers there is not proper bounds checking, arrays can be overrun (I said it was fast). 

Currently the array bounds are checked at the end of a print function, however this is already too late, if it has over run std::terminate is executed.