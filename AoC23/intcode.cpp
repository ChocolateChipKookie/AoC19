#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <array>
#include <iterator>
#include <map>
#include <queue>

using lli = long long;

class intcode
{
public:
	intcode(const std::string& input_path = "Text.txt") : input_file(input_path)
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
			case 1: add(operands);						break;
			case 2: mul(operands);						break;
			case 3: input(operands, running, current_input);		break;
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


	//Input variables
	unsigned inputs = 1;

	inline void input(std::array<lli*, 3>& operands, bool& running, std::vector<lli>::iterator& in)
	{
		if(inputs == 0)
		{
			running = false;
			return;
		}

		*operands[0] = *in;
		++in;
		inputs--;
		i += 2;
	}


	//Output variables

	inline void output(std::array<lli*, 3>& operands, bool& running, std::vector<lli>& outputs)
	{
		//OUTPUT
		outputs.push_back(*operands[0]);
		i += 2;
		if(outputs.size() == 3)
		{
			running = false;
		}
	}

private:
	std::string input_file;
	std::map<lli, lli> context;
	lli i = 0;
	lli relative = 0;
	std::vector<lli> inputs_;
	std::map<lli, lli> context_;

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
};

void t1()
{
	std::vector<intcode> computers(50);
	std::vector<std::queue<std::pair<lli, lli>>> queues(50);

	for(unsigned i = 0; i < 50; ++i)
	{
		computers[i].inputs = 1;
		computers[i].run({ i });
	}

	while(true)
	{
		for(unsigned i = 0; i < 50; ++i)
		{
			std::vector<lli> out;

			if(queues[i].empty())
			{
				computers[i].inputs = 1;
				out = computers[i].run({-1});
			}
			else
			{
				std::vector<lli> inputs{ queues[i].front().first, queues[i].front().second, -1};
				queues[i].pop();
				computers[i].inputs = 2;
				out = computers[i].run(inputs);	
			}
			if (!out.empty())
			{
				if (out[0] == 255)
				{
					std::cout << "First = " << out[2] << std::endl;
					return;
				}
				else
				{
					queues[out[0]].push({ out[1], out[2] });
				}
			}
		}
	}
}

void t2()
{
	std::vector<intcode> computers(50);
	std::vector<std::queue<std::pair<lli, lli>>> queues(50);

	for (unsigned i = 0; i < 50; ++i)
	{
		computers[i].inputs = 1;
		computers[i].run({ i });
	}

	std::pair<lli, lli> nat;
	std::pair<lli, lli> previous_delivered;

	while (true)
	{
		unsigned idling = 0;
		for (unsigned i = 0; i < 50; ++i)
		{
			std::vector<lli> out;

			if (queues[i].empty())
			{
				computers[i].inputs = 1;
				out = computers[i].run({ -1 });
				if(out.empty())
					++idling;
			}
			else
			{
				const std::vector<lli> inputs{ queues[i].front().first, queues[i].front().second };
				queues[i].pop();
				computers[i].inputs = 2;
				out = computers[i].run(inputs);
			}
			if (!out.empty())
			{
				if (out[0] == 255)
					nat = { out[1], out[2] };
				else
					queues[out[0]].push({ out[1], out[2] });
			}
		}

		if(idling == 50)
		{
			if(previous_delivered.second == nat.second)
			{
				std::cout << "Second = " << nat.second << std::endl;
				return;
			}
			previous_delivered = nat;

			computers[0].inputs = 2;
			std::vector<lli> out = computers[0].run({ nat.first, nat.second });
			if (!out.empty())
			{
				if (out[0] == 255)
					nat = { out[1], out[2] };
				else
					queues[out[0]].push({ out[1], out[2] });
			}
		}
	}
}

/*
 * First = 21897
 * Second = 16424
 */

int main(int argc, char* argv[])
{
	t1();
	t2();
}