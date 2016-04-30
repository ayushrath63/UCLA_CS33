# Bits, Bytes and Integers

_Bits_ are 0 or 1, represented by voltage levels in hardware.

A _byte_ is an 8 bit value, ranging from 0b00000000 to 0b11111111, 0 to 255 in
decimal or 0x00 to 0xFF in hexadecimal

#### Boolean Algebra

Boolean algebra is an algebraic representation of logic, 1 is encoded as true,
0 as false.

There are 4 low level operations that can be used on bits.

And: ```A & B = 1 if A = 1 and B = 1```

Or: ```A | B = 1 when either A = 1 or B = 1```

Not:``` ~A = 1 if A = 0 and vice versa```

Exclusive Or (XOR):```A^B = 1 when A = 1 or B = 1 but not if A = B = 1```

Examples:

```
01101001 & 01010101 = 01000001
01101001 | 01010101 = 01111101
01101001 ^ 01010101 = 00111100
~01010101 = 10101010
```

#### Representing and Manipulating Sets Using Bit Vectors

A width w bit vector represents subsets of {0,...,w-1} a<sub>j</sub> = 1 if j is
an element of A.

{7,**6**,**5**,4,**3**,2,1,**0**} -> 01101001 -> {0,3,5,6}

& -> intersection
| -> union
^ -> symmetric difference
~ -> complement

#### Bit-Level Operations in C

The operations &, |, ~, ^ are available in C and can be applied to primitive data
types, where the inputs are used as bit vectors. These are different from the
logical operators &&, ||, !

Example:
```
~0x41 -> 0xBE
!0x41 -> 0x00
```

#### Shift Operations

Shift operations involve moving bits in a bit vector in a linear fashion

Left Shift _x_ << _y_:
 - Shift bit vector _x_ left _y_ positions, throw away extra bits on the left, fill with 0's on the right
Right Shift _x_ >> _y_:
 - Shift bit vector _x_ right _y_ positions, throw away extra bits on the right,
 - Logical shift: Fill with 0's on the left
 - Arithmetic Shift: replicate most significant bit on the left
Undefined behavior if _y_ < 0 or _y_ >= _x_

#### Encoding Integers

Unsigned Integers: B2U(x) = x<sub>0</sub> \* 2<sup>0</sup> + ... + x<sub>w-1</sub> * 2<sup>w-1</sup>

Two's Complements: B2T(x) = -x<sub>w-1</sub>\*2<sup>w-1</sup> + (x<sub>0</sub> \* 2<sup>0</sup> + ... + x<sub>w-2</sub> \* 2<sup>w-2</sup>)

#### Numeric Ranges
- Unsigned Values:
  - UMin = 0 -> 000...0
  - UMax = 2^w - 1 -> 111...11
- Two's Complement Values:
  - TMin  = -2^(w-1) -> 100...0
  - TMax  = 2^(w-1) -> 011...1
  - -1 -> 111...1

Observations:
- |Tmin| = TMax + 1
- UMax = 2\*TMax + 1
- Unsigned and Signed values have the same representations when the leftmost bit is 0

#### Signed vs. Unsigned in C
- Constants
  - By default are considered signed
  - Considered unsigned if has "U" as a suffix
- Bit pattern is maintained when casting from signed to unsigned
- If there is a mix of signed and unsigned values, signed values are implicitly cast to unsigned
- Examples:
  - 0U == 0
  - -1 < 0
  - -1 > 0U
  - 2147483647 > -2147483647-1
  - 2147483647U < -2147483647-1
  - -1 > -2
  - (unsigned)-1 > -2
  - 2147483647 == 2147483648U
  - 2147483647 > (int)2147483648U

#### Sign Extension

Task: Given w-bit signed integer x, convert it to w+k bit integer with the same value

