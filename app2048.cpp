#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include <vector>
#include <algorithm>

using namespace std;
int gridSize = 4;
bool gameOver;

    //Print out the gamegrid to console
void printGameBoard(int grid[4][4]){
    for(int x=0; x<gridSize; x++){
        for(int y = 0; y<gridSize; y++){
            cout << to_string(grid[x][y]) +"\t";
            //Newline for each row
            if(y==gridSize-1){
                cout << endl;
            }  
        }
    }
}

void insertNewTile(int grid[4][4]){
    //Initalise empty vectors to store all empty tile co-ords in
    vector<int> rowIndex;
    vector<int> colIndex;
    int numEmptyTiles = 0;
    for(int i=0; i<gridSize; i++){
        for(int j=0; j<gridSize; j++){
            if(grid[i][j] == 0){
                rowIndex.push_back(i);
                colIndex.push_back(j);
                numEmptyTiles++;
            } 
        }
    }
    if(numEmptyTiles==0){
        gameOver = true;
        cout << "~~~~ Game Over Man, Game Over! ~~~~\n";
        exit(1);
    }
    //The default value to insert into the grid
    int val = 2;
    //10% chance of inserting a 4 rather that a 2
    int cutOff = int(rand() % 11 + 0);
    if(cutOff == 1){
        val = 4; 
    } 
    //Get a random co-ords from the vectors
    int randIndex = int(rand() % numEmptyTiles + 0);  
    //Insert either the val (2 or 4) into the selected co-ord 
    grid[rowIndex[randIndex]][colIndex[randIndex]] = val;   
}   

// Merges a single row of all adjacent like tiles (starting from the left)
// and merges titles with either a 0 between them or when they are directly adjacent 
void mergeRow(int row[4], char direction){
    int newRow[4] = {0,0,0,0};
  //If right, then flip the array and then perfom normal leftsided merge on it
    if(direction == 'R'){
        for(int low = 0, high = gridSize-1; low < high; low++, high--){
            swap(row[low],row[high]);
            }
    }
    //Get all tiles that should merge together
    for(int i=0; i<gridSize-1;i++){
        //If the element next to the current element is the same
        if(row[i] == row[i+1]){
            //Double the current element
            row[i] *= 2;
            //Set the next element to 0
            row[i+1] =0;
        //If there is a 0 between two like elements (i.e 2_0_2)
        }else if(i < 2 && row[i] == row[i + 2] && row[i + 1] == 0){
             //Double the current element
            row[i] *= 2;
            //Set the tile one element over to 0
            row[i + 2] = 0;
        //Finally if there is two 0's between two like elements
        }else if(i < 1 && row[i] == row[i + 3] && row[i + 1] == 0 && row[i + 2] == 0){
            //Double the current element
            row[i] *= 2;
            //Set the tile two elements away to 0
            row[i+3] = 0;
        }
    }
    int a = 0;
    //Populate a row with the expected outcome in L to R order
    for(int i=0; i<gridSize; i++) {
      if (row[i] != 0) {
        newRow[a] = row[i];
        a++;
      }
    }
    //Assign the new tiles to the gameGrid
    if(direction == 'L'){
        for (int i = 0; i < gridSize; i++){ 
            row[i] = newRow[i];
        }
    //If right, then insert the values in reverse order, conisiding with the array flip above.
    }else if(direction == 'R'){
        for (int i = 0; i < gridSize; i++){ 
            row[gridSize-1-i] = newRow[i];
        }
    }
} 

//Merge each row either left or right
void mergeRowsHorizontally(int grid[4][4], char direction){
    if(direction == 'L'){
        for(int i=0; i<gridSize;i++){
            mergeRow(grid[i], direction);
        }
    }else if(direction = 'R'){
        for(int i=0; i<gridSize;i++){
            mergeRow(grid[i], direction);
        }
    }
}

