/*
 *  Abdulkadir Pazar
 *  150180028
 *  To compile: g++ -std=c++11 main.cpp -o main
 */

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>

using namespace std;

struct Node
{
    vector<pair<char, int>> data;//keeps pairings as pair<char,int>
    Node* children[10];//pointers to children

    Node(){}//constructor for root
    Node(vector<pair<char, int>> data){this->data = data;}//constructor with data
};

struct QueueNode
{
    Node* data;
    QueueNode* next;

    QueueNode(){data = NULL;next = NULL;}
    QueueNode(Node* data){this->data = data; next = NULL;}
};

class Queue
{
    QueueNode* front;
    QueueNode* back;
    int size;

    Queue(){front = NULL;back = NULL;size = 0;};
    void enqueue(Node*);
    Node* dequeue();
    bool is_empty(){return front == NULL;};
    friend class Tree;
};

class Tree
{
    Node* root;
    string words[3];
    vector<char> distinct_letters;//keep distinct letters
    vector<char> last_digits;//letters respresenting last digits of numbers
    vector<pair<char, int>> result;
    bool solution_found = false;
    int n_visited_nodes = 0; 
    int max_n_nodes = 0;
    int curr_n_nodes = 0;

public:
    Tree(string*);//ctor takes words as argument
    void buildTree(Node*, int);//build tree recursively
    void DFS(Node*, int);//perform dfs on a tree write result to result when found
    void BFS(Node*);//perform bfs on a tree write result to result when found
    void printResult(string, string, float);//utility function to print result in requested format
    void writeToFile(string);//utility function to write result to file in requested format
    Node* getRoot(){return root;}
};

//utility functions to find index
template <typename T>
int find_index(const vector<T> &v, const T &val)
{
    for (int i = 0; i < v.size(); i++)
        if (v[i] == val)
            return i;
    return -1;
}

int get_index_from_letter(const vector<pair<char, int>> &v, const char &val)
{
    for (int i = 0; i < v.size(); i++)
        if (v[i].first == val)
            return i;
    return -1;
}

//checks whether different letters has same number 
bool duplicate_number(Node* n)
{
    int exists[10];
    for(int i = 0; i < 10; i++)
        exists[i] = 0;

    for(int i = 0; i < n->data.size(); i++)
        exists[n->data[i].second]++;

    for(int i = 0; i < 10; i++)
        if(exists[i] > 1) return true;

    return false;
}

//checks whether last digits of a number is 0
bool last_digit_check(Node* n, const vector<char>& last_digit_letters)
{
    for (auto const &letter : last_digit_letters)
        if (find_index(n->data, {letter, 0}) != -1)
            return true;
    return false;
}

//check whether equation is satisfied
bool sum_check(Node* n, string* words)
{
    int augend = 0, addend = 0, sum = 0;
    int base = 1, temp = 0;
    string w1 = words[0];
    string w2 = words[1];
    string w3 = words[2];

    for(int i = w1.length()-1; i >= 0; i--)
    {
        for(int j = 0; i < n->data.size(); j++)
        {
            if(w1[i] == n->data[j].first)
            {
                temp = n->data[j].second;
                break;
            }
        }
        augend += base * temp;
        base *= 10;
    }

    base = 1;
    temp = 0;
    for(int i = w2.length()-1; i >= 0; i--)
    {
        for(int j = 0; j < n->data.size(); j++)
        {
            if(w2[i] == n->data[j].first)
            {
                temp = n->data[j].second;
                break;
            }
        }
        addend += base * temp;
        base *= 10;
    }

    base = 1;
    temp = 0;
    for(int i = w3.length()-1; i >= 0; i--)
    {
        for(int j = 0; j < n->data.size(); j++)
        {
            if(w3[i] == n->data[j].first)
            {
                temp = n->data[j].second;
                break;
            }
        }
        sum += base * temp;
        base *= 10;
    }

    if(augend+addend == sum) return true;
    else return false;
}

void Queue::enqueue(Node *data)
{
    QueueNode* new_qn = new QueueNode(data);

    if (back == NULL)
    {
        front = new_qn;
        back = new_qn;
    }
    else
    {
        back->next = new_qn;
        back = new_qn;
    }
    size++;
}

Node *Queue::dequeue()
{
    if (is_empty()) return NULL;

    QueueNode* temp = front;
    Node *data = temp->data;

    front = front->next;
    if (is_empty()) back = NULL;

    delete temp;
    size--;

    return data;
}

