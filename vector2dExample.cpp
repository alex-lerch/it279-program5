// Quick example of setting up a 2d vector when can't initialize at declaration
// A vector of lists would work similarly
// Mary Elaine Califf
#include <vector>
#include <iostream>
using namespace std;

int main()
{
    vector<vector <int>> myMatrix; // declared empty

    // suppose I now decide I want a 4x4 matrix
    for (int i = 0; i < 4; i++)
        myMatrix.push_back(vector<int>(4, 1));

    // now let's print our matrix
    for (int i = 0; i < myMatrix.size(); i++)
    {
        for (int j = 0; j < myMatrix[i].size(); j++) {
            cout << " " << myMatrix[i][j];
        }
        cout << endl;
    }

}