/*
Christopher Browning
COMP 3310 - Artifical Intelligene
Programming Project 1 - Peg Puzzle
Program generates a modal of a pegp uzzle, and accepts user input to determine which peg to remove
once peg is removed, program then interates recursivly through a depth first search to find the solution path
once path is found, prgram displays path, nodes generated, and maximum branching factor.
*/



#include <iostream>
#include <cctype> 

void displayPyramid (char pyramidArray [][9]); //Takes a 2d array, and properly formats and displays it to the screen
void buildPyramid (char pyramidArray [][9]); //Handles actually CONSTRUCTING the pyramid initially
void pegSelection (char pyramidArray[][9]); //allows the user to input a cordinate to select which peg will be removed to start the "game"
void searchIteration (char array[15], bool &solutionFound); //the actually search function
bool solutionCheck(char array[15]); //checks to see if the puzzle has been solved		
void makeMoves(char array[15], int pegToMove, int spaceToMoveTo, int pegToRemove);//Only checks to see if a move is valid and performs it.	
void undoMove(char array[15], int pegToMove, int spaceToMoveTo, int pegToRemove);
bool validMove(char array[15], int pegToMove, int spaceToMoveTo, int pegToRemove, int &localBranchingFactor);//checks to see if a move is valid
void loadIntoPath(char array[15]); //takes the current depth and the currently created array to store the array for later recall once goal state is found
void displayToWorkable(char displayArray[][9], char workingArray[15]); //translates the dispalyable array into a workable array
void workableToDisplay(char displayArray[][9], char workingArray[15]);//translates the workable array into a displayable one
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int nodesGenerated = 0; //as stated made global for simplicity
int maxBranchingFactor = 0;	//as stated made global for simplicity
int pathDepth = 0; //tracking for path depth, made global for simplicity
char pathArray[14][15]; //a 2d array, each row of the array is a stage of the path taken to the goal state

int main () //main sets up preliminary values and then interacts with the user
{
	std::cout << "GNU Terry Prachett \n \n \n \n"; //This is staying, I will explain it if asked and am willing to take a grade cut.
	int totalNodeCount = 0;
	char pegSelectedConfirmation = 'a';
	char displayArray [5][9];
	char intialWorkingArray[15];
	char endPause;
	bool solutionFound = false;
	for(int x = 0 ; x <= 12; x++)
		for(int y = 0; y <= 14; y++)
			pathArray[x][y] = ' ';
	while (pegSelectedConfirmation != 'y') //Loops until a valid selection is made
	{
		buildPyramid(displayArray); //inital build of pyramid
		displayPyramid(displayArray); //inital display of pyramid
		pegSelection(displayArray); //peg selection
		displayPyramid(displayArray); //redisplay of pyramid for peg selction confirmation
		std::cout << "Confirm Peg Selection Y/N: ";
		std::cin >> pegSelectedConfirmation;
		pegSelectedConfirmation = std::tolower(pegSelectedConfirmation);
	}
	displayToWorkable(displayArray, intialWorkingArray);
	loadIntoPath(intialWorkingArray);
	searchIteration(intialWorkingArray, solutionFound);
	if(solutionFound)
	{
		std::cout << "Solution found! \n";
		std::cout << "Nodes generated: " << nodesGenerated;
		std::cout << "\n Max branching factor: " << maxBranchingFactor;
		std::cout << "\n";
	}
	for(int y = 0 ; y < 14; y++)
	{
		for(int x = 0; x < 15; x++)
		{
			intialWorkingArray[x] = pathArray[y][x];
		}
		workableToDisplay(displayArray,intialWorkingArray);
		displayPyramid(displayArray);
	}
	std::cin >> endPause;
	return 0;
}

void displayPyramid (char pyramidArray [][9]) //displys the pyramid with proper formating
{
	for(int y = 0; y <= 4; y++)
	{
		for(int x = 0; x <= 8; x++)
			std::cout << pyramidArray[y][x];
		std::cout << "\n";
		//std::cout << "TEST"; //DEBUGGING
	}

}

