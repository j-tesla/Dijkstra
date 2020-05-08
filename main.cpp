#include <iostream>
#include <random>

using namespace std;
/*                                                                                                      // fixit prob
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
                else edges[i][j] = edges[j][i] = (static_cast<int>(prob() < 0.19)) * (rand()%distance_range + 1) // fixit prob
            }
        }
    }


    int E () {                              // returns the number of edges in the graph
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

    int V () {                              // returns number of vertices in the graph
        return this->size;
    }

private:
    int size;
    int** edges;
    /*
     * THE MATRIX REPRESENTATION OF GRAPH:
     *      edges[i][j] is distance between nodes i and j if directly connected or 0 if not connected directly
     */

};

int main() {
    std::cout << "Hello, World!" << std::endl;


    return 0;
}


// learn inheritance in c++
