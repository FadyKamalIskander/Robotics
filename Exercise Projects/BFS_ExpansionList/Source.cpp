#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm>

using namespace std;

class Map
{
public:
    const static int mapWidth = 6;
    const static int mapHeight = 5;
    vector<vector<int>> grid = {
        { 0, 1, 0, 0, 0, 0 },
        { 0, 1, 0, 0, 0, 0 },
        { 0, 1, 0, 0, 0, 0 },
        { 0, 1, 0, 0, 0, 0 },
        { 0, 0, 0, 1, 1, 0 } };
};

class Planner : Map
{
public:
    vector<int> start{ 0, 0 };
    vector<int> goal{ mapHeight - 1, mapWidth - 1 };
    int cost{ 1 };
    vector<vector<int>> movements{ {-1,0}, {0,-1}, {1,0}, {0,1} };
    vector<string> movements_arrows{ "^", "<" , "v", ">"};
};

template <class T>
void print2DVector(vector<vector<T>> input)
{
    for (auto x : input)
    {
        for (auto y : x)
        {
            cout << y << " ";
        }
        cout << endl;
    } 
}

/*#### TODO: Code the search function which will generate the expansion list ####*/

void search(Map map, Planner planner)
{
    int currExp = 0;
    int currO = 0;
    vector<pair<int, int>> openList = { {0,0} };
    vector<pair<int, int>> rejectionList = { {0,0} };
    vector<int> cellCost = {0};
    vector<vector<int>> expansionVector (map.mapHeight, vector<int>(map.mapWidth, -1));
    vector<vector<string>> policyVector (map.mapHeight, vector<string>(map.mapWidth, "-"));

    while (!openList.empty())
    {
        auto currOpenCell = openList[currO];
        int currCost = cellCost[currO];
        expansionVector[currOpenCell.first][currOpenCell.second] = currExp;
        currExp++;
        bool isNewItemAdded = false;

        for (int i = currOpenCell.first - 1; (i <= currOpenCell.first + 1); i++)
        {
            for (int j = currOpenCell.second - 1; (j <= currOpenCell.second + 1); j++)
            {
                if (((i == currOpenCell.first - 1) || (i == currOpenCell.first + 1)) && ((j == currOpenCell.second - 1) || (j == currOpenCell.second + 1))) continue;
                if ((i < map.mapHeight) && (i >= 0) && (j < map.mapWidth) && (j >= 0) && (map.grid[i][j] != 1))
                {
                    pair<int, int> curr = {i, j};
                    if (!(std::find(openList.begin(), openList.end(), curr) != openList.end()) && !(std::find(rejectionList.begin(), rejectionList.end(), curr) != rejectionList.end()))
                    {
                        openList.push_back(curr);
                        cellCost.push_back(currCost + 1);
                        isNewItemAdded = true;

                    }
                }
            }
        }
        if (isNewItemAdded || (openList.size() > 1))
        {
            auto ir = find(openList.begin(), openList.end(), currOpenCell);
            auto index = ir - openList.begin();
            openList.erase(ir);
            cellCost.erase(cellCost.begin() + index);
            rejectionList.push_back(currOpenCell);
        }
        expansionVector[openList[currO].first][openList[currO].second] = currExp;
        if (openList[currO].first - currOpenCell.first == 1)
        {
            policyVector[currOpenCell.first][currOpenCell.second] = "v";
        }
        else if (currOpenCell.first - openList[currO].first == 1)
        {
            policyVector[currOpenCell.first][currOpenCell.second] = "^";
        }
        else if (openList[currO].second - currOpenCell.second == 1)
        {
            policyVector[currOpenCell.first][currOpenCell.second] = ">";
        }
        else if (currOpenCell.second - openList[currO].second == 1)
        {
            policyVector[currOpenCell.first][currOpenCell.second] = "<";
        }
        if ((openList[currO].first == planner.goal[0]) && (openList[currO].second == planner.goal[1]))
        {
            cout << "[" << cellCost[currO] << ", " << openList[currO].first << ", " << openList[currO].second << "]" << endl;
            for (int i = 0; i < expansionVector.size(); i++)
            {
                auto z = expansionVector[i];
                for (int j = 0; j < z.size(); j++)
                {
                    cout << z[j] << " ";
                }
                cout << endl;
            }
            for (int i = 0; i < policyVector.size(); i++)
            {
                auto z = policyVector[i];
                for (int j = 0; j < z.size(); j++)
                {
                    cout << z[j] << " ";
                }
                cout << endl;
            }



            break;
        }
    }
}

/*############ Don't modify the main function############*/
int main()
{
    // Instantiate map and planner objects
    Map map;
    Planner planner;

    // Search for the expansions
    search(map, planner);

    return 0;
}
