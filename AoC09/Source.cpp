#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>
#include <map>

using lli = long long;


void intcode(int input)
{
	std::ifstream ifs("Text1.txt");
	std::istream_iterator<lli> begin(ifs), end;
	std::vector<lli> inputs_{ begin, end };
	ifs.close();

	std::map<lli, lli> inputs;

	for(lli i = 0; i < inputs_.size(); ++i)
	{
		inputs[i] = inputs_[i];
	}

	lli i = 0;
	lli relative = 0;
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
			*op1 = input;
			i += 2;
			break;
		case 4:
			std::cout << *op1 << std::endl;
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
end:;
}

void t1()
{
	std::cout << "First: ";
	intcode(1);
}

void t2()
{
	std::cout << "Second: ";
	intcode(2);
	std::cout << std::endl;

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