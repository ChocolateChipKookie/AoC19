#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>
#include <map>
#include <set>
#include <Windows.h>
#include <algorithm>
#include <thread>

using lli = long long;

std::map<std::pair<lli, lli>, std::pair<std::pair<lli, lli>, int>> undiscovered;
std::set<std::pair<lli, lli>> discovered;
std::map<std::pair<lli, lli>, char> map_;
std::map<int, int> dx{ {1, 0}, {2, 0}, {3, -1}, {4, 1} };
std::map<int, int> dy{ {1, -1}, {2, 1}, {3, 0}, {4, 0} };

void gotoxy(int x, int y) {
	COORD pos = { x, y };
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);
}

void print_map(bool print_location, std::pair<lli, lli> location = { 0, 0 })
{
	gotoxy(0, 3);

	std::pair<lli, lli> dx = { map_.begin()->first.first, map_.begin()->first.first };
	std::pair<lli, lli> dy = { map_.begin()->first.second, map_.begin()->first.second };

	for (auto m : map_)
	{
		dx.first = min(dx.first, m.first.first);
		dy.first = min(dy.first, m.first.second);
		dx.second = max(dx.second, m.first.first);
		dy.second = max(dy.second, m.first.second);
	}

	std::string res;
	res.reserve(740);

	for (int y = -21; y <= 19; ++y)
	{
		for (int x = -21; x <= 19; ++x)
		{
			char c;

			if (map_.find({ x, y }) == map_.end())
			{
				c = ' ';
			}
			else
			{
				c = map_[{x, y}];
				if (c == '#')
				{
					c = 178;
				}
			}

			if (print_location)
			{
				if (location == std::pair<lli, lli>{ x, y })
				{
					c = 'x';
				}
			}

			res += c;
		}
		res += '\n';
	}
	std::cout << res;
}

void print_map_not_discovered(std::map<std::pair<lli, lli>, char>& map_not_discovered, bool print_location, std::pair<lli, lli> location = { 0, 0 })
{
	gotoxy(0, 3);

	static int color = 15;

	std::pair<lli, lli> dx = { map_.begin()->first.first, map_.begin()->first.first };
	std::pair<lli, lli> dy = { map_.begin()->first.second, map_.begin()->first.second };

	for (auto m : map_)
	{
		dx.first = min(dx.first, m.first.first);
		dy.first = min(dy.first, m.first.second);
		dx.second = max(dx.second, m.first.first);
		dy.second = max(dy.second, m.first.second);
	}

	std::string res;

	for (int y = -21; y <= 19; ++y)
	{
		for (int x = -21; x <= 19; ++x)
		{
			char c;
			if (map_.find({ x, y }) == map_.end())
			{
				c = map_not_discovered[{ x, y }];
				if (c == 0)
				{
					map_not_discovered[{ x, y }] = '#';
					c = 176;
				}
				if (c == '#')
					c = 176;
			}
			else
			{
				c = map_[{x, y}];
				if (c == '#')
					c = 219;
				if (print_location && location == std::pair<lli, lli>{ x, y })
					c = 'x';
			}
			res += c;
		}
		res += '\n';
	}
	std::cout << res;
}

int get_relation(std::pair<lli, lli> p1, std::pair<lli, lli> p2)
{
	if (p1.first == p2.first && p1.second - 1 == p2.second)
		return 1;
	if (p1.first == p2.first && p1.second + 1 == p2.second)
		return 2;
	if (p1.first - 1 == p2.first && p1.second == p2.second)
		return 3;
	if (p1.first + 1 == p2.first && p1.second == p2.second)
		return 4;
	return 0;
}

