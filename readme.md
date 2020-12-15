# Hamiltonian Search Tools

These are tools I built to help me produce a solution for Codewars' 
[Square sums](https://www.codewars.com/kata/square-sums) puzzle.

This isn't a solution, this just helped me get there.

Developed in [VS2019](https://visualstudio.microsoft.com/vs/) with 
[remote development](https://code.visualstudio.com/docs/remote/remote-overview) 
on a Linux box. All together, these use a sort of 
[MapReduce](https://wikipedia.org/wiki/MapReduce) strategy applied to a 
backtracking algorithm.

The C++ code produces an executable that helps determine which solution is 
easiest to arrive at via backtracking given `N` and a starting node (the 
"map" part of MapReduce). `manager.py` orchestrates parallel processing 
and collating the results for a given `N` (the "reduce" part). `batch.sh` 
just runs the manager over all `N` of interest.