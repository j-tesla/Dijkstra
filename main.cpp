#include <iostream>
#include <random>
#include <ctime>


namespace MyRandom
{
    // Initialize our generator twister with a random seed based on the clock (once at system startup)
    std::mt19937_64 generator{static_cast<std::mt19937::result_type>(std::time(nullptr)) };
}

double getRandomNumber(int min, int max)
{
    std::uniform_real_distribution <double> die ( min, max ); // we can create a distribution in any function that needs it
    return die(MyRandom::generator); // and then generate a random number from our global generator
}


class Pair {
public:
    int node;
    int value;

    Pair(const int node, const int value) : node(node), value(value) {}

    //Copy constructor default
    Pair(const Pair &pair) = default;

    void SetPair(const Pair &pair) {
        node = pair.node;
        value = pair.value;
    }



};

void SwapPairs(Pair &a, Pair &b) {
    Pair temp(a);
    a.SetPair(b);
    b.SetPair(temp);

}



class Graph {

    int size;
    int** edges;
    /*
     * THE MATRIX REPRESENTATION OF GRAPH:
     *      edges[i][j] is distance between nodes i and j if adjacent or 0 if not
     */


public:

    // constructors:
    explicit Graph(int size, float density = 0, int distance_range_min = 1.0, int distance_range_max = 10.0): size(size) {
        edges = new int*[size];

        for (int i = 0; i < size; ++i) {
            edges[i] = new int[size];
        }
        for (int i = 0; i < size; ++i) {
            for (int j = i; j < size; ++j) {
                if (i == j) edges[i][j] = 0;
                else edges[i][j] = edges[j][i] = (static_cast<int >(getRandomNumber(0.0, 1.0) < density)) * (getRandomNumber(distance_range_min, distance_range_max));
            }
        }
    }

    // printing graph << operator overloading
    friend std::ostream &operator<<(std::ostream &os, const Graph &graph) {
        for (int i = 0; i < graph.size; ++i) {
            for (int j = 0; j < graph.size; ++j) {


                os << graph.edges[i][j] << " | ";
            }
            os << std::endl;
        }
        return os;
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
    std::vector <int> neighbours (int x) {
        std::vector <int> neighbours;
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

        if (distance == 0) std::cout << "Edge removed if existed.\n";
    }



    ~Graph () {
        for (int i = 0; i < size; ++i) {
            delete [] edges[i];
        }
        delete [] edges;
    }


};

//
class PriorityQueue {                                                       // todo build it

    std::vector <Pair> pq;

public:

    // Constructor default
    PriorityQueue () = default;

    void insert (Pair pair) {

        pq.push_back(pair);
        int j = pq.size() - 1;
        bool flag = true;
        while (flag and ((j-1)/2 >= 0)) {
            if (pq[j].value < pq[(j-1)/2].value) {
                SwapPairs(pq[j], pq[(j-1)/2]);
                j = (j-1)/2;
            } else {
                flag = false;
            }
        }
    }

    int size () {
        return pq.size();
    }

    Pair pop() {
        Pair top(pq[0]);
        pq[0].SetPair(pq.back());
        pq.pop_back();

        int j = 0;
        bool flag = true;

        while ( flag and (2*j + 2 <= pq.size() - 1) ) {
            if( pq[2*j + 1].value > pq[2*j + 2].value) {                                                        // todo work in progress
                if (pq[j].value < pq[2*j +2].value) {
                    SwapPairs(pq[j], pq[2*j + 2]);
                    j = 2*j + 2;
                } else flag = false;
            } else {
                if (pq[j].value < pq[2*j +1].value) {
                    SwapPairs(pq[j], pq[2*j + 1]);
                    j = 2*j + 1;
                } else flag = false;
            }
        }
    }
};

//
class ShortestPath {                                                                                            // todo built it too

public:

    // Constructor default
    ShortestPath() = default;
};





int main() {
    std::cout << "Hello, World!" << std::endl;

    const int SIZE = 50;

    float density[2] = {0.2, 0.4};

    Graph graph0 (SIZE, density[0], 10);

    //std::cout << graph0;

    std::vector<bool> closed(50, false);
    int closedsetSize = 0;


    return 0;
}


// learn inheritance in c++
