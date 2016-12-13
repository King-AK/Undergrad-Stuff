/* 
 * CS:APP Data Lab 
 * 
 * VJ Davey akdavey
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

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
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
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
  //not x ; not y; our result is not (not x or not y), this means it is x and y 
  return ~(~x | ~y);
}
/* 
 * copyLSB - set all bits of result to least significant bit of x
 *   Example: copyLSB(5) = 0xFFFFFFFF, copyLSB(6) = 0x00000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int copyLSB(int x) {
  //bit shift left to make lsb the msb of the word, make it the sign bit
  //bit shift right to make the new sign bit the lsb once again.
  //all bits should be the same now
  return (x<<31)>>31;
}
/* 
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
  //multiplying 8 times n and right shifting x will pull the specific byte we want
  //255 is a block of 1s, performing an & operation will get rid of all extra bits to the left of the byte we want, and leave the bytes we want in tact
  return 255 & (x>>(n<<3));
}
/* 
 * isEqual - return 1 if x == y, and 0 otherwise 
 *   Examples: isEqual(5,5) = 1, isEqual(4,5) = 0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int isEqual(int x, int y) {
  //any two numbers will be equal if there is no difference between them, bringing it to zero
  //using the bang operator will return 1 on 0, and 0 on anything that isnt 0
  return !(x^y);
}
/* 
 * bitMask - Generate a mask consisting of all 1's 
 *   lowbit and highbit
 *   Examples: bitMask(5,3) = 0x38
 *   Assume 0 <= lowbit <= 31, and 0 <= highbit <= 31
 *   If lowbit > highbit, then mask should be all 0's
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int bitMask(int highbit, int lowbit) {

  int mask = ~0; //a mask consisting of all 1s corresponds to negative one (or not zero)
  highbit = (mask << highbit) << 1; //the highbit(untouched to the left) will include this portion of bits
  lowbit = mask << lowbit ; //the low bits will consist of these portion of bits
  return(highbit ^ lowbit ) & lowbit; // doing xor here in addition to & will make a mask of one bits at the overlap point
  
}
/* 
 * reverseBytes - reverse the bytes of x
 *   Example: reverseBytes(0x01020304) = 0x04030201
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 25
 *   Rating: 3
 */
int reverseBytes(int x) {
  int mask = 255; //byte block filled with 1s
  int byte0 = x & mask; //1st byte
  int byte1 = (x >> 8) & mask;//second byte
  int byte2 = (x >> 16) & mask;//third byte
  int byte3 = (x >>24) & mask;//fourth byte
  return byte3 | (byte2 << 8) | (byte1 << 16) | (byte0 << 24); //combine all the bytes in reverse order using bit shifts. Use of OR will keep bits intact.
  
}
/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
  //not zero is negative 1, that plus one gives zero, but for anything else will return a negative integer on positive and positive on negative
  //doing OR gives x if zero, and some negative number if x isnt zero
  //31 bit shift means if x was zero, you now get one, if x wasnt zero you get zero 
  return (((~x + 1)|x) >> 31) + 1;
}
/* 
 * leastBitPos - return a mask that marks the position of the
 *               least significant 1 bit. If x == 0, return 0
 *   Example: leastBitPos(96) = 0x20
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2 
 */
int leastBitPos(int x) {
  //returns a negative number for anything that isnt zero. Using & operator will mark the lsb
  return (~x+1)&x;
}
/* 
 * minusOne - return a value of -1 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
int minusOne(void) {
  return ~0; //not zero returns a chain of 1s which is negative one
}
/* 
 * TMax - return maximum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmax(void) {
  //return 2147483647; // this is the largest integer 32bit twos complement can give (I looked at the error message)
  return ~(1<<31); // results in the binary number 1 turning to 1 followed by a block of zeroes, to not that, which is some huge ass number
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
  
  int a,b;
  a = 33 + ~n; //gives how many bits are remaining after using n low order bits
  b = ((x<<a)>> a);//makes all the high bits zero
  b = !(b ^ x); //checks equality, sees if x would actually fit inside b
  return b;
}
/* 
 * addOK - Determine if can compute x+y without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int addOK(int x, int y) {
  //overflow occurs in two's complement when adding two negatives makes a positive, or two positives make a negative 
  int sum = x + y;
  //going to do 31 bit shifts to get the signs
  int x_sign = x >> 31;
  int y_sign = y >> 31;
  int sum_sign = sum >> 31;
  //check to see if our signs match
  return !( ~(x_sign ^ y_sign) & (x_sign ^ sum_sign));
}
/* 
 * isGreater - if x > y  then return 1, else return 0 
 *   Example: isGreater(4,5) = 0, isGreater(5,4) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isGreater(int x, int y) {
  //pull the signs
  int x_sign = x >> 31;
  int y_sign = y >> 31;
  //since we are dealign with twos complement, make sure we're dealing with the same with a bang
  int value = !((x +~y) >>31); 
  //if the x sign is 0 when y is one, return 1
  //otherwise we're testing if the value of x is greater than y
  return (!x_sign & y_sign) | (value & (!x_sign | y_sign));
}
/* 
 * isNegative - return 1 if x < 0, return 0 otherwise 
 *   Example: isNegative(-1) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int isNegative(int x) {
  //1 will be the sign bit if x is negative , do a 31 bit shift and see if its equal to 1's binary value (returns only 1 on &)
  return (x >> 31) & 1;
}
/*
 * multFiveEighths - multiplies by 5/8 rounding toward 0.
 *   Should exactly duplicate effect of C expression (x*5/8),
 *   including overflow behavior.
 *   Examples: multFiveEighths(77) = 48
 *             multFiveEighths(-22) = -13
 *             multFiveEighths(1073741824) = 13421728 (overflow)
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 3
 */
int multFiveEighths(int x) {
  x = (x << 2) + x; // x times 4 plus x == x times five
  int add = 7 & (x >> 31); //
  return (x + add ) >> 3 ; //a bitshift of 3 is equal to division by 8
}
/* 
 * sm2tc - Convert from sign-magnitude to two's complement
 *   where the MSB is the sign bit
 *   Example: sm2tc(0x80000005) = -5.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 4
 */
int sm2tc(int x) {
  int mask = 1<<31; //get a block of zeros with a one on the sign bit
  int neg = (mask & x) >> 31; //gives one for negatives and zero for positives
  int pos = ~neg; //gives zero on negatives, one on positives
  int sign = mask & x; // block of zeros with the sign bit of x in the sign bit being the only thing untouched
  
  pos = pos &x; //block of zeros with 0& on negs and 1& on pos
  neg = neg & x;//block of zeros with 1& on negs and 0& on pos
  neg = ~(neg + ~0);//use overflow to get -1 on negs and 0 on pos
  return (neg + pos + sign); //combine the three to get two's complement
}
/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x) {
  
  return 2;
}
