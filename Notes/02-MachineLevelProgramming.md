# Machine Level Programming

## Basics

## Control

#### Conditional Codes

There are 4 single bit registers for condition codes:
 - CF - Carry Flag (for unsigned)
 - SF - Sign Flag (for signed)
 - ZF - Zero Flag
 - OF - Overflow Flag (for signed)

These are implicitly set by arithmetic operations.

Example: ```addq Src,Dest``` <--> ```t = a+b```

- CF set if carry out from most significant bit (unsigned overflow)
- ZF set if ```t == 0```
- SF set if ```t < 0```
- OF set if two's complement (signed) overflow -> ```(a > 0 && b > 0 && t < 0) || (a < 0 && b < 0 && t >= 0)```

These are not set by ```leaq```

They can be explicitly set by other instructions

Compare Instruction:

cmpq src1,src2

This is like computing src2 - src1 without writing to a destination register

Test Instruction:

testq src1,src2

 - ZF set when a&b == 0
 - SF set when a&b < 0

sets condition codes based on src1&src2

SetX Tnstructions:

Takes a condition code and places it in a register. It sets the low order of a destination
based on a combination of condition codes.

SetX | Condition |  Description
-|-|-
sete |ZF| Equal / Zero
setne |~ZF| Not Equal / Not Zero
sets |SF| Negative
setns |~SF| Nonnegative
setg |~(SF^OF)&~ZF| Greater (Signed)
setge |~(SF^OF)| Greater or Equal (Signed)
setl |(SF^OF)| Less (Signed)
setle |(SF^OF) or ZF| Less or Equal (Signed)
seta |~CF&~ZF| Above (unsigned)
setb |CF| Below (unsigned)

Example:
```
int gt (long x, long y)
{
  return x > y;
}
```

Register | Use(s)
-|-
%rdi | argument x
%rsi | argument y
%rax | return val

```
cmpq %rsi, %rdi   # compare x:y
setg %al          # set last byte of %rax when >
movzbl %al, %eax  # Zero rest of %rax
ret
```

#### Conditional instructions

jX instructions: Jump to different part of code depending on condition codes

jX | Condition | Description
-|-|-
jmp |1| Unconditional
je |ZF| Equal / Zero
jne |~ZF| Not Equal / Not Zero
js |SF| Negative
jns |~SF| Nonnegative
jg |~(SF^OF)&~ZF| Greater (Signed)
jge |~(SF^OF)| Greater or Equal (Signed)
jl |(SF^OF)| Less (Signed)
jle |(SF^OF) or ZF| Less or Equal (Signed)
ja |~CF&~ZF| Above (unsigned)
jb |CF| Below (unsigned)

###### Condition Branch Example (Old Style)

Generation:
>$ gcc –Og -S –fno-if-conversion control.c

No optimization, generate assembly.

```
int absdiff (long x, long y)
{
  long result;
  if (x > y)
    result = x-y;
  else
    result = y-x;
  return result;
}
```

Register | Use(s)
-|-
%rdi | argument x
%rsi | argument y
%rax | return val

```
absdiff:          #This is a label, will be replaced with address when linker runs
  cmpq %rsi, %rdi # x:y
  jle .L4         # checks if x < y using conditional codes, if it is, changes %rip (instruction pointer)
  movq %rdi, %rax
  subq %rsi, %rax
  ret
.L4:              # x <= y, also a label
  movq %rsi, %rax
  subq %rdi, %rax
  ret
```

Expressing with Goto Code:

C allows the goto statement, jump to position designated by label

```
int absdiff (long x, long y)
{
  long result;
  if (x > y)
    result = x-y;
  else
    result = y-x;
  return result;
}
```

is equivalent to:

```
long absdiff_j
(long x, long y)
{
long result;
  int ntest = x <= y;
  if (ntest) goto Else;
  result = x-y;
  goto Done;
Else:
  result = y-x;
Done:
  return result;
}
```

