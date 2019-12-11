#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>
#include <map>
#include <set>
#include <algorithm>

using lli = long long;


std::map<lli, lli> context;
lli relative = 0;

//While programming, it was all in the same function, but i like having formatted answers
void t1()
{
	std::ifstream ifs("Text.txt");
	std::istream_iterator<lli> begin(ifs), end;
	std::vector<lli> inputs_{ begin, end };
	ifs.close();

	std::map<lli, lli>& inputs = context;

	std::vector<lli> results;

	for (lli i = 0; i < inputs_.size(); ++i)
	{
		inputs[i] = inputs_[i];
	}

	lli i = 0;

	std::map<int, int> dx;
	dx[0] = 0, dx[1] = 1, dx[2] = 0, dx[3] = -1;
	std::map<int, int> dy;
	dy[0] = 1, dy[1] = 0, dy[2] = -1, dy[3] = 0;


	std::map<std::pair<lli, lli>, bool> painted;
	int direction = 0;
	std::pair<lli, lli> location{ 0, 0 };

	int output = -1;

	std::pair<lli, lli> x_range;
	std::pair<lli, lli> y_range;

	while (true)
	{
		int mode1 = inputs[i] / 100 % 10;
		int mode2 = inputs[i] / 1000 % 10;
		int mode3 = inputs[i] / 10000;
		int code = inputs[i] % 100;

		lli* op1 = nullptr;
		lli* op2 = nullptr;
		lli* op3 = nullptr;

		switch (mode1)
		{
		case 0:
			op1 = &inputs[inputs[i + 1]];
			break;
		case 1:
			op1 = &inputs[i + 1];
			break;
		case 2:
			op1 = &inputs[relative + inputs[i + 1]];
			break;
		}

		switch (mode2)
		{
		case 0:
			op2 = &inputs[inputs[i + 2]];
			break;
		case 1:
			op2 = &inputs[i + 2];
			break;
		case 2:
			op2 = &inputs[relative + inputs[i + 2]];
			break;
		}

		switch (mode3)
		{
		case 0:
			op3 = &inputs[inputs[i + 3]];
			break;
		case 1:
			op3 = &inputs[i + 3];
			break;
		case 2:
			op3 = &inputs[relative + inputs[i + 3]];
			break;
		}

		switch (code)
		{
		case 1:

			*op3 = *op1 + *op2;
			i += 4;
			break;
		case 2:
		{
			*op3 = (*op1) * (*op2);
			i += 4;
			break;
		}
		case 3:
			*op1 = painted[location];
			i += 2;
			break;
		case 4:
			if (output == -1)
			{
				output = *op1;
			}
			else
			{
				direction = *op1 == 0 ? (direction + 1) % 4 : (direction + 3) % 4;
				//Paint
				painted[location] = output;

				location.first += dx[direction];
				location.second += dy[direction];


				x_range.first = std::min(location.first, x_range.first);
				x_range.second = std::max(location.first, x_range.second);
				y_range.first = std::min(location.second, y_range.first);
				y_range.second = std::max(location.second, y_range.second);

				output = -1;
			}
			i += 2;
			break;
		case 5:
			if (*op1 != 0)
			{
				i = *op2;
			}
			else
			{
				i += 3;
			}
			break;
		case 6:
			if (*op1 == 0)
			{
				i = *op2;
			}
			else
			{
				i += 3;
			}
			break;
		case 7:
			*op3 = *op1 < *op2;
			i += 4;
			break;
		case 8:
			*op3 = *op1 == *op2;
			i += 4;
			break;
		case 9:
			relative += *op1;
			i += 2;
			break;
		case 99:
			goto end;

		default:
			throw std::runtime_error{ "Err" };
		}
	}
end:
	std::cout << "First: " <<  painted.size() << std::endl;
	return;
}

