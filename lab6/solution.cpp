#include <iostream>
#include <string>
#include <vector>
using namespace std;

int maximalRectangle(vector<vector<char>>& matrix){
    int clmns = matrix[0].size(), rows = matrix.size(), ans = 0;
    vector<int> height(clmns, 0);
    for (int i = 0; i < matrix.size(); i++){
        for (int j = 0; j < rows; j++){
            if (matrix[i][j] == 0) height[j] = 0;
            else height[j]++;
        }

        ans = max(ans, RectangleArea(height));
    }

    return ans;
}

int RectangleArea(vector<int> &height){
    
}