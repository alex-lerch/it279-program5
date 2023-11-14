
/*-------------------------------------------------------------------------------------*
 *                                                                                     *
 *   File: Graph.cpp                                                                   *
 *                                                                                     *
 *   Desc: implemetation file for a representation of a weighted directed              *
 *         graph                                                                       *
 *                                                                                     *
 *   Author: Alex Lerch                                                                *
 *                                                                                     *
 *   Functions:                                                                        *
 *      readGraph(std::string)  ......................  reads in new graph             *
 *      getVertexIndex(std::string)  .................  finds index of a vertex        *
 *      printGraph()  ................................  prints graph structure         *
 *      computeTopologicalSort()  ....................  computes and prints the sort   *
 *      setupInDegreeVector()  .......................  creates list of dependencies   *
 *      printTopologicalSortOutput(list<string>,int) .  prints sort to screen          *
 *                                                                                     *
 *-------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------*
 *   header files                                                                      *
 *-------------------------------------------------------------------------------------*/
#include "Graph.h"
#include<fstream>
#include<iostream>
#include<queue>


/*-------------------------------------------------------------------------------------*
 *   function implementations                                                          *
 *-------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------*
 *   function name: readGraph(std::string)                                             *
 *                                                                                     *
 *   description: reads in a new graph replacing old graph                             *
 *                                                                                     *
 *   returns: true if the file could be read. false otherwise                          *
 *-------------------------------------------------------------------------------------*/
bool Graph::readGraph(std::string fileName) {

    // variables used
    std::string curString; // the current line of the file being read
    std::ifstream infile; // used to read the file to be read
    std::string fromVertexName; // the name of the 'from' vertex being added to the graph
    std::string toVertexName; // the name of the 'to' vertex being added to the graph
    INDEX fromVertexIndex; // the index of the 'from' vertex being added to the graph
    INDEX toVertexIndex; // the index of the 'to' vertex being added to the graph
    int newEdgeCost; // the cost of the new edge being added to the graph

    // setting up to read the file
    infile.open(fileName);

    // make sure file is open. return false if could not be opened
    if ( !infile.is_open() ) {
        return false;
    }

    // clear old graph
    adjacencyList.clear();
    vertexNameList.clear();

    // get the number of vertices
    infile >> numVertices;

    // create lists using the numVertices as the size
    adjacencyList.resize(numVertices);
    vertexNameList.resize(numVertices);
    
    // add each vertex name to vertexNameList
    for (int i = 0; i < numVertices; i++) {
        infile >> curString;
        vertexNameList[i] = curString;
    }

    // get the number of edges
    infile >> numEdges;

    /* cycle through file edges, get data needed for edge and add edge to the ajdacency list */
    for (int i = 0; i < numEdges; i++) {
        // get the pieces of data we need to add a new edge to the adjacency list
        infile >> fromVertexName;
        infile >> toVertexName;
        infile >> newEdgeCost;

        // find the indices of the vertices involved
        fromVertexIndex = getVertexIndex(fromVertexName);
        toVertexIndex = getVertexIndex(toVertexName);

        // add edge to the adjacency list
        adjacencyList[fromVertexIndex].push_back(Graph::Edge(toVertexIndex, newEdgeCost));
    }

    // close the file
    infile.close();

    return true;

}



/*-------------------------------------------------------------------------------------*
 *   function name: getVertexIndex(string)                                             *
 *                                                                                     *
 *   description: finds the index of a vertex based on the name of the vertex          *
 *                                                                                     *
 *   returns: the index of the specified vertex or -1 if not found                     *
 *-------------------------------------------------------------------------------------*/
INDEX Graph::getVertexIndex(std::string nameOfVertexToFind) {
    for (INDEX curIndex = 0; curIndex < vertexNameList.size(); curIndex++) {
        if (vertexNameList[curIndex] == nameOfVertexToFind) {
            return curIndex;
        }
    }

    // return if vertex could not be found
    return -1;
}



/*-------------------------------------------------------------------------------------*
 *   function name: printGraph()                                                       *
 *                                                                                     *
 *   description: prints the graph to std::cout using the same file format as          *
 *                fileName in the readGraph(std::string) function                      *
 *                                                                                     *
 *   returns: n/a                                                                      *
 *-------------------------------------------------------------------------------------*/
void Graph::printGraph() {

    // print the number of vertices
    std::cout << numVertices << "\n";

    // print the vertices with each vertex getting their own line
    for (std::string vertex : vertexNameList) {
        std::cout << vertex << "\n";
    }

    // print the number of edges
    std::cout << numEdges << "\n";

    // print the edges with each edge getting their own line
    for (INDEX curIndex = 0; curIndex < numVertices; curIndex++) {
        for (std::list<Edge>::iterator iter = adjacencyList[curIndex].begin(); iter != adjacencyList[curIndex].end(); ++iter) {
            std::cout << vertexNameList[curIndex] << " " << vertexNameList[iter->toIndex] << " " << iter->cost << "\n";
        }
    }

}



