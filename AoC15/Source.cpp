#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>
#include <map>
#include <set>
#include <algorithm>

using lli = long long;

std::map<std::pair<lli, lli>, std::pair<std::pair<lli, lli>, int>> undiscovered;
std::set<std::pair<lli, lli>> discovered;
std::map<std::pair<lli, lli>, char> map_;
std::map<int, int> dx{ {1, 0}, {2, 0}, {3, -1}, {4, 1} };
std::map<int, int> dy{ {1, -1}, {2, 1}, {3, 0}, {4, 0} };

std::vector<int> move_xy(std::pair<lli, lli> current_pos, std::pair<lli, lli> pos)
{
	if (current_pos == pos) return {};

	std::map<std::pair<lli, lli>, std::vector<int>> directions;
	std::set<std::pair<lli, lli>> visited;
	visited.insert({ current_pos });
	directions[current_pos] = {};

	std::map<std::pair<lli, lli>, std::pair<std::pair<lli, lli>, int>> to_visit;

	for (int i = 1; i <= 4; ++i)
	{
		std::pair<lli, lli> next = { current_pos.first + dx[i], current_pos.second + dy[i] };
		if (map_.find(next) != map_.end() && map_[next] == ' ')
		{
			to_visit[next] = { current_pos, i };
		}
	}

	while (true)
	{
		std::map<std::pair<lli, lli>, std::pair<std::pair<lli, lli>, int>> next_visit;

		for (auto visit : to_visit)
		{
			directions[visit.first] = directions[visit.second.first];
			directions[visit.first].push_back(visit.second.second);

			if (visit.first == pos)
			{
				return directions[visit.first];
			}

			visited.insert(visit.first);

			for (int i = 1; i <= 4; ++i)
			{
				std::pair<lli, lli> next = { visit.first.first + dx[i], visit.first.second + dy[i] };
				if (visited.find(next) == visited.end() && map_.find(next) != map_.end() && map_[next] == ' ')
				{
					next_visit[next] = { visit.first, i };
				}
			}
		}

		to_visit = next_visit;
	}
}

std::map<lli, lli> context;
std::pair<lli, lli> position;

