/*
* EECS 348 Assignment 6
* Sudoku Solver 
* Inputs: puzzle1.txt, puzzle2.txt, puzzle3.txt, puzzle4.txt, and puzzle5.txt
* Output: All possible solutions and number of solutions to each puzzle
* Author: John Vitha
* KU ID: 3158626
* Creation Date: 10/29/2025
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

/*
* Initialize a struct containing the row and column of the best candidate and and a vector of candidates.
*/
struct Cell {
    int row; // Initialize row
    int col; // Initialize column
    vector<int> cands; // Initialize candidates
    Cell() : row(-1), col(-1), cands({}) {} // Initialize values for empty Cell
};

/*
* Read the file into a vector of int vectors representing a sudoku board.
*/
vector<vector<int>> read_file(ifstream &file){
    string cell_val; // Initialize value of a cell
    vector<vector<int>> sudoku_board(9, vector<int>(9)); // Initialize an empty sudoku board
    for (int i = 0; i < 9; i++){ // Loop through each row
        for(int j = 0; j < 9; j++){ // Loop through each column
            file >> cell_val; // Read cell value from file
            if(cell_val == "_"){ // If empty space
                sudoku_board[i][j] = 0; // Store in board as 0
            } else { // If not empty space
                sudoku_board[i][j] = stoi(cell_val); // Store in board as integer
            }
        }
    }
    return sudoku_board; // Return the sudoku board
}

/*
* Print a sudoku board in a human-readable format.
*/
void print_board(const vector<vector<int>>& sudoku_board){
    for(int i = 0; i < 9; i++){ // Loop through each row
        for(int j = 0; j < 9; j++){ // Loop through each column
            cout << sudoku_board[i][j] << " "; // Print each cell in row i followed by a space
        }
        cout << endl; // Next line
    }
}

/*
* Take a board and row as parameters and return every taken value in the row. 
*/
vector<int> row_vals(const vector<vector<int>>& sudoku_board, int row){
    vector<int> vals; // Initialize an empty vector of taken row values
    for(int i = 0; i < 9; i++){ // Loop through each column
        if(sudoku_board[row][i] != 0){ // If the cell is not empty
            vals.push_back(sudoku_board[row][i]); // Add the value to the vector
        }
    }
    return vals; // Return a vector of taken row values
}

/*
* Take a board and column as parameters and return every taken value in the column. 
*/
vector<int> col_vals(const vector<vector<int>>& sudoku_board, int col){
    vector<int> vals; // Initialize an empty vector of taken column values
    for(int i = 0; i < 9; i++){ // Loop through each row
        if(sudoku_board[i][col] != 0){ // If the cell is not empty
            vals.push_back(sudoku_board[i][col]); // Add the value to the vector
        }
    }
    return vals; // Return a vector of taken column values
}

vector<int> three_by_three_vals(const vector<vector<int>>& sudoku_board, int row, int col){
    vector<int> vals; // Initialize an empty vector of taken 3x3 values
    int row0 = row / 3 * 3; // Calculate index of first row in 3x3
    int col0 = col / 3 * 3; // Calculate index of first column 3x3
    for(int i = row0; i < row0+3; i++){ // Loop through each row
        for(int j = col0; j < col0+3; j++){ // Loop through each column
            if(sudoku_board[i][j] != 0){ // If the cell is not empty
                vals.push_back(sudoku_board[i][j]); // Add the value to the vector
            }
        }
    }
    return vals; // Return a vector of taken 3x3 values
}

vector<int> get_candidates(const vector<int>& rowVals, const vector<int>& colVals, const vector<int>& boxVals) {
    vector<int> taken; // Initialize an empty vector of taken values

    for(int val : rowVals) if(find(taken.begin(), taken.end(), val) == taken.end()) taken.push_back(val); // Get taken row values
    for(int val : colVals) if(find(taken.begin(), taken.end(), val) == taken.end()) taken.push_back(val); // Get taken column values, no repeats
    for(int val : boxVals) if(find(taken.begin(), taken.end(), val) == taken.end()) taken.push_back(val); // Get taken 3x3 values, no repeats

    vector<int> cands; // Initialize an empty vector of candidates
    for(int i = 1; i <= 9; i++){ // Loop through each row
        if(find(taken.begin(), taken.end(), i) == taken.end()){ // If not taken
            cands.push_back(i); // Add value to candidates vector
        }
    }
    return cands; // Return candidates vector
}

/*
* Take a board as a parameter and return the next candidates to be checked.
*/
Cell find_next(const vector<vector<int>>& board){
    Cell best; // Initialize an empty best candidates struct
    for(int i = 0; i < 9; i++){ // Loop through each row
        for(int j = 0; j < 9; j++){ // Loop through each column
            if(board[i][j]  == 0){ // If cell is empty
                vector<int> cands = get_candidates(row_vals(board, i), col_vals(board, j), three_by_three_vals(board, i, j)); // Get next candidate
                if(cands.empty()){ // If there are no candidates
                    return Cell(); // Dead end, return
                }
                if((best.row == -1) || cands.size() < best.cands.size()){ // If this space is more constrained
                    best.row = i; // Update best row
                    best.col = j; // Update best column
                    best.cands = cands; // Update best candidates
                    if(best.cands.size() == 1){ // If only one candidate
                        return best; // No other options, return the candidate to be checked
                    }
                }
            }
        }
    }
    return best; // Return the best candidate to check
}

void solve(vector<vector<int>>& board, vector<vector<vector<int>>>& solutions){
    Cell spot = find_next(board); // Get next most contrained empty space
    if(spot.row == -1){ // If there are no empty spaces
        solutions.push_back(board); // Store the solution
        return; // Done with this branch
    }
    if(spot.cands.empty()){ // If there are no candidates
        return; // Backtrack
    }
    for(size_t i = 0; i < spot.cands.size(); i++){ // Loop through each candidate
        board[spot.row][spot.col] = spot.cands[int(i)]; // Place the value in the space
        solve(board, solutions); // Recurse with this placement
        board[spot.row][spot.col] = 0; // Backtrack
    }
}

/*
* 
*/
int main(){
    ifstream file; // Initialize file

    for(int i = 1; i < 6; i++){ // Loop through each file
        string filename = "puzzle" + to_string(i) + ".txt"; // Build filename
        file.open(filename); // Open the file
        if(!file.is_open()) { // If file is not open
            cerr << "Could not open " << filename << endl; // Tell the user
            continue; // Try reading next file
        }
        vector<vector<int>> sudoku_board = read_file(file); // Read file into vector of vectors board
        vector<vector<vector<int>>> solutions; // Vector of boards
        solve(sudoku_board, solutions); // Recursively call solve and store solutions
        if(solutions.empty()){ // If no solutions found
            cout << "No solution found." << endl; // Tell the user
        } else{ // If solutions found
            for(size_t j = 0; j < solutions.size(); j++){ // Loop through solutions vector
                print_board(solutions[int(j)]); // Output solution
                cout << endl; // Next line
            }
            cout << "Solutions found: " << solutions.size() << endl << endl; // Output number of solutions found
        } 
        file.close(); // Close file
    }
}