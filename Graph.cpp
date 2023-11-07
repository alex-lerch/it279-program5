
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
#include<iostream> // asl debug

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
    int fromVertexIndex; // the index of the 'from' vertex being added to the graph
    int toVertexIndex; // the index of the 'to' vertex being added to the graph
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
int Graph::getVertexIndex(std::string nameOfVertexToFind) {
    for (int i = 0; i < vertexNameList.size(); i++) {
        if (vertexNameList[i] == nameOfVertexToFind) {
            return i;
        }
    }

    // return if vertex could not be found
    return -1;
}