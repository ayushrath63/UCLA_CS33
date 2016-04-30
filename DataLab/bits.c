/* 
 * CS:APP Data Lab 
 * 
 * <Ayush Rath 904581137>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#include "btest.h"

/*
 * Instructions to Students:
 *
 * STEP 1: Fill in the following struct with your identifying info.
 */
id_struct studentID =
{
  /* UID number: */
  "904581137",
  /* Last Name: */
  "Rath",
  /* First Name: */
  "Ayush",
};

#if 0
/*
 * Instructions to Students:
 *
 * STEP 2: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the btest harness to check your solutions for correctness.
 */


#endif
/*
 * bitParity - returns 1 if x contains an odd number of 0's
 *   Examples: bitParity(5) = 0, bitParity(7) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int bitParity(int x) {
  
  //compares adjacent pairs of bytes, sets lest significant of each pair
  //to 1 if odd number of bits in each pair, each 2nd bit has an uneeded value
  x ^= x >> 1;

  // compares the number of odd numbers in adjacent pairs, sets LSB of each
  // set of 4 adjacent bits to 1 if contains an odd number of 1s
  x ^= x >> 2;

  // repeat above for sets of 4 bits, store result in LSB of set of 8 bits
  x ^= x >> 4;

  //repeats above for sets of 8 bits, store results in LSB of set of 16 bits
  x ^= x >> 8;

  // repeats above for set of 16 bits, stores result in LSB of x
  x ^= x >> 16;

  //return the least significant bit of x
  return (x & 0x1);
}

/* 
 * rotateRight - Rotate x to the right by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateRight(0x87654321,4) = 0x76543218
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 25
 *   Rating: 3 
 */
int rotateRight(int x, int n) {
  //create a copy of x
  int temp = x;
  
  //shift x by n to the right
  x >>= n;

  //shift the n least significant bits in temp all the way to the left
  temp <<= ((33 + ~n)  & 31);

  //undo unwanted sign extension
  x &= ~(~0 << ((33 + ~n) & 31)); 

  x |= temp;

  return x;
}

/* 
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 2
 */
int byteSwap(int x, int n, int m) {

  //create variables with 1s in the nth and mth bytes respectively 
  //n << 3 and m << 3 are multiplication by 8
  int byte_n = 255 << (n << 3);
  int byte_m = 255 << (m << 3);

  //create variable to mask bytes to be swapped to 0
  int bytesToSwap = ~(byte_m | byte_n);

  // extract the mth and nth bytes of x using &
  byte_n &= x;
  byte_m &= x;
  
  //shift bytes n and m all the way to the right
  byte_n >>= (n << 3);
  byte_m >>= (m << 3);

  //undo any unwanted sign extension
  byte_n &= 255;
  byte_m &= 255;

  //swap bytes n and m using shifts
  byte_n <<= (m << 3);
  byte_m <<= (n << 3);

  x &= bytesToSwap;
    
  x |= byte_n;
  x |= byte_m;

  return x;
}

/* 
 * fitsShort - return 1 if x can be represented as a 
 *   16-bit, two's complement integer.
 *   Examples: fitsShort(33000) = 0, fitsShort(-32768) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 1
 */
int fitsShort(int x) {
  //shift x right by 15, will be all 1 or 0 if can be 16-bit tc int
  //compare against x >> 31 (all bits are the same as sign bit)
  int temp = (x >> 31);
  x >>= 15;

  return !(x ^ temp);
}

/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
  //implementation of and using NOT and NOR
  return ~(~x | ~y);
}

/* 
 * subOK - Determine if can compute x-y without overflow
 *   Example: subOK(0x80000000,0x80000000) = 1,
 *            subOK(0x80000000,0x70000000) = 0, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int subOK(int x, int y) {
  //used to check most significant bit
  int xSign = (x >> 31) & 1;
  int ySign = (y >> 31) & 1;

  //stores sign of (x-y)
  int resSign = ((x + (~y + 1)) >> 31) & 1;

  //overflow occurs if x > 0, y < 0, x-y < 0
  //or if x < 0, y > 0, x-y > 0
  //i.e. x & y have different signs and sign of x does not match sign of x-y
  int t1 = xSign ^ ySign;
  int t2 = xSign ^ resSign;

  return !(t1&t2);
}

/* 
 * isGreater - if x > y  then return 1, else return 0 
 *   Example: isGreater(4,5) = 0, isGreater(5,4) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isGreater(int x, int y) {
  // temp y stores the sign bit of x-y
  int xMinusY = x + (~y +1);
  int temp = (xMinusY >> 31) & 1;

  //stores signs of x and y
  int xSign = (x >> 31) & 1;
  int ySign = (y >> 31) & 1;
  
  //x > y if 
    //x > 0 & y < 0, or
    //x < 0, y < 0 & x-y > 0, or
    //x > 0 & > 0 & x-y > 0 
  
  // x <0, y >0 & temp > 0 -> overflow
  int of = (xSign ^ ySign) & (xSign ^ temp);
  // no overflow, temp > 0 -> x > y
  // !!(xMinusY^0) handles case when x = y;
  int t1 = (!of) & (!temp) & (!!(xMinusY^0));
  
  // x > 0, y < 0 -> x > y
  int t2 = (!xSign) & ySign;

  return t1 | t2;
}

/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
  //shift x right n times and store value
  //Compare to the x >> 31 (sign bit in all positions)
  int temp = x >> 31;
  x >>= ((n + (~1+1)) & 31);
  return !(x ^ temp);
}

/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */

int negate(int x) {
  return ~x + 1;
}

/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
  //will be -1 if x is TMax or -1
  int temp = (x + 1) ^ x;
  
  //temp is changed to 1 if x is TMax or -1, it is 0 is any other case
  temp = !(~temp);

  //~x will be 0 if x = -1, and TMin if x is TMax
  //apply ! twice to ~x to get 0 if x = -1, 1 otherwise

  //temp = 1 iff x = TMax or -1, !!(~x) = 1 iff x != -1
  //so temp & !!(~x) = 1 iff x = TMax
  return temp & !!(~x);
}
