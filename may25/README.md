# May 2025: Number Cross 5 

The puzzle can be seen here: [Number Cross 5](https://www.janestreet.com/puzzles/number-cross-5-index/)

I used three layers of nested backtracking (find_digits -> tile -> increment) to find the solution to the puzzle. 
Due to the high number of possible solutions, I increased the speed through memoization (of both impossible row pairs and impossible digit placement), aggressive pruning, and compiler optimization. The code finds the correct solution in under an hour. 
