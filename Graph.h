
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
#include<queue>
#include "DisjointSet.h"
#include<tuple>
#include<climits>

/*-------------------------------------------------------------------------------------*
 *   class: Graph                                                                      *
 *                                                                                     *
 *   public:                                                                           *
 *                                                                                     *
 *      public functions:                                                              *
 *        readGraph                                                                    *
 *        printGraph                                                                   *
 *        computeTopologicalSort                                                       *
 *        computeShortestPaths                                                         *
 *        computeMinimumSpanningTree                                                   *
 *                                                                                     *
 *   private:                                                                          *
 *                                                                                     *
 *      private structs:                                                               *
 *         AdjListVertex                                                               *
 *         PathVertex                                                                  *
 *         Edge                                                                        *
 *                                                                                     *
 *      private data members:                                                          *
 *         adjacencyList                                                               *
 *         vertexNameList                                                              *
 *         numVertices                                                                 *
 *         numEdges                                                                    *
 *                                                                                     *
 *      private member functions:                                                      *
 *         getVertexIndex                                                              *
 *         setupInDegreeVector                                                         *
 *         printTopologicalSortOutput                                                  *
 *         printPathList                                                               *
 *         printShortestPathsOutput                                                    *
 *         buildPathRepresentation                                                     *
 *         populateEdgesList                                                           *
 *         calculateSpanningTreeTotalCost                                              *
 *         printMinimumSpanningTreeOutput                                              *
 *-------------------------------------------------------------------------------------*/
class Graph{
    private:
        /*-------------------------------------------------------------------------------------*
         *   private structs                                                                   *
         *-------------------------------------------------------------------------------------*/
        /* represents an edge in the the adjacency list */
        struct AdjListVertex{
            public:
                /* data members */
                // the index of the vertex that is adjacent to the vertex in the adjacencyList
                int toIndex;

                // cost of the edge between vector vertex and destination
                int cost;

                /* operator overload */
                bool operator<(const AdjListVertex& rhs) const {
                    return std::tie(cost, toIndex) < std::tie(rhs.cost, rhs.toIndex);
                }

                /* constructor */
                AdjListVertex(int toIndexValue = 0, int costValue = 0, int fromIndexValue = 0) 
                    : toIndex(toIndexValue), cost(costValue) {}
        };


        /* represents a vertex in the overall path */
        struct PathVertex { 
            public:
                /* data members */
                // is the vertex already found
                bool found;

                // the total distance of the vertex
                int totalDistance; 

                // the index of the previous vertex in the path
                int prevVertexIndex; 

                /* constructor */
                PathVertex(bool foundValue = false, int totalDistanceValue = INT_MAX, int prevVertexValue = INT_MAX)
                    : found(foundValue), totalDistance(totalDistanceValue), prevVertexIndex(prevVertexValue) {}
        };


        /* represents an edge in the nextShortestPath queue and in the minimum spanning tree */
        struct Edge {
            public:
                /* data members */
                // the index of the from vertex
                int fromVertexIndex; 

                // the index of the vertex that fromVertex is going to
                int toVertexIndex; 

                // the total path cost to go to toVertex
                int cost; 

                /* operator overloads to allow us to compare the totalPathCost of two Edge items */
                bool operator<(const Edge& rhs) const {
                    return std::tie(cost, fromVertexIndex, toVertexIndex) < std::tie(rhs.cost, rhs.fromVertexIndex, rhs.toVertexIndex);
                }
                bool operator>(const Edge& rhs) const {
                    return std::tie(cost, fromVertexIndex, toVertexIndex) > std::tie(rhs.cost, rhs.fromVertexIndex, rhs.toVertexIndex);
                }
                bool operator==(const Edge& rhs) const {
                    return std::tie(cost, fromVertexIndex, toVertexIndex) == std::tie(rhs.cost, rhs.fromVertexIndex, rhs.toVertexIndex);
                }

                /* constructor */
                Edge(int fromVertexValue = -1, int toVertexValue = -1, int costValue = 0)
                    : fromVertexIndex(fromVertexValue), toVertexIndex(toVertexValue), cost(costValue) {}
        };

        /*-------------------------------------------------------------------------------------*
         *   private data members                                                              *
         *-------------------------------------------------------------------------------------*/
        // representation of the graph using an adjacency list
        std::vector< std::list<AdjListVertex> > adjacencyList;

        // stores the names of the vertices in the graph
        std::vector<std::string> vertexNameList;

        // number of vertices in the graph
        int numVertices;

        // number of edges in the graph
        int numEdges;

        /*-------------------------------------------------------------------------------------*
         *   private member functions                                                          *
         *-------------------------------------------------------------------------------------*/
        // returns the index of a specified vertex
        int getVertexIndex(std::string nameOfVertexToFind);

        // creates the in-degree vector used in computeTopologicalSort function
        std::vector<int> setupInDegreeVector();

        // prints the correct output for the computeTopologicalSort function
        void printTopologicalSortOutput(std::list<std::string>& topologicalSortOrdering, int numVerticesInSortList);

        // prints out the list passed
        void printPathList(std::list<std::string>& pathList);

        // prints the correct output for the computeShortestPaths function
        void printShortestPathsOutput(std::string startingVertexName, std::vector<PathVertex>& pathRepresentation);

        // builds the pathRepresentation vector
        void buildPathRepresentation(std::vector<PathVertex>& pathRepresentation,
            std::priority_queue< Edge, std::vector<Edge>, std::greater<Edge> >& nextShortestPathQueue);

        // adds the edges of the graph to edgesList. helper function for computeMinimumSpanningTree
        void populateEdgesList(std::list<Edge>& edgesList);

        // calculates the total cost of the minimum spanning tree
        unsigned int calculateSpanningTreeTotalCost(std::list<Edge>& minSpanTreeEdges);

        // prints the correct output for the computeMinimumSpanningTree function
        void printMinimumSpanningTreeOutput(std::list<Edge>& minSpanTreeEdges, unsigned int minSpanTreeTotalCost);

    public:
        // constructor
        Graph(int numVerticesValue = 0, int numEdgesValue = 0) : 
            numVertices(numVerticesValue), numEdges(numEdgesValue) 
        {
            adjacencyList.resize(numVerticesValue);
            vertexNameList.resize(numVerticesValue);
        }

        /*-------------------------------------------------------------------------------------*
         *   public member functions                                                           *
         *-------------------------------------------------------------------------------------*/
        // reads in a new graph replacing old graph
        bool readGraph(std::string fileName);

        // prints the graph to cout using the same file format as fileName in readGraph
        void printGraph(); 

        // prints a topological sort of the graph or print error if sort not possible
        void computeTopologicalSort(); 

        // prints out the path and cost to each vertex from vertexName
        void computeShortestPaths(std::string startingVertexName);

        // computes the minimum spanning tree, then prints the edges and total weight of the tree
        // precondition: graph must be connected
        void computeMinimumSpanningTree();

};


#endif