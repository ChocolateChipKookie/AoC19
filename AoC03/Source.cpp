#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>

const char* input_file = "input.txt";

void t1()
{
	std::ifstream ifs(input_file);
	std::istream_iterator<int> begin(ifs), end;
	std::vector<int> inputs{ begin, end };
	ifs.close();

	std::ifstream t(input_file);
	std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());


	std::cout << "Solution 1: " << res << std::endl;
}
void t2()
{
	std::ifstream ifs(input_file);
	std::istream_iterator<int> begin(ifs), end;
	std::vector<int> inputs{ begin, end };
	
	std::ifstream t(input_file);
	std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());


	std::cout << "Solution 2: " << res << std::endl;
}

int main(int argc, char* argv[])
{
	t1();
	t2();
	char t;
	std::cin >> t;
}
