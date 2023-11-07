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
    // create sample graph for tests
    Graph graph(4, 5);
    graph.vertexNameList[0] = "springfield";
    graph.vertexNameList[1] = "stlouis";
    graph.vertexNameList[2] = "denver";
    graph.vertexNameList[3] = "elpaso";

    /* tests getVertexIndex function */
    std::cout << "test graph.getVertexIndex(\"stlouis\"):\nexpected: 1\nresult: " << graph.getVertexIndex("stlouis") << "\n" << std::endl;
    std::cout << "test graph.getVertexIndex(\"springfield\"):\nexpected: 0\nresult: " << graph.getVertexIndex("springfield") << "\n" << std::endl;
    std::cout << "test graph.getVertexIndex(\"elpaso\"):\nexpected: 3\nresult: " << graph.getVertexIndex("elpaso") << "\n" << std::endl;
    std::cout << "test graph.getVertexIndex(\"denver\"):\nexpected: 2\nresult: " << graph.getVertexIndex("denver") << "\n" << std::endl;
    
    /* tests readGraph function */
    graph.readGraph("graph1.txt");
    std::cout << "\ntest readGraph(\"graph1.txt\"): \nactual: \n";
    for (int i = 0; i < graph.numVertices; i++) {
        std::cout << graph.vertexNameList[i] << "'s edges: \n";
        for (Graph::Edge edge : graph.adjacencyList[i]) {
            std::cout << graph.vertexNameList[edge.toIndex] << " " << edge.cost << "\n";
        }
        std::cout << "\n";
    }
    std::cout << std::endl;

    /* tests for invalid file passed into readGraph */
    bool isValidfile = graph.readGraph("pie.txt");
    std::cout << "\ntest for graph.readGraph with invalid file: \nexpected: 0\nactual: " << isValidfile << "\n" << std::endl;
    
}
