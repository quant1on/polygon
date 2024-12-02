#include <iostream>
#include <string>
#include <vector>
#include <stack>
using namespace std;

int RectangleArea(int height[], int n){
    pair<int, int> limits[n];
    stack<int> dp;
    int i = 0, max_area = 0;

    while (i < n){
        if (dp.empty() || height[dp.top()] <= height[i]){
            dp.push(i);
            i++;
        }
        else{
            limits[dp.top()].second = i - 1;
            dp.pop();
        }
    }

    while (!dp.empty()){
        limits[dp.top()].second = n - 1;
        dp.pop();
    }

    i--;

    while (i >= 0){
        if (dp.empty() || height[dp.top()] <= height[i]){
            dp.push(i);
            i--;
        }
        else{
            limits[dp.top()].first = i + 1;
            dp.pop();
        }
    }

    while (!dp.empty()){
        limits[dp.top()].first = 0;
        dp.pop();
    }

    int area;
    for (int j = 0; j < n; j++){
        area = height[j] * (limits[j].second - limits[j].first + 1);
        max_area = max(max_area, area);
    }

    return max_area;
}



int maximalRectangle(vector<vector<char>>& matrix){
    int clmns = matrix[0].size(), rows = matrix.size(), ans = 0;

    int height[clmns];
    for (int i = 0; i < clmns; i++) height[i] = 0;

    for (int i = 0; i < rows; i++){
        
        for (int j = 0; j < clmns; j++){
            if (matrix[i][j] == '0') height[j] = 0;
            else height[j]++;
        }

        ans = max(ans, RectangleArea(height, clmns));
    }

    return ans;
}


int main(){
    vector<vector<char>> test;
    test = {
        {'1', '0', '1', '0', '0'},
        {'1', '0', '1', '1', '1'},
        {'1', '1', '1', '1', '1'},
        {'1', '0', '0', '1', '0'}
    };

    cout << maximalRectangle(test);
}