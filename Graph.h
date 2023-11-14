
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
 *                                                                                     *
 *   private:                                                                          *
 *                                                                                     *
 *      private structs:                                                               *
 *         Edge                                                                        *
 *         PathVertex                                                                  *
 *         QueueVertex                                                                 *
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
 *-------------------------------------------------------------------------------------*/
class Graph{
    private:
        /*-------------------------------------------------------------------------------------*
         *   private structs                                                                   *
         *-------------------------------------------------------------------------------------*/
        /* represents an edge in the the graph */
        struct Edge{
            public:
                /* data members */
                // the index of the vertex that is adjacent to the vertex in the adjacencyList
                int toIndex;

                // cost of the edge between vector vertex and destination
                int cost;

                /* constructor */
                Edge(int toIndexValue = 0, int costValue = 0) : toIndex(toIndexValue), cost(costValue) {}
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


        /* represents a vertex in the nextShortestPath queue */
        struct QueueVertex {
            public:
                /* data members */
                // the index of the from vertex
                int fromVertexIndex; 

                // the index of the vertex that fromVertex is going to
                int toVertexIndex; 

                // the total path cost to go to toVertex
                int totalPathCost; 

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

                /* constructor */
                QueueVertex(int fromVertexValue = -1, int toVertexValue = -1, int totalPathCostValue = 0)
                    : fromVertexIndex(fromVertexValue), toVertexIndex(toVertexValue), totalPathCost(totalPathCostValue) {}
        };

        /*-------------------------------------------------------------------------------------*
         *   private data members                                                              *
         *-------------------------------------------------------------------------------------*/
        // representation of the graph using an adjacency list
        std::vector< std::list<Edge> > adjacencyList;

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
            std::priority_queue< QueueVertex, std::vector<QueueVertex>, std::greater<QueueVertex> >& nextShortestPathQueue);

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


};


#endif