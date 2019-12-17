#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <array>
#include <iterator>
#include <map>

using lli = long long;

std::map<std::pair<lli, lli>, char> map;
lli x = 0, y = 0;
lli max_x{ 0 }, max_y{ 0 };

namespace ic
{
	const std::string input_file{ "Text.txt" };
	std::map<lli, lli> context;
	lli i = 0;
	lli relative = 0;

	void reset_computer()
	{
		i = relative = 0;
		std::ifstream ifs(input_file);
		std::vector<lli> inputs_{ std::istream_iterator<lli>(ifs), std::istream_iterator<lli>() };
		ifs.close();
		context.clear();
		for (lli i = 0; i < inputs_.size(); ++i)
			context[i] = inputs_[i];
	}

	inline int nth_digit(int number, int digit_number)
	{
		const auto pow_10 = [](int pow)
		{
			int res = 1;
			for (unsigned i = 0; i < pow; ++i)res *= 10;
			return res;
		};
		return (number / pow_10(digit_number)) % 10;
	};

	inline lli* calculate_operand(std::map<lli, lli>& inputs, int mode, int operator_pos)
	{
		switch (mode)
		{
		case 0: return &inputs[inputs[i + operator_pos]];
		case 1: return &inputs[i + operator_pos];
		case 2: return &inputs[relative + inputs[i + operator_pos]];
		default: return nullptr;
		}
	}


	inline void add(std::array<lli*, 3>& operands)
	{
		*operands[2] = *operands[0] + *operands[1];
		i += 4;
	}
	inline void mul(std::array<lli*, 3>& operands)
	{
		*operands[2] = (*operands[0]) * (*operands[1]);
		i += 4;
	}
	inline void jump(std::array<lli*, 3>& operands, bool zero)
	{
		if ((*operands[0] == 0) == zero)
			i = *operands[1];
		else
			i += 3;
	}
	inline void less(std::array<lli*, 3>& operands)
	{
		*operands[2] = *operands[0] < *operands[1];
		i += 4;
	}
	inline void equal(std::array<lli*, 3>& operands)
	{
		*operands[2] = *operands[0] == *operands[1];
		i += 4;
	}
	inline void add_relative(std::array<lli*, 3>& operands)
	{
		relative += *operands[0];
		i += 2;
	}

	//Input variables

	inline void input(std::array<lli*, 3>& operands, std::vector<lli>::iterator& in)
	{
		*operands[0] = *in;
		++in;
		i += 2;
	}

	//Output variables

	bool first_task = true;

	inline void output(std::array<lli*, 3>& operands, bool& running, std::vector<lli>& outputs)
	{
		if (first_task)
		{
			char c = static_cast<char>(*operands[0]);
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
			std::cout << c;
		}
		else
		{
			if (*operands[0] > 127)
			{
				std::cout << *operands[0];
			}
		}


		i += 2;
	}

	std::vector<lli> intcode(std::vector<lli> inputs = {})
	{
		std::map<lli, lli>& input_code = context;
		auto current_input = inputs.begin();
		std::vector<lli> outputs;

		bool running = true;
		std::array<lli*, 3> operands{};
		while (running)
		{
			for (int k = 0; k < 3; ++k)
			{
				const int mode = nth_digit(input_code[i], k + 2);
				operands[k] = calculate_operand(input_code, mode, k + 1);
			}

			switch (input_code[i] % 100)
			{
			case 1: add(operands);						break;
			case 2: mul(operands);						break;
			case 3: input(operands, current_input);		break;
			case 4: output(operands, running, outputs);	break;
			case 5: jump(operands, false);			break;
			case 6: jump(operands, true);			break;
			case 7: less(operands);						break;
			case 8: equal(operands);					break;
			case 9: add_relative(operands);				break;
			case 99: running = false;					break;
			default: throw std::runtime_error{ "Err" };
			}
		}
		return outputs;
	}
}

std::map<int, int> dx{ {0, 0}, {1, -1}, {2, 0}, {3, 1} };
std::map<int, int> dy{ {0, -1}, {1, 0}, {2, 1}, {3, 0} };


void t1()
{
	ic::reset_computer();
	ic::intcode();

	std::vector<std::pair<lli, lli>> pairs;

	for(int i = 0; i < max_y; ++i)
	{
		for (int j = 0; j < max_x; ++j)
		{
			std::pair < lli, lli > current{ j, i };
			int hashes = 0;
			for (int k = 1; k <= 4; ++k)
			{
				std::pair<lli, lli> next = { current.first + dx[k], current.second + dy[k] };

				if (map.find(next) != map.end())
				{
					if(map[next] == '#')
					{
						++hashes;
					}
				}
			}

			if (hashes == 4)
			{
				pairs.push_back(current);
			}

		}
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

	ic::first_task = false;
	ic::reset_computer();
	ic::context[0] = 2;

	std::cout << "Second: ";
	ic::intcode(program); 
	std::cout << std::endl;

	/*
	//Printing out the full path
	 
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
 * First: 3192
 * Second: 684691
 */

int main(int argc, char* argv[])
{
	t1();
	t2();
}