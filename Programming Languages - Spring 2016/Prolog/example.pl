/* prolog tutorial 2.14 DFA parser */

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
      write([]), nl.


delta(0,a,1).   
delta(0,b,0).
delta(1,a,1).
delta(1,b,2).
delta(2,a,2).
delta(2,b,2).
 
start(0).

final(2).

