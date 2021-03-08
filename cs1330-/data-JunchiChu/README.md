# data

Handout [linked here](http://cs.brown.edu/courses/csci0330/data/). Good luck cracking these puzzles 🧩 We're rooting for you 🥳

### Written Questions

Please write your answers for the questions below (see Section 4 of the handout).

**IEEE floating point represents values closest to zero using its denormalized form. All other values, with the exception of a few special cases, are represented using its normalized form.**  

1. **What is the smallest positive value that can be represented as a normalized double-precision number? How much larger is the next smallest value?**  
   Write your answer here!


s = 0 [we make it positive]
exp =  0 0 ... 1 [10 zeros and give the last digit to 1--note that exp shouldn't be 0]

frac = 0 0 0 ...0 [all zeros]
M = 1.<frac>
bias = 2^10-1

the smallest positive value = (-1)^s * 2^(E) * M = (-1)^(0) * 2 ^(1-2^10+1)* 1 
= 2 ^(-1022)


now if we set frac = 0 0 0 ... 0 1 [51 zeros and the last digit is 1]

exp unchanged

so we got the next smallest one = 2^(-1022)*(1+2^(-52));

How much larger?

we do a subtraction:

2^(-1022)*(1+2^(-52))-2^(-1022) = 2^(-1074);

2. **What is the smallest positive value that can be represented as a denormalized double-precision number? How much greater than this is the next smallest value?**  
   Write your answer here!
   
s = 0

exp = 0 0 0..0 [all zeros]

bias = 2^10 -1;
E = -1022;
M = 0.000...01 [the last  digit is 1 , otherwise are all zeros]
smallest positive value of denormalized number :
(-1)^s * 2^(E) *M = 2^(-1022-52) = 2^(-1074); 


If we set frac now is 0...010
so we should get 
the next smallest one:
2^(-1022)*(0.00....10) [the last digit is 0, the one before the last digit set to 1]
=2^(-1022)*2^(-51)
=2^(-1073)

How much larger?
just do a subtraction:
 2^(-1073)-2^(-1074)= 2^(-1074)

### Known Bugs || Unsolved Puzzle Explanations
If you were unable to solve any of the puzzles, please describe the approaches you’ve tried and/or general procedure on how you would try to do it, even if you couldn’t get it all the way working. We may be able to give you partial credit based on what you write here 😊