General Conditional Expression Translation (Using Branches):

C code:
```
val = Test ? Then_Expr : Else_Expr;
```

Goto version:
```
  ntest = !Test;
  if (ntest) goto Else;
  val = Then_Expr;
  goto Done;
Else:
  val = Else_Expr;
Done:
. . .
```

###### Using Conditional Move:
```
if(test) dest <- src
```

Conditional move moves a value from ```src``` to ```dest``` if the condition ```test``` is met

C Code:
```
val = Test
  ? Then_Expr
  : Else_Expr;
```

Goto Code:
```
result = Then_Expr;
eval = Else_Expr;
nt = !Test;
if (nt) result = eval;
return result;
```

absdiff using this style:
```
absdiff:
  movq %rdi, %rax # x
  subq %rsi, %rax # result = x-y
  movq %rsi, %rdx
  subq %rdi, %rdx # eval = y-x
  cmpq %rsi, %rdi # x:y
  cmovle %rdx, %rax # if <=, result = eval
  ret
```

Note the used of %rdx, conditional move increases the number of registers used,
but also avoids branching (branches bad for processor pipeline) and avoids changing %rip.

Downside of conditonal move, both values computed:
- ```val = Test(x) ? Hard1(x) : Hard2(x);``` -> bad for expensive computations
- ```val = p ? *p : 0;``` -> could attempt to dereference NULL
- ```val = x > 0 ? x*=7 : x+=3;``` -> possible side effects

#### Loops

###### Do-While Loop:

This is the closest to assembly

Example:

this function counts the number of 1's in _x_

C Code:
```
long pcount_do(unsigned long x)
{
  long result = 0;
  do {
    result += x & 0x1;
    x >>= 1;
  } while (x);
  return result;
}
```

Goto Version:
```
long pcount_goto(unsigned long x)
{
  long result = 0;
  loop:
    result += x & 0x1;
    x >>= 1;
    if(x) goto loop;
    return result;
}
```

Register | Use(s)
-|-
%rdi | argument x
%rax | return val

Assembly:
```
  movl $0, %eax     # result = 0
.L2:                # loop:
  movq %rdi, %rdx
  andl $1, %edx     # t = x & 0x1
  addq %rdx, %rax   # result += t
  shrq %rdi         # x >>= 1
  jne .L2           # if (x) goto loop
  ret
```

General Do-While Translation:

C:
```
do
 BODY
 while(test)
```

Goto:
```
loop:
  BODY
  if (Test)
  goto loop
```

###### While

C:
```
while(test)
  BODY
```

Goto (inefficient):
```
  goto test;
loop:
  BODY
  test:
  if (test)
  goto loop;
done:
```

Goto (more efficient):
```
if (!test)
  goto done;
loop:
  BODY
  if (test)
    goto loop;
done:
```

###### For

C:
```
for (Init; Test; Update )
  BODY
```

Converted to while:
```
while (Test) {
  BODY
  Update;
}
```

This can be expressed in goto form the same way as a while loop, the initial
test can be optimized away (replaced by init).

#### Switch Statements

C Example:
```
long switch_eg
(long x, long y, long z)
{
  long w = 1;
  switch(x) {
    case 1:
      w = y*z;
      break;
    case 2:
      w = y/z;
      /* Fall Through */
    case 3:
      w += z;
      break;
    case 5:
    case 6:
      w -= z;
      break;
    default:
      w = 2;
  }
  return w;
}
```

Switch statements are represented in assembly using a jump table, which stores
the addresses of different blocks of code for each case.

Register | Use(s)
-|-
%rdi | argument x
%rsi | argument y
%rdx | argument z
%rcx | used for division
%rax | return val

First, setup the switch statement:
```
switch_eg:
  movq %rdx, %rcx
  cmpq $6, %rdi     # x:6
  ja .L8            # Use default (unsigned values > 6)
  jmp *.L4(,%rdi,8) # goto *JTab[x]
```

