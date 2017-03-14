# Creator: Jessie Cisneros

import sys

class Edge(object):
    def __init__(self, u, v, w):
        self.source = u
        self.sink = v
        self.capacity = w
    def __repr__(self):
        return "%s->%s:%s" % (self.source, self.sink, self.capacity)

# Vertex contains all information about the voter
class Vertex(object):
    def __init__(self, name, keep, eliminate):
        self.name = name
        self.keep = keep
        self.eliminate = eliminate
    def __repr__(self):
        return "%s" % self.name

class FlowNetwork(object):
    def __init__(self):
        # All edges of u
        self.adj = {}
        # The flow on the graph by edge
        self.flow = {}
        # All vertices on the graph
        self.vertexList = []
        # Seperate list for cat and dog lovers
        self.catLovers = []
        self.dogLovers = []

    def add_vertex(self, vertex):
        self.adj[vertex] = []
        self.vertexList.append(vertex)

    def get_edges(self, v):
        return self.adj[v]

    def get_edge(self,u,v):
        # O(DogLovers) or O(CatLovers)
        for edge in self.adj[u]:
            if edge.sink == v:
                return edge

    # Save the cat lovers and dog lovers in seperate lists
    def retrieve_lovers(self, catLovers, dogLovers):
        self.catLovers = catLovers
        self.dogLovers = dogLovers

    # Add an edge to u to v with capacity w and v to u with capacity 0
    def add_edge(self, u, v, w=0):
        if u == v:
            raise ValueError("u == v")
        edge = Edge(u,v,w)
        redge = Edge(v,u,0)
        edge.redge = redge
        redge.redge = edge
        self.adj[u].append(edge)
        self.adj[v].append(redge)
        self.flow[edge] = 0
        self.flow[redge] = 0

    # The crucial component of this program. The Edmond Karp algorithm
    # returns the maximum flow that the graph can output from the source
    # node to the sink node
    def edmondsKarp(self):
        # Retrieve the source and sink node
        source = self.vertexList[0]
        sink = self.vertexList[-1]
        TotalFlow = 0

        maxFlow = 0
        while True:
            # Uses BFS to fins the shortest augmented path
            maxFlow, parent = self.BreadthFirstSearch(source,sink)
            # If there are no more augmented paths
            if maxFlow == 0:
                return TotalFlow
            TotalFlow += maxFlow
            v = sink

            # Trace back the path from the source to sink and
            # update the residual graph
            while v != source:
                u = parent[v]
                edge = self.get_edge(u,v)
                self.flow[edge] += maxFlow
                self.flow[edge.redge] -= maxFlow
                v = u

    # Searches the network from the starting vertex to all its
    # neighbors and keeps repeating this scenerio until a path is
    # found to the end node
    # This path will always be the shortest path from the source node
    # to the sink node
    def BreadthFirstSearch(self, start, end):
        # Keeps track of how we got to a certain vertex on the path
        parents = {}

        queue = []
        queue.append(start)
        while queue:
            u = queue.pop(0)
            for edge in self.get_edges(u):
                v = edge.sink
                residual = edge.capacity - self.flow[edge]
                # If there is still capacity on that edge
                # and we have not visit it
                if residual > 0 and not v in parents:
                    parents[v] = u
                    if v != end:
                        queue.append(v)
                    else:
                        return 1, parents
        return 0, parents

    # Searches the network for cat lovers that are not reachable
    # to the source node and dog lovers that are reachable from
    # the source node
    def minimumVertexCover(self):
        recorded = 0
        result = None
        minimumVertexes = []

        for vertex in self.catLovers:
            result = self.BreadthFirstSearch(self.vertexList[0],vertex)
            # If there does not exit a path from the source to this catlover
            if result[0] == 0:
                minimumVertexes.append(vertex)
                recorded = recorded + 1

        for vertex in self.dogLovers:
            result = self.BreadthFirstSearch(self.vertexList[0],vertex)
            # If there does exist a path from the source to this doglover
            if result[0] != 0:
                minimumVertexes.append(vertex)
                recorded = recorded + 1

        return recorded, minimumVertexes


    # Provides trace information on what animals are
    # kept and what each happy person keeps and eliminates
    # Uses standard error for output
    def traceInfo(self, minimumVertexList):
        totalVertexList = self.catLovers + self.dogLovers
        keepVerticeList = []
        keepAnimalList = []
        sentence = ""

        # Remove the source and the sink node
        for vertice in totalVertexList:
            if vertice.name != 's' and vertice.name != 't':
                if vertice not in minimumVertexList:
                    keepVerticeList.append(vertice)

        # Output what animals we are keeping
        for vertice in totalVertexList:
            if vertice.keep not in keepAnimalList:
                sentence = "Keeping " + vertice.keep + "\n"
                sys.stderr.write(sentence)
                keepAnimalList.append(vertice.keep)

        # Output all voters', besides what the minimum vertex cover found,
        # keeping and elimination votes
        for vertice in keepVerticeList:
            sentence = "Happy person: " + "+" + vertice.keep + ",-" + vertice.eliminate + "\n";
            sys.stderr.write(sentence)

        sys.stderr.write("\n")


