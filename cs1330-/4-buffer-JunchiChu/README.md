# buffer

### Level 1

I use objdump to see all the machine code, then I search for function  lights off, which is the very last few digits in my expilot string. If you gdb into program you will know rsp has been subtracted for 64 bits, so the first 56 bits we just overwrite it, and the last 8 bits should represent the return address. All I need to do is overwrite the last 8 digits in little endian form so that it can return to the lightsoff function (90 13 40)

### Level 2

Do the same thing as the previous level, search for sandwich function, get that address, I think it is c0 13 40 in little endian. First step is to overwrite the return address so it can point to sandwich function, and then keep overwrite it. That should also touch the arguments that pass into the sandwich structure. The first 4 digits we make sure they add up is larger than 20, and the next 8 digits is where we stroe id in sandwich_order. We want to check our cookie, which is df ff 19 34. So we put cookie after the sammich_types. That should set id == cookie and total_sammiches > 20 

### Level 3

We overwrite the buffer again, when we overwrite the return address, we want to return to the beginning of our buffer. Note that buffer is considered as the first parameter to Gets, so if we gdb into getbuf (set a breakpoint first), print /x rdi should tell us where the beginning of the buffer is. Overwrite the return address with rdi, and then we need to find the rbp value, which is 8 bits below the return address.

We do x/x $rbp to see the address, also overwrite that right under the return address. After that, we need to write our machine code, note that rax is in convention used as a return value, if we want to return our cookie. We must movl $0x3419ffdf, %eax. Then push the address of the next instruction right after getbuf was called, so search getbuf in test_expilot and find the next instruction. We push that in our machine code, push $0x401620. Now, we should return. So ret at the end. I also padding the rest with 90, so the buffer is fulfilled. 

machine code level 3:

   5:   b8 df ff 19 34          mov    $0x3419ffdf,%eax
   a:   68 75 16 40 00          pushq  $0x401620
   f:   c3                      retq




### Level 4

For this level, our stack is going up and down, but we got a huge allocated space. I decided to pad with nop instruction first, so when the instruction go to the beginning of the buffer, it should touch nop to slide all way the down until the real machine code hit. So it is important to put your assembly code at the end. The structure is: nop......nop.... assembly code .....old rbp...return address. 

Since the stack could move 240 bits away from the original place, I add 240 to the return address. So in the worst case, it can still touch the machine code nop and won't screw up the stack. However, now we need to restore the old rbp value so that we can return back to test_expilot after we finish the machine code. But the stack is shifting each time, a constant rbp overwriting won't work. Since rsp and rbp has relative address 0x20, which I can find it in gdb ( there is a sub 0x20, rsp ). So we move 0x20(rsp) to rbp will always get the correct address of rbp in machine code.

Be carefully in here we want to use leal 0x20(%rsp) %rbp, because movl only pass a copy but we want the 0x20(%rsp) itself be moved into %rbp, so when rsp moves the rbp should move with rsp. And we also move cookie into rax and push the return address of the next line that where callee finishes. Finally, we include ret to make sure the function returns.    

machine code level 4:
0000000000000000 <.text>:
   0:   48 8d 6c 24 20          lea    0x20(%rsp),%rbp
   5:   b8 df ff 19 34          mov    $0x3419ffdf,%eax
   a:   68 75 16 40 00          pushq  $0x401675
   f:   c3                      retq