/*-------------------------------------------------------------------------------------*
 *   function name: computeTopologicalSort()                                           *
 *                                                                                     *
 *   description: prints to standard output a topological sort of the graph or an      *
 *                error message indicating that a topological sort of the graph is     *
 *                not possible.                                                        *
 *                                                                                     *
 *   returns: n/a                                                                      *
 *-------------------------------------------------------------------------------------*/
void Graph::computeTopologicalSort() {

    // variables used
    std::vector<int> inDegreeVector; // stores the number of dependencies for each vertex
    std::queue<INDEX> vertexQueue; // queue that processes vertices
    std::list<std::string> topologicalSortOrdering; // the order of the topological sort
    INDEX queueVertexIndex; // the index of the current vertex being looked at
    int topologicalSortCount = 0; // the number of vertices in the topological sort order

    // set up the in-degree array from the graph
    inDegreeVector = setupInDegreeVector();

    // add any vertices with in-degree zero to the queue
    for (INDEX curIndex = 0; curIndex < inDegreeVector.size(); curIndex++) {
        if ( inDegreeVector[curIndex] == 0) {
            vertexQueue.push(curIndex);
        }
    }

    // while the queue is not empty
    while ( !vertexQueue.empty() ) {
        // remove the first vertex
        queueVertexIndex = vertexQueue.front();
        vertexQueue.pop();

        // add name of curVertexIndex to the topological sorting list
        topologicalSortOrdering.push_back(vertexNameList[queueVertexIndex]);
        topologicalSortCount++;

        // for each edge from that vertex
        for (std::list<Edge>::iterator edge = adjacencyList[queueVertexIndex].begin(); edge != adjacencyList[queueVertexIndex].end(); ++edge) {

            // reduce the in-degree by one
            inDegreeVector[edge->toIndex]--;

            // if the in-degree for the vertex is 0, then put it in the queue
            if (inDegreeVector[edge->toIndex] == 0) {
                vertexQueue.push(edge->toIndex);
            }
        }
    }

    // print the topological sort
    printTopologicalSortOutput(topologicalSortOrdering, topologicalSortCount);
    
}



/*-------------------------------------------------------------------------------------*
 *   function name: setupInDegreeVector()                                              *
 *                                                                                     *
 *   description: creates the in-degree vector which stores the number of dependencies *
 *                each vertex in the graph has.                                        *
 *                                                                                     *
 *   returns: the vector that stores the in-degree data for the graph                  *
 *-------------------------------------------------------------------------------------*/
std::vector<int> Graph::setupInDegreeVector() {

    // create in-degree vector
    std::vector<int> inDegreeVector(numVertices);

    // cycle through the edges for each vertex and increment the vertex when it is pointed at
    for (INDEX curIndex = 0; curIndex < adjacencyList.size(); curIndex++) {
        for (std::list<Edge>::iterator iter = adjacencyList[curIndex].begin(); iter != adjacencyList[curIndex].end(); ++iter) {
            inDegreeVector[iter->toIndex]++;
        }
    }

    // return the vector
    return inDegreeVector;
}



/*-------------------------------------------------------------------------------------*
 *   function name: printTopologicalSortOutput()                                       *
 *                                                                                     *
 *   description: prints the correct output for the computeTopologicalSort function    *
 *                to the screen.                                                       *
 *                                                                                     *
 *   returns: n/a                                                                      *
 *-------------------------------------------------------------------------------------*/
void Graph::printTopologicalSortOutput(std::list<std::string>& topologicalSortOrdering, int numVerticesInSortList) {

    // variables used
    int count = 0; // the count used to know when we are at the last vertex in the ordering

    // print out the title
    std::cout << "Topological Sort:\n";

    // if the graph can be sorted then print out the order
    if (numVertices == numVerticesInSortList) {
        for (std::list<std::string>::iterator curVertex = topologicalSortOrdering.begin(); curVertex != topologicalSortOrdering.end(); ++curVertex) {

            // increase the count 
            count++;

            // if we are at the last vertex then don't print the next arrow
            if (count == numVerticesInSortList) {
                std::cout << *curVertex << "\n";
            }

            // we are not at the last vertex so we need to print the next arrow
            else {
                std::cout << *curVertex << " --> ";
            }
        }
    }

    // the graph cannot be sorted and so a message indicating so is printed
    else {
        std::cout << "This graph cannot be topologically sorted.\n";
    }
}



/*-------------------------------------------------------------------------------------*
 *   function name: computeShortestPaths(std::string)                                  *
 *                                                                                     *
 *   description: prints out the path and cost from the parameter vertex to each       *
 *                other reachable vertex in the graph.                                 *
 *                                                                                     *
 *   returns: n/a                                                                      *
 *-------------------------------------------------------------------------------------*/