void mergeRowsVertically(int grid[4][4], char direction){
        int col0[gridSize]{0};
        int col1[gridSize]{0};
        int col2[gridSize]{0};
        int col3[gridSize]{0};
        //take a copy of each column and store it as a row
        for(int i=0; i<gridSize; i++){
            col0[i] = grid[i][0];
            col1[i] = grid[i][1];
            col2[i] = grid[i][2];
            col3[i] = grid[i][3];
        }
    if(direction == 'U'){
        //Merge the rows left
        mergeRow(col0, 'L');
        mergeRow(col1, 'L');
        mergeRow(col2, 'L');
        mergeRow(col3, 'L');
    }else if(direction == 'D'){
        //Merge the rows right
        mergeRow(col0, 'R');
        mergeRow(col1, 'R');
        mergeRow(col2, 'R');
        mergeRow(col3, 'R');
    }
    //Replace the grid with the new merge cols
        for (int i = 0; i < 4; i++) {
            grid[i][0] = col0[i];
            grid[i][1] = col1[i];
            grid[i][2] = col2[i];
            grid[i][3] = col3[i];
        }
}

//Return True if the key pressed results in a valid move.
//I.e the gameboard is different after the tiles have been shifted in the chosen direction occurs.
bool isValidMove(int grid[4][4], char direction){
    int gridCopy[4][4]{0};
    for(int i=0; i<gridSize; i++){
        for(int j=0; j<gridSize; j++){
            gridCopy[i][j] = grid[i][j];
        }
    }
    if(direction == 'U' || direction == 'D'){
        //Preview the chosen move on the copy 
        mergeRowsVertically(gridCopy, direction);
    }else if(direction == 'L' || direction == 'R'){
        //Preview the chosen move on the copy 
        mergeRowsHorizontally(gridCopy, direction);
    }
    cout << endl;
    //If the copy is not the same as the original, return true
    for(int i=0; i<gridSize; i++){
        for(int j=0; j<gridSize; j++){
            if(gridCopy[i][j] != grid[i][j]){
                return true;
            }
        }
    }
    //Otherwise the output is the same as the original
    cout << "~ Invalid Move ~\n";
    return false;
    }

int main(){
    //gameOver used to infinitly loop while a move can be played
    gameOver = false;
    //Set the size of both columns and rows of the gameboard
    static int gridSize = 4;
    //Matrix to keep track of the gameboard and initialize all elements to 0
     int grid[4][4]{0};
     for(int x=0; x<gridSize; x++){
        for(int y = 0; y<gridSize; y++){
            grid[x][y] = 0;
        }
     }
    //insert the initial game tile
    insertNewTile(grid);
    //Print the game header and the game grid 
    cout << "\n~~~~~~~~~ 2048 Game ~~~~~~~~~~\n\n";
    printGameBoard(grid);
    //Infinite loop until the end state is reached
    while(gameOver == false){
       //Take a copy of the gameboard before an arrow is pushed
        int gridCheck[4][4];
        for (int i = 0; i < gridSize-1; i++) {
            for (int j = 0; j < gridSize-1; j++) {
                gridCheck[i][j] = grid[i][j];
             }
        }   
       //Check which key has been pushed
        char keyPressed;
        //while(!kbhit()){}
        keyPressed = getch();
        if(keyPressed==0 || keyPressed==-32 || keyPressed==27){
            keyPressed=getch();
            //If the down arrow key is pressed shift all tiles down
            if(keyPressed==80){
                if(isValidMove(grid, 'D')){ 
                    cout << "\nDown arrow pressed.\n"<<endl;
                    mergeRowsVertically(grid, 'D');
                    insertNewTile(grid);
                    printGameBoard(grid);
                }
            }
            //If the up down key is pressed shift all tiles up
            else if(keyPressed==72){
                if(isValidMove(grid, 'U')){
                    cout << "\nUp arrow pressed.\n"<<endl;
                    mergeRowsVertically(grid, 'U');
                    insertNewTile(grid);
                    printGameBoard(grid);
                }
            }
            //If the left arrow key is pressed shift all tiles left
            else if(keyPressed==75){
                if(isValidMove(grid, 'L')){
                    cout << "\nLeft arrow pressed.\n"<<endl;
                    mergeRowsHorizontally(grid, 'L');
                    insertNewTile(grid);
                    printGameBoard(grid);
                }
            }        
            //If the up right key is pressed shift all tiles right
            else if(keyPressed==77){
                if(isValidMove(grid, 'R')){
                    cout << "\nRight arrow pressed.\n"<<endl;
                    mergeRowsHorizontally(grid, 'R');
                    insertNewTile(grid);
                    printGameBoard(grid);
                }
            //Else if Escape is hit, exit the game
            }else{
                cout << "\nExiting Game\n";
                gameOver = true;
                exit(0);
            }

        }
    }//Close GameOver while loop
}