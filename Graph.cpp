
/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 *   File: Graph.cpp                                                                                          *
 *                                                                                                            *
 *   Desc: implemetation file for a representation of a weighted directed                                     *
 *         graph                                                                                              *
 *                                                                                                            *
 *   Author: Alex Lerch                                                                                       *
 *                                                                                                            *
 *   Functions:                                                                                               *
 *      readGraph(std::string)  .....................................  reads in new graph                     *
 *      getVertexIndex(std::string)  ................................  finds index of a vertex                *
 *      printGraph()  ...............................................  prints graph structure                 *
 *      computeTopologicalSort()  ...................................  computes and prints the sort           *
 *      setupInDegreeVector()  ......................................  creates list of dependencies           *
 *      printTopologicalSortOutput(list<string>,int) ................  prints sort to screen                  *
 *      computeShortestPaths(std::string)  ..........................  calculates and prints paths            *
 *      printPathList(list<string>)  ................................  prints the list passed                 *
 *      printShortestPathsOutput(string, vector<PathVertex>) ........  prints output for computeShortestPaths *
 *      buildPathRepresentation(vector<PathVertex>, priority_queue) .  builds the pathRepresentation vector   *
 *      computeMinimumSpanningTree()  ...............................  prints edges, weight of minimum tree   *
 *      populateEdgesList(list<Edge>)  ..............................  adds the edges of a graph to a list    *
 *      calculateSpanningTreeTotalCost(list<Edge>)  .................  calculates the total cost of the tree  *
 *      printMinimumSpanningTreeOutput(list<Edge>, unsigned int)  ...  prints the output for spanning tree    *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------*
 *   header files                                                                      *
 *-------------------------------------------------------------------------------------*/
#include "Graph.h"
#include<fstream>
#include<iostream>
#include<queue>

/*-------------------------------------------------------------------------------------*
 *   constants                                                                         *
 *-------------------------------------------------------------------------------------*/