lines = 0
test_cases = 0
current_case = 0
num_of_cats = 0
num_of_dogs = 0
num_of_voters = 0
voter = 0
finished = False
restart_lines = False

vertex = None
verticeList = []
catLovers = []
dogLovers = []
g = FlowNetwork()

# Restarts the variables for the next Test Case
def cleanup():
  global voter
  global vertex
  global verticeList
  global catLovers
  global dogLovers
  global g

  voter = 0
  vertex = None
  verticeList = []
  catLovers = []
  dogLovers = []
  g = FlowNetwork()

def error():
  print("Correct Input:")
  print("# of testcases")
  print("# of cats, # of dogs, # of voters")
  print("v lines of votes")
  print("ex: C1 and D1")
  sys.exit(0)

def testingInput(string):
  try:
    testing1 = string[0].isalpha()
    testing2 = string.isdigit()
    if testing1 == 'D' and int(testing2) > num_of_dogs:
        error()
    elif testing1 == 'C' and int(testing2) > num_of_cats:
        error()
  except:
    error()

# Creates the edges on the graph
def create_edges():
  # Provides the graph a copy of both list of vertices
  g.retrieve_lovers(catLovers, dogLovers)

  # Create the source and sink vertice and add it to the graph
  source = Vertex('s', None, None)
  sink = Vertex('t', None, None)
  g.vertexList = [source] + g.vertexList
  g.vertexList = g.vertexList + [sink]
  g.adj[source] = []
  g.adj[sink] = []

  # Create the edge from a catLover to a dogLover if they differences
  for catVertex in catLovers:
    for dogVertex in dogLovers:
      if catVertex.keep == dogVertex.eliminate or catVertex.eliminate == dogVertex.keep:
        g.add_edge(catVertex,dogVertex,1)

  # Create an edge from the source node to the cat lovers
  for cat in g.catLovers:
      g.add_edge(source,cat,1)
  # Create an edge from the dog lovers to the sink node
  for dog in g.dogLovers:
      g.add_edge(dog,sink,1)

# For each line the user provides the program
for line in sys.stdin:
  # Split everything on the line into elements in an array
  store = line.split()

  # If ther user has input nothing
  if not line.strip():
    continue
  # If it is the first line I see, then it is the test case variable
  elif lines == 0:
    try:
        test_cases = int(store[0])
    except:
        error()

  # If it is the second line I see, then it is the cats, dogs and voters varibale
  elif lines == 1:
    try:
      num_of_cats = int(store[0])
      num_of_dogs = int(store[1])
      num_of_voters = int(store[2])
    except:
        error()
  # The rest of the lines then must be all votes
  # I will continue looping until I have all votes
  elif lines >= 2:
    # Make a vertex out for the votes and name it after the number of voters recieved
    # The number starts at 0 to voter-1
    vertex = Vertex(voter,store[0],store[1])

    # Differentiate if the voter is a CatLover or DogLover
    if 'C' in vertex.keep:
      catLovers.append(vertex)
    elif 'D' in vertex.keep:
      dogLovers.append(vertex)

    # Add the vertice to the graph
    g.add_vertex(vertex)
    verticeList.append(vertex)
    voter = voter + 1

    # If we have reached the number of votes the user has stated then
    # start creating edges and compute the Max Flow and Minimum Vertex Cover
    if voter == num_of_voters:
      create_edges()
      maxFlow = g.edmondsKarp()
      minCover, coverList = g.minimumVertexCover()
      # If the maxflow and minCover are the same then we know we have our answer
      if maxFlow == minCover:
          print(len(g.vertexList) - maxFlow - 2)
          g.traceInfo(coverList)
      restart_lines = True

  lines = lines + 1

  # Restart for the next test case or terminate program
  if restart_lines == True:
    cleanup()
    restart_lines = False
    # If we have reached the number of test cases then terminate the program
    current_case = current_case + 1
    if current_case == test_cases:
        sys.exit(0)
    elif current_case < test_cases:
        # Restart it when the user needs to input the amount of dogs, cats and voters
        lines = 1
