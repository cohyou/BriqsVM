# GimiqsVM
Virtual Machine with 128bit Lisp-like CONS CELLs(Briqs) for Programming Language

## Briq?
Briq is 128bit Lisp-like 'CONS CELL'.
We call formar pointer 'P', and latter 'Q'.

```
+-----+-----+
|  P  |  Q  |
+-----+-----+
```

Briq has bit patterns below.

```
0     2     4     6     8    10    12    14    16(byte)
+-----+-----+-----+-----+-----+-----+-----+-----+
|Type1|Type2|BcktP|BcktQ|   DntrP   |   DntrQ   |
+-----+-----+-----+-----+-----+-----+-----+-----+
```

- Type1...main type of briq  (2byte)
- Type2...sub type of briq   (2byte)
- BcktP...bucket index of P  (2byte)
- BcktQ...bucket index of Q  (2byte)
- DntrP...denoter index of P (4byte)
- DntrQ...denoter index of Q (4byte)

## Internal of VM
GimiqsVM has 65536 buckets, and each bucket can have 4294967295 briqs.