const int END_OF_PATH = -1; // represents the end vertex when creating paths


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

    /*-------------------------------------------------------------------------------------*
     *   variables used                                                                    *
     *-------------------------------------------------------------------------------------*/
    std::string curString; // the current line of the file being read
    std::ifstream infile; // used to read the file to be read
    std::string fromVertexName; // the name of the 'from' vertex being added to the graph
    std::string toVertexName; // the name of the 'to' vertex being added to the graph
    int fromVertexIndex; // the index of the 'from' vertex being added to the graph
    int toVertexIndex; // the index of the 'to' vertex being added to the graph
    int newEdgeCost; // the cost of the new edge being added to the graph

    /*-------------------------------------------------------------------------------------*
     *   read in the file, delete the old graph, and create the new graph                  *
     *-------------------------------------------------------------------------------------*/
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
        adjacencyList[fromVertexIndex].push_back(Graph::AdjListVertex(toVertexIndex, newEdgeCost));
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
int Graph::getVertexIndex(std::string nameOfVertexToFind) {

    for (int curIndex = 0; curIndex < vertexNameList.size(); curIndex++) {
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
    for (int curIndex = 0; curIndex < numVertices; curIndex++) {
        for (std::list<AdjListVertex>::iterator iter = adjacencyList[curIndex].begin(); iter != adjacencyList[curIndex].end(); ++iter) {
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

    /*-------------------------------------------------------------------------------------*
     *   variables used                                                                    *
     *-------------------------------------------------------------------------------------*/
    std::vector<int> inDegreeVector; // stores the number of dependencies for each vertex
    std::queue<int> vertexQueue; // queue that processes vertices
    std::list<std::string> topologicalSortOrdering; // the order of the topological sort
    int queueVertexIndex; // the index of the current vertex being looked at
    int topologicalSortCount = 0; // the number of vertices in the topological sort order

    /*-------------------------------------------------------------------------------------*
     *   calculate and print the topological sort                                          *
     *-------------------------------------------------------------------------------------*/
    // set up the in-degree array from the graph
    inDegreeVector = setupInDegreeVector();

    // add any vertices with in-degree zero to the queue
    for (int curIndex = 0; curIndex < inDegreeVector.size(); curIndex++) {
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
        for (std::list<AdjListVertex>::iterator edge = adjacencyList[queueVertexIndex].begin(); edge != adjacencyList[queueVertexIndex].end(); ++edge) {

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
    for (int curIndex = 0; curIndex < adjacencyList.size(); curIndex++) {
        for (std::list<AdjListVertex>::iterator iter = adjacencyList[curIndex].begin(); iter != adjacencyList[curIndex].end(); ++iter) {
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

    // if the graph can be sorted then print out the order
    if (numVertices == numVerticesInSortList) {

        // print out the title
        std::cout << "Topological Sort:\n";

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
 *   description: computes and prints out the path and cost from the parameter vertex  *
 *                to each other reachable vertex in the graph.                         *
 *                                                                                     *
 *   returns: n/a                                                                      *
 *-------------------------------------------------------------------------------------*/
void Graph::computeShortestPaths(std::string startingVertexName) {

    /*-------------------------------------------------------------------------------------*
     *   variables used                                                                    *
     *-------------------------------------------------------------------------------------*/
    // holds the Edge items we want to consider for the shortest path
    std::priority_queue< Edge, std::vector<Edge>, std::greater<Edge> > nextShortestPathQueue;

    // the index of the starting vertex that we are finding the paths for
    int startingVertexIndex = getVertexIndex(startingVertexName); 

    // represents the array that stores all information needed for the path to all vertices
    std::vector<PathVertex> pathRepresentation(numVertices); 

    /*-------------------------------------------------------------------------------------*
     *   compute the shortest paths for the starting vertex                                *
     *-------------------------------------------------------------------------------------*/
    // setup values for the starting vertex and update the number of paths found
    pathRepresentation[startingVertexIndex].totalDistance = 0;
    pathRepresentation[startingVertexIndex].prevVertexIndex = END_OF_PATH;
    pathRepresentation[startingVertexIndex].found = true;

    /* fill the nextShortestPath queue with edges going out of the starting vertex */
    // for each edge adjacent to the starting vertex
    for ( AdjListVertex curEdge : adjacencyList[startingVertexIndex]) {

        // add the edge as a Edge object to the queue
        nextShortestPathQueue.push(Edge(startingVertexIndex, curEdge.toIndex, curEdge.cost)); 
    }
    
    // build the pathRepresentation
    buildPathRepresentation(pathRepresentation, nextShortestPathQueue);

    // print out the paths that can be found
    printShortestPathsOutput(startingVertexName, pathRepresentation);
}



/*-------------------------------------------------------------------------------------*
 *   function name: printPathList(list<string>)                                        *
 *                                                                                     *
 *   description: prints out the list that is passed. helper function for              *
 *                printShortestPathsOutput.                                            *
 *                                                                                     *
 *   returns: n/a                                                                      *
 *-------------------------------------------------------------------------------------*/
void Graph::printPathList(std::list<std::string>& pathList) {

    /*-------------------------------------------------------------------------------------*
     *   variables used                                                                    *
     *-------------------------------------------------------------------------------------*/
    // counts the number of vertices we have processed
    int count = 0;

    /*-------------------------------------------------------------------------------------*
     *   print the path list                                                               *
     *-------------------------------------------------------------------------------------*/
    /* go through the path list and print each entry with an arrow only if it is not the last item */
    for (std::string curVertexName : pathList) {

        // if we are at the last item
        if (count == pathList.size() - 1) {
            std::cout << curVertexName;
        }
        else { // we are not at the last item
            std::cout << curVertexName << " --> ";
        }

        // update number of items processed
        count++;
    }

}


/*-------------------------------------------------------------------------------------*
 *   function name: printShortestPathsOutput(string, vector<PathVertex>)               *
 *                                                                                     *
 *   description: prints the correct output for the computeShortestPaths function      *
 *                                                                                     *
 *   returns: n/a                                                                      *
 *-------------------------------------------------------------------------------------*/
void Graph::printShortestPathsOutput(std::string startingVertexName, std::vector<PathVertex>& pathRepresentation) {

    /*-------------------------------------------------------------------------------------*
     *   variables used                                                                    *
     *-------------------------------------------------------------------------------------*/
    // the index of the vertex that we are starting with
    int startingVertexIndex = getVertexIndex(startingVertexName);

    // the index of a vertex that we are wanting to add to the path
    int vertexIndexToAdd;

    // represents a path from one vertex to another
    std::list<std::string> path; 

    /*-------------------------------------------------------------------------------------*
     *   print the output                                                                  *
     *-------------------------------------------------------------------------------------*/
    /* print label for output and go through each vertex we found a path for to print out that path */
    std::cout << "Shortest paths from " << startingVertexName << ":\n";
    int curIndex = 0;
    for (PathVertex curVertex : pathRepresentation) {
        
        // if a path to curVertex has been found
        if (curVertex.found && (startingVertexIndex != curIndex)) {

            /* build the list that holds the vertex names of the path */
            vertexIndexToAdd = curIndex;
            do {
                // add the vertexIndexToAdd to the path list
                path.push_front(vertexNameList[vertexIndexToAdd]);

                // update vertexIndexToAdd
                vertexIndexToAdd = pathRepresentation[vertexIndexToAdd].prevVertexIndex;

            } while (pathRepresentation[vertexIndexToAdd].prevVertexIndex != END_OF_PATH);

            // add the starting vertex to the path
            path.push_front(startingVertexName);

            /* print out the list to the screen */
            printPathList(path);

            // print weight of the path
            std::cout << " || Weight: " << curVertex.totalDistance << "\n";

            // clear the path list
            path.clear();


        }
        else if (startingVertexIndex != curIndex) {
            std::cout << "No path from " << startingVertexName << " to " << vertexNameList[curIndex] << " found.\n";
        }

        // update curIndex
        curIndex++;
    }
}



/*-------------------------------------------------------------------------------------*
 *   function name: buildPathRepresentation(vector<PathVertex>, priority_queue)        *
 *                                                                                     *
 *   description: builds the pathRepresentation vector which is used to represent all  *
 *                the paths we have found from the starting vertex to all other        *
 *                vertices.                                                            *
 *                                                                                     *
 *   returns: n/a                                                                      *
 *-------------------------------------------------------------------------------------*/
void Graph::buildPathRepresentation(std::vector<PathVertex>& pathRepresentation,
    std::priority_queue< Edge, std::vector<Edge>, std::greater<Edge> >& nextShortestPathQueue) {

    /*-------------------------------------------------------------------------------------*
     *   variables used                                                                    *
     *-------------------------------------------------------------------------------------*/
    // the number of paths we have found. 1 because we consider the starting vertex to be found
    int numPathsFound = 1; 

    // represents the next shortest path that we want to add
    Edge nextShortestPath; 

    /*-------------------------------------------------------------------------------------*
     *   find the correct paths and add them to the pathRepresentation                     *
     *-------------------------------------------------------------------------------------*/
    /* while the nextShortestPath is not empty and we have not found all of the paths */
    while ( !nextShortestPathQueue.empty() && (numPathsFound != numVertices)) {

        // take the next shortest path
        nextShortestPath = nextShortestPathQueue.top();
        nextShortestPathQueue.pop();

        // if nextShortestPath has not been found yet, update pathRepresentation, increment numPathsFound, and add adjacent edges
        if (!pathRepresentation[nextShortestPath.toVertexIndex].found) {

            // update pathRepresentation
            pathRepresentation[nextShortestPath.toVertexIndex].prevVertexIndex = nextShortestPath.fromVertexIndex;
            pathRepresentation[nextShortestPath.toVertexIndex].found = true;
            pathRepresentation[nextShortestPath.toVertexIndex].totalDistance = nextShortestPath.cost;

            // increment numPathsFound
            numPathsFound++;

            /* add adjacent edges */
            // for each edge adjacent to nextShortestPath
            for (AdjListVertex curEdge : adjacencyList[nextShortestPath.toVertexIndex]) {

                // if the edge/path has not been found yet, enqueue new Edge item onto the queue
                if (!pathRepresentation[curEdge.toIndex].found) {

                    // enqueue new item onto the queue
                    nextShortestPathQueue.push(Edge(nextShortestPath.toVertexIndex, curEdge.toIndex, nextShortestPath.cost + curEdge.cost));
                }
            }
        }
    }
}



/*-------------------------------------------------------------------------------------*
 *   function name: computeMinimumSpanningTree()                                       *
 *                                                                                     *
 *   description: computes the minimum spanning tree, then prints the edges, edge      *
 *                weight, and total weight of the tree.                                *
 *                                                                                     *
 *   precondition: the graph must be connected                                         *
 *                                                                                     *
 *   returns: n/a                                                                      *
 *-------------------------------------------------------------------------------------*/
void Graph::computeMinimumSpanningTree() {

    /*-------------------------------------------------------------------------------------*
     *   variables used                                                                    *
     *-------------------------------------------------------------------------------------*/
    // list of the edges in the graph
    std::list<Edge> edgesList;

    // the minimum spanning tree
    std::list<Edge> minSpanTreeEdges;

    // is the spanning tree complete
    bool spanningTreeComplete = false;

    // current edge we are looking at
    Edge curEdge;

    // the disjoint set that represents which vertices are connected in the spanning tree
    DisjointSet vertexDisjSet(numVertices);

    // the total cost of the minimum spanning tree
    unsigned int minSpanTreeTotalCost;


    /*-------------------------------------------------------------------------------------*
     *   compute the minimum spanning tree                                                 *
     *-------------------------------------------------------------------------------------*/
    // populate edgesList with the edges we need from the adjacency list of the graph
    populateEdgesList(edgesList);

    // sort the edgesList
    edgesList.sort();

    // until we're down to one tree(or out of edges)
    while (!spanningTreeComplete) {

        // get the next edge we want to try to add to the spanning tree
        curEdge = edgesList.front();
        edgesList.pop_front();

        // if the vertices for the curEdge are not in the same tree
        if (vertexDisjSet.find(curEdge.fromVertexIndex) != vertexDisjSet.find(curEdge.toVertexIndex)) {

            // add curEdge to the minimum spanning tree
            minSpanTreeEdges.push_back(curEdge);

            // connect the two trees and check if the spanning tree is complete
            if (vertexDisjSet.doUnion(curEdge.fromVertexIndex, curEdge.toVertexIndex)) {
                spanningTreeComplete = true;
            }

        }
    }

    /*-------------------------------------------------------------------------------------*
     *   compute total cost of the spanning tree                                           *
     *-------------------------------------------------------------------------------------*/
    minSpanTreeTotalCost = calculateSpanningTreeTotalCost(minSpanTreeEdges);

    /*-------------------------------------------------------------------------------------*
     *   print the output for the minimum spanning tree                                    *
     *-------------------------------------------------------------------------------------*/
    printMinimumSpanningTreeOutput(minSpanTreeEdges, minSpanTreeTotalCost);

}



/*-------------------------------------------------------------------------------------*
 *   function name: populateEdgesList()                                                *
 *                                                                                     *
 *   description: sorts the edges of the graph                                         *
 *                                                                                     *
 *   returns: n/a                                                                      *
 *-------------------------------------------------------------------------------------*/
void Graph::populateEdgesList(std::list<Edge>& edgesList) {

    /*-------------------------------------------------------------------------------------*
     *   add the edges to the list                                                         *
     *-------------------------------------------------------------------------------------*/
    // initialize the current index to the first vertex in the adjacency list
    int curIndex = 0;

    /* go through the adjacency list */
    for (std::list<AdjListVertex> curVertexEdges : adjacencyList) {

        /* go through the edges that are adjacent to the current vertex in the adjacency list we are looking at */
        for (AdjListVertex curEdge : curVertexEdges) {

            // add the edge to edgesList
            edgesList.push_back(Edge(curIndex, curEdge.toIndex, curEdge.cost));
            
        }

        // move the current index to the next vertex in the adjacency list
        curIndex++;
    }

}



/*-------------------------------------------------------------------------------------*
 *   function name: calculateSpanningTreeTotalCost(list<Edge>)                         *
 *                                                                                     *
 *   description: calculates the total cost of the minimum spanning tree               *
 *                                                                                     *
 *   returns: the total cost of the minimum spanning tree                              *
 *-------------------------------------------------------------------------------------*/
unsigned int Graph::calculateSpanningTreeTotalCost(std::list<Edge>& minSpanTreeEdges) {

    // initialize the totalCost
    int totalCost = 0;

    // go through the minimum spanning tree and add the cost of each edge to the total
    for (Edge curEdge : minSpanTreeEdges) {
        totalCost += curEdge.cost;
    }

    // return the total cost of the minimum spanning tree
    return totalCost;
}



/*-------------------------------------------------------------------------------------*
 *   function name: printMinimumSpanningTreeOutput(list<Edge>)                         *
 *                                                                                     *
 *   description: prints the correct output for the computeMinimumSpanningTree         *
 *                function                                                             *
 *                                                                                     *
 *   returns: n/a                                                                      *
 *-------------------------------------------------------------------------------------*/
// prints the correct output for the computeMinimumSpanningTree function
void Graph::printMinimumSpanningTreeOutput(std::list<Edge>& minSpanTreeEdges, unsigned int minSpanTreeTotalCost) {

    std::cout << "Minimum Spanning Tree:\n";

    /* go through the spanning tree and print out the edges and their cost */
    for (Edge curEdge : minSpanTreeEdges) {
        std::cout << vertexNameList[curEdge.fromVertexIndex] << " -- " << vertexNameList[curEdge.toVertexIndex] << " || " << "Weight: " << curEdge.cost << "\n";
    }

    // print out the total cost of the minimum spanning tree
    std::cout << "Total Cost: " << minSpanTreeTotalCost << "\n";
}