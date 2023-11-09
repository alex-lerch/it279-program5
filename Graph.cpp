
/*-------------------------------------------------------------------------------------*
 *                                                                                     *
 *   File: Graph.cpp                                                                   *
 *                                                                                     *
 *   Desc: implemetation file for a representation of a weighted directed              *
 *         graph                                                                       *
 *                                                                                     *
 *   Author: Alex Lerch                                                                *
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
 *   public member function implementations                                            *
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