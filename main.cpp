#include <iostream>
#include <random>

using namespace std;
/*                                                          // fixit prob                                                        // fixit prob
default_random_engine generator;
uniform_real_distribution<float> distribution(0,1);
float prob
*/
class Graph {

public:

    // constructors:
    Graph(int size): size(size) {
        edges = new int*[size];

        for (int i = 0; i < size; ++i) {
            edges[i] = new int[size];
        }
        for (int i = 0; i < size; ++i) {
            edges[i][i] = 0;
        }
    }

    Graph(int size, float density, int distance_range): size(size) {
        edges = new int*[size];

        srand(time(0));         // seed time
        for (int i = 0; i < size; ++i) {
            edges[i] = new int[size];
        }
        for (int i = 0; i < size; ++i) {
            for (int j = i; j < size; ++j) {
                if (i == j) edges[i][j] = 0;
                else edges[i][j] = edges[j][i] = (static_cast<int>(/*prob()*/0 < density)) * (rand()%distance_range + 1); // fixit prob
            }
        }
    }

    // returns the number of edges in the graph
    int E () {
        int count = 0;
        for (int i = 0; i < size; ++i) {
            for (int j = i + 1; j < size; ++j) {
                if (this->edges[i][j] > 0) {
                    count++;
                }
            }
        }
        return count;
    }

    // returns number of vertices in the graph
    int V () const {
        return size;
    }

    // returns 0(false) if not adjacent or the distance between the nodes if adjacent
    int get_edge (int x, int y) {
        return edges[x][y];
    }

    // returns a vector container of nodes(int)
    vector <int> neighbours (int x) {
        vector <int> neighbours;
        for (int i = 0; i < size; ++i) {
            if(edges[x][i]) {
                neighbours.push_back(i);
            }
        }

        return neighbours;
    }

    // add or change edge between two nodes. no distance parameter to remove the edge
    void set_edge (int x, int y, int distance = 0) {
        edges[x][y] = edges[y][x] = distance;

        if (distance == 0) cout << "Edge removed if existed.\n";
    }



    ~Graph () {
        for (int i = 0; i < size; ++i) {
            delete [] edges[i];
        }
        delete [] edges;
    }

private:
    int size;
    int** edges;
    /*
     * THE MATRIX REPRESENTATION OF GRAPH:
     *      edges[i][j] is distance between nodes i and j if adjacent or 0 if not
     */

};

//
class PriorityQueue : public Graph {                                // todo build it

};

//
class ShortestPath : public Graph {                                 // todo built it too

};





int main() {
    std::cout << "Hello, World!" << std::endl;


    return 0;
}


// learn inheritance in c++
