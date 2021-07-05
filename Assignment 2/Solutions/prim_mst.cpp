/*
 * prim_mst.cpp
 *
 *  Created on: Apr 4, 2021
 *      Author: tardis
 */
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

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
	void FindPrimMST(string startVertex);
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


void Graph::FindPrimMST(string startVertex)
{
	vector<pair<int, pair<string, string> > > MST;
	vector<pair<int, pair<string, string> > > cost;
	vector<string> visited;

	while (visited.size() != numVertices) {
		for (int i = 0; i < edges.size(); i++)
			if (edges[i][0] == startVertex)
				if (find(visited.begin(), visited.end(), edges[i][1]) == visited.end()) {
					pair<string, string> p(startVertex, edges[i][1]);
					cost.push_back(make_pair(stoi(edges[i][2]), p));
				}

		pair<string, string> minVertex("-1", "-1");
		int minCost = INT_MAX;

		if (visited.size() == 0) {
			for (int i = 0; i < cost.size(); i++)
				if (cost[i].second.first == "GP" && cost[i].second.second == "Hipp") {
					minCost = cost[i].first;
					minVertex = cost[i].second;
					if (find(MST.begin(), MST.end(), make_pair(minCost, minVertex)) == MST.end())
						MST.push_back(make_pair(minCost, minVertex));
					if (find(visited.begin(), visited.end(), minVertex.first) == visited.end())
						visited.push_back(minVertex.first);
				}

			minCost = INT_MAX;
			for (int i = 0; i < cost.size(); i++)
				if (cost[i].second.first == "GP" && cost[i].second.second.substr(0, 2) == "Ch" && cost[i].first < minCost) {
					minCost = cost[i].first;
					minVertex = cost[i].second;
				}

			if (find(MST.begin(), MST.end(), make_pair(minCost, minVertex)) == MST.end())
				MST.push_back(make_pair(minCost, minVertex));

			if (find(visited.begin(), visited.end(), minVertex.second) == visited.end()) {
				visited.push_back(minVertex.second);
				for (int i = 0; i < edges.size(); i++)
					if (edges[i][0] == minVertex.second)
						if (find(visited.begin(), visited.end(), edges[i][1]) == visited.end()) {
							pair<string, string> p(minVertex.second, edges[i][1]);
							cost.push_back(make_pair(stoi(edges[i][2]), p));
						}
			}
		}


		//sort(MST.begin(), MST.end());
		int total = 0;
		for (int i = 0; i < MST.size(); i++) {
			cout << MST[i].second.first << " " << MST[i].second.second << " "<< MST[i].first << endl;
			total += MST[i].first;
		}

		cout << total << endl;
		cout << "* * * * * * * * * *" << endl;

		for (int i = 0; i < cost.size(); i++)
			if (cost[i].first < minCost)
				if (find(visited.begin(), visited.end(), cost[i].second.second) == visited.end()) {
					if ((cost[i].second.first == "GP" && cost[i].second.second.substr(0, 2) == "Ch") || (cost[i].second.first.substr(0, 2) == "Ch" && cost[i].second.second == "GP")) {
						minCost = cost[i].first;
						minVertex = cost[i].second;
					}
					else if ((cost[i].second.first == "Hipp" && cost[i].second.second == "GP") || (cost[i].second.first == "GP" && cost[i].second.second == "Hipp")) {
						minCost = cost[i].first;
						minVertex = cost[i].second;
					}
					else if ((cost[i].second.first == "Hipp" && cost[i].second.second.substr(0, 3) != "Bas") || (cost[i].second.first.substr(0, 3) == "Bas" && cost[i].second.second != "Hipp")) {
						minCost = cost[i].first;
						minVertex = cost[i].second;
					}
					else if ((cost[i].second.first.substr(0, 2) == "Hp" && cost[i].second.second.substr(0, 2) != "Hp") || (cost[i].second.first.substr(0, 2) != "Hp" && cost[i].second.second.substr(0, 2) == "Hp")) {
						minCost = cost[i].first;
						minVertex = cost[i].second;
					}
					else if (cost[i].second.first.substr(0, 2) == "Ch" || cost[i].second.second.substr(0, 2) == "Ch" || cost[i].second.first == "GP" || cost[i].second.second == "GP") {
						minCost = cost[i].first;
						minVertex = cost[i].second;
					}
				}

		bool flag = false;
		//cout << minVertex.first << minVertex.second << "-----before" << endl;
		for (int i = 1; i < edges.size(); i++) {
			pair<string, string> curr(edges[i][0], edges[i][1]);
			pair<string, string> prev(edges[i - 1][0], edges[i - 1][1]);

			if (minVertex.first == curr.first && minVertex.second == curr.second && curr.first == prev.second && curr.second == prev.first) {
				string temp = minVertex.first;
				minVertex.first = minVertex.second;
				minVertex.second = temp;
				flag = true;
			}
		}
		//cout << minVertex.first << minVertex.second << "-----after" << endl;
		//cout << "* * * * * * * * * * * " << endl;

		if (find(MST.begin(), MST.end(), make_pair(minCost, minVertex)) == MST.end())
			MST.push_back(make_pair(minCost, minVertex));

		if (find(visited.begin(), visited.end(), startVertex) == visited.end())
			visited.push_back(startVertex);

		if (flag) {
			if (find(visited.begin(), visited.end(), minVertex.first) == visited.end())
				visited.push_back(minVertex.first);
			startVertex = minVertex.first;
		}
		else if (!flag) {
			if (find(visited.begin(), visited.end(), minVertex.second) == visited.end())
				visited.push_back(minVertex.second);
			startVertex = minVertex.second;
		}
	}

/*	cout << "Edges of Prim's Minimum Spinning Tree " << endl;
	int total = 0;
	for (int i = 0; i < MST.size(); i++) {
		cout << MST[i].first << "-" << MST[i].second.first << "&" << MST[i].second.second << endl;
		total += MST[i].first;
	}

	cout << "Minimum total cost: " << total << endl;
	cout << "* * * * * * * * * * * *" << endl;*/
	sort(MST.begin(), MST.end());
	int total = 0;
	for (int i = 0; i < MST.size(); i++) {
		cout << MST[i].second.first << " " << MST[i].second.second << " "<< MST[i].first << endl;
		total += MST[i].first;
	}

	cout << total << endl;
}

int main() {
	string fname;
	cin >> fname;
	ifstream city_plan(fname);
	vector<string> edge;
	Graph g;

	string line;
	while (city_plan >> line) {
		stringstream ss(line);
		string substr;

		while (ss.good()) {
			getline(ss, substr, ',');
			edge.push_back(substr);
		}

		g.AddEdge(edge);
		reverse(edge.begin(), edge.end());

		vector<string> rev;
		rev.push_back(edge[1]);
		rev.push_back(edge[2]);
		rev.push_back(edge[0]);

		g.AddEdge(rev);
		edge.clear();
	}

	// g.PrintGraph();
	g.FindPrimMST("GP");

	return 0;
}
