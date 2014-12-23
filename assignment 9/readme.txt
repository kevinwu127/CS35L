
I did make clean check first to make sure everything was working fine. 

rm -f *.o *.ppm *.tmp srt srt.tgz
gcc -std=gnu99 -g -O2 -Wall -Wextra -Wno-unused-parameter   -c -o main.o main.c
gcc -std=gnu99 -g -O2 -Wall -Wextra -Wno-unused-parameter   -c -o raymath.o raymath.c
gcc -std=gnu99 -g -O2 -Wall -Wextra -Wno-unused-parameter   -c -o shaders.o shaders.c
gcc -std=gnu99 -g -O2 -Wall -Wextra -Wno-unused-parameter -o srt main.o raymath.o shaders.o -lm
time ./srt 1-test.ppm >1-test.ppm.tmp && mv 1-test.ppm.tmp 1-test.ppm

real	0m46.602s
user	0m46.617s
sys	0m0.001s    //will keep this value in mind to contrast against multithreading
time ./srt 2-test.ppm >2-test.ppm.tmp && mv 2-test.ppm.tmp 2-test.ppm
./srt: Multithreading is not supported yet.

real	0m0.002s
user	0m0.000s
sys	0m0.001s    //Obviously, multithreading just returned the error case


My analysis of the homework
1. First got a sense of the overall logic of the program
 - Noticed that the program had four nested for loops that went through each
   pixel and ray-traced them. 
 - From that it was sort of noticeable that we could use multithreads there.

2. An "embarrasingly parallel" program, I created a for loop that divided the
input in the number of threads (nthreads) that were passed in.

3. I created threads using pthread_create and then joined them with
pthread_join.  

4. The most challenging part was creating the 3rd and 4th argument for the
pthread_create function. I tried create a separate struct but later realized
that this could be done with an array of ints.
  - I created a multithread function that essentially handled the cases for
    the pixels within the threads, making sure that each thread wasn't
stepping into other threads. 


After following the following steps,  I got the following 


time ./srt 1-test.ppm >1-test.ppm.tmp && mv 1-test.ppm.tmp 1-test.ppm

real	0m46.200s
user	0m46.178s
sys	0m0.003s
time ./srt 2-test.ppm >2-test.ppm.tmp && mv 2-test.ppm.tmp 2-test.ppm

real	0m23.421s
user	0m46.507s
sys	0m0.003s
time ./srt 4-test.ppm >4-test.ppm.tmp && mv 4-test.ppm.tmp 4-test.ppm

real	0m11.743s
user	0m46.471s
sys	0m0.003s
time ./srt 8-test.ppm >8-test.ppm.tmp && mv 8-test.ppm.tmp 8-test.ppm

real	0m6.249s
user	0m48.734s
sys	0m0.000s
for file in 1-test.ppm 2-test.ppm 4-test.ppm 8-test.ppm; do \
  diff -u 1-test.ppm $file || exit; \
done

We can see a great improvement going from 1 thread to 8. About 80% reduction.
Dividing the work among threads greatly improved the performance of the
program. Even though there is some overhead associated with creating threads,
this time is overcome.
