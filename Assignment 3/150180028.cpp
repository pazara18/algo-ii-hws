/*
 * Author: Abdulkadir Pazar
 * Student ID: 150180028 
 * Date: 17.05.2021
 */
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>

using namespace std;

int match = 1;
int mismatch_i = -2;
int gap = -4;

struct dp
{
    int i;
    pair<int,int> traceback;
};

int findMax(int n1, int n2, int n3, int n4)
{
    int max = 0;
    int arr[4] = {n1, n2, n3, n4};
    for(int i = 0; i < 4; i++)
        if(arr[i] > max)
            max = arr[i];
    return max;
}

void SW(string a, string b, ostream& of)
{
    int max_score = 0; 
    int n = a.size(), m = b.size();
    dp dp[n+1][m+1];
    int similarity, max;
    string s;

    vector<pair<int,int>> max_ind;
    set<string> res;
    
    for(int i = 0; i < n+1; i++)
    {
        for(int j = 0 ; j < m+1; j++)
        {
            dp[i][j].i = 0;
            dp[i][j].traceback.first = 0;
            dp[i][j].traceback.second = 0;
        }
    }

    for(int i = 0; i < n+1; i++)
    {    
        for(int j = 0 ; j < m+1; j++)
        {
            if(i == 0 || j == 0)
            { 
                dp[i][j].i = 0;
                dp[i][j].traceback.first = 0;
                dp[i][j].traceback.second = 0;
            }
            else 
            {
                similarity = (a[i-1] == b[j-1]) ? match : mismatch_i;
                max = findMax(dp[i-1][j-1].i + similarity, dp[i-1][j].i + gap, dp[i][j-1].i + gap, 0);
                dp[i][j].i = max;
                if(max == dp[i-1][j-1].i + similarity) {dp[i][j].traceback.first = i-1; dp[i][j].traceback.second = j-1;}
                else if(max == dp[i-1][j].i + gap) {dp[i][j].traceback.first = i-1; dp[i][j].traceback.second = j;}
                else if(max == dp[i][j-1].i + gap) {dp[i][j].traceback.first = i; dp[i][j].traceback.second = j-1;}
            }
        }
    }
    
    for(int i = 0; i < n+1; i++)
        for(int j = 0 ; j < m+1; j++)
            if(dp[i][j].i > max_score) 
                max_score = dp[i][j].i;
    
    for(int i = 0; i < n+1; i++)
        for(int j = 0 ; j < m+1; j++)
            if((dp[i][j].i == max_score) && (max_score != 0))
                max_ind.push_back({i,j});

    for(auto i : max_ind)
    {
        s = "";
        pair<int,int> j = i;
        while(dp[j.first][j.second].i != 0)
        {
            s += a[j.first - 1];
            j = dp[j.first][j.second].traceback;
        }  
        reverse(s.begin(), s.end());
        res.insert(s);
    }
    of << a << " - " << b << endl << "Score: " << max_score << " Sequence(s):";
    for(auto i : res) of << " \"" << i << "\"";
    of << endl;
}

int main(int argc, char** argv)
{
    fstream in(argv[1]);
    ofstream out(argv[2]);
    vector<string> words;
    string s;
    
    while(in >> s) 
        words.push_back(s);
    in.close();
    sort(words.begin(), words.end(), std::greater<string>());
    
    for(auto i = words.rbegin(); i != words.rend(); i++)
    {
        s = *i;
        words.pop_back();
        for(auto j = words.rbegin(); j != words.rend(); j++) 
            SW(s, *j, out);
    }
    out.close();
    return 0;
}