std::vector<int> move_xy(std::pair<lli, lli> current_pos, std::pair<lli, lli> pos)
{
	if (current_pos == pos) return {};

	std::map<std::pair<lli, lli>, std::vector<int>> directions;
	std::set<std::pair<lli, lli>> visited;
	visited.insert({ current_pos });
	directions[current_pos] = {};

	std::map<std::pair<lli, lli>, std::pair<std::pair<lli, lli>, int>> to_visit;
	
	for(int i = 1; i <= 4; ++i)
	{
		std::pair<lli, lli> next = { current_pos.first + dx[i], current_pos.second + dy[i] };
		if (map_.find(next) != map_.end() && map_[next] == ' ')
		{
			to_visit[next] =  { current_pos, i };
		}
	}
	
	while(true)
	{
		std::map<std::pair<lli, lli>, std::pair<std::pair<lli, lli>, int>> next_visit;
		
		for(auto visit : to_visit)
		{
			directions[visit.first] = directions[visit.second.first];
			directions[visit.first].push_back(visit.second.second);

			if(visit.first == pos)
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

int intcode(int input = 1)
{
	std::map<lli, lli>& inputs = context;
	
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

	while(true)
	{
		auto undis = *undiscovered.begin();
		undiscovered.erase(undiscovered.begin());

		auto moves = move_xy(position, undis.second.first);

		for(auto m : moves)
		{
			intcode(m);
			position.first += dx[m];
			position.second += dy[m];
		}

		int res = intcode(undis.second.second);
		if(res == 0)
		{
			discovered.insert(undis.first);
			map_[undis.first] = '#';
		}
		if(res == 1)
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
		if(res == 2)
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

		if(visited.find(next) == visited.end())
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

	gotoxy(0, 1);
	std::cout << "Second: " << recursive_oxygen_fill(visited, tank_position) - 1;
}

void fill_oxygen(std::pair<lli, lli> pos, std::map<std::pair<lli, lli>, char>& map_not_discovered)
{
	std::set<std::pair<lli, lli>> visited;

	std::set<std::pair<lli, lli>> to_visit;
	to_visit.insert(pos);

	while (!to_visit.empty())
	{
		std::set<std::pair<lli, lli>> new_to_visit;

		for (auto v : to_visit)
		{
			map_[v] = '0';
			visited.insert(v);

			for (int i = 1; i <= 4; ++i)
			{
				std::pair<lli, lli> next = { v.first + dx[i], v.second + dy[i] };

				if (map_not_discovered[next] == ' ' && visited.find(next) == visited.end())
				{
					new_to_visit.insert(next);
				}
			}
		}

		to_visit = new_to_visit;
		print_map_not_discovered(map_not_discovered, false);
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}
}


std::map<int, int> opposite{{1, 2}, {2, 1}, {3, 4}, {4, 3}};

void display()
{
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
	}

	int dfs;
	std::cout << "\nBFS(0) or DFS(1): ";
	std::cin >> dfs;
	if(dfs == 1)
	{
		std::map<std::pair<lli, lli>, char> map_not_discovered = map_;

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

		std::set<std::pair<lli, lli>> undiscovered;

		undiscovered.insert({ 0, -1 });
		undiscovered.insert({ 0,  1 });
		undiscovered.insert({ -1, 0 });
		undiscovered.insert({ 1, 0 });

		int res = 0;

		std::pair<lli, lli> tank_position;

		std::vector<int> trace;

		while (!undiscovered.empty())
		{
			std::pair<lli, lli> undis;

			int direction = 0;

			for (int i = 1; i <= 4; ++i)
			{
				std::pair<lli, lli> next = { position.first + dx[i], position.second + dy[i] };
				if (undiscovered.find(next) != undiscovered.end())
				{
					undis = *undiscovered.find(next);
					direction = i;
					break;
				}
			}

			if (direction == 0)
			{
				int back_dir = trace.back();
				trace.pop_back();
				intcode(back_dir);
				position.first += dx[back_dir];
				position.second += dy[back_dir];
				print_map_not_discovered(map_not_discovered, true, position);

				continue;
			}

			undiscovered.erase(undiscovered.find(undis));

			int res = intcode(direction);
			if (res == 0)
			{
				discovered.insert(undis);
				map_[undis] = '#';
			}
			else if (res == 1 || res == 2)
			{
				trace.push_back(opposite[direction]);

				discovered.insert(undis);
				position = undis;

				for (int i = 1; i <= 4; ++i)
				{
					std::pair<lli, lli> next = { undis.first + dx[i], undis.second + dy[i] };
					if (discovered.find(next) == discovered.end())
					{
						undiscovered.insert(next);
					}
				}

				map_[undis] = res == 1 ? ' ' : 'o';
				if (res == 2)
				{
					tank_position = position;
					break;
				}
			}
			print_map_not_discovered(map_not_discovered, true, position);
		}
		fill_oxygen(tank_position, map_not_discovered);
	}
	else
	{
	std::map<std::pair<lli, lli>, char> map_not_discovered = map_;

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

	std::pair<lli, lli> tank_position;

	std::vector<int> trace;

	while (!undiscovered.empty())
	{
		std::pair<std::pair<lli, lli>, std::pair<std::pair<lli, lli>, int>> undis = *undiscovered.begin();

		undiscovered.erase(undiscovered.find(undis.first));

		auto moves = move_xy(position, undis.second.first);

		for (auto m : moves)
		{
			intcode(m);
			position.first += dx[m];
			position.second += dy[m];
			print_map_not_discovered(map_not_discovered, true, position);
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
				break;
			}
		}
		print_map_not_discovered(map_not_discovered, true, position);
	}
	fill_oxygen(tank_position, map_not_discovered);
	}

}

/*
 * First: 210
 * Second: 290
 */

int main(int argc, char* argv[])
{
	t1();
	t2();
	display();
}