Tree::Tree(string* w)
{
    root = new Node();//create empty root
    for(int i = 0; i < 3;i++) words[i] = w[i];
    for (auto const &word : words)
    {
        for (int i = 0; i < word.length(); i++)
        {
            if (find_index(distinct_letters, word[i]) == -1)//store unique letters when unique letter is read
                distinct_letters.push_back(word[i]);

            if (i == 0 && find_index(last_digits, word[i]) == -1)//store last digits to last digits vector
                last_digits.push_back(word[i]);
        }
    }
    buildTree(root, 0);
}

void Tree::buildTree(Node *r, int layer)
{
    if (layer < distinct_letters.size())//tree has depth of # of unique letters
    {
        for (int j = 0; j <= 9; j++)
        {
            vector<pair<char, int>> data(r->data);//copy data from node
            data.push_back({distinct_letters[layer], j});//add new letter to the copied data
            Node *nn = new Node(data);//create new node with data
            r->children[j] = nn;//add as children
            buildTree(nn, layer + 1);//call function to create subtree
        }
    }
}

void Tree::DFS(Node *r, int depth)
{
    if(!solution_found && depth != distinct_letters.size() + 1)
    {
        n_visited_nodes++;
        curr_n_nodes++;
        if (curr_n_nodes > max_n_nodes)
            max_n_nodes = curr_n_nodes;

        if (duplicate_number(r))//if duplicate number is observed return
        {
            curr_n_nodes--;
            return;
        }
        if (last_digit_check(r, last_digits))//if last digit is 0 return
        {
            curr_n_nodes--;
            return;
        }

        if (!solution_found && depth == distinct_letters.size() && sum_check(r, words))//if solution is not found and sum check returns true
        {
            result = r->data;
            solution_found = true;
            curr_n_nodes--;
            return;
        }

        for (int i = 0; i <=9; i++)//call dfs on children
            DFS(r->children[i], depth + 1);

        curr_n_nodes--;
    }
}

void Tree::BFS(Node *r)
{
    if(!solution_found)
    {
        Queue q;
        q.enqueue(r);

        while (!q.is_empty())
        {
            if (q.size > max_n_nodes)
                max_n_nodes = q.size;

            n_visited_nodes++;
            Node *curr_node = q.dequeue();//get node from queue

            if (duplicate_number(curr_node))
                continue;

            if (last_digit_check(curr_node, last_digits))
                continue;

            if (!solution_found && curr_node->data.size() == distinct_letters.size() && sum_check(curr_node, words))//if all checks are satisfied get result
            {
                result = curr_node->data;
                solution_found = true;

                return;
            }

            for (int i = 0; i <=9; i++)
                q.enqueue(curr_node->children[i]);
        }
    }
}

void Tree::printResult(string algorithm, string filename, float time)//print in required format
{
    cout << "Algorithm: " << algorithm << endl;
    cout << "Number of the visited nodes: " << n_visited_nodes << endl;
    cout << "Maximum number of nodes kept in the memory: " << max_n_nodes << endl;
    cout << "Running time: "  << time << " seconds" << endl;

    if (solution_found)
    {
        cout << "Solution: ";
        for (int i = 0; i < result.size(); i++)
        {
            cout << result[i].first << ": " << result[i].second;
            if (i != result.size() - 1)
                cout << ", ";
        }
        cout << endl;
        writeToFile(filename);
    }
    else cout << "No solution found" << endl;
}

void Tree::writeToFile(string filename)//write to file if result is found
{
    ofstream out(filename);

    out << "\t0\t1\t2\t3\t4\t5\t6\t7\t8\t9\n";
    for (auto const &p : result)
    {
        out << (char)tolower(p.first);
        for (int i = 0; i <= 9; i++)
        {
            if (i == p.second) out << "\t1";
            else out << "\t.";
        }
        out << endl;
    }
    out.close();
}

int main(int argc, char** argv)
{
    if (argc != 6)
    {
        cerr << "Enter Algorithm Addend Augend Sum Outfilename as arguments!" << endl;
        return 1;
    }

    string algorithm = argv[1];
    string words[3] = {argv[2], argv[3], argv[4]};
    string filename = argv[5];

    if (algorithm != "DFS" && algorithm != "BFS") 
    {
        cout << "Enter either BFS or DFS as algorithm!" << endl;
        return 1;
    }

    Tree tree(words);

    auto start_timer = chrono::high_resolution_clock::now();
    if (algorithm == "BFS") tree.BFS(tree.getRoot());
    else tree.DFS(tree.getRoot(), 0);
    auto stop_timer = chrono::high_resolution_clock::now();
    float time_in_s = chrono::duration_cast<chrono::microseconds>(stop_timer - start_timer).count() / 1000000.f;

    tree.printResult(algorithm, filename, time_in_s);
    return 0;
}