Jump table structure:
- Each target requires 8 bytes, the base address is at .L4

Jump Table:
```
.section .rodata
  .align 8
.L4:
  .quad .L8 # x = 0
  .quad .L3 # x = 1
  .quad .L5 # x = 2
  .quad .L9 # x = 3
  .quad .L8 # x = 4
  .quad .L7 # x = 5
  .quad .L7 # x = 6
```

Each label .LX in the table refers to the code associated with each x value.
The code ```jmp *.L4(,%rdi,8) # goto *JTab[x]``` from the initial setup scales The
x value in %rdi by 8 (byte size), then goes to that location past .L4

Code for each case:
```
L3:                 # Case 1
  movq %rsi, %rax   # y
  imulq %rdx, %rax  # y*z
  ret
.L5:                # Case 2
  movq %rsi, %rax
  cqto
  idivq %rcx # y/z
  jmp .L6           # goto merge
.L9: # Case 3
  movl $1, %eax     # w = 1
.L6:                # merge:
  addq %rcx, %rax   # w += z
  ret
.L7:                # Case 5,6
  movl $1, %eax     # w = 1
  subq %rdx, %rax   # w -= z
  ret
.L8: # Default:
  movl $2, %eax     # 2
  ret
```

## Procedures

#### Stack structure

The x86_64 stack is a region of memory managed using stack disciplines. The register
%rsp contains the lowest stack address (address of top)

###### Push

```
pushq Src
```

- Fetches operand at Src
- decrements %rsp by 8
- write operand at address given by %rsp

###### Pop

```
popq dest
```

- reads value at address given by %rsp
- increments %rsp by 8
- stores value at dest (must be register)

Example:

C:
```
long mult2(long a, long b)
{
  long s = a * b;
  return s;
}

void multstore(long x, long y, long *dest)
{
  long t = mult2(x,y);
  *dest = t;
}
```

Assembly:
```
0000000000400550 <mult2>:
  400550: mov %rdi,%rax       # a
  400553: imul %rsi,%rax      # a * b
  400557: retq                # Return

0000000000400540 <multstore>:
  400540: push %rbx             # Save %rbx
  400541: mov %rdx,%rbx         # Save dest
  400544: callq 400550 <mult2>  # mult2(x,y)
  400549: mov %rax,(%rbx)       # Save at dest
  40054c: pop %rbx              # Restore %rbx
  40054d: retq                  # Return
```

###### Procedure Control Flow

The stack is used to support the procedure call and return
```
call LABEL
```
- Push return address on stack
- jump to LABEL

Return address:
- Address of the next instruction right after call
- Example from disassembly

Procedure return: ```ret```
- Pop address from stack
- Jump to address

###### Passing Data

The first six items are stored in the following registers:
- %rdi
- %rsi
- %rdx
- %rcx
- %r8
- %r9
Any others are stored on the stack

The return value is stored at %rax

###### Stack-Based Languages

- Languages that support recursion
  - e.g., C, Pascal, Java
  - Code must be “Reentrant”
    - Multiple simultaneous instantiations of single procedure
  - Need some place to store state of each instantiation
    - Arguments
    - Local variables
    - Return pointer
- Stack discipline
  - State for given procedure needed for limited time
    - From when called to when return
  - Callee returns before caller does
- Stack allocated in Frames
  - state for single procedure instantiation

###### Stack Frames

Contents:
  - Return information
  - Local storage (if needed)
  - Temporary space (if needed)

Management:
  - allocated when procedure entered
    - pushed by ```call```
  - deallocated during return
    - popped by ```returns```

%rsp (stack pointer) points to the bottom of the stack
%rbp (optional) acts as the frame pointer.

###### Register Saving Conventions

When procedure yoo calls who:
  - yoo is the caller
  - who is the callee

Conventions
  - “Caller Saved”
    - Caller saves temporary values in its frame before the call
  - “Callee Saved”
    - Callee saves temporary values in its frame before using
    - Callee restores them before returning to caller

