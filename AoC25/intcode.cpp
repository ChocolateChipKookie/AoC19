#include <iostream>
#include <sstream>
#include <fstream>
#include <utility>
#include <vector>
#include <array>
#include <iterator>
#include <map>

using lli = long long;

class intcode
{
public:
	explicit intcode(std::string input_path = "Text.txt") : input_file(std::move(input_path))
	{
		load();
	}

	void load()
	{
		std::ifstream ifs(input_file);
		inputs_ = std::vector<lli>{ std::istream_iterator<lli>(ifs), std::istream_iterator<lli>() };
		ifs.close();

		std::map<lli, lli> context;
		for (lli i = 0; i < inputs_.size(); ++i)
			context_[i] = inputs_[i];
		reset();
	}

	void reset()
	{
		i = relative = 0;
		context = context_;
	}

	std::vector<lli> run(std::vector<lli> inputs = {})
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
			case 1: add(operands);							break;
			case 2: mul(operands);							break;
			case 3: input(operands, running, current_input);break;
			case 4: output(operands, running, outputs);		break;
			case 5: jump(operands, false);				break;
			case 6: jump(operands, true);				break;
			case 7: less(operands);							break;
			case 8: equal(operands);						break;
			case 9: add_relative(operands);					break;
			case 99: running = false;						break;
			default: throw std::runtime_error{ "Err" };
			}
		}
		return outputs;
	}

	//Input variables

	unsigned inputs = 0;

	inline void input(std::array<lli*, 3>& operands, bool& running, std::vector<lli>::iterator& in)
	{
		if (inputs == 0)
		{
			running = false;
			return;
		}

		*operands[0] = *in;
		++in;
		i += 2;
		--inputs;
	}

	//Output variables

	inline void output(std::array<lli*, 3>& operands, bool& running, std::vector<lli>& outputs)
	{
		//OUTPUT
		if(*operands[0] < 128)
			std::cout << static_cast<char>(*operands[0]);
		else
			std::cout << *operands[0];

		i += 2;
	}

private:
	
	std::string input_file;
	std::map<lli, lli> context;
	lli i{0}, relative{0};
	std::vector<lli> inputs_;
	std::map<lli, lli> context_;

	static inline int nth_digit(int number, int digit_number)
	{
		const auto pow_10 = [](unsigned pow)
		{
			int res = 1;
			for (unsigned i = 0; i < pow; ++i)res *= 10;
			return res;
		};
		return (number / pow_10(digit_number)) % 10;
	};

	inline lli* calculate_operand(std::map<lli, lli>& inputs, int mode, int operator_pos) const
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
};

void t1()
{
	intcode ic;
	ic.inputs = 0;
	ic.run();
	while (true) {
		std::string instruction;
		std::getline(std::cin, instruction);
		
		std::vector<lli> program;

		for (char c : instruction)
		{
			program.emplace_back(static_cast<lli>(c));
		}
		program.emplace_back(static_cast<lli>('\n'));
		ic.inputs = program.size();
		ic.run(program);
	}
}

void t2()
{
}

/*
 * First: 34095120
 * Second: Press key
 * 
 * Items : Planetoid, semiconductor, food, fixed point
 * LAYOUT:
 
                   *
                   |
   6-5-4-3-2-1 F H-I
     | | |     | |
   8-7 9 A-----E-G
         |       |
	   c-B       J
         |
		 D

* - GOAL
1 - Holodeck
2 - Stables
	Semiconductor
3 - Observatory
	planetoid
4 - Crew quarters
	food ration
5 - Sick Bay
	Fixed point
6 - Passages
	klein bottle
7 - Arcade
8 - Warp drive maintenence
	weather machine
9 - Storage
A - Kitchen
	giant electromagnet
B - Navigation
C - Science lab
	infinite loop
D - Hot chocolate fountain
	Pointer
E - Holodeck 
	Coin
F - Hallway
	escape pod
G - Engineering
	photons
H - Gift wrapping
I - Security checkpoint
J - Corridor
	Molten lava
 */

int main(int argc, char* argv[])
{
	t1();
}

