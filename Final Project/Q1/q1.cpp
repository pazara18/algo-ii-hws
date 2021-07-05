/***********************************************************
2021 Spring - BLG 336E-Analysis of Algorithms II
Final Project
Question on Greedy Algorithms
Modified Dijkstra Algorithms for Maximum Capacity Path
Submitted: 15.06.2021 
**********************************************************/

/***********************************************************
STUDENT INFORMATION
Full Name : Abdulkadir Pazar
Student ID: 150180028
**********************************************************/

// Some of the libraries you may need have already been included.
// If you need additional libraries, feel free to add them
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>

// Do not change this definition
#define MAX_INT 1000


using namespace std;

class Graph{
public:
	int node_count;
	int edge_count;
	int** adjacency_matrix;

	Graph(){};
	void read_file(char* filename);
	void print_adjacency_matrix(); // in case you need
	int weight(int i, int j){return this->adjacency_matrix[i][j];}
	~Graph();
};

Graph::~Graph(){
	for(int i = 0; i < node_count; i++){
		delete [] adjacency_matrix[i];
	}
}

void Graph::print_adjacency_matrix(){	
	// Prints the adjacency matrix
	for(int i = 0; i < this->node_count; i++){
		for(int j = 0; j < this->node_count; j++){
			cout<<this->adjacency_matrix[i][j]<<", ";
		}
		cout<<endl;
	}
}

void Graph::read_file(char* filename){
	ifstream in(filename);
	if (!in.is_open())
	{
		cerr << "File could not be opened" << endl;
		exit(1);
	}
	string line;
	getline(in, line);
	node_count = stoi(line);
	edge_count = 0;

	adjacency_matrix = new int*[node_count];
	for(int i = 0; i < node_count; i++)
		adjacency_matrix[i] = new int[node_count];

	for(int i = 0; i < node_count; i++)
		for(int j = 0; j < node_count;j++)
			adjacency_matrix[i][j] = 0;
	int src, dest, wt;
	string src_s, dest_s, wt_s;
	while (getline(in, line))
	{
		stringstream ss(line);
		getline(ss, src_s, '\t');
		getline(ss, dest_s, '\t');
		getline(ss, wt_s);

		src = stoi(src_s);
		dest = stoi(dest_s);
		wt = stoi(wt_s);

		adjacency_matrix[src][dest] = wt;
		adjacency_matrix[dest][src] = wt;

		edge_count++;
	}
}

int findMaxWtFringe(vector<int> &status, vector<int> &wt)
{
	int fringe_index = -1;
	int fringe_wt = MAX_INT * -1;
	for (unsigned int i = 0; i < status.size(); i++)
	{
		if (status[i] == 1 && wt[i] > fringe_wt)
		{
			fringe_index = i;
			fringe_wt = wt[i];
		}
	}
	return fringe_index;
}

void Modified_Dijkstra(Graph* graph){
	vector<int> status(graph->node_count, 0);
	vector<int> wt(graph->node_count, -1);
	vector<int> dad(graph->node_count, -1);
	int s = 0;
	status[s] = 2;
	wt[s] = MAX_INT;

	for (int w = 0; w < graph->node_count; w++)
	{
		if (graph->adjacency_matrix[s][w] != 0)
		{
			status[w] = 1;
			wt[w] = graph->adjacency_matrix[s][w];
			dad[w] = 0;
		}
	}
	int v = findMaxWtFringe(status, wt);
	while (v != -1)
	{
		status[v] = 2;
		for (int w = 0; w < graph->node_count; w++)
		{
			if (graph->adjacency_matrix[v][w] != 0)
			{
				if (status[w] == 0)
				{
					status[w] = 1;
					wt[w] = min(wt[v], graph->adjacency_matrix[v][w]);
					dad[w] = v;
				} 
				else if (status[w] == 1 && wt[w] < min(wt[v], graph->adjacency_matrix[v][w]))
				{
					wt[w] = min(wt[v], graph->adjacency_matrix[v][w]);
					dad[w] = v;
				}
			}
		}
		v = findMaxWtFringe(status, wt);
	}
	/*********************************************/
	/***** DO NOT CHANGE THE FOLLOWING LINES *****/
	/**** THEY PRINT OUT THE EXPECTED RESULTS ****/
	/*********************************************/

	// The following line prints wt array (or vector).
	// Do not change anything in the following lines.
	cout<<"###########RESULTS###########"<<endl;
	cout<<endl;
	
	cout<<"1. WT ARRAY"<<endl;
	cout<<"------------------------"<<endl;
	cout<<"  ";
	for(int i = 0; i < graph->node_count - 1; i++){
		cout << wt[i] << ", ";
	}
	cout << wt[graph->node_count - 1] << endl;
	
	// The following lines print the final path.
	// Do not change anything in the following lines.
	int iterator = graph->node_count - 1;
	vector<int> path_info;
	path_info.push_back(iterator);
	while(iterator != 0){
		path_info.push_back(dad[iterator]);
		iterator = dad[iterator];
	}
	cout<<endl;
	cout<<"2. MAXIMUM CAPACITY PATH"<<endl;
	cout<<"------------------------"<<endl;
	cout<<"  ";
	vector<int>::iterator it;
    for (it = path_info.end() - 1; it > path_info.begin(); it--)
        cout << *it << " -> ";
    cout<<*path_info.begin()<<endl;
    
    cout<<endl;
    cout<<"3. MAXIMUM CAPACITY"<<endl;
    cout<<"------------------------"<<endl;
    cout<<"  ";
    cout<<wt[graph->node_count - 1]<<endl;
    cout<<"#############################"<<endl;
    
    return;
}

int main(int argc, char **argv){
	Graph* graph = new Graph();
	graph->read_file(argv[1]);
	Modified_Dijkstra(graph);
	
	return 0;	
}
