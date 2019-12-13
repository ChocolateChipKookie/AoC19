#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>
#include <map>
#include <set>
#include <algorithm>
#include <Windows.h>
#include <thread>

using lli = long long;

lli relative = 0;

void gotoxy(int x, int y) {
	COORD pos = { x, y };
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);
}

void t1()
{
	std::ifstream ifs("Text.txt");
	std::istream_iterator<lli> begin(ifs), end;
	std::vector<lli> inputs_{ begin, end };
	ifs.close();

	std::map<lli, lli> inputs;

	std::map<std::pair<int, int>, int> tiles;
	std::pair<int, int> current_tile{ -100000000, -100000000 };

	std::vector<lli> results;

	for (lli i = 0; i < inputs_.size(); ++i)
	{
		inputs[i] = inputs_[i];
	}

	int blocks = 0;

	lli i = 0;

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
			*op1 = 1;
			i += 2;
			break;
		case 4:
			if (current_tile.first == -100000000)
			{
				current_tile.first = *op1;
			}
			else if (current_tile.second == -100000000)
			{
				current_tile.second = *op1;
			}
			else{
				tiles[current_tile] = *op1;
				if (*op1 == 2) blocks++;
				current_tile = { -100000000, -100000000 };
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
	std::cout << "First: " <<  blocks << std::endl;
	return;
}

void print_game(std::map<std::pair<int, int>, int>& tiles)
{
	std::string res;
	res.reserve(740);

	gotoxy(0, 2);
	std::cout << "Score: " << tiles[{-1, 0}] << std::endl;
	for (unsigned y = 0; y < 20; ++y)
	{
		for (unsigned x = 0; x < 37; ++x)
		{
			char c;
			switch (tiles[{x, y}])
			{
			case 0:
				c = ' ';
				break;
			case 1:
				c = 178;
				break;
			case 2:
				c = '#';
				break;
			case 3:
				c = '_';
				break;
			case 4:
				c = 'o';
				break;
			}
			res += c;
		}
		res += '\n';
	}
	std::cout << res;
}

void t2(bool display)
{
	std::ifstream ifs("Text.txt");
	std::istream_iterator<lli> begin(ifs), end;
	std::vector<lli> inputs_{ begin, end };
	ifs.close();

	std::map<lli, lli> inputs;

	std::map<std::pair<int, int>, int> tiles;
	std::pair<int, int> current_tile{ -100000000, -100000000 };

	std::vector<lli> results;

	for (lli i = 0; i < inputs_.size(); ++i)
	{
		inputs[i] = inputs_[i];
	}

	inputs[0] = 2;

	int blocks = 0;

	lli i = 0;

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
			{
			auto ball = std::find_if(std::begin(tiles), std::end(tiles), [&](const auto& pair)
				{
					return pair.second == 4;
				});


			auto paddle = std::find_if(std::begin(tiles), std::end(tiles), [&](const auto& pair)
				{
					return pair.second == 3;
				});

			int ball_x = ball->first.first;
			int paddle_x = paddle->first.first;

			if(ball_x < paddle_x)
			{
				*op1 = -1;
			}
			else if(ball_x > paddle_x)
			{
				*op1 = 1;
			}
			else
			{
				*op1 = 0;
			}

			i += 2;
			if (display && tiles.size() >= 740)
			{
				print_game(tiles);
				std::this_thread::sleep_for(std::chrono::milliseconds(30));
			}
			break;
			}
		case 4:
			if (current_tile.first == -100000000)
			{
				current_tile.first = *op1;
			}
			else if (current_tile.second == -100000000)
			{
				current_tile.second = *op1;
			}
			else {
				tiles[current_tile] = *op1;
				if (*op1 == 2) blocks++;
				current_tile = { -100000000, -100000000 };
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
	if(display)
	{
		print_game(tiles);
	}
	else
	{
		std::cout << "Second: " << tiles[{-1, 0}]<< std::endl;

	}
}

/*
First: 251
Second: 12779
*/

int main(int argc, char* argv[])
{
	t1();
	t2(false);
	t2(true);
}