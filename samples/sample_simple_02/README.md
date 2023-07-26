# Sample with PoC for posgi

Notes:

* does work in Clang and GCC
* see Makefile for differences in compile and linking

```bash
# compile and test with clang
compiler=clang make clean all ; ./main ; make clean

# compile and test with gcc
compiler=gcc make clean all ; ./main ; make clean
```
