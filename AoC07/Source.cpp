#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <map>
#include <array>

const std::string input_file = "Text.txt";

int64_t intcode(int64_t phase, int64_t input)
{
	std::ifstream ifs(input_file);
	std::istream_iterator<int64_t> begin(ifs), end;
	std::vector<int64_t> inputs{ begin, end };
	ifs.close();

	//std::ifstream t("Text.txt");
	//std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

	bool phase_time = true;

	for (unsigned i = 0; i < inputs.size();)
	{
		bool mode1 = inputs[i] / 100 % 10;
		bool mode2 = inputs[i] / 1000 % 10;
		bool mode3 = inputs[i] / 10000;
		int64_t code = inputs[i] % 100;

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
			if (phase_time)
			{
				mode1 ? inputs[i + 1] : inputs[inputs[i + 1]] = phase;
				phase_time = false;
			}
			else
			{
				mode1 ? inputs[i + 1] : inputs[inputs[i + 1]] = input;
			}
			i += 2;
			break;
		case 4:
			return (mode1 ? inputs[i + 1] : inputs[inputs[i + 1]]);
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
		case 99:
			throw std::runtime_error{ "Halt" };
		default:
			throw std::runtime_error{ "Err" };
		}
	}
end:;
}

std::vector<std::vector<int64_t>> permutations;

void print64_tArr(int64_t a[], int64_t n)
{
	permutations.emplace_back(5, 0);
	for (int64_t i = 0; i < n; i++)
		permutations.back()[i] = a[i];
}

// Generating permutation using Heap Algorithm 
void heapPermutation(int64_t a[], int64_t size, int64_t n)
{
	// if size becomes 1 then print64_ts the obtained 
	// permutation 
	if (size == 1)
	{
		print64_tArr(a, n);
		return;
	}

	for (int64_t i = 0; i < size; i++)
	{
		heapPermutation(a, size - 1, n);

		// if size is odd, swap first and last 
		// element 
		if (size % 2 == 1)
			std::swap(a[0], a[size - 1]);

		// If size is even, swap ith and last 
		// element 
		else
			std::swap(a[i], a[size - 1]);
	}
}

void t1()
{
	std::vector<int64_t> results;

	for(auto& permutation :permutations)
	{
		try
		{
			int64_t input = 0;
			for (int64_t phase : permutation)
			{
				input = intcode(phase, input);
			}
			std::cout << input << std::endl;
			results.push_back(input);
		}
		catch(std::runtime_error& error)
		{
			std::cout << "No output" << std::endl;
		}
	}

	std::cout << "Max = " << *std::max_element(results.begin(), results.end()) << std::endl;
}

std::map<int, std::pair<std::vector<int64_t>, int>> context;

int64_t intcode2(int64_t phase, int64_t input, int id)
{
	//std::ifstream t("Text.txt");
	//std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

	std::vector<int64_t>& inputs = context[id].first;

	for (unsigned i = context[id].second; i < inputs.size();)
	{
		bool mode1 = inputs[i] / 100 % 10;
		bool mode2 = inputs[i] / 1000 % 10;
		bool mode3 = inputs[i] / 10000;
		int64_t code = inputs[i] % 100;

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
			mode1 ? inputs[i + 1] : inputs[inputs[i + 1]] = phase == -1 ? input : phase;
			phase = -1;
			i += 2;
			break;
		case 4:
			{
			auto res = (mode1 ? inputs[i + 1] : inputs[inputs[i + 1]]);
			context[id].second = i + 2;
			return res;
			}
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
			throw std::runtime_error{ "Halt" };
		default:
			throw std::runtime_error{ "Err" };
		}
	}
end:;
}

void t2()
{
	std::ifstream ifs(input_file);
	std::istream_iterator<int64_t> begin(ifs), end;
	std::vector<int64_t> inputs{ begin, end };
	ifs.close();

	std::vector<int64_t> results;

	for(auto& permutation : permutations)
	{
		for (int i = 0; i < 5; ++i)
		{
			context[i] = { inputs, 0 };
		}

		int64_t result = 0;
		try
		{
			while (true)
			{
				int64_t input = result;
				for (auto i = 0; i < 5; ++i)
				{
					input = intcode2(permutation[i], input, i);
					permutation[i] = -1;
				}
				result = input;
			}
		}
		catch (std::runtime_error & error)
		{
			std::cout << error.what() << " res = " << result << std::endl;
			results.push_back(result);
		}
	}

	std::cout << "Top result = " << *std::max_element(results.begin(), results.end()) << std::endl;

}

int main(int argc, char* argv[])
{
	//int64_t numbers[]{ 0, 1, 2, 3, 4 };
	//heapPermutation(numbers, 5, 5);
	//t1();

	int64_t numbers[]{ 5, 6, 7, 8, 9 };
	heapPermutation(numbers, 5, 5);
	t2();


	char t;
	std::cin >> t;
}
