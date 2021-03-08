# traps

Write your explanation of `level_4` here!

I type gdb traps --> layout asm --> layout regs
at the very first I just typed random things cause there is no way to know the password. I notice at level_four+48 there is a read four numbers, so definitely I know the next time I will input 4 numbers, but still don't know what number should be there.
And then i saw level_four+174 has a string not equal function, after that the program check test %eax,%eax, if it is not equal then it will hit the trap.

0x555555555d00 <level_four+179> test   %eax,%eax                           │
   │0x555555555d02 <level_four+181> je     0x555555555d1c <level_four+207>     │
   │0x555555555d04 <level_four+183> mov    -0x48(%rbp),%rax                    │
   │0x555555555d08 <level_four+187> mov    %rax,%rsi                           │
   │0x555555555d0b <level_four+190> mov    $0x4,%edi                           │
   │0x555555555d10 <level_four+195> callq  0x555555555487 <pop_trap>


so when I step in function strings not equal, I check the two arguments which are %rdi and %rsi
I type x/s $rsi = "rice" and x/s $rdi = "pokp", obviously we should fix %rdi to point to "rice"
From the previous instuctions we know that we will load effective address from
-0x15(%rbp) to %rax and then move to %rdi.

0x555555555cf1 <level_four+164> lea    -0x15(%rbp),%rax                    │
   │0x555555555cf5 <level_four+168> mov    %rdx,%rsi                           │
   │0x555555555cf8 <level_four+171> mov    %rax,%rdi

So it is important to keep track of -0x15(%rbp).
At line level_four+143, the instruction is mov %dl,-0x15(%rbp), so we keep a eye on %dl
After trying a few times, I found that level_four+134 is where dl got updated.

<level_four+134> movzbl (%rdx,%rax,1),%edx

All I need to do is for each iteration, make sure the things stored in %dl is equal to the ascii value of "r"=114 "i"=105 "c"=99 "e"=101.

If I try "2 1 0 0" as input password, the password is incorrect at the third iteration and the last iteration, because %dl was 112 and 107, but "c"=99 and "e"=101. The string for $rdi is "rikp", which is not expected.

After repeating the process, honestly it was not hard to figure out the answer is 2 1 2 2.
