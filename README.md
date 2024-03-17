# Traveling salesman project poc for 0x41γων C++ intro course

NB: The standard format for tsp problems is `.tsp` [which includes a documentation written here](http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/tsp95.pdf)
To read every subformat I would've needed to import the lib or write something that would've added complexity to the program.
I opted for a simple python script to transform some of the `.tsp` formats to simple matrices in a text file.
The program will refuse to compute anything that is not a n*n matrix with 0 along the diagonal. TODO!(to implement)

### Results and conclusions
##### Exact answers
Only bruteforce and dynamic programming give exact results, the rest are approximation (that can be really close to the original with various optimisations). 
Multithread results are highly impacted by the amount of syncs one does.  

Bruteforce whether single or multithreaded can solve easily up to 15/16 cities, at just 17 cities we can see that it took 14 hours with 22 threads whilst running instantly with the same result with dynamic programming.
I could solve fri26 with the dynamic approach (26 cities) in 14 minute(s) 33 second(s) whilst it would've taken too long(?) for the bruteforce approach.

For a matrix with size 15 and optimal result 242
- Bruteforce single threaded: 7 minute(s) 54 second(s)
- Bruteforce multi threaded 16 threads:  3 minute(s) 32 second(s)
- Bruteforce multi threaded 6 threads:  4 minute(s) 4 second(s)

17x17
gr17 : 2085

Running with threads n: 22
Final minimum distance: 2085
Path: 0 3 12 6 7 5 16 13 14 2 10 9 1 4 8 11 15
Execution time: 14 hour(s) 18 minute(s) 29 second(s)

Using dynamic solver
Minimum distance: 2085
Execution time: 0 hour(s) 0 minute(s) 0 second(s)

26x26 matrix (fri26)
Minimum distance: 937
Path:
Execution time: 0 hour(s) 14 minute(s) 33 second(s)

#### Approximation algorithms
Didn't implement these..

### TODO
- [ ] Tests to avoid regressions when trying to optimize
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