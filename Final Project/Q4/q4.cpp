/* * * * * * * * * * * * * * * * * * * * * * * * * *
 * BLG 336E Analysis of Algorithms II
 * Spring 2021
 * Student Name: Abdulkadir Pazar
 * Student ID: 150180028
 * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>

using namespace std;

class Solution {
public:
    bool isBipartite(vector<vector<int> >& graph)
    {
        int node_count = graph.size();
        vector<int> color(node_count, -1);
        queue<pair<int, int>> queue;
        for (int i = 0; i < node_count; i++)
        {
            if (color[i] == -1)
            {
                queue.push({i, 0});
                color[i] = 0;
                while (!queue.empty())
                {
                    pair<int, int> p = queue.front();
                    queue.pop();
                    int current_node = p.first;
                    int current_color = p.second;
                    for (int j : graph[current_node])
                    {
                        if (color[j] == current_color)
                            return false;
                        if (color[j] == -1)
                        {
                            color[j] = (current_color) ? 0 : 1;
                            queue.push({j, color[j]});
                        }
                    }
                }
            }
        }
        return true;
    }
};

// This function is provided to check if your graph data is stored well or not
void printv(vector <vector <int> > v) {
    for (size_t i = 0; i < v.size(); i++) {
        for (size_t j = 0; j < v[i].size(); j++)
            cout << v[i][j] << " ";
        cout << endl;
    }
}

int main()
{
    // Solution class for bipartite-ness problem
    Solution s;

    // Read data filename from std input stream and open it with file handler/pointer
    string fname;
    cin >> fname;
    ifstream graphFileHandler(fname);

    // An array of integers to store neighbours of a vertex
    vector <int> adj;
    // A 2D array of integers to store neighbours of each vertex in a graph
    vector<vector <int> > graph;

    string line;
    // Read from the file until there is no new line left
    while (graphFileHandler >> line) {
        // Save line as string stream object
        stringstream ss(line);
        // Get a string variable
        string substr;

        // Until the end of the line, read the line as substings separated by commas
        while (ss.good()) {
            getline(ss, substr, ',');
            // Push the substring in adjacency list, we got a neighbour here
            adj.push_back(stoi(substr));
        }

        // When line ends, push all the neighbours of the vertex into the graph
        graph.push_back(adj);

        // Clear the array, before the next iteration begins
        // Next iteration, we will read the subsequent line from the file and it will contain neighbours of some other guy
        adj.clear();
    }

    // Check if data file is read in the way it is supposed to be
    // D O   N O T   P R I N T   T H E   G R A P H   I N   Y O U R   S O L U T I O N
    // The expected output only includes a True or a False not the graph data itself
    // Do not uncomment this line
    // printv(graph);

    // Save the return value of the function
    bool answer = s.isBipartite(graph);

    // If answer is True, print True otherwise False
    if (answer == 1)
        cout << "True" <<endl;
    else
        cout << "False" << endl;

    return 0;
}
