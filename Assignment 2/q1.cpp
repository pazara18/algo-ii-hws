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

using namespace std;
typedef pair<int, int> int_pair;

//struct for nodes read from file
struct Node
{
    string source;
    string destination;
    int distance;
};

//struct for places in city
struct Place
{
    int index;
    string name;
    bool isPalace = false;
    bool isChurch = false;
    bool isBasilica = false;
    bool isHome = false;
    bool isHippodrome = false;
};

//struct for graph
struct Graph
{
	int V, E;
	vector<pair<int, int_pair>> edges;
    vector<Node> MST;

	Graph(int V, int E){this->V = V;this->E = E;}
	void addEdge(int u, int v, int w){edges.push_back({w, {u, v}});}
	int kruskalMST(vector<Place>&);
    void printMST(int);
};

//struct for disjoint sets used in kruskals algorithm
struct DJSets
{
	int *parent, *rank;
	int n;

	DJSets(int n)
	{
		this->n = n;
		parent = new int[n+1];
		rank = new int[n+1];
		for (int i = 0; i <= n; i++)
		{
			rank[i] = 0;
			parent[i] = i;
		}
	}

	int find(int u)
	{
		if (u != parent[u])
			parent[u] = find(parent[u]);
		return parent[u];
	}

	void merger(int x, int y)
	{
		x = find(x), y = find(y);
		if (rank[x] > rank[y])
			parent[y] = x;
		else
			parent[x] = y;
		if (rank[x] == rank[y])
			rank[y]++;
	}
};

//returns weight of mst and adds places to mst vector 
int Graph::kruskalMST(vector<Place> &locations)
{
	int mst_wt = 0;
    //make sure no hippodrome is connected to a basilica
    for(unsigned int i = 0; i<edges.size();i++)
    {
        if(locations[edges[i].second.first].isHippodrome && locations[edges[i].second.second].isBasilica)
            edges[i].first = INT32_MAX;
        if(locations[edges[i].second.first].isBasilica && locations[edges[i].second.second].isHippodrome)
            edges[i].first = INT32_MAX;
    }
    //make sure no two homes are connected
    for(unsigned int i = 0; i<edges.size(); i++)
        if(locations[edges[i].second.first].isHome && locations[edges[i].second.second].isHome)
            edges[i].first = INT32_MAX;

	sort(edges.begin(), edges.end());

    //create church connection first
    int church_index;
    for(unsigned int i = 0; i<edges.size();i++)
    {
        if(locations[edges[i].second.first].isPalace && locations[edges[i].second.second].isChurch)
        {
            church_index = i;
            break;
        }
    }
    edges[church_index].swap(edges[0]);

    //create hippodrome connection first
    int hipp_index;
    for(unsigned int i = 0; i<edges.size();i++)
    {
        if(locations[edges[i].second.first].isPalace && locations[edges[i].second.second].isHippodrome)
        {
            hipp_index = i;
            break;
        }
    }
    edges[hipp_index].swap(edges[1]);

    sort(edges.begin() + 2, edges.end());
    
	DJSets ds(V);

	vector<pair<int, int_pair>>::iterator it;
	for (it=edges.begin(); it!=edges.end(); it++)
	{
		int u = it->second.first;
		int v = it->second.second;
     
		int set_u = ds.find(u);
		int set_v = ds.find(v);
		if (set_u != set_v)//this node is going to be added
		{
            int i = 0;
            int j = 0;
            while(locations[i].index != u) i++;
            while(locations[j].index != v) j++;
            Node temp;
            temp.source = locations[i].name;
            temp.destination = locations[j].name;
            temp.distance = it->first;
            MST.push_back(temp);
			mst_wt += it->first;//increment weight
			ds.merger(set_u, set_v);//merge sets
		}
	}
	return mst_wt;
}

void Graph::printMST(int weight)//print in ascending edge weight order
{
    vector<int> edges;
    for(unsigned int i = 0; i < MST.size(); i++)
    {
        int dist;
        dist = MST[i].distance;
        edges.push_back(dist);
    }
    sort(edges.begin(), edges.end());
    for(unsigned int i = 0; i < MST.size(); i++)
    {
        int j = 0;
        while(edges[i] != MST[j].distance) j++;
        cout << MST[j].source << " " << MST[j].destination << " " << MST[j].distance << "\n";
    }
	cout << weight << "\n";
}

int main(int argc, char** argv)
{
    ifstream file; 
    string filename;
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
        if(name == "GP"){ p.isPalace = true;}
        if(name == "Hipp"){ p.isHippodrome = true;}
        if(!(name.find("Bas",pos))){ p.isBasilica = true;}
        if(!(name.find("Ch",pos))){ p.isChurch = true;}
        if(!(name.find("Hp",pos))){ p.isHome = true;}
        landmarks.push_back(p);
    }
	
	int V = landmarks.size();
    int E = fvect.size();
	Graph g(V, E);//create graph

    for(int i= 0; i < E; i++)//add locations to graph
    {
        int src = 0;
        int dest = 0;
        while(fvect[i].source != landmarks[src].name) src++;
        while(fvect[i].destination != landmarks[dest].name) dest++;
        int dist = fvect[i].distance;
        g.addEdge(src, dest, dist);
    }

	int mst_wt = g.kruskalMST(landmarks);
    g.printMST(mst_wt);

	return 0;
}
