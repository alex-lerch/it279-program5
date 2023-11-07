
/*-------------------------------------------------------------------------------------*
 *                                                                                     *
 *   File: Graph.h                                                                     *
 *                                                                                     *
 *   Desc: header file for a representation of a weighted directed graph               *
 *                                                                                     *
 *   Author: Alex Lerch                                                                *
 *                                                                                     *
 *-------------------------------------------------------------------------------------*/
#ifndef GRAPH_H_
#define GRAPH_H_

/*-------------------------------------------------------------------------------------*
 *   header files                                                                      *
 *-------------------------------------------------------------------------------------*/
#include<vector>
#include<list>
#include<string>


/*-------------------------------------------------------------------------------------*
 *   class declaration                                                                 *
 *-------------------------------------------------------------------------------------*/
class Graph{
    public://private:
        /* private structs */
        // represents an edge in the the graph
        struct Edge{
            public:
                // data members
                int toIndex; // the index of the vertex that is adjacent to the vertex in the adjacencyList
                int cost; // cost of the edge between vector vertex and destination

                // constructor
                Edge(int toIndexValue = 0, int costValue = 0) : toIndex(toIndexValue), cost(costValue) {}
        };

        /* private data members */
        std::vector< std::list<Edge> > adjacencyList; // representation of the graph using an adjacency list
        std::vector<std::string> vertexNameList; // stores the names of the vertices in the graph
        int numVertices; // number of vertices in the graph
        int numEdges; // number of edges in the graph

        /* private methods */
        int getVertexIndex(std::string nameOfVertexToFind); // returns the index of a specified vertex

    //public:
        // constructor
        Graph(int numVerticesValue = 0, int numEdgesValue = 0) : 
            numVertices(numVerticesValue), numEdges(numEdgesValue) 
        {
            adjacencyList.resize(numVerticesValue);
            vertexNameList.resize(numVerticesValue);
        }

        /* public member functions */
        bool readGraph(std::string fileName); // reads in a new graph replacing old graph


};


#endif