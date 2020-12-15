// hamiltonian.cpp : Defines the entry point for the application.
//

#include "hamiltonian.h"

struct step {
    uint node; // node index in graph
    uint edge; // edge index in graph node
};
typedef std::vector<step> steps;

void print_path(vector path) {
    cout << "Path: [";
    for (uint i = 0; i < path.size(); i++) {
        if (i > 0) {
            cout << ", ";
        }
        cout << path[i];
    }
    cout << "]" << endl;
}

void print_sums(vector path) {
    cout << "Sums: [";
    for (uint i = 0; i < path.size() - 1; i++) {
        if (i != 0) cout << ",";
        cout << (path[i] + path[i + 1]);
    }
    cout << "]" << endl;
}

void find_paths(uint n, vector2 graph) {
    steps path = steps(n);
    int idx;
    uint size = n + 1;
    vector visited = vector(size);
    for (uint i = 1; i <= n; i++) {
        idx = 0;
        visited[i] = true;
        path[idx].node = i;
        path[idx].edge = 0;
        while (idx >= 0) {
            if (path[idx].edge < graph[path[idx].node].size()) {
                uint v = graph[path[idx].node][path[idx].edge];
                path[idx].edge++;
                if (visited[v]) continue;
                idx++;
                path[idx].node = v;
                path[idx].edge = 0;
                visited[v] = true;
                continue;
            }
            if (idx == n - 1) {
                vector found = vector(n);
                for (uint i = 0; i < n; i++) {
                    found[i] = path[i].node;
                }
                print_path(found);
                //cout << backtrack << endl;
                //return;
            }
            visited[path[idx].node] = false;
            idx--;
        }
    }
}

void find_path(uint n, vector2 graph, uint start, unsigned long long limit = 0xffffffffffffffffull) {
    cout << "Searching..." << endl << flush;
    steps path = steps(n);
    int idx = 0;
    uint size = n + 1;
    vector visited = vector(size);
    unsigned long long backtrack = 0;
    visited[start] = true;
    path[idx].node = start;
    path[idx].edge = 0;
    while (idx >= 0) {
        if (path[idx].edge < graph[path[idx].node].size()) {
            uint v = graph[path[idx].node][path[idx].edge];
            path[idx].edge++;
            if (visited[v]) continue;
            idx++;
            path[idx].node = v;
            path[idx].edge = 0;
            visited[v] = true;
            continue;
        }
        if (idx == n - 1) {
            vector found = vector(n);
            for (uint i = 0; i < n; i++) {
                found[i] = path[i].node;
            }
            print_path(found);
            cout << "Backtrack total: " << backtrack << endl;
            return;
        }
        backtrack++;
        if (backtrack && backtrack == limit) {
            cout << "Backtrack limit reached: " << backtrack << endl;
            return;
        }
        if ((backtrack & 0x7fff) == 0) cout << "Backtrack: " << backtrack << endl << flush;
        visited[path[idx].node] = false;
        idx--;
    }
    cout << "No path found" << endl;
}


vector gen_squares(uint n) {
    vector squares;
    for (uint i = 2, j = i * i; j < n * 2; i++, j = i * i) {
        squares.push_back(j);
    }
    reverse(squares.begin(), squares.end());
    return squares;
}

void print_squares(vector squares) {
    cout << "Squares: [";
    for (uint i = 0; i < squares.size(); i++) {
        if (i) {
            cout << ", ";
        }
        cout << squares[i];
    }
    cout << "]" << endl << flush;
}

vector2 gen_graph(uint n, vector squares) {
    vector2 graph;
    uint size = n + 1;
    graph.resize(size);
    int y = 0;
    for (vector& node : graph) {
        if (y > 0) {
            for (uint square : squares) {
                int x = square - y;
                if ((uint)x > n || x == y) continue;
                if (x < 1) break;
                node.push_back(x);
            }
        }
        y++;
    }
    return graph;
}

void print_graph(vector2 graph) {
    uint i = 0;
    for (uint i = 1; i < graph.size(); i++) {
        cout << i << " [";
        for (uint j = 0; j < graph[i].size(); j++) {
            if (j > 0) {
                cout << ", ";
            }
            cout << graph[i][j];
        }
        cout << "]" << endl;
    }
}

vector gen_grid(uint n, vector squares) {
    vector graph;
    set sqr(squares.begin(), squares.end());
    uint size = (n + 1) * (n + 1);
    graph.resize(size);
    for (uint y = 0, offset = 0; y <= n; y++, offset += n + 1) {
        if (!y) continue;
        for (uint x = 0, idx = offset; x <= n; x++, idx++) {
            if (!x || x == y) continue;
            graph[idx] = sqr.find(x + y) != sqr.end();
        }
    }
    return graph;
}

void print_grid(uint n, vector grid) {
    n += 1;
    for (uint y = 0, offset = 0; y < n; y++, offset += n) {
        for (uint x = 0, idx = offset; x < n; x++, idx++) {
            if (x == 0 || y == 0) continue;
            cout << (grid[idx] ? "#" : ".") << " ";
        }
        cout << endl;
    }
}

int main(int argc, char* argv[]) {
    uint n, start;
    if (argc >= 2) {
        n = stoi(argv[1]);
        if (n < 2) return -1;
        vector squares = gen_squares(n);
        print_squares(squares);
        vector2 graph = gen_graph(n, squares);
        if (argc >= 3) {
            start = stoi(argv[2]);
            if (start < 1 || start > n) return -1;
            unsigned long long limit = 0;
            if (argc == 4) {
                limit = stoi(argv[3]);
            }
            find_path(n, graph, start, limit);
        }
        else {
            find_paths(n, graph);
        }
        cout << "Done." << endl;
        return 0;
    }
    cout << argv[0] << " <n> [start [backtrack limit]]" << endl;
    return 0;
}
