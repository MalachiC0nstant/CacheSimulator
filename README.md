# CacheSimulator
The purpose of this mini project is to simulate the workings of a LRU (least recently used) cache.
The program accepts 4 command line arguments: nk (the capacity of the cache in kb), assoc (the associativity of the cache), blocksize (the size of a single cache block in bytes), and repl (replacement policy).
Given the input, the program will simulate the respective cache and will print out the following information respectively: total number of misses, % of misses, total number of read misses, % of read misses, total number of write misses, & of write misses.
Using 28 traces from SPEC CPU 2006, https://www.spec.org/cpu2006/Docs/readme1st.html, the program can be tested.

Example of running the program:
Compiling in terminal: g++ cache.cpp -o cache
Running program using PERL's benchmark trace: gzip -dc 400.perlbench-41B.trace.txt.gz | ./cache 128 16 64 r
Output: 234 0.0234% 212 0.0238583% 22 0.0197448% 

