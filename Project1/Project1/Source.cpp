#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm> //swap, max
#include <math.h> //abs
#include <tuple> //tuple

using namespace std;

// custom compare for my priority queue
class compareh
{
public:
	bool operator()(tuple<int, int, vector<vector<int>>> matrix1, tuple<int, int, vector<vector<int>>> matrix2)
	{
		return get<0>(matrix1) + get<1>(matrix1) > get<0>(matrix2) + get<1>(matrix2);
	}
};

void general_search(vector< vector<int> > matrix, int choice_num);
int manhat(vector<vector<int>> node);
int misplace(vector<vector<int>> node);
bool goalcheck(vector< vector<int> > matrix);
void expand(tuple<int, int, vector<vector<int>>> node, priority_queue< tuple<int, int, vector<vector<int>>>, vector<tuple<int, int, vector<vector<int>>>>, compareh > &nodes, int choice_num);

int total;
int node11, node12, node13, node21, node22, node23, node31, node32, node33;

void general_search(vector< vector<int> > matrix, int choice_num)
{
	int h_n = 0;

	if (choice_num == 2)
	{
		h_n = misplace(matrix);
	}
	else if (choice_num == 3)
	{
		h_n = manhat(matrix);
	}

	// creates initial node
	tuple< int, int, vector<vector<int>> > node(h_n, 0, matrix);

	// create queue storing matrix
	priority_queue< tuple<int, int, vector<vector<int>>>, vector<tuple<int, int, vector<vector<int>>>>, compareh > nodes;

	// puts node in queue
	nodes.push(node);

	// Output to txt file
	ofstream myfile;
	myfile.open("out.txt", std::ios_base::app);


	// header information
	if (choice_num == 2)
	{
		myfile << "Algorithm: A*" << endl;
		myfile << "Heuristic function: CountingTilesOutOfPlace()" << endl;
	}

	if (choice_num == 3)
	{
		myfile << endl;
		myfile << "Algorithm: A*" << endl;
		myfile << "Heuristic function: DistanceOfTilesOutOfPlace()" << endl;

	}

	tuple<int, int, vector<vector<int>>> temp = nodes.top();
	vector<vector<int>> temp1 = get<2>(temp);
	
	myfile << endl;
	myfile << "Initial state" << endl;
	myfile << "===============" << endl;

	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
			myfile << temp1[y][x] << " ";
		myfile << endl;
	}

	int counter = -1;
	int max_size = 0;
	int attempts = 1;
	int maxAttempts = 200;

	while (counter != 0 && counter != 1)
	{
		// if queue is empty and solution has not been found
		if (nodes.empty())
		{
			myfile << "Failure: No solution found" << endl;

			counter = 0;
			return;
		}

		if (attempts == maxAttempts)
		{
			myfile << endl;
			myfile << "Unable to find a solution" << endl;

			counter = 0;
			return;
		}

		temp = nodes.top();
		get<2>(node) = get<2>(temp);

		nodes.pop();

		// if goal state has been found
		if (goalcheck(get<2>(node)) == true)
		{
			myfile << endl;
			myfile << "Goal reached!" << endl;
			temp1 = get<2>(node);

			myfile << "Number of steps: " << attempts-1 << endl;
			myfile << "--------------------------------------------------" << endl;
			counter = 1;

			return;
		}

		expand(temp, nodes, choice_num);

		int curr_size = nodes.size();

		max_size = max(max_size, curr_size);
		temp = nodes.top();
		temp1 = get<2>(temp);

		myfile << endl;
		myfile << "state: " << attempts << endl;
		myfile << "==========" << endl;

		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
				myfile << temp1[y][x] << " ";
			myfile << endl;
		}
	
		// increment attempt counter
		attempts += 1;
	}

	// Close out.txt file
	myfile.close();
}

// ================================================
// Target state
// ================================================
bool goalcheck(vector< vector<int> > matrix)
{
	vector< vector<int> > goal;

	int num_col = 3;
	int num_row = 3;
	int init_val = 0;

	goal.resize(num_col, vector<int>(num_row, init_val));

	goal[0][0] = node11;
	goal[0][1] = node12;
	goal[0][2] = node13;
	goal[1][0] = node21;
	goal[1][1] = node22;
	goal[1][2] = node23;
	goal[2][0] = node31;
	goal[2][1] = node32;
	goal[2][2] = node33;

	if (matrix == goal)
	{
		return true;
	}
	else if (matrix != goal)
	{
		return false;
	}
}

