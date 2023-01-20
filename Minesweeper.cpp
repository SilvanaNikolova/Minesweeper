/**
*
* Solution to course project #7
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2022/2023
*
* @author Silvana Nikolova
* @idnumber 0MI0600152 @compiler VC
*
* 
*
*/

#include <iostream>;
using namespace std;

int MINESCOUNT;

void freeArr(char** arr, int size)
{
	for (size_t i = 0; i < size; i++)
	{
		delete[] arr[i];
	}

	delete[] arr;
}

char** initializeArr(int size)
{
	char** arr = new char* [size];

	//create array
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			arr[j] = new char[size];
		}
	}

	//fill default array
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			arr[i][j] = '-';
		}
	}

	return arr;
}

bool isValidInput(int size, int minesCount)
{
	return size >= 3 && size <= 10 && minesCount >= 1 && minesCount <= size * 3;
}

int myStrcmp(const char* s1, const char* s2) {
	while (*s1 && (*s1 == *s2)) {
		s1++, s2++;
	}
	return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int isValidCommand(const char* cmd)
{
	if (myStrcmp(cmd, "open") == 0)
	{
		return 1;
	}
	if (myStrcmp(cmd, "mark") == 0)
	{
		return 2;
	}
	if (myStrcmp(cmd, "unmark") == 0)
	{
		return 3;
	}

	return 0;
}

void printArr(char** arr, int size)
{
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			cout << arr[i][j];
		}

		cout << endl;
	}
}

int convertToNumber(const char* arr)
{
	int i = 0;
	int num = 0;

	while (arr[i] != '\0')
	{
		if (arr[i] < 48 || arr[i] > 57)
		{
			return -1;
		}
		num = num * 10 + arr[i] - '0';

		i++;
	}

	return num;
}

bool isValidNumber(int x, int y)
{
	return x >= 0 && y >= 0;
}

bool isValidRowAndCol(int row, int col, int size)
{
	return (row >= 0) && (row < size)
		&& (col >= 0) && (col < size);
}

bool isMine(int row, int col, char** arr)
{
	return arr[row][col] == '*';
}

void placeMines(char** myBoard, int size)
{
	for (size_t i = 0; i < MINESCOUNT;)
	{
		srand(time(0));
		int random = rand() % (size * size);
		int x = random / size;
		int y = random % size;

		if (myBoard[x][y] != '*')
		{
			myBoard[x][y] = '*';
			i++;
		}
	}
}

int countNeighbourMines(char** myBoard, int row, int col, int size)
{
	/*
	Count all the mines in the 8 adjacent
	cells

		N.W   N   N.E
		  \   |   /
		   \  |  /
		W----Cell----E
			 / | \
		   /   |  \
		S.W    S   S.E

	Cell-->Current Cell (row, col)
	N -->  North        (row-1, col)
	S -->  South        (row+1, col)
	E -->  East         (row, col+1)
	W -->  West            (row, col-1)
	N.E--> North-East   (row-1, col+1)
	N.W--> North-West   (row-1, col-1)
	S.E--> South-East   (row+1, col+1)
	S.W--> South-West   (row+1, col-1)
	*/
	int count = 0;

	if (isValidRowAndCol(row - 1, col, size) && isMine(row - 1, col, myBoard))
	{
		count++;
	}

	if (isValidRowAndCol(row + 1, col, size) && isMine(row + 1, col, myBoard))
	{
		count++;
	}

	if (isValidRowAndCol(row, col + 1, size) && isMine(row, col + 1, myBoard))
	{
		count++;
	}

	if (isValidRowAndCol(row, col - 1, size) && isMine(row, col - 1, myBoard))
	{
		count++;
	}

	if (isValidRowAndCol(row - 1, col + 1, size) && isMine(row - 1, col + 1, myBoard))
	{
		count++;
	}

	if (isValidRowAndCol(row - 1, col - 1, size) && isMine(row - 1, col - 1, myBoard))
	{
		count++;
	}

	if (isValidRowAndCol(row + 1, col + 1, size) && isMine(row + 1, col + 1, myBoard))
	{
		count++;
	}

	if (isValidRowAndCol(row + 1, col - 1, size) && isMine(row + 1, col - 1, myBoard))
	{
		count++;
	}

	return count;
}