void buildPyramid (char pyramidArray [][9]) //Prebuilds the pyramid with Xs
{
	for(int x = 0; x <= 4; x++)
		for(int y = 0; y <= 8; y++)
			pyramidArray[x][y] = ' ';
	pyramidArray[0][4] = 'X';
	pyramidArray[1][3] = 'X';
	pyramidArray[1][5] = 'X';
	pyramidArray[2][2] = 'X';
	pyramidArray[2][4] = 'X';
	pyramidArray[2][6] = 'X';
	pyramidArray[3][1] = 'X';
	pyramidArray[3][3] = 'X';
	pyramidArray[3][5] = 'X';
	pyramidArray[3][7] = 'X';	
	pyramidArray[4][0] = 'X';
	pyramidArray[4][2] = 'X';
	pyramidArray[4][4] = 'X';
	pyramidArray[4][6] = 'X';
	pyramidArray[4][8] = 'X';
}

void pegSelection (char pyramidArray[][9]) //This massive wall of if statments makes sure the user selects a valid peg to be removed and then removes it.
{
	int xCoordinate = 10;
	int yCoordinate = 10;
	bool validSelection = false;
	while(validSelection == false)
 	{
 	std::cout << "Hello user, please select a peg to remove to continue....otherwise we will just sit here....\n";
	std::cout << "Enter the X-Coordinate: ";
	std::cin >> xCoordinate;
	std::cout << "Thanks, and the Y: ";
 	std::cin >> yCoordinate;
 		if (yCoordinate <= 5 && xCoordinate <= 5)
 		{
 			if (yCoordinate == 1)
 			{
 				if (xCoordinate == 1)
 				{
 					pyramidArray[0][4] = 'O';
 					validSelection = true;
 				}
 				else
 				{
 					std::cout << "Invalid Selection";
 				}
 			}
 			else if (yCoordinate == 2)
 			{
 				if (xCoordinate == 1)
 				{
 					pyramidArray[1][3] = 'O';
 					validSelection = true;
 				}
 				else if (xCoordinate == 2)
 				{
 					pyramidArray[1][5] = 'O';
 					validSelection = true;
 				}
 				else
 				{
 					std::cout << "Invalid Selection";
 				}
 			}
 			else if (yCoordinate == 3)
 			{
 				if (xCoordinate == 1)
 				{
 					pyramidArray[2][2] = 'O';
 					validSelection = true;
 				} 
 				else if (xCoordinate == 2)
 				{
 					pyramidArray[2][4] = 'O';
 					validSelection = true;
 				}
 				else if (xCoordinate == 3)
 				{
 					pyramidArray[2][6] = 'O';
 					validSelection = true;
 				}
 				else
 				{
 					std::cout << "I entered the part";
 					std::cout << "Invalid Selection";
 				}
 			}
 			else if (yCoordinate == 4)
 			{
 				if (xCoordinate == 1)
 				{
 					pyramidArray[3][1] = 'O';
 					validSelection = true;
 				}
 				else if (xCoordinate == 2)
 				{
 					pyramidArray[3][3] = 'O';
 					validSelection = true;
 				}
 				else if (xCoordinate == 3)
 				{
 					pyramidArray[3][5] = 'O';
 					validSelection = true;
 				}
 				else if (xCoordinate == 4)
 				{
 					pyramidArray[3][7] = 'O';
 					validSelection = true;
 				}
 				else
 				{
 					std::cout << "Invalid Selection";
 				}
 			}
 			else if (yCoordinate == 5)
 			{
 				if (xCoordinate == 1)
 				{
 					pyramidArray[4][0] = 'O';
 					validSelection = true;
 				}
 				else if (xCoordinate == 2)
 				{
 					pyramidArray[4][2] = 'O';
 					validSelection = true;
 				}
 				else if (xCoordinate == 3)
 				{
 					pyramidArray[4][4] = 'O';
 					validSelection = true;
 				}
 				else if (xCoordinate == 4)
 				{
 					pyramidArray[4][6] = 'O';
 					validSelection = true;
 				}
 				else if (xCoordinate == 5)
 				{
 					pyramidArray[4][8] = 'O';
 					validSelection = true;
 				}
 			}

 		}
 		else
 		{
 			std::cout << "Invalid Selection! Please enter an X value between 1 and 9, and a Y value between 1 and 5";
 		}
 		
 	}
}