void expand(tuple<int, int, vector<vector<int>>> node, priority_queue< tuple<int, int, vector<vector<int>>>, vector<tuple<int, int, vector<vector<int>>>>, compareh > &nodes, int choice_num)
{
	// find the blank position
	vector<vector<int>> mat = get<2>(node);

	int blank_x = -1;
	int blank_y = -1;

	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			if (mat[y][x] == 0)
			{
				blank_x = x;
				blank_y = y;
			}
		}
	}

	// ================================================
	// move up
	// ================================================
	vector<vector<int>> copy = mat;

	int h_n = 0;

	tuple< int, int, vector<vector<int>> > new_node;

	if (blank_y != 0)
	{
		swap(copy[blank_y][blank_x], copy[blank_y - 1][blank_x]);

		if (choice_num == 1)
		{
			h_n = 0;
		}
		else if (choice_num == 2)
		{
			h_n = misplace(copy);
		}
		else if (choice_num == 3)
		{
			h_n = manhat(copy);
		}

		get<0>(new_node) = h_n;
		get<1>(new_node) = get<1>(node) + 1;
		get<2>(new_node) = copy;

		total++;

		// push children into queue
		nodes.push(new_node);
	}

	// ================================================
	// move down
	// ================================================
	copy = mat;

	if (blank_y != 2)
	{
		swap(copy[blank_y][blank_x], copy[blank_y + 1][blank_x]);

		if (choice_num == 1)
		{
			h_n = 0;
		}
		else if (choice_num == 2)
		{
			h_n = misplace(copy);
		}
		else if (choice_num == 3)
		{
			h_n = manhat(copy);
		}

		get<0>(new_node) = h_n;
		get<1>(new_node) = get<1>(node) + 1;
		get<2>(new_node) = copy;

		total++;

		nodes.push(new_node);
	}

	// ================================================
	// move right
	// ================================================
	copy = mat;

	if (blank_x != 2)
	{
		swap(copy[blank_y][blank_x], copy[blank_y][blank_x + 1]);

		if (choice_num == 1)
		{
			h_n = 0;
		}
		else if (choice_num == 2)
		{
			h_n = misplace(copy);
		}
		else if (choice_num == 3)
		{
			h_n = manhat(copy);
		}

		get<0>(new_node) = h_n;
		get<1>(new_node) = get<1>(node) + 1;
		get<2>(new_node) = copy;

		total++;

		nodes.push(new_node);
	}

	// ================================================
	// move left
	// ================================================
	copy = mat;

	if (blank_x != 0)
	{
		swap(copy[blank_y][blank_x], copy[blank_y][blank_x - 1]);

		if (choice_num == 1)
		{
			h_n = 0;
		}
		else if (choice_num == 2)
		{
			h_n = misplace(copy);
		}
		else if (choice_num == 3)
		{
			h_n = manhat(copy);
		}

		get<0>(new_node) = h_n;
		get<1>(new_node) = get<1>(node) + 1;
		get<2>(new_node) = copy;

		total++;

		nodes.push(new_node);
	}
}

int manhat(vector<vector<int>> node)
{
	int sum = 0;

	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			int value = node[y][x];

			if (value != 0)
			{
				int goal_x = (value - 1) % 3;
				int goal_y = (value - 1) / 3;
				sum = sum + abs(x - goal_x) + abs(y - goal_y);
			}
		}
	}
	return sum;
}

// ================================================
// Count of misplaced tiles/nodes
// ================================================
int misplace(vector<vector<int>> node)
{
	int sum = 0;

	if (node[0][0] != node11)
		sum++;
	if (node[0][1] != node12)
		sum++;
	if (node[0][2] != node13)
		sum++;
	if (node[1][0] != node21)
		sum++;
	if (node[1][1] != node22)
		sum++;
	if (node[1][2] != node23)
		sum++;
	if (node[2][0] != node31)
		sum++;
	if (node[2][1] != node32)
		sum++;
	if (node[2][2] != node33)
		sum++;

	return sum;
}

int main()
{
	// matrix in which data will be stored
	vector< vector<int> > matrix;

	int i;
	int input1, input2, input3;
	char *inname = "in.txt";

	ifstream infile(inname);

	if (!infile)
	{
		cout << "There was a problem opening the file or it does not exist" << endl;
		return 0;
	}

	vector<int> input; //rows

	// ================================================
	// read from text file called "in.txt"
	// include initial and target tile confirgurations
	// NOTE: do not ask for entering initial and target data
	// ================================================
	cout << "Reading the initial state from text file.." << endl;
	infile >> input1 >> input2 >> input3;

	input.push_back(input1);
	input.push_back(input2);
	input.push_back(input3);
	
	// add row to matrix
	matrix.push_back(input);
	input.clear();

	cout << "Initial Board State:" << endl;
	cout << input1 << " " << input2 << " " << input3 << endl;

	// Reading 2nd row
	infile >> input1 >> input2 >> input3;
	input.push_back(input1);
	input.push_back(input2);
	input.push_back(input3);

	// add row to matrix
	matrix.push_back(input);
	input.clear();

	cout << input1 << " " << input2 << " " << input3 << endl;

	// Reading 3rd row
	infile >> input1 >> input2 >> input3;
	input.push_back(input1);
	input.push_back(input2);
	input.push_back(input3);

	//add row to matrix
	matrix.push_back(input);
	input.clear();

	cout << input1 << " " << input2 << " " << input3 << endl;
	cout << endl;

	// Read goal state from text file
	cout << "Reading the target goal from text file.." << endl;
	infile >> input1 >> input2 >> input3;
	node11 = input1;
	node12 = input2;
	node13 = input3;

	cout << "Target Board Goal:" << endl;
	cout << input1 << " " << input2 << " " << input3 << endl;

	// second row 
	infile >> input1 >> input2 >> input3;
	node21 = input1;
	node22 = input2;
	node23 = input3;

	cout << input1 << " " << input2 << " " << input3 << endl;

	// third row
	infile >> input1 >> input2 >> input3;
	node31 = input1;
	node32 = input2;
	node33 = input3;

	cout << input1 << " " << input2 << " " << input3 << endl;

	int choice_num = 0;

	cout << endl;
	cout << "Performing A* with the Misplaced Tile heuristic" << endl;
	general_search(matrix, 2);
	cout << "Complete" << endl;

	cout << endl;
	cout << "Performing A* with the Distance heuristic" << endl;
	general_search(matrix, 3);
	cout << "Complete" << endl;
	cout << endl;

	system("pause");

	return 0;
}
