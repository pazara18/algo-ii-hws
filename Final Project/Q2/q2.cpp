#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <limits>

using namespace std;

struct Point{
    int xCoordinate;
    int yCoordinate;
};

bool compareX(const Point& a, const Point& b)
{
    return a.xCoordinate > b.xCoordinate;
}

bool compareY(const Point& a, const Point& b)
{
    return a.yCoordinate > b.yCoordinate;
}

class PointSpace{
    int numberOfPoints;
    vector<Point> points;

public:
    void setNumberOfPoints(int n){
        numberOfPoints = n;
    }
    
    int getNumberOfPoints(){
        return numberOfPoints;
    }
    
    void addPoint(int x, int y){
        Point p = Point();
        p.xCoordinate = x;
        p.yCoordinate = y;
        
        points.push_back(p);
    }
     
    void printNumberOfPoints(){
        cout << "Total number of points: " << getNumberOfPoints() << endl;
    }
    
    void printAllPoints(){
        cout << "Points coordinates (x y): " << endl;
        
        for (std::vector<Point>::const_iterator i = points.begin(); i != points.end(); ++i){
            cout << i->xCoordinate << "\t" << i->yCoordinate << endl;
        }
    }
    
    float distance(Point p1, Point p2)
    {
        return sqrt((p1.xCoordinate - p2.xCoordinate)*(p1.xCoordinate - p2.xCoordinate) + (p1.yCoordinate - p2.yCoordinate)*(p1.yCoordinate - p2.yCoordinate));
    }

    float pairwiseDistances(vector<Point> P)
    {
        float min = numeric_limits<float>::max();
        for (unsigned int i = 0; i < P.size(); i++)
            for (unsigned int j = i+1; j < P.size(); j++)
                if (distance(P[i], P[j]) < min)
                    min = distance(P[i], P[j]);
        return min;
    }

    float min(float x, float y)
    {
        return (x < y) ? x : y;
    }

    float stripDistance(vector<Point> strip, float d)
    {
        float min = d;
        sort(strip.begin(), strip.end(), compareY);
    
        for (unsigned int i = 0; i < strip.size(); i++)
            for (unsigned int j = i+1; j < strip.size() && (strip[j].yCoordinate - strip[i].yCoordinate) < min; j++)
                if (distance(strip[i],strip[j]) < min)
                    min = distance(strip[i], strip[j]);
    
        return min;
    }
    
    float closestRecursive(vector<Point> P)
    {
        if (P.size() <= 3)
            return pairwiseDistances(P);
    
        int mid = P.size()/2;
        Point middlePoint = P[mid];

        vector<Point> Pleft;
        vector<Point> Pright;
        for(int i = 0; i < mid; i++)
            Pleft.push_back(P[i]);
        for(unsigned int i = mid; i < P.size(); i++)
            Pright.push_back(P[i]);

        float dl = closestRecursive(Pleft);
        float dr = closestRecursive(Pright);
    
        float d = min(dl, dr);
    
        vector<Point> strip;
        for (unsigned int i = 0; i < P.size(); i++)
            if (abs(P[i].xCoordinate - middlePoint.xCoordinate) < d)
                strip.push_back(P[i]);

        return min(d, stripDistance(strip,d));
    }

    double FindClosestPairdistance(){
        sort(points.begin(), points.end(), compareX);
        return closestRecursive(points);
    }
};

int main(int argc, char* argv[]){
    //define a point space
    PointSpace pointSpace;

    //get file name
    string inputFileName = argv[1];
    
    string line;
    ifstream infile(inputFileName);

    //read the number of total points (first line)
    getline(infile, line);
    pointSpace.setNumberOfPoints(stoi(line));

    //read points' coordinates and assign each point to the space (second to last line)
    int x, y;
    while (infile >> x >> y){
       pointSpace.addPoint(x, y);
    }
    
    //print details of point space (not necessary for assignment evaluation: calico will not check this part)
    pointSpace.printAllPoints();
    pointSpace.printNumberOfPoints();
    
    //find and print the distance between closest pair of points (calico will check this part)
    double closestdistance = pointSpace.FindClosestPairdistance();
    cout << "Distance between the closest points: " << closestdistance << endl;
    
    return 0;
}



