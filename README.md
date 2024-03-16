# Traveling salesman project poc for 0x41γων C++ intro course

### Take into account
- The starting node doesn't matter any valid route will touch all the cities
- Don't overcount ie possibilities are less than (n-1)! for bruteforce but (n-1)!/2


### TODO 
- [] Tests to avoid regressions when trying to optimize
- [] multithread (Which seems to be of limited application due to time spent spawning threads)
- [] Other implementation(s)


### Assumptions

#### For bruteforce we need to assume
- Symmetric TSP

#### If we take another approach we might need to assume
- Triangle inequality