Rule: Make k copies of sign bit and put to left, i.e. x' = x<sub>w-1</sub>, x<sub>w-1</sub>, ..., x<sub>w-1</sub>, x<sub>w-2</sub>, x<sub>w-3</sub>, ..., x<sub>0</sub>

Example:
```
-2 -> 10 as a two bit integer
-2 -> 1110 as a 4 bit integer
16 -> 01111 as a 5 bit integer
16 -> 00001111 as an 8 bit integer
```

#### Unsigned & Signed Addition

Given two unsigned integers _u_ and _v_, each _w_ bits, _w_+1 bits are needed to store
all possible values of _u_+_v_. For example, if two 4 bit integers with value 15
are added, the result is 30, which cannot be represented using only 4 bits.

At the bit level, signed and unsigned addition are identical, the difference is
in interpretation. Overflow can occur in both the positive and negative direction.

#### Integer Multiplication

Unsigned multiplication of two _w_ bit numbers can result in a number up to _2w_
bits wide.

In signed multiplication, underflow or overflow can occur.

Multiplication by 2 can be performed using a left shift.

Examples:
_u_ << 3 == _u_ \* 8
(_u_ << 5) - (_u_ << 3) == _u_ \* 24

Most machines can shift faster than they can multiply.

Unsigned power of 2 division can similarly be performed using a logical right shift.

#### Using Unsigned

The code:
```
unsigned i;
for(i = cnt - 2; i >= 0; i--)
  a[i] += a[i+1];
```
Will result in an infinite loop, since i cannot be < 0.

This can be solved by doing the following:
```
size_t i;
for(i = cnt - 2; i < cnt; i--)
  a[i] += a[i+1];
```

#### Memory Organization

Programs refer to data by address. Memory can be conceptualized as a large array
of bytes.

Any given computer has a _word_ size, the nominal size of integer data and addresses.

Until recently, most machines used 32 bits as the word size, limiting the number
of addresses to 2^32, or 4GB of storage. Now many computers use 64 bit words, allowing
up to 18 exabytes of storage.

In word-oriented memory, an address refers to the first byte in a word.

#### Byte Ordering

There are two main conventions for ordering bytes within a word, based on _endianness_.

Big Endian, where the least significant byte has the highest address (Sun, PPC, Internet).

Little Endian, most significant byte has highest address (x86, ARM, iOS, Windows).

#### Printing the Byte Representation of Data

The byte representation of data can be printed using the following C code:
```
typedef unsigned char *pointer;
void show_bytes(pointer start, size_t len)
{
  size_t i;
  for (i = 0; i < len; i++)
    printf(”%p\t0x%.2x\n",start+i, start[i]);
  printf("\n");
}
```

#### Representing Strings

Strings in C are represented by an array of characters terminated by a 00 byte.
Endianness is not an issue when dealing with strings, since each char is one byte.

#### Integer C Puzzles

```
int x = foo();
int y = bar();
unsigned ux = x;
unsigned uy = y;
```
 - x < 0 -> ((2*x) < 0) is not always true, since underflow can occur
 - ux >= 0 is always true
 - x & 7 == 7 -> (x << 30) < 0 is always true (32 bit), since the last 3 bits of x are 1
 - ux > -1 is not always true, since -1 interpreted as unsigned is UMax
 - x > y -> -x < -y is not always true, since -TMin cannot always be stored, it is true in all other cases
 - x \* x >= 0 is not always true, since overflow can occur
 - y > 0 && y > 0 -> x + y > 0 is not always true since overflow can occur
 - x >= 0 -> -x <=0 is always true (TMin is not an issue since x <= 0)
 - (x|-x) >> 31 == -1 is not always true, since if x = 0, (x|-x) has 0 as the most significant bit
 - ux >> 3 == ux/8 is true
 - x >> 3 == x/8 is not true for negative values, true for x >= 0
 - x & (x-1) != 0 is false for x = 0, since 0 - 1 is represented by 0b11111111, so 0 & -1 would be 0b11111111
