#include <iostream>
#include <random>
#include <ctime>
#include <vector>
#include <utility>

namespace MyRandom
{
    // Initialize our generator twister with a random seed based on the clock (once at system startup)
    std::mt19937_64 generator{static_cast<std::mt19937::result_type>(std::time(nullptr)) };
}

// random real number generation
double getRandomNumber(double min, double max)
{
    std::uniform_real_distribution <double> die ( min, max ); // we can create a distribution in any function that needs it
    return die(MyRandom::generator); // and then generate a random number from our global generator
}

// random integer generation
int getRandomNumber(int min, int max)
{
    std::uniform_int_distribution <int> die ( min, max );           // we can create a distribution in any function that needs it
    return die(MyRandom::generator);                             // and then generate a random number from our global generator
}

// parent class Graph
class Graph {

    int size;
    std::vector<double> edges;

public:

    // constructors:
    explicit Graph(int size, float density = 0, double distance_range_min = 1.0, double distance_range_max = 10.0): size(size) {

        edges.resize(size*size);

        // below code for randomly selecting density fraction of edges to fill with some value
        std::vector <std::pair <int, int> > edge_pairs;
        for (int i = 0; i < size; ++i) {
            for (int j = i+1; j < size; ++j) {
                edge_pairs.emplace_back(i, j);
            }
        }
        int selected_size = static_cast<int >(density*size*(size -1)/2);
        int selected[selected_size];

        std::vector <bool> is_not_selected((size * (size - 1)) / 2, true);


        int ii = 0;
        while (ii < selected_size) {
            int rand_int = getRandomNumber(0, (size*(size - 1))/2 - 1);
            if (is_not_selected[rand_int]) {
                selected[ii] = rand_int;
                is_not_selected[rand_int] = false;
                ii++;
            }
        }

        // filling selected edges with some value
        for (int l = 0; l < selected_size; ++l) {
            std::pair <int, int> edge_pair = edge_pairs[selected[l]];
            double random_length = 0;
            while (random_length == 0) {
                random_length = getRandomNumber(distance_range_min, distance_range_max);
            }
            set_edge(edge_pair.first, edge_pair.second, random_length);
        }

        // no loops
        for (int i = 0; i < size; ++i) {
            set_edge(i, i, 0);
        }
    }

