/* Checks to see if every element in a list is a member
   of the two other list or is isn't a member of either. 
   Used in intersection                                 */
sameElements([],_,_).
sameElements([H|T],X,Y) :- member(H,X), member(H,Y), sameElements(T,X,Y).
sameElements([H|T],X,Y) :- nomember(H,X), nomember(H,Y), sameElements(T,X,Y).

/* Checks to see if every elment of a list is a member of
   the second list or third list                        */
checkList([],_,_).
checkList([H|T],X,Y) :- member(H,X), checkList(T,X,Y).
checkList([H|T],X,Y) :- nomember(H,X), member(H,Y), checkList(T,X,Y).

/* Checks if a list contains an element */
member(X,[X|_]).
member(X,[_|T]) :- member(X,T).

/* Checks if a list doesn't contain an element */
nomember(_,[]) :- !.
nomember(X,[H|T]) :- X \= H, nomember(X,T).

/* Checks to see if every element of a list is a
   member of another list */
subset([],_).
subset([H|T],K) :- member(H,K), subset(T,K). 

/* Checks to see if a list contains no same elements
   of another list */
disjoint([],_).
disjoint([X|Y],K) :- nomember(X,K), 
                     disjoint(Y,K).

union(L,K,M) :- checkList(M,L,K), 
                subset(L,M), 
                subset(K,M).

intersection(L,K,M) :- sameElements(L,K,M),
                       sameElements(K,L,M), 
                       subset(M,L), 
                       subset(M,K).

/* Checks if the difference list is a subset of L
   then makes sure that every element of L is a member 
   of either the second or third list. Lastly I make 
   sure that the difference list doesn't contain any 
   elements of the second list                       */ 
difference(L,K,M) :- subset(M,L), 
                     checkList(L,K,M), 
                     disjoint(M,K).
