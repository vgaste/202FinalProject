/*
Name: M08FinalProject.cpp
Author: Valeria Gastelum
Last Updated: 05/08/25
*/

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <unordered_set>
#include <ctime>
using namespace std;

random_device rd;
mt19937 global_rng(rd());

void displayBoard(const vector<vector<int>>& board)
{
    for(int i = 0; i < 9; i++)
    {
        if(i % 3 == 0 && i != 0)
        {
            cout<<"------+-------+------"<<endl;
        }
        for(int j = 0; j < 9; ++j)
        {
            cout<<(board[i][j] ? to_string(board[i][j]): ".")<<" ";
            if((j + 1) % 3 == 0 && j != 0)
            {
                cout<<"| ";
            }
        }
        cout<<endl;
    }
}

void solveSudoku(vector<vector<int>>& board);
bool generatePuzzle(vector<vector<int>>& board, int difficulty);
bool getValidInput(int& value);

void mainMenu()
{
    vector<vector<int>> board(9, vector<int>(9, 0));
    int choice;

    while(true)
    {
        cout<<"Main Menu:"<<endl;
        cout<<"1. Sudoku Solver"<<endl;
        cout<<"2. Sudoku Generator"<<endl;
        cout<<"3. Exit"<<endl;
        cout<<"Choose an option (1-3): ";
        cin>>choice;

        while(cin.fail() || choice < 1 || choice > 3)
        {
            if(cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout<<"Invalid choice, try again (1-3): ";
            cin>>choice;
        }

        switch(choice)
        {
            case 1:
            cout<<"Enter the Sudoku puzzle (9x9, use 0 for empty spaces):"<<endl;
            for(int i = 0; i < 9; ++i)
            {
                string line;
                cout<<"Enter row "<<i + 1<<" (9 digits, use 0 for empty): ";
                cin>>line;
                
                while(line.length() != 9 || any_of(line.begin(), line.end(), [](char c){ return !isdigit(c) || c < '0' || c > '9'; }))
                {
                    cout << "Invalid input. Enter exactly 9 digits (0–9) for row " << i + 1 << ": ";
                    cin >> line;
                }
                for(int j = 0;j < 9; ++j)
                {
                    board[i][j] = line[j] - '0';   
                }
            }
            solveSudoku(board);
            break;

            case 2:
            int difficulty;
            cout<<"Select difficulty"<<endl;
            cout<<"1. Easy"<<endl;
            cout<<"2. Medium"<<endl;
            cout<<"3. Hard"<<endl;
            cout<<"Choose an option (1-3): ";
            cin>>difficulty;
            while(cin.fail() || difficulty < 1 || difficulty > 3)
            {
                if(cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                cout<<"Invalid choice, try again (1-3): ";
                cin>>difficulty;
            }
            if(generatePuzzle(board, difficulty))
            {  
                if(difficulty == 1) cout<<"Easy";
                else if(difficulty == 2) cout<<"Medium";
                else cout<<"Hard";
                cout<<" Sudoku generated: "<<endl;
                displayBoard(board);
            }
            else
            {
                cout<<"Error in puzzle generation."<<endl;
            }
            break;

            case 3:
            cout<<"Exiting program"<<endl;
            return;

            default:
            cout<<"Error"<<endl;
            break;
        }
    }
}

bool getValidInput(int& value)
{
    while(true)
    {
        cin>>value;
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"Invalid input. Enter number between 0 and 9: ";
        }
        else if(value < 0 || value > 9)
        {
            cout<<"Invalid number. Enter number between 0 and 9: ";
        }
        else
        {
            break;
        }
    }
    return true;
}

bool isValid(const vector<vector<int>>& board, int row, int col, int num)
{
    for(int i = 0; i < 9; i++)
    {
        if(board[row][i] == num || board[i][col] == num)
        {
            return false;
        }
    }

    int sRow = row - row % 3;
    int sCol = col - col % 3;

    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j < 3; ++j)
        {
            if(board[sRow+i][sCol+j] == num)
            {
                return false;
            }
        }
    }
    return true;
}

bool fillBoard(vector<vector<int>>& board)
{
    for(int row = 0; row < 9; ++row)
    {
        for(int col = 0; col < 9; ++col)
        {
            if(board[row][col] == 0)
            {

                vector<int> numbers{1, 2, 3, 4, 5, 6, 7, 8, 9};
                shuffle(numbers.begin(), numbers.end(), global_rng);

                for(int num: numbers)
                {
                    if(isValid(board, row, col, num))
                    {
                        board[row][col] = num;
                        if(fillBoard(board))
                        {
                            return true;
                        }
                        board[row][col] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

bool solveSudokuHelper(vector<vector<int>>& board, int& sCount)
{
    for(int row = 0; row < 9; ++row)
    {
        for(int col = 0; col < 9; ++col)
        {
            if(board[row][col] == 0)
            {
                for(int num = 1; num <= 9; ++num)
                {
                    if(isValid(board, row, col, num))
                    {
                        board[row][col] = num;
                        if(solveSudokuHelper(board, sCount))
                        {
                            if(sCount > 1) return false;
                        }

                        board[row][col] = 0;
                    }
                }
                return false;
            }
        }
    }
    sCount++;
    return true;
}

bool generatePuzzle(vector<vector<int>>& board, int difficulty)
{
    int cellsToErase = 0;
    if(difficulty == 1) cellsToErase = 30;
    else if(difficulty == 2) cellsToErase = 40;
    else if(difficulty == 3) cellsToErase = 50;

    int attempts = 1500;
    if(difficulty == 2) attempts = 3000;
    else if(difficulty == 3) attempts = 5000;

    int maxRetries = 5;
    while(maxRetries-- > 0)
    {
        board = vector<vector<int>>(9, vector<int>(9,0));
        fillBoard(board);

        int erased = 0;
        int remainingAttempts = attempts;
        vector<vector<int>> tempBoard(9, vector<int>(9));

        uniform_int_distribution<> dis(0, 8);

        while(erased < cellsToErase && remainingAttempts > 0)
        {
            int row = dis(global_rng);
            int col =  dis(global_rng);

            if(board[row][col] == 0) continue;

            int backup = board[row][col];
            board[row][col] = 0;
            tempBoard = board;

            int sCount = 0;
            bool isSolved = solveSudokuHelper(tempBoard, sCount);

            if(sCount == 1) ++erased;
            else board[row][col] = backup;
            --remainingAttempts;
        }
        if(erased == cellsToErase) return true;
    }
    return false;
}

bool solveBoard(vector<vector<int>>& board)
{
    for(int row = 0; row < 9; ++row)
    {
        for(int col = 0; col < 9; ++col)
        {
            if(board[row][col] == 0)
            {
                for(int num = 1; num <= 9; ++num)
                {
                    if(isValid(board, row, col, num))
                    {
                        board[row][col] = num;
                        if(solveBoard(board)) return true;
                        board[row][col] = 0;
                    }
                }

                return false;
            }
        }
    }
    return true;
}

void solveSudoku(vector<vector<int>>& board)
{
    vector<vector<int>> tempBoard = board;
    int sCount = 0;
    solveSudokuHelper(tempBoard, sCount);

    if(solveBoard(board))
    {
        cout<<"Solved sudoku:"<<endl;
        displayBoard(board);

        if(sCount > 1)
        {
            cout << "Note: This sudoku has multiple solutions, this is one of them.\n";
        }
    }
    else
    {
        cout<<"No solution found"<<endl;
    }
}

int main()
{
    mainMenu();
    return 0;
}
