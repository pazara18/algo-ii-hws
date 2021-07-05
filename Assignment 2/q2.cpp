/*
 * Name-Surname: Abdulkadir Pazar
 * Student No: 150180028
 * Date: 19.04.2021
 */

#include <iostream>
#include <fstream> // ifstream
#include <string> //string
#include <vector> // vector
#include <algorithm> //find(), sort()
#include <limits> //INT32_MAX
#include <queue> //priority_queue

using namespace std;
typedef pair<int, int> int_pair;


//struct for nodes read from file
struct Node
{
    string source;
    string destination;
    int distance;
};
 
//edge struct used in graph
struct Edge 
{
    int source;
    int dest;
    int weight;
};

//struct used to store place data
struct Place
{
    int index;
    string name;
    bool isMistanbul = false;
    bool isMankara = false;
    bool isSafe = false;
    bool isResting = false;
    bool isEnemy = false;
};
 
//struct for the node of minheap
struct HeapNode 
{
    int vertex;
    int weight;
};

//comparison struct used for comparing nodes of stl queue 
struct comp
{
    bool operator()(const HeapNode &lhs, const HeapNode &rhs) const 
    {
        return lhs.weight > rhs.weight;
    }
};

struct Graph
{
    vector<vector<Edge>> adjList;//adjacency list representation of graph
 
    Graph(vector<Edge> const &edges, int N)//ctor for graph
    {
        adjList.resize(N);
        for (Edge const &edge: edges)
            adjList[edge.source].push_back(edge);
    }
    void findShortestPath(vector<Place>&, int, int, int);//dijktra spa function
    void print_route(vector<int> const &prev, const vector<Place>& landmarks, int i);
    bool isEnemyNear(int, const vector<Place>&);//utility function to find if vertice has near enemy neighbor
};

void Graph::findShortestPath(vector<Place>& landmarks, int source, int dest, int N)
{
    priority_queue<HeapNode, vector<HeapNode>, comp> min_heap;
    min_heap.push({source, 0});
 
    vector<int> dist(N, INT32_MAX);//set all distances to inf
    dist[source] = 0;//set sources distance to itself 0
    vector<bool> done(N, false);//bool vector to keep track of done nodes
    done[source] = true;
    vector<int> prev(N, -1);//previous vertices vector used in print
 
    while (!min_heap.empty())
    {
        HeapNode node = min_heap.top();
        min_heap.pop();
        int u = node.vertex;
        for (auto const& i: adjList[u])//for the neighbors of all vertices
        {
            int v = i.dest;
            int weight = i.weight;
            if (!done[v] && (dist[u] + weight) < dist[v] && !landmarks[u].isEnemy && !isEnemyNear(u, landmarks))//if all criteria are satisfied
            {
                dist[v] = dist[u] + weight;//update distance
                prev[v] = u;
                min_heap.push({v, dist[v]});
            }
        }
        done[u] = true;
    }
    print_route(prev, landmarks, dest);//print route from Ma to Mo
    cout << dist[dest] << "\n";//print distance
}

void Graph::print_route(vector<int> const &prev, const vector<Place>& landmarks, int dest)//print route
{
    vector<string> places;
    while(dest >= 0)
    {
        places.push_back(landmarks[dest].name);
        dest = prev[dest];
    }

    for(int i = places.size() - 1; i >= 0; i--)
    {
        cout << places[i] << " ";
    }
}

bool Graph::isEnemyNear(int u, const vector<Place>& landmarks)
{
    vector<Edge> neighbors = adjList[u];
    for(unsigned int i = 0; i < adjList[u].size(); i++)
        if(landmarks[neighbors[i].dest].isEnemy && neighbors[i].weight < 5)//for all neighbors check if it is enemy and closer than 5
            return true;
    return false;
}

int main(int argc, char** argv)
{
    ifstream file; 
    string filename = "path_info_1.txt";
    string src, dst, dist;
    vector<Node> fvect;
    cin >> filename;

    file.open(filename);
    while(!file.eof())//read info from file
    {
        getline(file, src, ',');
        getline(file, dst, ',');
        getline(file, dist, '\n');

        Node n;
        n.source = src;
        n.destination = dst;
        n.distance = stoi(dist);
        fvect.push_back(n);
    }
    file.close();

    vector<Place> landmarks;
    vector<string> place_names;

    for(unsigned int i = 0; i < fvect.size(); i++)//add new location name to place names
        if(find(place_names.begin(), place_names.end(), fvect[i].source) == place_names.end())
            place_names.push_back(fvect[i].source);

    for(unsigned int i = 0; i < fvect.size(); i++)//add new location name to place names
        if(find(place_names.begin(), place_names.end(), fvect[i].destination) == place_names.end())
            place_names.push_back(fvect[i].destination);
    
    for(unsigned int i = 0; i < place_names.size(); i++)//create place struct for places
    {
        Place p;
        string name = place_names[i];
        p.name = name;
        p.index = i;
        int pos = 0;
        if(name == "Mo"){ p.isMistanbul = true;}
        if(name == "Ma"){ p.isMankara = true;}
        if(!(name.find("S",pos))){ p.isSafe = true;}
        if(!(name.find("E",pos))){ p.isEnemy = true;}
        if(!(name.find("R",pos))){ p.isResting = true;}
        landmarks.push_back(p);
    }
	
    int E = fvect.size(); 
    vector<Edge> edges;
    int source;
    int destination;

    for(int i= 0; i < E; i++)//add locations to graph
    {
        int src = 0;
        int dest = 0;
        while(fvect[i].source != landmarks[src].name) src++;
        while(fvect[i].destination != landmarks[dest].name) dest++;
        int dist = fvect[i].distance;
        edges.push_back({src, dest, dist});
        if (landmarks[src].name == "Ma") source = src;
        if (landmarks[dest].name == "Mo") destination = dest;
    }
    Graph g(edges, E);
    g.findShortestPath(landmarks, source, destination, place_names.size());

	return 0;
}