Caller Saved Registers:
- %rax - return value
- %rdi - Argument
- %rsi - Argument
- %rdx - Argument
- %rcx - Argument
- %r8 - Argument
- %r9 - Argument
- %r10 - Caller Saved Temporary
- %r11 - Caller Saved Temporary

Callee Saved Temporaries:
- %rbx
- %r12
- %r13
- %r14
- %rbp - Special
  - Callee must save & restore
  - May be used as frame pointer
  - Can mix & match
- %rsp - Special
  - Restored to original value upon 
  - exit from procedure

#### Recursion

Example:

Counts number of 1s

C:
```
/* Recursive popcount */
long pcount_r(unsigned long x)
{
  if (x == 0)
    return 0;
  else
    return (x & 1) + pcount_r(x >> 1);
}
```

Assembly:
```
pcount_r:
  movl $0, %eax
  testq %rdi, %rdi
  je .L6
  pushq %rbx
  movq %rdi, %rbx
  andl $1, %ebx
  shrq %rdi
  call pcount_r
  addq %rbx, %rax
  popq %rbx
.L6:
  ret
```

## Data

#### Arrays

```
T a[L];
```
is an array of type T with L elements. It is a contiguously allocated region of
L*sizeof(T) bytes in memory


Array access example:

C:
```
int get_digit(int[5] z, int digit)
{
  return z[digit]
}
```

Assembly:
```
  # %rdi = z
  # %rsi = digit
movl (%rdi,%rsi,4), %eax # z[digit]
```

Array Loop Example:

C;
```
void zincr(zip_dig z)
{
  size_t i;
  for (i = 0; i < ZLEN; i++)
  z[i]++;
}
```

Assebmly:
```
  # %rdi = z
  movl $0, %eax # i = 0
  jmp .L3 # goto middle
.L4: # loop:
  addl $1, (%rdi,%rax,4) # z[i]++
  addq $1, %rax # i++
.L3: # middle
  cmpq $4, %rax # i:4
  jbe .L4 # if <=, goto loop
  rep; ret
```

#### Multidimensional Arrays

2D Array example
  - Declaration: ```T A[R][C];```
  - 2D array of type T
  - R rows, C columns
  - R*C*sizeof(T) bytes;
  - Each array is stored contiguously in memory according to row-major order
    - Row-major order means a row is stored before any elements in the next column are


**Nested Array Example:**

Row Access Example:

C:
```
#define PCOUNT 4
typedef zipdig int[5];

zip_dig pgh[PCOUNT] =
{{1, 5, 2, 0, 6},
{1, 5, 2, 1, 3 },
{1, 5, 2, 1, 7 },
{1, 5, 2, 2, 1 }};

int *get_pgh_zip(int index)
{
  return pgh[index];
}
```

Assembly:
```
# %rdi = index
leaq (%rdi,%rdi,4),%rax   # 5 * index
leaq pgh(,%rax,4),%rax    # pgh + (20 * index), note pgh is a label in the assembly
```

Element Access Example:

C:
```
int get_pgh_digit(int index, int dig)
{
  return pgh[index][dig];
}
```

Assembly:
```
leaq (%rdi,%rdi,4), %rax  # 5*index
addl %rax, %rsi           # 5*index+dig
movl pgh(,%rsi,4), %eax   # M[pgh + 4*(5*index+dig)]
```

**Multi-Level Array Example:**

Element Access:

C:
```
zip_dig cmu = { 1, 5, 2, 1, 3 };
zip_dig mit = { 0, 2, 1, 3, 9 };
zip_dig ucb = { 9, 4, 7, 2, 0 };
#define UCOUNT 3
int *univ[UCOUNT] = {mit, cmu, ucb};

int get_univ_digit(size_t index, size_t digit)
{
  return univ[index][digit];
}
```

