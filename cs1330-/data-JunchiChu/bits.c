#if 0
INTEGER CODING RULES:

	Complete the body of each function with one
	or more lines of C code that implements the function. Your code
	must consist of a sequence of variable declarations, followed by
	a sequence of assignment statements, followed by a return statement.

	The general style is as follows:

	int funct(...) {
		/* brief description of how your implementation works */
		int var1 = expr1;
		...
		int varM = exprM;

		varJ = exprJ;
		...
		varN = exprN;

		return exprR;
	}

	Each "expr" is an expression using ONLY the following:
	1. Integer constants 0 through 255 (0xFF), inclusive. You are
	   not allowed to use big constants such as 0xFFFFFFFF.
	2. Function arguments and local variables (no global variables).
	3. Unary integer operations ! ~
	4. Binary integer operations & ^ | + << >>

	Some of the problems restrict the set of allowed operators even further.
	Each "expr" may consist of multiple operators. You are not restricted to
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
#endif
/* Copyright (C) 1991-2016 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 9.0.0.  Version 9.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2014, fourth edition, plus
   Amd. 1  and Amd. 2 and 273 characters from forthcoming  10646, fifth edition.
   (Amd. 2 was published 2016-05-01,
   see https://www.iso.org/obp/ui/#iso:std:iso-iec:10646:ed-4:v1:amd:2:v1:en) */
/* We do not support C11 <threads.h>.  */
/*
 * bit_and - Compute x&y using only ~ and |
 *   Examples: bit_and(6, 5) = 4
 *             bit_and(3, 12) = 0
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bit_and(int x, int y)
{   /*According to logical rules (demorgan law) that NOT (NOT X OR NOT Y) is the same
      as X AND Y
      It is obvious to use ~ and | to approach the solution.
     */
    int set_up = (~x | ~y);
    
    int demorgan = ~(set_up);
    
    return demorgan;
}
/*
 * negate - Return -x
 *   Examples: negate(1) = -1
 *             negate(0) = 0
 *             negate(-33) = 33
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x)
{   /*2's complement the formula is that you flip the bits first and then
      just add one to it.
     */
    return (~x) + 1;
}
/*
 * is_equal - Return 1 if x == y, else return 0
 *   Examples: is_equal(5, 5) = 1
 *             is_equal(4, 5) = 0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int is_equal(int x, int y)
{   /* I use the nature of bitwise XOR, if all bits are the same XOR gives us 0
        but in this assignment we return 1 if x == y, so we need to apply a logical
       NOT. 
     */
    return !(x ^ y);
}
/*
 * div_pwr_2 - Compute x/(2^n), for 0 <= n <= 30
 *   Round toward zero
 *   Examples: div_pwr_2(15, 1) = 7
 *             div_pwr_2(-33, 4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int div_pwr_2(int x, int n)
{
    /* According to the lecture formula: when x is positive we only need to shift n position 
       to get x/2^n. But if x is negative we should use (x + (1<<n)-1) >> n.
       To know whether x is positive or negative, we shift x >> 31
       And we set x_positive | x_negative 
       if x is positive, set_or_clear = 0...0000
       and the x_negative variable will be & with all zeros so we only keep the value of x_positive
       if x is negative, set_or_clear = 1...1111
       and x_positive will be & with all zeros with the same reason.
     */
    
    int set_or_clear = x >> 31;
    
    int x_positive =  ((~set_or_clear) & (x >> n));
    
    int x_negative =  ((((x + (1 << n) + ((1 << 31) >> 31)))>>n) & (set_or_clear));
    
    return x_positive | x_negative;
 
}
/*
 * leastBitPos - Return a mask that marks the position of the
 *   least significant 1 bit. If x == 0, return 0
 *   Example: leastBitPos(96) = 0x20
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 4
 */
