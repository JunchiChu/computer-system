a description of your strategy for maintaining compaction (i.e. how are you preventing your heap from turning into a bunch of tiny free blocks?)

Answer: My plan one was extend a huge heap, like 3000 and then slice the heap when the requested size is smaller. But I realize this is buggy, and lower my util. So I change it to, extend the heap whatever they ask for. If you ask for 10, I gave you 10, if you ask for 300, I gave you 300. Thats it, not more free lunch. This plan can increase my util by a lot and avoid other bugs.

So the first step is to check the free list, if there is one candidate can help . If not, we just extend the heap by whatever they ask for. The advantage of this plan can also avoid crush the heap by extending way too much or waste space.

your mm_realloc() implementation strategy

Answer:
There are 5 cases I handle:

case0: edge case (size=0,ptr=null) and when requested size is less than original size, just return the payload. I tried to combine the next unallocated block but they will increase my free list with lots of tiny blocks, which lower my util. So I just return payload of current pointer.

case1: if requested size is larger, and combine the previous unallocated block is enough. So pull the previous block off from free list and combine it with curent block, also copy the memory of ptr because now pointer is pointing to the previous block.

case2:  if requested size is larger, and combine the next unallocated block is enough. So pull the next block off from free list and combine it with current block, note that this case you don't copy the memory of ptr because now pointer is pointing to the current block. If you still copy the memory it won't hurt but lower your performance.

case3: If requested size is larger, only combine one of the previous or next unallocated block cannot help. Then we check to see if it can help if we combine both previous and next. The same process and also need to copy ptr content to previous pointer since the pointer is pointing to prev.

case4: Even combine previous and next unallocated block, it doesn't work out. Check the free list to see if there is a free block can help. This is similar to malloc part, which you iterate the free list and check if there is one available. You need to copy the ptr content since a block in the free list don't know where ptr points to and ptr must be freed to improve the performance.

case5: After checking previous/next/or combined/free list, none of them can help. The only way to solve is to extend the heap, just call malloc again and also need to copy the ptr content. After copied the cotent, must free ptr to improve performance.

unresolved bugs with your program

Answer: no bugs at least from what I saw. Performance is good, got 83% util and 100 throughput.

Any other optimizations:

for mm init, I called sbrk once to avoid call it twice.

for malloc, I only extend the heap whatever they ask to, so I avoid a lots of rearrange code to imporve my both util and throughput. Also I check the free list to see if there is a block available.

for realloc, I use 5 cases to avoid waste memory. Check the details for all 5 cases above or in my code.

Thank you.

trace#  name                  consistent   util   passed?
-----------------------------------------------------------------------
 0      amptjp-bal.rep        yes       89.4     PASSED
 1      cccp-bal.rep          yes       91.6     PASSED
 2      cp-decl-bal.rep       yes       95.0     PASSED
 3      expr-bal.rep          yes       96.3     PASSED
 4      random-bal.rep        yes       87.8     PASSED
 5      random2-bal.rep       yes       84.9     PASSED
 6      binary-bal.rep        yes       53.7     PASSED
 7      binary2-bal.rep       yes       47.4     PASSED
 8      coalescing-bal.rep    yes       99.2     PASSED
 9      coalescing2-bal.rep   yes       99.3     PASSED
 10     realloc-bal.rep       yes       78.3     PASSED
 11     realloc2-bal.rep      yes       83.9     PASSED

Computing performance index from average util 83.897898 and throughput score 100.000000

Thank you!