void Graph::computeShortestPaths(std::string startingVertexName) {

    /* structs needed */
    // represents a vertex in the overall path
    struct PathVertex { 
        public:
            // data members
            bool found; // is the vertex already found
            int totalDistance; // the total distance of the vertex
            INDEX prevVertexIndex; // the index of the previous vertex in the path

            // constructor
            PathVertex(bool foundValue = false, int totalDistanceValue = INT_MAX, INDEX prevVertexValue = INT_MAX)
                : found(foundValue), totalDistance(totalDistanceValue), prevVertexIndex(prevVertexValue) {}
    };

    // represents a vertex in the nextShortestPath queue
    struct QueueVertex {
        public:
            // data members
            INDEX fromVertexIndex; // the index of the from vertex
            INDEX toVertexIndex; // the index of the vertex that fromVertex is going to
            int totalPathCost; // the total path cost to go to toVertex

            /* operator overloads to allow us to compare the totalPathCost of two QueueVertex items */
            bool operator<(const QueueVertex& rhs) const {
                return totalPathCost < rhs.totalPathCost;
            }
            bool operator>(const QueueVertex& rhs) const {
                return totalPathCost > rhs.totalPathCost;
            }
            bool operator==(const QueueVertex& rhs) const {
                return totalPathCost == rhs.totalPathCost;
            }

            // constructor
            QueueVertex(INDEX fromVertexValue = -1, INDEX toVertexValue = -1, int totalPathCostValue = 0)
                : fromVertexIndex(fromVertexValue), toVertexIndex(toVertexValue), totalPathCost(totalPathCostValue) {}
    };

    // variables used
    std::priority_queue< QueueVertex, std::vector<QueueVertex>, std::greater<QueueVertex> > nextShortestPathQueue; // holds the QueueVertex items we want to consider for the shortest path
    INDEX startingVertexIndex = getVertexIndex(startingVertexName); // the index of the starting vertex that we are finding the paths for
    std::vector<PathVertex> pathRepresentation(numVertices); // represents the array that stores all information needed for the path to all vertices
    int numPathsFound = 0; // the number of paths we have found
    QueueVertex nextShortestPath; // represents the next shortest path that we want to add

    // setup values for the starting vertex and update the number of paths found
    pathRepresentation[startingVertexIndex].totalDistance = 0;
    pathRepresentation[startingVertexIndex].prevVertexIndex = -1;
    pathRepresentation[startingVertexIndex].found = true;
    numPathsFound = 1;

    /* fill the nextShortestPath queue with edges going out of the starting vertex */
    // for each edge adjacent to the starting vertex
    for ( Edge curEdge : adjacencyList[startingVertexIndex]) {

        // add the edge as a QueueVertex object to the queue
        nextShortestPathQueue.push(QueueVertex(startingVertexIndex, curEdge.toIndex, curEdge.cost)); 
    }

    /* while the nextShortestPath is not empty and we have not found all of the paths */
    while ( !nextShortestPathQueue.empty() && (numPathsFound != numVertices)) {

        // take the next shortest path
        nextShortestPath = nextShortestPathQueue.top();
        nextShortestPathQueue.pop();

        // if nextShortestPath has not been found yet, update pathRepresentation, increment numPathsFound, and add adjacent edges
        if (pathRepresentation[nextShortestPath.toVertexIndex].found == false) {

            // update pathRepresentation
            pathRepresentation[nextShortestPath.toVertexIndex].prevVertexIndex = nextShortestPath.fromVertexIndex;
            pathRepresentation[nextShortestPath.toVertexIndex].found = true;
            pathRepresentation[nextShortestPath.toVertexIndex].totalDistance = nextShortestPath.totalPathCost;

            // increment numPathsFound
            numPathsFound++;

            /* add adjacent edges */
            // for each edge adjacent to nextShortestPath
            for (Edge curEdge : adjacencyList[nextShortestPath.toVertexIndex]) {

                // if the edge/path has not been found yet, enqueue new QueueVertex item onto the queue
                if (pathRepresentation[curEdge.toIndex].found == false) {

                    // enqueue new item onto the queue
                    nextShortestPathQueue.push(QueueVertex(nextShortestPath.toVertexIndex, curEdge.toIndex, nextShortestPath.totalPathCost + curEdge.cost));
                }
            }
        }
    }

    // asl debug print out the pathRepresentation
    std::cout << "pathRepresentation:" << std::endl;
    int index = 0;
    for (PathVertex curVertex : pathRepresentation) {
        std::cout << "vertex name = " << vertexNameList[index] << std::endl;
        std::cout << "\tfound = " << curVertex.found << std::endl;
        std::cout << "\ttotal distance = " << curVertex.totalDistance << std::endl;
        std::cout << "\tprevious vertex = " << vertexNameList[curVertex.prevVertexIndex] << std::endl;
        index++;
    }


}