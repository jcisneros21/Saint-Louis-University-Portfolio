/* base case is when the computation of mod
   returns 0 */
evenly(X,Y) :- 0 is X mod Y.
evenly(X,Y) :- R is Y+1, X > R, evenly(X,R).

prime(1).
prime(2).
prime(X) :- not(evenly(X,2)). 
