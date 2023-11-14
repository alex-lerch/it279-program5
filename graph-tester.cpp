/* tester file for the graph class */

#include<iostream>
#include "Graph.h"
#include<fstream>
#include<vector>
#include<list>
#include<string>

/* graph representation of sample graph

    springfield -2-> stlouis -6-> denver
        ^                ^           /
        |                |          /
         \               4         /
          7              |        /
           \----------elpaso <-5-

    4
    springfield
    stlouis
    denver
    elpaso
    5
    springfield stlouis 2
    stlouis denver 6
    denver elpaso 5
    elpaso springfield 7
    elpaso stlouis 4

*/

/* graph1.txt:

    5
    MCO
    LGA
    PVD
    LAX
    SFO
    12
    MCO LGA 1099
    LGA MCO 1099
    MCO PVD 1355
    PVD MCO 1355
    MCO LAX 2360
    LAX MCO 2360
    LGA SFO 2224
    SFO LGA 2224
    PVD LAX 2400
    LAX PVD 2400
    LAX SFO 400
    SFO LAX 400

*/

int main() {

    Graph graph;
    graph.readGraph("aslgraph.txt");
    graph.computeShortestPaths("springfield");
}


// asl debug print the queue 
/*
std::cout << "the queue when pushed all startingVertex's adjacent vertices:" << std::endl;;
while(!nextShortestPath.empty()) {
    QueueVertex curVertex = nextShortestPath.top();
    nextShortestPath.pop();
    std::cout << "from: " << curVertex.fromVertexIndex << "\nto: " << curVertex.toVertexIndex << "\ntotal path cost: " << curVertex.totalPathCost << std::endl;
} */


/*
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
*/