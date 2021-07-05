#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

class Graph
{
	int numVertices;
	vector<string> vertices;
	vector<vector <string> > edges;
	void SetVertices();
public:
	Graph();
	void AddEdge(vector<string> edge);
	void PrintGraph();
	void FindDijkstraShortestPath(string startVertex, string endVertex);
};

Graph::Graph()
{
	numVertices = 0;
}

void Graph::AddEdge(vector<string> edge)
{
	edges.push_back(edge);
	SetVertices();
}

void Graph::SetVertices()
{
	for (int i = 0; i < edges.size(); i++)
		for (int j = 0; j < edges[i].size() - 1; j++)
			if (find(vertices.begin(), vertices.end(), edges[i][j]) == vertices.end()) {
				vertices.push_back(edges[i][j]);
				numVertices++;
			}
}

void Graph::PrintGraph()
{
	cout << "GRAPH" << endl;
	for (int i = 0; i < edges.size(); i++) {
		for (int j = 0; j < edges[i].size(); j++)
			cout << edges[i][j] << " ";
		cout << endl;
	}
	cout << "* * * * * * * * * *" << endl;
}


void Graph::FindDijkstraShortestPath(string startVertex, string endVertex)
{
	vector<pair<int, pair<string, string> > > tree;
	vector<pair<int, string> > distance(numVertices);
	vector<pair<string, vector<string> > > shortest_path(numVertices);
	vector<string> visited;

	for (int i = 0; i < vertices.size(); i++) {
		distance[i].second = vertices[i];
		if (distance[i].second == "Ma")
			distance[i].first = 0;
		else
			distance[i].first = INT_MAX;

		// store spots in shortest paths for each node
		shortest_path[i].first = vertices[i];
	}

	int idx_neighbour, idx_current;
	string currentVertex = startVertex;

	while (visited.size() != numVertices) {
		for (int i = 0; i < edges.size(); i++) {
			if (edges[i][0] == currentVertex)
				if (find(visited.begin(), visited.end(), edges[i][1]) == visited.end()) {
					for (int j = 0; j < distance.size(); j++)
						if (distance[j].second == edges[i][1])
							idx_neighbour = j;
						else if (distance[j].second == currentVertex)
							idx_current = j;


					int new_distance = (distance[idx_current].first) + stoi(edges[i][2]);

					if (new_distance < distance[idx_neighbour].first) {
						distance[idx_neighbour].first = new_distance;
						shortest_path[idx_neighbour].second.clear();
						for (int k = 0; k < shortest_path[idx_current].second.size(); k++)
							shortest_path[idx_neighbour].second.push_back(shortest_path[idx_current].second[k]);
						shortest_path[idx_neighbour].second.push_back(currentVertex);
					}
				}
		}

		visited.push_back(currentVertex);

		int min_distance = INT_MAX;
		int min_id;
		for (int i = 0; i < distance.size(); i++)
			if (find(visited.begin(), visited.end(), distance[i].second) == visited.end())
				if (distance[i].first < min_distance) {
					min_distance = distance[i].first;
					min_id = i;
				}

		currentVertex = distance[min_id].second;
	}

//	cout << "Dijkstra's Shortest Path from Mankara to every other spot " << endl;
//	for (int i = 0; i < distance.size(); i++) {
//		cout << "Shortest path to " << distance[i].second << ": ";
//
//
//		for (int j = 0; j < shortest_path[i].second.size(); j++)
//			cout << shortest_path[i].second[j] << " ";
//
//		cout << "" << distance[i].first << endl;
//	}

	int endVertexID;
	for (int i = 0; i < shortest_path.size(); i++) {
		if (shortest_path[i].first == endVertex)
			endVertexID = i;
	}

	int endVertexID2;
	for (int i = 0; i < distance.size(); i++) {
		if (distance[i].second == endVertex)
			endVertexID2 = i;
	}

	for (int i = 0; i < shortest_path[endVertexID].second.size(); i++)
		cout << shortest_path[endVertexID].second[i] << " ";
	cout << shortest_path[endVertexID].first << " " << distance[endVertexID2].first << endl;
}

bool isNumber(string s)
{
    for (int i = 0; i < s.length(); i++)
        if (isdigit(s[i]) == false)
            return false;

    return true;
}


int main() {
	string fname;
	cin >> fname;
	ifstream city_plan(fname);
	vector<string> edge;
	Graph g;
	vector<string> dist_less_than_five;

	string line;
	while (city_plan >> line) {
		stringstream ss(line);
		string substr;

		while (ss.good()) {
			getline(ss, substr, ',');
			edge.push_back(substr);
		}

		bool flag = false;
		for (int i = 0; i < edge.size(); i++) {
			if (edge[i].substr(0, 1) == "E") {
				if (stoi(edge[2]) < 5 && (edge[i == 0 ? 1 : 0] != "Ma" && edge[i == 0 ? 1 : 0] != "Mo"))
					dist_less_than_five.push_back(edge[i == 0 ? 1 : 0]);
				edge.clear();
				flag = true;
			}
		}

		if (!flag)
			for (int j = 0; j < dist_less_than_five.size(); j++) {
				if (edge[0] == dist_less_than_five[j]) {
					edge.clear();
					flag = true;
				}
				else if (edge[1] == dist_less_than_five[j]) {
					edge.clear();
					flag = true;
				}
			}


		if (!flag) {
			g.AddEdge(edge);
			reverse(edge.begin(), edge.end());

			vector<string> rev;
			rev.push_back(edge[1]);
			rev.push_back(edge[2]);
			rev.push_back(edge[0]);

			g.AddEdge(rev);
			edge.clear();
		}
	}

	//g.PrintGraph();
	g.FindDijkstraShortestPath("Ma", "Mo");

	return 0;
}
