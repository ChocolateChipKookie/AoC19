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

	for (unsigned i = 0; i < inputs.size();)
	{
		bool mode1 = inputs[i] / 100 % 10;
		bool mode2 = inputs[i] / 1000 % 10;
		bool mode3 = inputs[i] / 10000;
		int code = inputs[i] % 100;

		switch (code)
		{
		case 1:
			mode3 ? inputs[i + 3] : inputs[inputs[i + 3]] = (mode1 ? inputs[i + 1] : inputs[inputs[i + 1]]) + (mode2 ? inputs[i + 2] : inputs[inputs[i + 2]]);
			i += 4;
			break;
		case 2:
			mode3 ? inputs[i + 3] : inputs[inputs[i + 3]] = (mode1 ? inputs[i + 1] : inputs[inputs[i + 1]]) * (mode2 ? inputs[i + 2] : inputs[inputs[i + 2]]);
			i += 4;
			break;
		case 3:
			mode1 ? inputs[i + 1] : inputs[inputs[i + 1]] = 1;
			i += 2;
			break;
		case 4:
			if ((mode1 ? inputs[i + 1] : inputs[inputs[i + 1]]) != 0)
			{
				std::cout << (mode1 ? inputs[i + 1] : inputs[inputs[i + 1]]);
				goto end;
			}
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
void t2()
{
	std::ifstream ifs("Text.txt");
	std::istream_iterator<int> begin(ifs), end;
	std::vector<int> inputs{ begin, end };
	ifs.close();

	//std::ifstream t("Text.txt");
	//std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

	for (unsigned i = 0; i < inputs.size();)
	{
		bool mode1 = inputs[i] / 100 % 10;
		bool mode2 = inputs[i] / 1000 % 10;
		bool mode3 = inputs[i] / 10000;
		int code = inputs[i] % 100;

		switch (code)
		{
		case 1:
			mode3 ? inputs[i + 3] : inputs[inputs[i + 3]] = (mode1 ? inputs[i + 1] : inputs[inputs[i + 1]]) + (mode2 ? inputs[i + 2] : inputs[inputs[i + 2]]);
			i += 4;
			break;
		case 2:
			mode3 ? inputs[i + 3] : inputs[inputs[i + 3]] = (mode1 ? inputs[i + 1] : inputs[inputs[i + 1]]) * (mode2 ? inputs[i + 2] : inputs[inputs[i + 2]]);
			i += 4;
			break;
		case 3:
			mode1 ? inputs[i + 1] : inputs[inputs[i + 1]] = 5;
			i += 2;
			break;
		case 4:
			if ((mode1 ? inputs[i + 1] : inputs[inputs[i + 1]]) != 0)
			{
				std::cout << (mode1 ? inputs[i + 1] : inputs[inputs[i + 1]]);
				goto end;
			}
			i += 2;
			break;
		case 5:
			if ((mode1 ? inputs[i + 1] : inputs[inputs[i + 1]]) != 0)
			{
				i = (mode2 ? inputs[i + 2] : inputs[inputs[i + 2]]);
			}
			else
			{
				i += 3;
			}
			break;
		case 6:
			if ((mode1 ? inputs[i + 1] : inputs[inputs[i + 1]]) == 0)
			{
				i = (mode2 ? inputs[i + 2] : inputs[inputs[i + 2]]);
			}
			else
			{
				i += 3;
			}
			break;
		case 7:
			mode3 ? inputs[i + 3] : inputs[inputs[i + 3]] = (mode1 ? inputs[i + 1] : inputs[inputs[i + 1]]) < (mode2 ? inputs[i + 2] : inputs[inputs[i + 2]]);
			i += 4;
			break;
		case 8:
			mode3 ? inputs[i + 3] : inputs[inputs[i + 3]] = (mode1 ? inputs[i + 1] : inputs[inputs[i + 1]]) == (mode2 ? inputs[i + 2] : inputs[inputs[i + 2]]);
			i += 4;
			break;
		case 99:
			goto end;

		default:
			throw std::runtime_error{ "Err" };
		}
	}
end:;
}

/*
 * First 7265618
 * Second 7731427
 */

int main(int argc, char* argv[])
{
	std::cout << "First ";
	t1();
	std::cout << "\nSecond ";
	t2();
}