bool isMatchingMines(char** playboard, char** myBoard, int size)
{
	bool isMatching = true;
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			if (myBoard[i][j] == '*' && playboard[i][j] != '*')
			{
				isMatching = false;
				break;
			}

			if (myBoard[i][j] != '*' && playboard[i][j] == '*')
			{
				isMatching = false;
				break;
			}
		}

		if (!isMatching)
		{
			return false;
		}
	}

	return true;
}

bool openCommand(int row, int col, char** playboard, char** myBoard, int size)
{
	//return true if the player opens a mine and loses the game
	if (myBoard[row][col] == '*')
	{
		playboard[row][col] = '*';
		return true;
	}

	//get count of neighbour mines and display it on the board
	int count = countNeighbourMines(myBoard, row, col, size);
	playboard[row][col] = count + '0';

	bool wMines, nwMines, nMines, neMines, eMines, seMines, sMines, swMines;
	wMines = nwMines = nMines = neMines = eMines = seMines = sMines = swMines = false;

	//the while loop is to open all the cells before hitting a mine
	while (!wMines && !nwMines && !nMines && !neMines && !eMines && !seMines && !sMines && !swMines)
	{
		if (isValidRowAndCol(row, col - 1, size))
		{
			if (isMine(row, col - 1, myBoard))
			{
				break;
			}
			count = countNeighbourMines(myBoard, row, col - 1, size);
			if (count != 0)
			{
				wMines = true;
			}

			playboard[row][col - 1] = count + '0';

			if (count == 0)
			{
				int temp = col;
				while (count == 0)
				{
					temp--;

					if (temp <= 0 || isMine(row, temp, myBoard))
					{
						break;
					}

					count = countNeighbourMines(myBoard, row, temp - 1, size);
					playboard[row][temp - 1] = count + '0';
				}

				wMines = true;
			}
		}

		if (isValidRowAndCol(row - 1, col - 1, size))
		{
			if (isMine(row - 1, col - 1, myBoard))
			{
				break;
			}
			count = countNeighbourMines(myBoard, row - 1, col - 1, size);
			if (count != 0)
			{
				nwMines = true;
			}

			playboard[row - 1][col - 1] = count + '0';

			if (count == 0)
			{
				int tempRow = row;
				int tempCol = col;

				while (count == 0)
				{
					tempCol--;
					tempRow--;

					if (tempCol <= 0 || tempRow <= 0 || isMine(tempRow, tempCol, myBoard))
					{
						break;
					}

					count = countNeighbourMines(myBoard, tempRow - 1, tempCol - 1, size);
					playboard[tempRow - 1][tempCol - 1] = count + '0';
				}

				nwMines = true;
			}
		}

		if (isValidRowAndCol(row - 1, col, size))
		{
			if (isMine(row - 1, col, myBoard))
			{
				break;
			}
			count = countNeighbourMines(myBoard, row - 1, col, size);
			if (count != 0)
			{
				nMines = true;
			}

			playboard[row - 1][col] = count + '0';

			if (count == 0)
			{
				int tempRow = row;

				while (count == 0)
				{
					tempRow--;

					if (tempRow <= 0 || isMine(tempRow, col, myBoard))
					{
						break;
					}

					count = countNeighbourMines(myBoard, tempRow - 1, col, size);
					playboard[tempRow - 1][col] = count + '0';
				}

				nMines = true;
			}
		}

		if (isValidRowAndCol(row - 1, col + 1, size))
		{
			if (isMine(row - 1, col + 1, myBoard))
			{
				break;
			}
			count = countNeighbourMines(myBoard, row - 1, col + 1, size);
			if (count != 0)
			{
				neMines = true;
			}

			playboard[row - 1][col + 1] = count + '0';

			if (count == 0)
			{
				int tempRow = row;
				int tempCol = col;

				while (count == 0)
				{
					tempRow--;
					tempCol++;

					if (tempRow <= 0 || tempCol == size - 1 || isMine(tempRow, tempCol, myBoard))
					{
						break;
					}

					count = countNeighbourMines(myBoard, tempRow - 1, tempCol + 1, size);
					playboard[tempRow - 1][tempCol + 1] = count + '0';
				}

				neMines = true;
			}
		}

		if (isValidRowAndCol(row, col + 1, size))
		{
			if (isMine(row, col + 1, myBoard))
			{
				break;
			}
			count = countNeighbourMines(myBoard, row, col + 1, size);
			if (count != 0)
			{
				eMines = true;
			}

			playboard[row][col + 1] = count + '0';

			if (count == 0)
			{
				int tempCol = col;

				while (count == 0)
				{
					tempCol++;

					if (tempCol == size - 1 || isMine(row, tempCol, myBoard))
					{
						break;
					}

					count = countNeighbourMines(myBoard, row, tempCol + 1, size);
					playboard[row][tempCol + 1] = count + '0';
				}

				eMines = true;
			}
		}

		if (isValidRowAndCol(row + 1, col + 1, size))
		{
			if (isMine(row + 1, col + 1, myBoard))
			{
				break;
			}
			count = countNeighbourMines(myBoard, row + 1, col + 1, size);
			if (count != 0)
			{
				seMines = true;
			}

			playboard[row + 1][col + 1] = count + '0';

			if (count == 0)
			{
				int tempRow = row;
				int tempCol = col;

				while (count == 0)
				{
					tempRow++;
					tempCol++;

					if (tempRow == size - 1 || tempCol == size - 1 || isMine(tempRow, tempCol, myBoard))
					{
						break;
					}

					count = countNeighbourMines(myBoard, tempRow + 1, tempCol + 1, size);
					playboard[tempRow + 1][tempCol + 1] = count + '0';
				}

				seMines = true;
			}
		}

		if (isValidRowAndCol(row + 1, col, size))
		{
			if (isMine(row + 1, col, myBoard))
			{
				break;
			}
			count = countNeighbourMines(myBoard, row + 1, col, size);
			if (count != 0)
			{
				sMines = true;
			}

			playboard[row + 1][col] = count + '0';

			if (count == 0)
			{
				int tempRow = row;

				while (count == 0)
				{
					tempRow++;

					if (tempRow == size - 1 || isMine(tempRow, col, myBoard))
					{
						break;
					}

					count = countNeighbourMines(myBoard, tempRow + 1, col, size);
					playboard[tempRow + 1][col] = count + '0';
				}

				sMines = true;
			}
		}

		if (isValidRowAndCol(row + 1, col - 1, size))
		{
			if (isMine(row + 1, col - 1, myBoard))
			{
				break;
			}
			count = countNeighbourMines(myBoard, row + 1, col - 1, size);
			if (count != 0)
			{
				swMines = true;
			}
			playboard[row + 1][col - 1] = count + '0';

			if (count == 0)
			{
				int tempRow = row;
				int tempCol = col;

				while (count == 0)
				{
					tempRow++;
					tempCol--;

					if (tempRow == size - 1 || tempCol <= 0 || isMine(tempRow, tempCol, myBoard))
					{
						break;
					}

					count = countNeighbourMines(myBoard, tempRow + 1, tempCol - 1, size);
					playboard[tempRow + 1][tempCol - 1] = count + '0';
				}

				swMines = true;
			}
		}
	}

	return false;
}