void searchIteration (char array[15], bool &solutionFound) //the actual recursvie funtion, uses a local copy of the array and branching factor, combined with the funtion "validMove" to pregress its way through the search space
{
	//std::cout << "I have iterated! \n"; //debugging
	int localBranchingFactor = 0; //used to help determine the max branching factor
	solutionFound = solutionCheck(array); //checking for a solution BEFORE creating nodes 
	if(!solutionFound)
	{
		char localChangeableArray[15];//local copy of the state, so when the funtion changes it, it is self contained.
		for(int x = 0; x <15; x++) //filling hte local copy with the correct information
		{
			localChangeableArray[x] = array[x];
		}
		loadIntoPath(localChangeableArray); //the actual "storing" of the current state in the 2d array.
		pathDepth++; //used to determine where to store the state in the 2d array, increased here to move "down" one level in the 2d array, for each successive recursion
		
		if(validMove(array,0,3,1,localBranchingFactor) && !solutionFound)//each of these if statements, takes a legal move from the game, and checks to see if it is possible, or is the solution has been found yet
		{
			makeMoves(localChangeableArray,0,3,1); //if it is, we make the move
			searchIteration(localChangeableArray, solutionFound);//recurse
			undoMove(localChangeableArray,0,3,1);//then come back and undo the move, for the next if statement
		}
		if(validMove(array,0,5,2,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,0,5,2);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,0,5,2);
		}
		if(validMove(array,1,6,3,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,1,6,3);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,1,6,3);
		}
		if(validMove(array,1,8,4,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,1,8,4);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,1,8,4);
		}
		if(validMove(array,2,7,4,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,2,7,4);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,2,7,4);
		}
		if(validMove(array,2,9,5,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,2,9,5);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,2,9,5);
		}

		if(validMove(array,3,10,6,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,3,10,6);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,3,10,6);
		}
		if(validMove(array,3,12,7,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,3,12,7);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,3,12,7);
		}
		if(validMove(array,3,5,4,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,3,5,4);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,3,5,4);
		}
		if(validMove(array,3,0,1,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,3,0,1);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,3,0,1);
		}
		if(validMove(array,4,11,7,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,4,11,7);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,4,11,7);
		}
		if(validMove(array,4,13,8,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,4,13,8);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,4,13,8);
		}
		if(validMove(array,5,12,8,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,5,12,8);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,5,12,8);
		}
		if(validMove(array,5,14,9,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,5,14,9);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,5,14,9);
		}
		if(validMove(array,5,0,2,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,5,0,2);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,5,0,2);
		}
		if(validMove(array,5,3,4,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,5,3,4);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,5,3,4);
		}
		if(validMove(array,6,8,7,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,6,8,7);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,6,8,7);
		}
		if(validMove(array,6,1,3,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,6,1,3);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,6,1,3);
		}
		if(validMove(array,7,9,8,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,7,9,8);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,7,9,8);
		}
		if(validMove(array,7,2,4,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,7,2,4);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,7,2,4);
		}
		if(validMove(array,8,6,7,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,8,6,7);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,8,6,7);
		}
		if(validMove(array,8,1,4,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,8,1,4);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,8,1,4);
		}
		if(validMove(array,9,2,5,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,9,2,5);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,9,2,5);
		}
		if(validMove(array,9,7,8,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,9,7,8);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,9,7,8);
		}
		if(validMove(array,10,12,11,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,10,12,11);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,10,12,11);
		}
		if(validMove(array,10,3,6,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,10,3,6);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,10,3,6);
		}
		if(validMove(array,11,13,12,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,11,13,12);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,11,13,12);
		}
		if(validMove(array,11,4,7,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,11,4,7);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,11,4,7);
		}
		if(validMove(array,12,14,13,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,12,14,13);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,12,14,13);
		}
		if(validMove(array,12,5,8,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,12,5,8);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,12,5,8);
		}
		if(validMove(array,12,3,7,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,12,3,7);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,12,3,7);
		}
		if(validMove(array,12,10,11,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,12,10,11);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,12,10,11);
		}
		if(validMove(array,13,4,8,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,13,4,8);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,13,4,8);
		}
		if(validMove(array,13,11,12,localBranchingFactor) && !solutionFound)	
		{
			makeMoves(localChangeableArray,13,11,12);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,13,11,12);
		}
		if(validMove(array,14,5,9,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,14,5,9);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,14,5,9);
		}
		if(validMove(array,14,12,13,localBranchingFactor) && !solutionFound)
		{
			makeMoves(localChangeableArray,14,12,13);
			searchIteration(localChangeableArray, solutionFound);
			undoMove(localChangeableArray,14,12,13);
		}
		if (maxBranchingFactor < localBranchingFactor) //the local branching factor has been handed to each "validMove" function that has been run, and iterated as valid moves are found, if the new branching factor found is higher then previous ones, we update the global varible
		{
			maxBranchingFactor = localBranchingFactor; //updating the global varible
		}
	}
	pathDepth--;//used to determine where to store the state in the 2d array, decreased here to move "up" one level in the 2d array so when we get out of this iteration of the recursion, the state is stored properly

}

 
bool solutionCheck(char array[15])// All this function does is check for the goal state.
{
 	int xCount = 0;
 		for (int x = 0; x<=15; x++) //loops through the state, looking for Xs
	{
		if(array[x] == 'X')
		{
			xCount++;
		}
	}
	if(xCount == 1) //if it finds EXACTLY one x, the puzzle is solved, and it loads the final state into the 2d array, as the recursions will back out and not hit another "save point"
	{
		std::cout << "Solution Found";
		loadIntoPath(array);
		
		return true;
	}
	else
	{
		return false;
	}
}