int leastBitPos(int x)
{   /*This is from my observation that for our input x, if we flip x 
      which is ~x, then x & ~x will always be 0. if we add one to ~x,
      which is negation of x, and we apply x & (~x +1 ), that gives whereever 
      the position that pair bits are both 1, and everything else will be all zeros.
      That pair of bit should be exactly at the least bit position
     */
     int negation = ~x + 1;
     
     return (negation & x);
}
/*
 * conditional - Compute the result of x ? y : z
 *   Examples: conditional(2, 4, 5) = 4
 *             conditional(0, 1, 2) = 2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z)
{
  /*Since x could be any positive or negative number, we use !! to preprocess x.
    Now x should only be either 0 or 1. 
    We left shift x 31 bits and then right shift x 31 bits, 
    in order to make it either 11...11111 or 0...000000
    if x is 0, then ~set_or_clear should be all ones, then we & with z should keep the 
    value of z, and set_or_clear with all zeros, that will throw away the value of y
    if x is non-zero, then set_or_clear should be all ones, that should keep the value of y
    and throw away the value of z. 
    We combine ret_y OR ret_z to accomplish the desired result. 
   */
  int a = !!x;
  
  int set_or_clear = (a << 31) >> 31;
  
  int ret_z = ((~set_or_clear) & z);
  
  int ret_y = (set_or_clear & y);
  
  return ret_z | ret_y;
}
/*
 * add_ok - Return 0 if x+y will overflow, 1 otherwise
 *   Examples: add_ok(0x80000000, 0x80000000) = 0
 *             add_ok(0x80000000, 0x70000000) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int add_ok(int x, int y)
{   /* We first shift x and y 31 bits to get the sign bit of them
       and then we also want to shift the sum of x+y to see the sign bit
       overflow happen if x and y are both positive number, which has 0 as the msb 
       but the sum of x and y has 1 as the msb. 
       Or if x and y and both negative number, which has 1 as the msb 
       but the sum of x and y has 0 as the msb.
       From the above observation, an overflow occur if the sign bit of sum are both different from 
       the sign bit of x and sign bit of y. 
       We use XOR to determine if both the difference happen, if yes, res =1, which means overflow
       but we want to return 0, so we apply a logical NOT at the end   
     */
    int sum = x + y;
  
    int x_sign = x >> 31;
  
    int y_sign = y >> 31;
  
    int sum_sign = sum >> 31;
  
    int res= (x_sign ^ sum_sign) & (y_sign ^ sum_sign);
  
    return !res;

}
/*
 * abs_val - Return the absolute value of x
 *   Examples: abs_val(-1) = 1
 *             abs_val(33) = 33
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 5
 */
int abs_val(int x)
{   /*If x is positive, we want to return x. When x is negative, we want to return 
      the negation of x. 
      To know whether x is positive or negative, we right shift x 31 bits.
      So for positive x set_clear is 0...00000
      For negative x set_clear is 1...11111
      if x is positive, we take advantage of ~set_clear are all ones
      set x_positive = (~set_clear & x) will keep the value of x 
      set x_negative is (~x & set_clear)+(set_clear & 1) will be thrown away
      if x is negative, then x_positive will be thrown away, but the value of 
      ~x will be kept and set_clear & 1 gives one more addition to make 2s complement.

      int set_clear = x >> 31;

      int x_positive = (~set_clear & x);

      int x_negative = (~x & set_clear)+(set_clear & 1);

      return x_positive | x_negative;

      This should definitely works but the problem is that it touch the limit of max operations.
      So I realize the steps can be shorten by using XOR gate.
      Because x XOR 0 = x and x XOR 1..1111 is ~x 
      if set_clear is 0..00000 then ~set_clear is 1..11111 and plus 1 make it 0
      if set_clear is 1..1111 then ~set_clear is 0..0000 and plus 1 make it 1
      so we got the desired result.  
     */
     int set_clear = x >> 31;
     
     int xor_gate = x^set_clear;

     return xor_gate + ((~set_clear) + 1);
}
/*
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 5
 */
int bang(int x)
{
    /*This is from observation that for non-zero numbers x
      at least x or negation of x should contain 1 as the msb.
      However, for 0 and negation of x, both should contain 0 as the msb.
      We set check_msb to indict if x is non-zero, then check_msb should 
      be 1...111111, otherwise check_msb is 0...000000
      if x is non-zero, then msb is 1..111111, we flip it and apply & 1 will
      get all zeros
      if x is zero, then msb is 0...0000, we flip it is 1...11111 and apply 
      & 00..000001 will result it 000..000001, which is desired.   
     */
    int check_msb = (x | ((~x) + 1)) >> 31;
  
    return ((~check_msb) & 1);

}