int intcode(int input)
{
	std::map<lli, lli>& inputs = context;

	lli i = 0;
	lli relative = 0;
	while (true)
	{
		int mode1 = inputs[i] / 100 % 10;
		int mode2 = inputs[i] / 1000 % 10;
		int mode3 = inputs[i] / 10000;
		int code = inputs[i] % 100;

		lli* op1 = nullptr;
		lli* op2 = nullptr;
		lli* op3 = nullptr;

		{
			switch (mode1)
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

			switch (mode2)
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

			switch (mode3)
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
		}

		switch (code)
		{
		case 1:
			//Adding
			*op3 = *op1 + *op2;
			i += 4;
			break;
		case 2:
			//Multiplication
			*op3 = (*op1) * (*op2);
			i += 4;
			break;
		case 3:
			//INPUT
			*op1 = input;
			i += 2;
			break;
		case 4:
			//Output
			return *op1;
			i += 2;
			break;
		case 5:
			//Conditional jump
			if (*op1 != 0)
				i = *op2;
			else
				i += 3;
			break;
		case 6:
			//Conditional jump
			if (*op1 == 0)
				i = *op2;
			else
				i += 3;
			break;
		case 7:
			//Less
			*op3 = *op1 < *op2;
			i += 4;
			break;
		case 8:
			//Equal
			*op3 = *op1 == *op2;
			i += 4;
			break;
		case 9:
			//Moving relative operator
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
	std::ifstream ifs("Text.txt");
	std::istream_iterator<lli> begin(ifs), end;
	std::vector<lli> inputs_{ begin, end };
	ifs.close();

	for (lli i = 0; i < inputs_.size(); ++i)
		context[i] = inputs_[i];

	position = { 0, 0 };
	map_.clear();
	undiscovered.clear();
	discovered.clear();
	map_[position] = ' ';

	discovered.insert(position);
	undiscovered.insert({ { 0, -1 }, {{0, 0}, 1} });
	undiscovered.insert({ { 0,  1 }, {{0, 0}, 2} });
	undiscovered.insert({ { -1, 0 }, {{0, 0}, 3} });
	undiscovered.insert({ {  1, 0 }, {{0, 0}, 4} });

	int result;

	while (true)
	{
		auto undis = *undiscovered.begin();
		undiscovered.erase(undiscovered.begin());

		auto moves = move_xy(position, undis.second.first);

		for (auto m : moves)
		{
			intcode(m);
			position.first += dx[m];
			position.second += dy[m];
		}

		int res = intcode(undis.second.second);
		if (res == 0)
		{
			discovered.insert(undis.first);
			map_[undis.first] = '#';
		}
		if (res == 1)
		{
			discovered.insert(undis.first);
			position = undis.first;

			for (int i = 1; i <= 4; ++i)
			{
				std::pair<lli, lli> next = { undis.first.first + dx[i], undis.first.second + dy[i] };
				if (discovered.find(next) == discovered.end())
				{
					undiscovered.insert({ next, {undis.first, i} });
				}
			}

			map_[undis.first] = ' ';
		}
		if (res == 2)
		{
			discovered.insert(undis.first);

			map_[undis.first] = 'o';
			position.first += dx[undis.second.second];
			position.second += dy[undis.second.second];
			result = move_xy(position, { 0, 0 }).size();
			break;
		}
	}

	std::cout << "First: " << result << std::endl;
}

int recursive_oxygen_fill(std::set<std::pair<lli, lli>>& visited, std::pair<lli, lli> pos)
{
	visited.insert(pos);
	if (map_[pos] == '#') return 0;

	int max = 0;

	for (int i = 1; i <= 4; ++i)
	{
		std::pair<lli, lli> next = { pos.first + dx[i], pos.second + dy[i] };

		if (visited.find(next) == visited.end())
		{
			int res = recursive_oxygen_fill(visited, next);
			if (res > max)
			{
				max = res;
			}
		}
	}

	return max + 1;
}

void t2()
{
	std::ifstream ifs("Text.txt");
	std::istream_iterator<lli> begin(ifs), end;
	std::vector<lli> inputs_{ begin, end };
	ifs.close();

	for (lli i = 0; i < inputs_.size(); ++i)
		context[i] = inputs_[i];

	position = { 0, 0 };
	map_.clear();
	undiscovered.clear();
	discovered.clear();
	map_[position] = ' ';

	discovered.insert(position);
	undiscovered.insert({ { 0, -1 }, {{0, 0}, 1} });
	undiscovered.insert({ { 0,  1 }, {{0, 0}, 2} });
	undiscovered.insert({ { -1, 0 }, {{0, 0}, 3} });
	undiscovered.insert({ {  1, 0 }, {{0, 0}, 4} });

	int res = 0;

	std::pair<lli, lli> tank_position;

	while (!undiscovered.empty())
	{
		auto undis = *undiscovered.begin();
		undiscovered.erase(undiscovered.begin());

		auto moves = move_xy(position, undis.second.first);

		for (auto m : moves)
		{
			intcode(m);
			position.first += dx[m];
			position.second += dy[m];
		}

		int res = intcode(undis.second.second);
		if (res == 0)
		{
			discovered.insert(undis.first);
			map_[undis.first] = '#';
		}
		if (res == 1 || res == 2)
		{
			discovered.insert(undis.first);
			position = undis.first;

			for (int i = 1; i <= 4; ++i)
			{
				std::pair<lli, lli> next = { undis.first.first + dx[i], undis.first.second + dy[i] };
				if (discovered.find(next) == discovered.end())
				{
					undiscovered.insert({ next, {undis.first, i} });
				}
			}

			map_[undis.first] = res == 1 ? ' ' : 'o';
			if (res == 2)
			{
				tank_position = position;
			}
		}
	}

	std::set<std::pair<lli, lli>> visited;
	std::cout << "Second: " << recursive_oxygen_fill(visited, tank_position) - 1;
}

/*
 * First: 210
 * Second: 290
 */

int main(int argc, char* argv[])
{
	t1();
	t2();
}