void makeMoves(char array[15], int pegToMove, int spaceToMoveTo, int pegToRemove) //All this function does is check to make sure the move is valid, and if it is performs it, modifying the array it was handed.
{
 	array[spaceToMoveTo] = 'X';
 	array[pegToRemove] = 'O';
 	array[pegToMove] = 'O';
}
void undoMove(char array[15], int pegToMove, int spaceToMoveTo, int pegToRemove) //undos previously made move to allow me to use only once copy of an array per iteration...literally just reversing the above function because I am lazy
{
 	array[spaceToMoveTo] = 'O';
 	array[pegToRemove] = 'X';
 	array[pegToMove] = 'X';
}

bool validMove(char array[15], int pegToMove, int spaceToMoveTo, int pegToRemove, int &localBranchingFactor)//checks to see if a move is valid, currently being used to determine branching factor and node generation
{

	if (array[spaceToMoveTo] == 'O' && array[pegToMove] == 'X' && array[pegToRemove] == 'X')
	{
		nodesGenerated++;
		localBranchingFactor++;
		//std::cout << "This is a valid move \n"; //debugging
	}
	return array[spaceToMoveTo] == 'O' && array[pegToMove] == 'X' && array[pegToRemove] == 'X';
}

void loadIntoPath(char array[15]) //loads a working array into the path storage array, will overwrite previous stored if needed
{
	for(int x = 0; x <= 14; x++)
	{
		pathArray[pathDepth][x] = array[x]; //uses the global varible pathdepth to determine on which "level" to store the path it was handed
	}
}

void displayToWorkable(char displayArray[][9], char workingArray[15]) //translaes a dispaly array to a working array
{
	workingArray[0] = displayArray[0][4];
	workingArray[1] = displayArray[1][3];
	workingArray[2] = displayArray[1][5];
	workingArray[3] = displayArray[2][2];
	workingArray[4] = displayArray[2][4];
	workingArray[5] = displayArray[2][6];
	workingArray[6] = displayArray[3][1];
	workingArray[7] = displayArray[3][3];
	workingArray[8] = displayArray[3][5];
	workingArray[9] = displayArray[3][7];
	workingArray[10] = displayArray[4][0];
	workingArray[11] = displayArray[4][2];
	workingArray[12] = displayArray[4][4];
	workingArray[13] = displayArray[4][6];
	workingArray[14] = displayArray[4][8];

	//for (int x = 0; x<= 14; x++)
	//	std::cout << workingArray[x] << ' ';
	//debugging
		
}

void workableToDisplay(char displayArray[][9], char workingArray[15])//translates a working array into a dispalyable one
{
	displayArray[0][4] = workingArray[0];
	displayArray[1][3] = workingArray[1];
	displayArray[1][5] = workingArray[2];
	displayArray[2][2] = workingArray[3];
	displayArray[2][4] = workingArray[4];
	displayArray[2][6] = workingArray[5];
	displayArray[3][1] = workingArray[6];
	displayArray[3][3] = workingArray[7];
	displayArray[3][5] = workingArray[8];
	displayArray[3][7] = workingArray[9];
	displayArray[4][0] = workingArray[10];
	displayArray[4][2] = workingArray[11];
	displayArray[4][4] = workingArray[12];
	displayArray[4][6] = workingArray[13];
	displayArray[4][8] = workingArray[14];
}