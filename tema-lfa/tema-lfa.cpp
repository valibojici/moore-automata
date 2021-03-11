#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>

std::vector<std::string> split(const std::string& s)
{
	std::vector<std::string> result;
	int poz = s.find(' ');
	int old_poz = 0;
	if (poz == std::string::npos)
	{
		for (char c : s)
			result.push_back(std::string(1,c));
		return result;
	}
	while (poz != std::string::npos)
	{
		result.push_back(s.substr(old_poz, poz - old_poz));
		old_poz = poz + 1;
		poz = s.find(' ', old_poz);
	}
	result.push_back(s.substr(old_poz, s.size() - old_poz));
	return result;
}

class Moore {
private:
	std::vector<std::string>node_vals;
	int nodes, edges;
	int start_node;
	std::vector<int> end_nodes;
	std::vector< std::vector<std::pair<int, std::string> > > ad_list;
	std::vector<std::string> words;

	std::pair<std::string, std::vector<int> > check_word(int index)
	{
		std::queue<std::pair<int,int> > q;
		std::string output;
		std::vector<int> history;
		
		auto word = split(words[index]);

		q.push({ 0, 0 });
		while (!q.empty())
		{
			int x = q.front().first;
			int word_idx = q.front().second;
			q.pop();

			history.push_back(x);
			output += node_vals[x];

			if (word_idx == word.size()) // daca s-a terminat cuvantul
			{
				for (int node : end_nodes)
					if (node == x)// daca nodul curent e stare finala atunci return
					{
						return { output, history };
					}
				continue;
			}

			for (auto& neighbor : ad_list[x])
			{
				std::string s;
				s += word[word_idx];
	
				if (neighbor.second == s)
				{
					q.push({ neighbor.first,word_idx + 1 });
				}
			}
		}
		// daca nu s-a gasit un drum
		return { "", std::vector<int>() };
	}
public:
	Moore() : nodes(0), edges(0), start_node(-1) {}
	Moore(std::istream& in)
	{
		in >> nodes >> edges;
		node_vals.reserve(nodes);
		ad_list.resize(nodes);
		for (int i = 0; i < nodes; ++i)
		{
			std::string val;
			in >> val;
			node_vals.push_back(val);
		}

		for (int i = 0; i < edges; ++i)
		{
			int x, y;
			std::string edge_val;
			in >> x >> y >> edge_val;
			ad_list[x].push_back({ y,edge_val });
		}

		in >> start_node;

		int nr;
		in >> nr;
		for (int i = 0; i < nr; ++i)
		{
			int x;
			in >> x;
			end_nodes.push_back(x);
		}

		in >> nr;
		in.get();
		for (int i = 0; i < nr; ++i)
		{
			std::string word;
			std::getline(in, word);
			words.push_back(word);
		}
	}

	void check_words()
	{
		for (int i=0;i<words.size();++i)
		{
			auto x = check_word(i);
			if (x.first != "")
			{
				std::cout << "DA\n" << x.first << "\nTraseu: ";
				for (int y : x.second)
					std::cout << y << " ";
				std::cout << '\n';
			}
			else
			{	
				std::cout << "NU\n";
			}
		}
	}

	friend std::ostream& operator<<(std::ostream&, const Moore&);
};
 
std::ostream& operator<<(std::ostream& out, const Moore& m)
{
	for (int i = 0; i < m.nodes; ++i)
	{
		out << i << "/" << m.node_vals[i] << " : ";
		for (auto& v : m.ad_list[i])
		{
			out << "(" << v.first << ", " << v.second << ") ";
		}
		out << "\n";
	}

	out << "start : " << m.start_node << '\n';
	out << "end : ";
	for (auto x : m.end_nodes)
	{
		out << x << ' ';
	}
	out << '\n';

	for (const std::string& word : m.words)
	{
		out << word << '\n';
	}
	return out;
}

int main()
{
	std::ifstream fin("input.txt");

	Moore m(fin);
	//std::cout << m;
	m.check_words();

	fin.close();
}
 