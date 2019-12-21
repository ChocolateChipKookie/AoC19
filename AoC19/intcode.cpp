#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <array>
#include <iterator>
#include <map>
#include <set>

using lli = long long;

namespace ic
{
	const std::string input_file{ "Text.txt" };
	std::map<lli, lli> context;
	lli i = 0;
	lli relative = 0;
	std::vector<lli> inputs_;
	std::map<lli, lli> context_;

	void load_intcode()
	{
		std::ifstream ifs(input_file);
		inputs_ = std::vector<lli>{ std::istream_iterator<lli>(ifs), std::istream_iterator<lli>() };
		ifs.close();

		std::map<lli, lli> context;
		for (lli i = 0; i < inputs_.size(); ++i)
			context_[i] = inputs_[i];
	}

	void reset_computer()
	{
		i = relative = 0;
		context = context_;
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

	int total = 0;
	int x = 0;
	bool second = true;

	int min_x;
	int max_x;
	bool is_beam;
	std::map<std::pair<lli, lli>, char> map;

	inline void output(std::array<lli*, 3>& operands, bool& running, std::vector<lli>& outputs)
	{
		//OUTPUT

		if(!second)
		{
			if (*operands[0] == 1)
			{
				++total;
				std::cout << '#';
			}
			else
			{
				std::cout << '.';
			}
			++x;
			if (x == 50)
			{
				x = 0;
				std::cout << '\n';
			}
		}
		else
		{
			is_beam = *operands[0] == 1;
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

void t1()
{
	ic::load_intcode();
	ic::reset_computer();
	ic::second = false;

	for(lli i = 0; i < 50; ++i)
	{
		for (lli j = 0; j < 50; ++j)
		{
			ic::reset_computer();
			ic::intcode({ j, i });
		}
	}
	std::cout << std::endl << "First: " << ic::total;
}

void t2()
{

	ic::load_intcode();
	ic::reset_computer();
	std::cout << "\nSecond: ";
	ic::second = true;

	lli x = 0;
	lli y = 0;
	lli min_x = 0;

	std::set<std::pair<lli, lli>> beam_pos;

	bool is_first = true;
	while (true)
	{
		ic::reset_computer();
		ic::intcode({ x, y });
		if(is_first)
		{
			if(!ic::is_beam)
			{
				++min_x;
			}
			else
			{
				beam_pos.insert({ x, y });
			}
			is_first = false;
			++x;
		}
		else
		{
			if (!ic::is_beam)
			{
				x = min_x;
				++y;
				is_first = true;
			}
			else
			{
				beam_pos.insert({ x, y });

				if(beam_pos.find({x - 99, y})!= beam_pos.end() && beam_pos.find({ x, y - 99}) != beam_pos.end())
				{
					std::cout << x - 99 << ' ' << y - 99;
					break;
				}
				++x;
			}
		}
	}
}

/*
 * First: 158
 * Second: 619 1165
 */

int main(int argc, char* argv[])
{
	t1();
	t2();
}