void t2()
{
	std::ifstream ifs("Text.txt");
	std::istream_iterator<lli> begin(ifs), end;
	std::vector<lli> inputs_{ begin, end };
	ifs.close();

	std::map<lli, lli>& inputs = context;

	std::vector<lli> results;

	for (lli i = 0; i < inputs_.size(); ++i)
	{
		inputs[i] = inputs_[i];
	}

	lli i = 0;

	std::map<int, int> dx;
	dx[0] = 0, dx[1] = 1, dx[2] = 0, dx[3] = -1;
	std::map<int, int> dy;
	dy[0] = 1, dy[1] = 0, dy[2] = -1, dy[3] = 0;


	std::map<std::pair<lli, lli>, bool> painted;
	int direction = 0;
	std::pair<lli, lli> location{ 0, 0 };

	painted[location] = true;

	int output = -1;

	std::pair<lli, lli> x_range;
	std::pair<lli, lli> y_range;

	while (true)
	{
		int mode1 = inputs[i] / 100 % 10;
		int mode2 = inputs[i] / 1000 % 10;
		int mode3 = inputs[i] / 10000;
		int code = inputs[i] % 100;

		lli* op1 = nullptr;
		lli* op2 = nullptr;
		lli* op3 = nullptr;

		switch (mode1)
		{
		case 0:
			op1 = &inputs[inputs[i + 1]];
			break;
		case 1:
			op1 = &inputs[i + 1];
			break;
		case 2:
			op1 = &inputs[relative + inputs[i + 1]];
			break;
		}

		switch (mode2)
		{
		case 0:
			op2 = &inputs[inputs[i + 2]];
			break;
		case 1:
			op2 = &inputs[i + 2];
			break;
		case 2:
			op2 = &inputs[relative + inputs[i + 2]];
			break;
		}

		switch (mode3)
		{
		case 0:
			op3 = &inputs[inputs[i + 3]];
			break;
		case 1:
			op3 = &inputs[i + 3];
			break;
		case 2:
			op3 = &inputs[relative + inputs[i + 3]];
			break;
		}

		switch (code)
		{
		case 1:

			*op3 = *op1 + *op2;
			i += 4;
			break;
		case 2:
		{
			*op3 = (*op1) * (*op2);
			i += 4;
			break;
		}
		case 3:
			*op1 = painted[location];
			i += 2;
			break;
		case 4:
			if (output == -1)
			{
				output = *op1;
			}
			else
			{
				direction = *op1 == 0 ? (direction + 1) % 4 : (direction + 3) % 4;
				//Paint
				painted[location] = output;

				location.first += dx[direction];
				location.second += dy[direction];


				x_range.first = std::min(location.first, x_range.first);
				x_range.second = std::max(location.first, x_range.second);
				y_range.first = std::min(location.second, y_range.first);
				y_range.second = std::max(location.second, y_range.second);

				output = -1;
			}
			i += 2;
			break;
		case 5:
			if (*op1 != 0)
			{
				i = *op2;
			}
			else
			{
				i += 3;
			}
			break;
		case 6:
			if (*op1 == 0)
			{
				i = *op2;
			}
			else
			{
				i += 3;
			}
			break;
		case 7:
			*op3 = *op1 < *op2;
			i += 4;
			break;
		case 8:
			*op3 = *op1 == *op2;
			i += 4;
			break;
		case 9:
			relative += *op1;
			i += 2;
			break;
		case 99:
			goto end;

		default:
			throw std::runtime_error{ "Err" };
		}
	}
end:

	std::cout << "Second:" << std::endl;

	for (int y = y_range.second; y >= y_range.first; --y)
	{
		for (int x = x_range.second; x >= x_range.first; --x)
		{
			std::cout << (painted[{x, y}] ? static_cast<char>(178) : ' ');
		}
		std::cout << std::endl;
	}
}

/*

First: 2469
Second:
 #  # #     ##  ####  ##  ####  ##  #  #
 # #  #    #  #    # #  # #    #  # #  #
 ##   #    #      #  #  # ###  #    #  #
 # #  #    #     #   #### #    # ## #  #
 # #  #    #  # #    #  # #    #  # #  #
 #  # ####  ##  #### #  # ####  ###  ##
 
*/

int main(int argc, char* argv[])
{
	t1();
	t2();
}