Assembly:
```
salq $2, %rsi               # 4*digit
addq univ(,%rdi,8), %rsi    # p = univ[index] + 4*digit
movl (%rsi), %eax           # return *p
ret
```

Computation:
- Element access Mem[Mem[univ+8*index]+4*digit]
- Must do two memory reads
  - First get pointer to row array
  - Then access element within array


Computation Differences Between Nested and Multi-Level Arrays:
- Nested: Mem[pgh+20*index+4*digit]
- Multi-Level: Mem[Mem[univ+8*index]+4*digit]

#### Structures

Structures are stored in memory as contiguous blocks, with each item in order of
declaration.

Getting pointer to array in struct:

C:
```
struct rec {
  int a[4];
  size_t i;
  struct rec *next;
};

int *get_ap
(struct rec *r, size_t idx)
{
  return &r->a[idx];
}
```
Assembly:
```
# r in %rdi, idx in %rsi
leaq (%rdi,%rsi,4), %rax
ret
```

Following linked list in struct:

C:
```
void set_val
(struct rec *r, int val)
{
  while (r)
  {
    int i = r->i;
    r->a[i] = val;
    r = r->next;
  }
}
```

Assembly:
```
.L11: # loop:
  movslq 16(%rdi), %rax     # i = M[r+16]
  movl %esi, (%rdi,%rax,4)  # M[r+4*i] = val
  movq 24(%rdi), %rdi       # r = M[r+24]
  testq %rdi, %rdi          # Test r
  jne .L11                  # if !=0 goto loop
```

Items in a structure are restricted to addresses that are multiples of their size.
e.g. shorts can only start on multiples of 2, doubles can only start on multiples
of 8, etc.

A structure itself is aligned to the alignment of the largest primitive data type within.

Accessing array elements in an array of structs:

C:
```
struct S3 {
  short i;
  float v;
  short j;
} a[10];

short get_j(int idx)
{
  return a[idx].j;
}
```

- Compute array offset 12 \* idx
- Element j is at offset 8 relative to the start of the struct.
- assembler gives offset a + 8, which is resolved during linking

Assembly:
```
# %rdi = idx
  leaq (%rdi,%rdi,2),%rax # 3*idx
  movzwl a+8(,%rax,4),%eax
```

Space in arrays can be saved by putting larger data types first, for example:
```
struct S4 {
  char c;
  int i;
  char d;
} *p;
```
```
struct S5 {
  int i;
  char c;
  char d;
} *p;
```

Here _S4_ takes up 12 bytes, while _S5_ takes up 8.

#### Union

A union is similar to a structure, but can only use one field at a time. Unions
are allocated according to the largest element.

```
union U1 {
  char c;
  int i[2];
  double v;
} *up;
```

_U1_ Would be aligned at 8 bytes.

#### Buffer Overflow

Buffer overflow is an exploit that involves exceeding the allocated memory size
of an array.

Buffer overflow is the number one technical cause of security vulnerabilities.

Example:
```
/* Get string from stdin */
char *gets(char *dest)
  {
    int c = getchar();
    char *p = dest;
    while (c != EOF && c != '\n') 
    {
      *p++ = c;
      c = getchar();
    }
  *p = '\0';
  return dest;
}

/* Echo Line */
void echo()
{
  char buf[4]; /* Way too small! */
  gets(buf);
  puts(buf);
}

void call_echo() {
  echo();
}
```

The function ```gets()``` does not specify a character limit. ```echo()```
does not have any protection for inputs of length greater than 4, and larger
inputs can corrupt the stack, if large enough, the  return address can be altered.

One method to avoid buffer overflows is to use ```fgets()``` instead of ```gets()```
which has protections on input size.

A system-level protection against buffer overflows is to use randomized stack
offsets by allocating a random number of bytes to the stack at the start of the
program, this makes it difficult to predict beginning of inserted code. Another
system level protection is to mark a region of memory "read-only" in x86, or to
not mark a region executable in x86_64.
