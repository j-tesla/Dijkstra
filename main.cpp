#include <iostream>
#include <random>
#include <ctime>
#include <vector>
#include <utility>
#include <climits>


namespace MyRandom
{
    // Initialize our generator twister with a random seed based on the clock (once at system startup)
    std::mt19937_64 generator{static_cast<std::mt19937::result_type>(std::time(nullptr)) };
}

double getRandomNumber(double min, double max)
{
    std::uniform_real_distribution <double> die ( min, max ); // we can create a distribution in any function that needs it
    return die(MyRandom::generator); // and then generate a random number from our global generator
}

int getRandomNumber(int min, int max)
{
    std::uniform_int_distribution <int> die ( min, max ); // we can create a distribution in any function that needs it
    return die(MyRandom::generator); // and then generate a random number from our global generator
}

class Graph {

    int size;
    double** edges;
    /*
     * THE MATRIX REPRESENTATION OF GRAPH:
     *      edges[i][j] is distance between nodes i and j if adjacent or 0 if not
     */


public:

    // constructors:
    explicit Graph(int size, float density = 0, double distance_range_min = 1.0, double distance_range_max = 10.0): size(size) {

        edges = new double*[size];                                                          // allocation

        for (int i = 0; i < size; ++i) {
            edges[i] = new double[size];                                                    // allocation
        }

        std::vector <std::pair <int, int> > xy;
        for (int i = 0; i < size; ++i) {
            for (int j = i+1; j < size; ++j) {
                xy.emplace_back(i, j);
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

        for (int l = 0; l < selected_size; ++l) {
            std::pair <int, int> edge_pair = xy[selected[l]];
            double random_length = 0;
            while (random_length == 0) {
                random_length = getRandomNumber(distance_range_min, distance_range_max);
            }
            edges[edge_pair.first][edge_pair.second] = edges[edge_pair.second][edge_pair.first] = random_length;
        }
        for (int i = 0; i < size; ++i) {
            edges[i][i] = 0;
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
                if (edges[i][j] > 0) {
                    count++;
                }
            }
        }
        return count;
    }

    // returns number of vertices in the graph
    [[nodiscard]] int V () const {
        return size;
    }

    // returns 0(false) if not adjacent or the distance between the nodes if adjacent
    double get_edge (int x, int y) {
        return edges[x][y];
    }

    // returns a vector container of nodes(int)
    std::vector <int> neighbours (int x) {
        std::vector <int> neighboursa;
        for (int i = 0; i < size; ++i) {
            if(edges[x][i]) {
                neighboursa.push_back(i);
            }
        }

        return neighboursa;
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

// MinHeap implementation of Priority queue
class PriorityQueue {

    std::vector <std::pair <int, double> > pq;

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
        if (l < pq.size() and pq[l].second < pq[i].second)
            smallest = l;
        if (r < pq.size() and pq[r].second < pq[smallest].second)
            smallest = r;
        if (smallest != i) {
            swap(pq[i], pq[smallest]);
            MinHeapify(smallest);
        }
    }

public:

    // Constructor default
    PriorityQueue () = default;


    // inserts new element in the heap
    void QInsert (std::pair <int, double > pair) {

        pq.push_back(pair);
        int j = pq.size() - 1;

        while ((j != 0) and (pq[parent(j)].second > pq[j].second)) {
            swap(pq[j], pq[parent(j)]);
            j = parent(j);
        }
    }
    // returns size of the heap
    int QSize () {
        return pq.size();
    }

    // returns and deletes the top element
    std::pair <int, double> QPop() {

        std::pair <int, double > root = pq[0];

        if (pq.empty()) {
            pq.pop_back();
            return root;
        }
        pq[0] = pq.back();
        pq.pop_back();

        MinHeapify(0);

        return root;
    }

    std::pair <int, double > getElement(int i) {
        return pq[i];
    }

    void QDecrease (int i, double new_value) {

        pq[i].second = new_value;
        while (i != 0 and pq[parent(i)].second > pq[i].second) {
            swap(pq[i], pq[parent(i)]);
            i = parent(i);
        }
    }

    void QDelete(int i) {
        QDecrease(i, INT_MIN);
        QPop();
    }

    int QSearch(int i) {
        for (int j = 0; j < pq.size(); ++j) {
            if (pq[j].first == i) {
                return j;
            }
        }
        return -1;
    }

    std::pair <int, double> QElement (int i) {
        if (i < pq.size()) return pq[i];
        else return {-1, 0};
    }

    bool QIsEmpty() {
        return pq.empty();
    }
};

//
class ShortestPath : public Graph, public PriorityQueue {

public:

    // Constructor
    ShortestPath(int size, float density, double distanceRangeMin, double distanceRangeMax) : Graph(size, density, distanceRangeMin, distanceRangeMax) {
    }

    double path_size(int start, int end) {
        std::vector<bool> IsInClosedSet(V(), false);
        int ClosedSetSize = 0;

        std::pair<int, double> root;
        root.first = start;
        root.second = 0;
        QInsert(root);

        while (ClosedSetSize < V() and !(QIsEmpty())) {

            std::pair <int, double > top = QPop();

            if (end == top.first) return top.second;

            std::vector<int> nei = neighbours(top.first);
            for (int & i : nei) {
                if (IsInClosedSet[i] == false) {

                    int search_result = QSearch(i);
                    double edge = get_edge(top.first, i);

                    if (search_result == -1) {
                        QInsert({i, edge + top.second});
                    } else if (QElement(search_result).second > edge + top.second) {
                        QDecrease(search_result, edge + top.second);
                    }

                }
            }
            IsInClosedSet[top.first] = true;

            ClosedSetSize++;

        }
        return -1;
    }

    double avg_path(int start) {
        double sum = 0;
        std::vector<bool> IsInClosedSet(V(), false);
        int ClosedSetSize = 0;

        std::pair<int, double> root;
        root.first = start;
        root.second = 0;
        QInsert(root);

        while (ClosedSetSize < V() and !(QIsEmpty())) {

            std::pair <int, double > top = QPop();

            std::vector<int> nei = neighbours(top.first);
            for (int & i : nei) {
                if (IsInClosedSet[i] == false) {

                    int search_result = QSearch(i);
                    double edge = get_edge(top.first, i);

                    if (search_result == -1) {
                        QInsert({i, edge + top.second});
                    } else if (QElement(search_result).second > edge + top.second) {
                        QDecrease(search_result, edge + top.second);
                    }

                }
            }
            IsInClosedSet[top.first] = true;
            sum += top.second;
            ClosedSetSize++;

        }

        return sum/ClosedSetSize;
    }

};





int main() {
    const int SIZE = 50;

    float density[2] = {0.2, 0.4};
    ShortestPath graph0 (SIZE, density[0], 1.0, 10.0); //graph1(SIZE, density[1], 1.0, 10.0);

  return 0;
}