bool markCommand(char** playboard, char** myBoard, int row, int col, int size)
{
	playboard[row][col] = '*';

	//this function checks if the player guessed all places of the mines
	return isMatchingMines(playboard, myBoard, size);
}

bool unmarkCommand(char** playboard, char** myBoard, int row, int col, int size)
{
	playboard[row][col] = '-';

	//this function checks if the player guessed all places of the mines
	return isMatchingMines(playboard, myBoard, size);
}

int playMinesweer(char** playboard, char** myBoard, int size)
{
	char command[100];
	char x[100], y[100];
	cout << "Enter input -> (command -> 'open', 'mark' and 'unmark') (row) (col)" << endl;
	cin >> command >> x >> y;

	//commandIndex = 1 -> open
	//commandIndex = 2 -> mark
	//commandIndex = 3 -> unmark
	//commandIndex = 0 -> invalid
	int commandIndex = isValidCommand(command);

	if (commandIndex == 0)
	{
		cout << "The command is invalid! Command can be 'open', 'mark' and 'unmark'. Please enter your input again!" << endl;
		playMinesweer(playboard, myBoard, size);
	}

	int row = convertToNumber(x);
	int col = convertToNumber(y);

	if (!isValidNumber(row, col))
	{
		cout << "The row/col is invalid! Please enter your input again!" << endl;
		playMinesweer(playboard, myBoard, size);
	}

	if (!isValidRowAndCol(row, col, size))
	{
		cout << "The row/col is invalid! Please enter your input again!" << endl;
		playMinesweer(playboard, myBoard, size);
	}

	if (commandIndex == 1)
	{
		if (playboard[row][col] != '-')
		{
			cout << "Invalid position! Please try again!" << endl;
			playMinesweer(playboard, myBoard, size);
		}

		//the funcion returns false if the player opens a mine
		bool isLosing = openCommand(row, col, playboard, myBoard, size);

		if (isLosing)
		{
			cout << "You lose!" << endl;
			cout << "Final state of the field: " << endl;
			printArr(playboard, size);
			return 0;
		}

		cout << "Your current state is: " << endl;
		printArr(playboard, size);

		playMinesweer(playboard, myBoard, size);
	}

	else if (commandIndex == 2)
	{
		if (playboard[row][col] != '-')
		{
			cout << "Invalid position! Please try again!" << endl;
			playMinesweer(playboard, myBoard, size);
		}

		//the funcion returns true if the player gueses the places of the mines
		bool isWinning = markCommand(playboard, myBoard, row, col, size);

		if (isWinning)
		{
			cout << "You won!" << endl;
			cout << "Final state of the field: " << endl;
			printArr(playboard, size);
			return 0;
		}

		cout << "Your current state is: " << endl;
		printArr(playboard, size);

		playMinesweer(playboard, myBoard, size);
	}

	else if (commandIndex == 3)
	{
		if (playboard[row][col] != '*')
		{
			cout << "Invalid position! Please try again!" << endl;
			playMinesweer(playboard, myBoard, size);
		}

		//the function returns true if the player guessed all the mines
		bool isWInning = unmarkCommand(playboard, myBoard, row, col, size);

		if (isWInning)
		{
			cout << "You won!" << endl;
			cout << "Final state of the field: " << endl;
			printArr(playboard, size);
			return 0;
		}

		cout << "Your current state is: " << endl;
		printArr(playboard, size);

		playMinesweer(playboard, myBoard, size);
	}
}

int main()
{
	int size, minesCount;
	cout << "Enter size of the field and count of mines." << endl << "Size: (from 3 to 10)" << endl;
	cin >> size;
	cout << "Count of mines: (from 1 to " << 3 * size << ')' << endl;
	cin >> minesCount;

	bool isValid = isValidInput(size, minesCount);

	while (!isValid)
	{
		cout << "Please enter valid data! Size should be bethween 3 and 10 and count should be between 1 and " << 3 * size << ')' << endl;
		cout << "Size: (from 3 to 10)" << endl;
		cin >> size;
		cout << "Count of mines: (from 1 to 3 * size)" << endl;
		cin >> minesCount;

		isValid = isValidInput(size, minesCount);
	}

	MINESCOUNT = minesCount;

	char** myBoard = initializeArr(size);
	char** playboard = initializeArr(size);

	placeMines(myBoard, size);

	cout << "Start of the game: " << endl;
	printArr(playboard, size);
	playMinesweer(playboard, myBoard, size);

	freeArr(myBoard, size);
	freeArr(playboard, size);
}
