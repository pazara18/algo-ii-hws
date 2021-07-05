/*
 * q3_maxProfit_skeleton.cpp
 *
 *  Created on: June 14th, 2021
 *      Author: Ugur Unal
 * Modified on: June 26th, 2021
 *     Student: Abdulkadir Pazar
 *  Student ID: 150180028
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <set>
#include <utility>
#include <vector>
#include <algorithm>

using namespace std;

pair<int, set<int>> MaxProfit(int numOfCrystals, vector<int> profits, vector<int> entryCosts)
{
	set<int> citiesToVisit;
	int N  = profits.size();
	int W = numOfCrystals;
	int matrix[N + 1][W + 1];
	for(int i = 0; i < N + 1; i++)
		for(int w = 0; w < W + 1; w++)
			matrix[i][w] = 0;

	for(int i = 1; i < N + 1; i++)
	{
		for(int w = 1; w < W + 1; w++)
		{
			if(entryCosts[i-1] > w)
				matrix[i][w] = matrix[i-1][w];
			else
				matrix[i][w] = max(matrix[i-1][w], profits[i-1] + matrix[i-1][w - entryCosts[i-1]]); 
		}
	}
	int maxProfit = matrix[N][W];
	int i = N;
	int w = W;
	while(i > 0 && w > 0)
	{
		if (matrix[i][w] != matrix[i-1][w])
		{
			citiesToVisit.insert(i);
			w = w - entryCosts[i-1];
		}
		i = i - 1;
	}
	int numOfCities = N;
	cout << "Dynaming Programming Table" << endl;
	for (int i = 0; i <= numOfCities; i++) {
		for (int j = 0; j <= numOfCrystals; j++) {
			cout << std::right << std::setw(3) << matrix[i][j];
		}
		cout << endl;
	}

	return pair<int, set<int>>(maxProfit, citiesToVisit);
}

int main() {
	int numOfCrystals;
	vector<int> profits;
	vector<int> entryCosts;
	
	string inputFilename;
	cout << "Enter the name of the input file: ";
	cin >> inputFilename;

	ifstream input(inputFilename);

	if (!input.is_open()) {
		cerr << "File named \"" << inputFilename << "\" could not open!" << endl;
		return EXIT_FAILURE;
	}

	string line;
	if (getline(input, line)) {
		numOfCrystals = stoi(line);
	}
	while (getline(input, line, ' ')) {
		profits.push_back(stoi(line));
		getline(input, line);
		entryCosts.push_back(stoi(line));
	}

	pair<int, set<int>> result = MaxProfit(numOfCrystals, profits, entryCosts);

	cout << "Max profit is " << result.first << "." << endl;
	cout << "Cities visited:";
	for (int cityNumber : result.second) {
		cout << " " << cityNumber;
	}
	cout << endl;
}
