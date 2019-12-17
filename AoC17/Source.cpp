#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>
#include <map>

using lli = long long;

std::map<std::pair<lli, lli>, char> map;
lli x = 0, y = 0;
lli max_x{ 0 }, max_y{ 0 };

std::map<lli, lli> context;
lli i = 0;
lli relative = 0;

void intcode(std::vector<lli> inputs_to_computer = {}, bool second = false)
{
	std::map<lli, lli>& inputs = context;

	auto in = inputs_to_computer.begin();

	while(true)
	{
		int mode1 = inputs[i] / 100 % 10;
		int mode2 = inputs[i] / 1000 % 10;
		int mode3 = inputs[i] / 10000;
		int code = inputs[i] % 100;

		lli* op1 = nullptr;
		lli* op2 = nullptr;
		lli* op3 = nullptr;

		switch(mode1)
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

		switch(mode2)
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

		switch(mode3)
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
			*op1 = *in;
			++in;
			i += 2;
			break;
		case 4:
			{
				if(!second)
				{
					char c = static_cast<char>(*op1);
					if (c == '\n')
					{
						if (x > max_x)
						{
							max_x = x;
						}
						x = 0;
						y++;
						if (y > max_y)
						{
							max_y = y;
						}
					}
					else
					{
						if (c == '.' || c == '#' || c == '^' || c == '<' || c == 'v' || c == '>')
						{
							map[{x, y}] = c;
							++x;
						}
					}
				}
				else
				{
					if(*op1 < 256)
					{
						//std::cout << static_cast<char>(*op1);
					}
					else
					{
						std::cout << *op1;
					}
				}
				i += 2;

				break;
			}
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
end:;
}

std::map<int, int> dx{ {0, 0}, {1, -1}, {2, 0}, {3, 1} };
std::map<int, int> dy{ {0, -1}, {1, 0}, {2, 1}, {3, 0} };


void t1()
{
	std::ifstream ifs("Text.txt");
	std::istream_iterator<lli> begin(ifs), end;
	std::vector<lli> inputs_{ begin, end };
	ifs.close();
	context.clear();
	for (lli i = 0; i < inputs_.size(); ++i)
	{
		context[i] = inputs_[i];
	}
	context[0] = 1;
	
	intcode();

	std::vector<std::pair<lli, lli>> pairs;

	for(int i = 0; i < max_y; ++i)
	{
		for (int j = 0; j < max_x; ++j)
		{
			std::pair < lli, lli > current{ j, i };
			int hashes = 0;
			for (int i = 1; i <= 4; ++i)
			{
				std::pair<lli, lli> next = { current.first + dx[i], current.second + dy[i] };

				if (map.find(next) != map.end())
				{
					if(map[next] == '#')
					{
						++hashes;
					}
				}
			}

			if(hashes == 4)
			{
				pairs.push_back(current);
				std::cout << 'o';
			}
			else
			{	
				std::cout << map[{j, i}];
			}

		}
		std::cout << '\n';
	}

	lli total = 0;

	for(auto& p : pairs)
	{
		total += p.first * p.second;
	}

	std::cout << "First: " << total << std::endl;
}

void t2()
{
	std::string main = "A,C,A,C,B,B,C,A,C,B\n";
	std::string a = "L,4,R,8,L,6,L,10\n";
	std::string b = "L,4,L,4,L,10\n";
	std::string c = "L,6,R,8,R,10,L,6,L,6\n";
	std::string feed = "n\n";
	std::string full_program = main + a + b + c + feed;

	std::vector<lli> program;
	program.reserve(full_program.size());
	for (auto c : full_program)
	{
		program.push_back(static_cast<lli>(c));
	}

	{
		i = 0;
		relative = 0;

		std::ifstream ifs("Text.txt");
		std::istream_iterator<lli> begin(ifs), end;
		std::vector<lli> inputs_{ begin, end };
		ifs.close();
		context.clear();
		for (lli i = 0; i < inputs_.size(); ++i)
		{
			context[i] = inputs_[i];
		}
		context[0] = 2;

		std::cout << "Second: ";
		intcode(program, true); 
		std::cout << std::endl;

	}

	/*
	std::map<std::pair<lli, lli>, char> new_map = map;

	std::pair<lli, lli> current_position;
	unsigned current_direction = 0;

	for (int i = 0; i < max_y; ++i)
	{
		for (int j = 0; j < max_x; ++j)
		{

			if (map[{j, i}] == '^' || map[{j, i}] == 'v'|| map[{j, i}] == '<'|| map[{j, i}] == '>')
			{
				current_position = { j, i };
				switch(map[{j, i}])
				{
				case '^': current_direction = 0; break;
				case '<': current_direction = 1; break;
				case 'v': current_direction = 2; break;
				case '>': current_direction = 3; break;
				}
				break;
			}
		}
	}

	int counter = 0;
	while(true)
	{
		std::pair<lli, lli> next = { current_position.first + dx[current_direction], current_position.second + dy[current_direction] };

		if(new_map[next] != '#')
		{
			std::cout << counter << ' ';
			counter = 0;
			next = { current_position.first + dx[(current_direction + 1) % 4], current_position.second + dy[(current_direction + 1) % 4] };
			if(new_map[next] == '#')
			{
				current_direction = (current_direction + 1) % 4;
				std::cout << "L ";
				
				continue;
			}
			next = { current_position.first + dx[(current_direction + 3) % 4], current_position.second + dy[(current_direction + 3) % 4] };
			if (new_map[next] == '#')
			{
				current_direction = (current_direction + 3) % 4;
				std::cout << "R ";
				continue;
			}
			throw std::runtime_error{ "End" };
		}

		current_position = next;
		++counter;
	}
	*/
}

/*
 * First: 2350741403
 * Second: 53088
 */

int main(int argc, char* argv[])
{
	t1();
	t2();
}