#Cats-and-Dogs

This program is based off the problem "Cats vs. Dogs" on the Kattis website.
A link will be provided at the bottom so you are able to see the problem for
yourself. This program computes the Maximum Flow and Minimum Vertex Cover for a
Network Flow Graph. Once the user has provided the necessary information for input,
the program will then create vertices for all votes and create edges for vertices
that are in conflict. Each edge on this network graph has a capacity of 1 when the
program starts.

An example of voters in conflict is that one voter voted for a cat to be kept and
a dog to be eliminated while another voter voted for a dog to be kept and a cat
to be a eliminated. If that voter that had voted for a specific animal to be kept
was the same as another voter that wanted to eliminate it, then these two
voters would be in conflict and an edge to these voters would be made. Voters
that had chosen cats to be kept are described as a cat lover and voters that had
chosen dogs to kept are described as a dog lover. Only edges from a cat lover to
a dog lover can be made. This graph that is created is represented as a Bipartite
graph, where no cat lover can be connected to another cat lover. This is the same
with a dog lover. This program creates a source and sink vertice and creates
edges from the source vertice to all cat lovers and edges from all dog lovers to
the sink vertice.

The algorithm that was used in computing the Maximum Flow is the Edmonds-Karp
algorithm. Edmonds-Karp is similar to the Ford Fulkerson algorithm, however
uses Breadth First Search to find the shortest augmented path from the source
vertex to the sink vertex. The algorithm continues to search the graph until there
does not exist a path from the source vertex to the sink vertex. The network flow
of this graph constantly changes due to the amount of flow that is passed into each
edge each time a breadth first search is made and is successful. The Minimum Vertex
Cover is also interpreted as the number of cat lover vertices that are not reachable
from the source vertice and the number of dog lover vertices that are reachable
from the sink vertice on the residual graph.

https://open.kattis.com/problems/catvsdog

## Run Time

The heart of the algorithm depends on the Edmunds-Karp Aglorithm which uses
Breadth First Search. The runtime for this algorithm is O(V^2E). Breadth First
Search has a running time of O(E) where it might search all edges. Edmonds-Karp
repeats Breadth First Search at most O(V) times where there might contain a path
from the source vertice to the sink vertice. Lastly, we look through this path
that Breadth First Search provided and update the flow network at most O(V) times
as we might look at all vertices in our path.

## Issues

There is not a full work up on error handling. I added some as every program should
have some error handling. Some indentation may be mixed as the beginning of the file
may have 4 spaces of indentation while other parts only two spaces. Everything
works as it should though.

Unfortunately, the only other issue that this program has is that the run time is not
sufficient enough to pass the test on Kattis's website. This probably is due to
the fact that python has a slower runtime than many other languages. Some of the
reasons for this is that python is dynamically typed rather than statically typed
and that python is an interpreted language rather than a compiled language. Both
statically typed and compiled languages are much faster to compute tasks in,
such as C++, as these languages don't waste time figuring out what data type a
variable is and other factors such as these. This project provided me the experience
to now see that a program not only depends on an algorithm for low running time
but as well as the language. Hopefully, in the future, I can be able to convert
the code I wrote here to C++ code so I can compare the results.

## Running the program

```
python3 catsvdogs.py
```
Then insert your information.

```
python3 catsvdogs.py < sample.in > sample.out
```
Where sample.in is a file with your information and sample.out is a file where you
want to contain the output of the program.
