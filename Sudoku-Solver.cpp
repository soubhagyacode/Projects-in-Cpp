#include <bits/stdc++.h>
using namespace std;

#define vv vector<vector<int>>
#define vvc vector<vector<char>>
#define vvs vector<vector<string>>
#define v vector<int>
#define vc vector<char>
#define vs vector<string>

class Solution {
public:
    // Helper arrays to mark the presence of numbers in rows, columns, and sub-grids
    int row[9][10], col[9][10], sub[9][10];

    bool rec(vvc &board,int x,int y){
        if(x == 9 && y == 0)
            return true;

        if(board[x][y] != '.'){
            if(y+1 == 9){
                y=0;x++;
            }
            else y++;

            if(rec(board,x,y))
                return true;
        }
        else{
            for(int num = 1;num<=9;num++){
                int s = ((x/3) * 3) + (y/3);
                if(!row[x][num] && !col[y][num] && !sub[s][num]){
                    row[x][num] = col[y][num] = sub[s][num] = 1;
                    board[x][y] = '0' + num;
                    int new_x,new_y;
                    if(y+1 == 9){
                        new_y=0;new_x = x+1;
                    }
                    else {
                        new_y = y+1;
                        new_x = x;
                    }
                    if(rec(board,new_x,new_y)) return true;
                    board[x][y] = '.';
                    row[x][num] = col[y][num] = sub[s][num] = 0;
                }
            }
        }

        return false;
    }
    void solveSudoku(vector<vector<char>>& board) {
        for(int i=0;i<9;i++){
            for(int j=0;j<9;j++){
                if(board[i][j] != '.'){
                    int num = board[i][j] - '0';
                    int s = ((i/3) * 3) + (j/3);
                    row[i][num] = col[j][num] = sub[s][num] = 1;
                }
            }
        }

        bool t = rec(board,0,0);
        if(!t){
            cout<<"Invalid Sudoku Board"<<endl; exit(0);
        }
    }
};

class Check{
public:
    bool isValidSudoku(vector<vector<char>>& board) {
        // Helper arrays to mark the presence of numbers in rows, columns, and sub-grids
        int row[9][9] = {0}, col[9][9] = {0}, sub[9][9] = {0};

        // Iterate over each cell in the 9x9 Sudoku board
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (board[i][j] != '.') {
                    int num = board[i][j] - '1';  // Convert character to an index (0-8)
                    int subGridIndex = (i / 3) * 3 + (j / 3);  // Calculate sub-grid index

                    // Check for duplicates in the row, column, and sub-grid
                    if (row[i][num] || col[j][num] || sub[subGridIndex][num]) {
                        return false;  // Invalid if duplicate found
                    }

                    // Mark the number as seen in the row, column, and sub-grid
                    row[i][num] = col[j][num] = sub[subGridIndex][num] = 1;
                }
            }
        }
        return true;  // Valid if no duplicates found
    }
};

main()
{
    cout<<"Input your sudoku board, use \".\" in empty places "<<endl;
    vector<vector<char>> board(9, vector<char>(9));
    for(int i=0;i<9;i++)
        for(int j=0;j<9;j++)
            cin>>board[i][j];
        cout<<endl;

    Check c;
    bool b = c.isValidSudoku(board); //Check if the given Sudoku board is Valid if not then code exits.
    if(!b)
    {
        cout<<"Terminated due to inValid Sudoku table"<<endl;
        exit(0);
    }


    Solution s;
    s.solveSudoku(board);

    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            cout<<board[i][j]<<" ";
            }
        cout<<endl;
    }
}