#pragma once
#include <iostream>
#include <cstdlib> // For rand() and RAND_MAX
#include <ctime>   // For time()
using namespace std;

class GameGrid {
public:
    static const int ROWS = 13;
    static const int COLS = 15;
    char grid[ROWS][COLS]; // 2D array to represent the grid

public:
    // Constructor to initialize the grid with empty characters
    GameGrid() {
        for (int i = 0; i < ROWS; ++i) 
        {
            for (int j = 0; j < COLS; ++j) 
            {
                grid[i][j] = '0'; // '0' represents an empty cell
            }
        }

        for (int i = 1; i < ROWS-1; ++i) {
            for (int j = 1; j < COLS-1; ++j) 
            {
                if ((i % 2 == 0 && j % 2 == 0))
                        grid[i][j] = 'S'; // '0' represents an empty cell
                    
            }
        }

        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                if(i==0 || j==0 || i==ROWS-1 || j ==COLS-1)
                    grid[i][j] = 'S'; // '0' represents an empty cell
            }
        }

        for (int i = 0; i < 25; ) { 
            int j = 1 + rand() % ROWS; 
            int m = 1 + rand() % COLS; 
            if (m < COLS && j < ROWS && grid[j][m] == '0' && m!=0 && j!=0 && m!=1&& j!=1) 
            {
                cout << "j: " << j << endl << "m: " << m << endl << endl;

                grid[j][m] = 'W'; 
                i++; 
            }
        }



    }

    // Method to display the grid
    void display() const {
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                cout << grid[i][j] << "   ";
            }
            cout << endl<<endl;
        }
    }

    // Method to place an item at a specific position
    void placeItem(int row, int col, char item) {
        if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
            grid[row][col] = item;
        }
        else {
            cout << "Position out of bounds!" << endl;
        }
    }

    // Method to clear the grid
    void clear() {
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                grid[i][j] = '.'; // Reset to empty cell
            }
        }
    }

    int getValue(int x, int y)
    {
        return grid[x][y];
    }
};