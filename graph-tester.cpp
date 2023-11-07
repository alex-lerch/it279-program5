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

    Graph graph;
    graph.readGraph("graph2.txt");
    graph.printGraph();
}
