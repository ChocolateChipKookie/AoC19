#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>



void t1()
{
	std::ifstream ifs("Text.txt");
	std::istream_iterator<int> begin(ifs), end;
	std::vector<int> inputs{ begin, end };
	ifs.close();

	//std::ifstream t("Text.txt");
	//std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

	for (unsigned i = 0; i < inputs.size(); i += 4)
	{
		switch (inputs[i])
		{
		case 1:
			inputs[inputs[i + 3]] = inputs[inputs[i + 1]] + inputs[inputs[i + 2]];
			break;
		case 2:
			inputs[inputs[i + 3]] = inputs[inputs[i + 1]] * inputs[inputs[i + 2]];
			break;
		case 99:
			goto end;

		default:
			throw std::runtime_error{ "Err" };
		}
	}
end:

	std::cout << "First " << inputs[0] << std::endl;
}
void t2()
{
	std::ifstream ifs("Text.txt");
	std::istream_iterator<int> begin(ifs), end;
	std::vector<int> inputs{ begin, end };
	ifs.close();

	std::vector<int> backup = inputs;

	//std::ifstream t("Text.txt");
	//std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());


	for(unsigned noun = 0; noun < inputs.size(); ++noun)
	{
		for (unsigned verb = 0; verb < inputs.size(); ++verb)
		{
			inputs = backup;
			inputs[1] = noun;
			inputs[2] = verb;
			bool exit = true;
			for (unsigned i = 0; i < inputs.size(); i += 4)
			{
				switch (inputs[i])
				{
				case 1:
					inputs[inputs[i + 3]] = inputs[inputs[i + 1]] + inputs[inputs[i + 2]];
					break;
				case 2:
					inputs[inputs[i + 3]] = inputs[inputs[i + 1]] * inputs[inputs[i + 2]];
					break;
				case 99:
					goto end;

				default:
					exit = false;
					goto end;
				}
			}
		end:
			if(exit && inputs[0] == 19690720)
			{
				std::cout << "Second: " <<100 * noun + verb << std::endl;
				goto end_;
			}
		}
	}
end_:;
}

/*
 * First 2890696
 * Second: 8226
 */

int main(int argc, char* argv[])
{
	t1();
	t2();
}