    // returns the number of edges in the graph
    int E () {
        int count = 0;
        for (int i = 0; i < size; ++i) {
            for (int j = i + 1; j < size; ++j) {
                if (get_edge(i, j) > 0) {
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
    double get_edge (const int x, const int y) {
        return edges[x*size +y];
    }

    // sets cost of edge (0 if the edge is absent)
    void set_edge (const int x, const int y, const double cost) {
        edges[x*size + y] = edges[y*size + x] = cost;
    }

    // returns a vector container of nodes(int)
    std::vector <int> neighbours (const int x) {
        std::vector <int> neighboursa;
        for (int i = 0; i < size; ++i) {
            if(static_cast<bool>(get_edge(x, i))) {
                neighboursa.push_back(i);
            }
        }

        return neighboursa;
    }

    /*
    // add or change edge between two nodes. no distance parameter to remove the edge
    void set_edge (int x, int y, int distance = 0) {
        edges[x][y] = edges[y][x] = distance;

        if (distance == 0) std::cout << "Edge removed if existed.\n";
    }
    */

    // destructor
    ~Graph () = default;

};

// printing graph (<< operator overloading)
std::ostream &operator<<(std::ostream& os, Graph& graph) {
    os << graph.V() << std::endl;
    for (int i = 0; i < graph.V(); ++i) {
        for (int j = 0; j < graph.V(); ++j) {
            if (i != j) os << i << ' ' << j << ' '  << graph.get_edge(i, j);
        }
    }
    return os;
}

// MinHeap implementation of Priority queue
class PriorityQueue {

    int graph_size;
    std::vector<int> pq;
    std::vector<double> costs;

    // parent node in the binary tree
    static int parent(const int j) {
        return (j-1)/2;
    }
    // left child node in the binary tree
    static int left(const int j) {
        return 2*j + 1;
    }
    // right child in the heap
    static int right(const int j) {
        return 2*j + 2;
    }

    // assuming the subtrees are MinHeaps incorporates the given node as the root of the tree there after
    void MinHeapify(int i) {

        int l = left(i);
        int r = right(i);
        int smallest = i;
        if (l < pq.size() and costs[pq[l]] < costs[pq[i]])
            smallest = l;
        if (r < pq.size() and costs[pq[r]] < costs[pq[smallest]])
            smallest = r;
        if (smallest != i) {
            std::swap(pq[i], pq[smallest]);
            MinHeapify(smallest);
        }
    }

public:

    // Constructor default
    explicit PriorityQueue (int size) : graph_size(size) {
        costs.resize(graph_size, INFINITY);
    }

    // inserts new element in the heap
    void QInsert (int node, double  cost) {

        pq.push_back(node);
        costs[node] = cost;
        int j = pq.size() - 1;

        while ((j != 0) and (costs[pq[parent(j)]] > costs[pq[j]])) {
            std::swap(pq[j], pq[parent(j)]);
            j = parent(j);
        }
    }


    // returns size of the heap
    int QSize () {
        return pq.size();
    }


    // returns and deletes the top element
    std::pair <int, double> QPop() {

        std::pair <int, double > root = {pq[0], costs[pq[0]]};

        pq[0] = pq.back();
        pq.pop_back();

        MinHeapify(0);

        return root;
    }

    // decreases the value of a node
    void QDecrease (int node, double new_value) {

        costs[pq[node]] = new_value;
        while (node != 0 and costs[pq[parent(node)]] > costs[pq[node]]) {
            std::swap(pq[node], pq[parent(node)]);
            node = parent(node);
        }
    }

    // search for a node in the queue
    int QSearch(int node) {
        for (int j = 0; j < pq.size(); ++j) {
            if (pq[j] == node) {
                return j;
            }
        }
        return -1;
    }

    // returns node at i-th position in the queue
    std::pair <int, double> QElement (int i) {
        if (i < pq.size()) return {pq[i], costs[pq[i]]};
        else return {-1, 0};
    }

    // to check whether the the queue is empty
    bool QIsEmpty() {
        return pq.empty();
    }
};

//
class ShortestPath : public PriorityQueue {

    Graph graph;

public:

    // Constructor
    ShortestPath(Graph& graph) : graph(graph), PriorityQueue(graph.V()) {
    }

    double PathSize(int start, int end) {

        std::vector<bool> IsInClosedSet(graph.V(), false);       // boolean values associated to with each node being in the closed set
        int ClosedSetSize = 0;

        QInsert(start, 0);                                              // start the queue with first starting node

        // until all the nodes go into the closed set
        while (ClosedSetSize < graph.V() and !(QIsEmpty())) {

            std::pair <int, double > top = QPop();

            if (end == top.first) return top.second;

            std::vector<int> nei = graph.neighbours(top.first);           // neighbors of the top node

            // adding neighbours into the open set, i.e the priority queue
            for (int & i : nei) {
                if (IsInClosedSet[i] == false) {

                    int search_result = QSearch(i);
                    double edge = graph.get_edge(top.first, i);

                    if (search_result == -1) {
                        QInsert(i, edge + top.second);
                    } else if (QElement(search_result).second > edge + top.second) {
                        QDecrease(search_result, edge + top.second);
                    }
                }
            }
            IsInClosedSet[top.first] = true;                        // top element into the closed set
            ClosedSetSize++;
        }
        return -1;
    }

    // returns average distance of a given node from all other nodes
    double AverageDistance(int start) {
        double sum = 0;                                             // sum of distances to find the average
        std::vector<bool> IsInClosedSet(graph.V(), false);       // boolean values associated to with each node being in the closed set
        int ClosedSetSize = 0;

        QInsert(start, 0);                                              // start the queue with first starting node

        // until all the nodes go into the closed set
        while (ClosedSetSize < graph.V() and !(QIsEmpty())) {

            std::pair <int, double > top = QPop();

            std::vector<int> nei = graph.neighbours(top.first);           // neighbours of the top node in the queue

            // adding neighbours into the open set, i.e the priority queue
            for (int & i : nei) {
                if (IsInClosedSet[i] == false) {

                    int search_result = QSearch(i);
                    double edge = graph.get_edge(top.first, i);

                    if (search_result == -1) {
                        QInsert(i, edge + top.second);
                    } else if (QElement(search_result).second > edge + top.second) {
                        QDecrease(search_result, edge + top.second);
                    }
                }
            }

            IsInClosedSet[top.first] = true;                        // top element into the closed set
            ClosedSetSize++;
            sum += top.second;                                      // contributing to average
        }
        return sum/ClosedSetSize;
    }
};





int main() {
    const int SIZE = 50;

    float density[2] = {0.2, 0.4};                                  // densities of the graphs

    // average path calculations:
    std::cout << "Average path calculations :\n";
    std::cout << "\nCase 1:\tDensity = 20%\nAverage path lengths for a few randomly generated graphs.:\n";
    for (int i = 0; i < 10; ++i) {
        Graph graph (SIZE, density[0], 1.0, 10.0);
        ShortestPath obj(graph);
        std::cout << obj.AverageDistance(0) << "\t\t";
    }

    std::cout << "\n\nCase 2:\tDensity = 40%\nAverage path lengths for a few randomly generated graphs.:\n";
    for (int i = 0; i < 10; ++i) {
        Graph graph (SIZE, density[1], 1.0, 10.0);
        ShortestPath obj(graph);
        std::cout << obj.AverageDistance(0) << "\t\t";
    }

    std::cout << "\n\nNote that the avg path lengths are calculated here by only considering paths from the first node for simplicity.\n";


  return 0;
}


