# Traveling salesman project poc for 0x41γων C++ intro course


NB: The standard format for tsp problems is `.tsp` [which includes a documentation written here](http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/tsp95.pdf).  
To read every subformat I would've needed to import the lib or write something that would've added complexity to the program.
I opted for a simple python script to transform some of the `.tsp` formats to simple matrices in a text file.
The program will refuse to compute anything that is not a n*n matrix with 0 along the diagonal.

### Results and conclusions
#### Exact answers
Only bruteforce and dynamic programming give exact results, the rest are approximation (that can be really close to the original with various optimisations).  
Multithread results are highly impacted by the amount of syncs one does, still bruteforce multithread is not fast.

Bruteforce whether single or multithreaded can solve easily up to 15/16 cities, at just 17 cities we can see that it took 14 hours with 22 threads whilst running instantly with the same result with dynamic programming.
fri26 was solved with the dynamic approach (26 cities) in 14 minute(s) 33 second(s) whilst it would've taken too long(?) with the bruteforce approach.

For a matrix with size 15.
- Bruteforce single threaded: 7 minute(s) 54 second(s)
- Bruteforce multi threaded 6 threads:  4 minute(s) 4 second(s)
- Bruteforce multi threaded 16 threads:  3 minute(s) 32 second(s)

For a matrix of size 17 (gr17)
- Bruteforce multi threaded 22 threads: 14 hour(s) 18 minute(s) 29 second(s)
- Dynamic solver: 0 minute(s) 1 second(s)

For a matrix of size 26 (fri26)
- Dynamic solver: 0 hour(s) 14 minute(s) 33 second(s)

#### Approximation algorithms
Didn't implement these..

### TODO
- [x] Tests to avoid regressions when trying to optimize (kindof)
- [x] multithread
- [x] implement dynamic programming
- [ ] implement approximation algorithms

### Resources
Forgot to include everything but these were really useful
https://cse442-17f.github.io/Traveling-Salesman-Algorithms/

Results for knowns tsp
http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/STSP.html

Wiki of course
https://en.wikipedia.org/wiki/Travelling_salesman_problem
