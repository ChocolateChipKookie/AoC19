#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <array>
#include <iterator>
#include <map>

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

	inline void output(std::array<lli*, 3>& operands, bool& running, std::vector<lli>& outputs)
	{
		//OUTPUT
		if(*operands[0] < 128)
		{
			std::cout << static_cast<char>(*operands[0]);
		}
		else
		{
			std::cout << std::endl << *operands[0] << std::endl;
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
	std::cout << "First: ";

	std::vector<std::string> instructions;

	instructions.emplace_back("NOT A J");
	instructions.emplace_back("AND D J");

	instructions.emplace_back("NOT B T");
	instructions.emplace_back("AND D T");
	instructions.emplace_back("OR T J");

	instructions.emplace_back("NOT C T");
	instructions.emplace_back("AND D T");
	instructions.emplace_back("OR T J");

	instructions.emplace_back("WALK");

	std::vector<lli> program;

	for(auto& instruction : instructions)
	{
		for(char c : instruction)
		{
			program.emplace_back(static_cast<lli>(c));
		}
		program.emplace_back(static_cast<lli>('\n'));
	}
	ic::intcode(program);
}

void t2()
{

	ic::load_intcode();
	ic::reset_computer();
	std::cout << "Second: ";

	std::vector<std::string> instructions;

	instructions.emplace_back("NOT A J");
	instructions.emplace_back("AND D J");

	instructions.emplace_back("NOT B T");
	instructions.emplace_back("AND D T");
	instructions.emplace_back("OR T J");

	instructions.emplace_back("NOT C T");
	instructions.emplace_back("AND D T");
	instructions.emplace_back("OR T J");

	instructions.emplace_back("AND H T");
	instructions.emplace_back("OR H T");
	instructions.emplace_back("OR E T");
	instructions.emplace_back("AND T J");



	instructions.emplace_back("RUN");

	std::vector<lli> program;

	for (auto& instruction : instructions)
	{
		for (char c : instruction)
		{
			program.emplace_back(static_cast<lli>(c));
		}
		program.emplace_back(static_cast<lli>('\n'));
	}
	ic::intcode(program);
}

/*
 * First: 19360288
 * Second: 1143814750
 */

int main(int argc, char* argv[])
{
	t1();
	t2();
}