# Traveling salesman project poc for 0x41γων C++ intro course

### Take into account
- [x] The starting node doesn't matter any valid route will touch all the cities
- Don't overcount ie possibilities are less than (n-1)! for bruteforce but (n-1)!/2

### Results

For this matrix with size 15 and optimal result 242
- Bruteforce single threaded: 7 minute(s) 54 second(s)
- Bruteforce multi threaded 16 threads:  3 minute(s) 32 second(s)
- Bruteforce multi threaded 6 threads:  4 minute(s) 4 second(s)

``` TOML
[Matrix]
0 10 20 30 40 50 60 70 80 90 25 35 45 55 65
10 0 15 25 35 45 55 65 75 85 95 20 30 40 50
20 15 0 18 28 38 48 58 68 78 88 23 33 43 53
30 25 18 0 14 24 34 44 54 64 74 29 39 49 59
40 35 28 14 0 16 26 36 46 56 66 19 13 23 33
50 45 38 24 16 0 12 22 32 42 52 17 27 37 47
60 55 48 34 26 12 0 11 21 31 41 15 25 35 45
70 65 58 44 36 22 11 0 13 23 33 18 28 38 48
80 75 68 54 46 32 21 13 0 17 27 14 24 34 44
90 85 78 64 56 42 31 23 17 0 19 12 22 32 42
25 95 88 74 66 52 41 33 27 19 0 10 20 30 40
35 20 23 29 19 17 15 18 14 12 10 0 11 21 31
45 30 33 39 13 27 25 28 24 22 20 11 0 12 22
55 40 43 49 23 37 35 38 34 32 30 21 12 0 16
65 50 53 59 33 47 45 48 44 42 40 31 22 16 0
```

Multithread results are highly impacted by the amount of syncs one does 

### TODO 
- [] Tests to avoid regressions when trying to optimize
- [x] multithread 
- [] Other implementation(s)


### Assumptions

#### For bruteforce we need to assume
- Symmetric TSP

#### If we take another approach we might need to assume
- Triangle inequality