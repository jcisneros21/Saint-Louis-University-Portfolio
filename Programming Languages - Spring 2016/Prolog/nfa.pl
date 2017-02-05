/* Worked with Joey on this problem */
/* #2 and #3, I did by myself */

/* Created a new fact with two parameters */
delta(1,3).
delta(3,2).
delta(1,0,2).
delta(2,1,2).
delta(2,1,4).
delta(4,0,3).
delta(3,0,4).
 
start(1).

final(3).
final(4).

parse(L) :- start(S), 
            trans(S,L).
            
trans(X,[A|B]) :- 
      delta(X,A,Y),   /*  X ---A---> Y */
      write(X),
      write('  '),
      write([A|B]),
      nl,
      trans(Y,B).  
trans(X,[]) :- 
      final(X),
      write(X),
      write('  '),
      write([]), 
      nl.
/* Made new rule for new fact */
/* Modied the DFA with the new rule to make a NFA */
trans(X,[A|B]) :-
      delta(X,Y),
      write(X),
      write('  '),
      write([A|B]),
      nl,
      trans(Y,[A|B]).
