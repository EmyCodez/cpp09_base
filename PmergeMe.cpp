#include "PmergeMe.hpp"

PmergeMe::PmergeMe() { }

PmergeMe::PmergeMe(const PmergeMe &other) { *this = other; }

PmergeMe& PmergeMe::operator=(const PmergeMe &other)
{
	if (this != &other)
	{
		this->vector = other.vector;
		this->list = other.list;
	}
	return *this;
}

PmergeMe::~PmergeMe() { }

void PmergeMe::setVector(std::vector<int>& v) { this->vector = v; }
void PmergeMe::setList(std::list<int>& l) { this->list = l; }

std::vector<int>& PmergeMe::getVector() { return this->vector; }
std::list<int>& PmergeMe::getList() { return this->list; }

int PmergeMe::processInput(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cerr << "Usage: ./PmergeMe [positive integers...]" << std::endl;
		return 1;
	}

	std::set<int> seen;

	for (int i = 1; i < argc; ++i)
	{
		std::string arg(argv[i]);
		for (size_t j = 0; j < arg.size(); ++j)
		{
			if (!std::isdigit(arg[j]))
			{
				std::cerr << "Error: Invalid input '" << arg << "'" << std::endl;
				return 1;
			}
		}

		int num = std::atoi(argv[i]);
		if (num < 0)
		{
			std::cerr << "Error: Negative number" << std::endl;
			return 1;
		}
		if (!seen.insert(num).second)
		{
			std::cerr << "Error: Duplicate number '" << num << "'" << std::endl;
			return 1;
		}
		vector.push_back(num);
		list.push_back(num);
	}

	if (vector.size() > MAX_LIMIT)
	{
		std::cerr << "Error: Too many elements (limit is " << MAX_LIMIT << ")" << std::endl;
		return 1;
	}

	return 0;
}

void PmergeMe::printVector(std::string word)
{
	std::cout << word;
	for (size_t i = 0; i < vector.size(); ++i)
		std::cout << vector[i] << " ";
	std::cout << std::endl;
}

void PmergeMe::printList()
{
	std::cout << "List: ";
	for (std::list<int>::iterator it = list.begin(); it != list.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl;
}

std::vector<int> PmergeMe::generateJacobsthalSequence(int n)
{
	std::vector<int> seq;
	seq.push_back(1);
	int j1 = 1, j2 = 0;

	while (true)
	{
		int j = j1 + 2 * j2;
		if (j >= n)
			break;
		seq.push_back(j);
		j2 = j1;
		j1 = j;
	}

	for (int i = 1; i < n; ++i)
	{
		if (std::find(seq.begin(), seq.end(), i) == seq.end())
			seq.push_back(i);
	}
	return seq;
}

void PmergeMe::fordJohnsonVector(std::vector<int>& data)
{
	if (data.size() <= 1)
		return;

	std::vector<int> mainChain;
	std::vector<int> pending;

	for (size_t i = 0; i + 1 < data.size(); i += 2)
	{
		if (data[i] < data[i + 1])
			std::swap(data[i], data[i + 1]);
		mainChain.push_back(data[i]);
		pending.push_back(data[i + 1]);
	}

	if (data.size() % 2 == 1)
		pending.push_back(data.back());

	fordJohnsonVector(mainChain);

	if (!pending.empty())
		mainChain.insert(std::lower_bound(mainChain.begin(), mainChain.end(), pending[0]), pending[0]);

	std::vector<int> jacob = generateJacobsthalSequence(pending.size());

	for (size_t i = 1; i < jacob.size(); ++i)
	{
		if (jacob[i] < (int)pending.size())
		{
			int val = pending[jacob[i]];
			auto pos = std::lower_bound(mainChain.begin(), mainChain.end(), val);
			mainChain.insert(pos, val);
		}
	}
	data = mainChain;
}

void PmergeMe::fordJohnsonList(std::list<int>& data)
{
	if (data.size() <= 1)
		return;

	std::list<int> mainChain;
	std::vector<int> pending;

	auto it = data.begin();
	while (it != data.end())
	{
		int first = *it;
		++it;
		if (it != data.end())
		{
			int second = *it;
			if (first < second)
				std::swap(first, second);
			mainChain.push_back(first);
			pending.push_back(second);
			++it;
		}
		else
		{
			pending.push_back(first);
		}
	}

	fordJohnsonList(mainChain);

	if (!pending.empty())
		mainChain.insert(std::lower_bound(mainChain.begin(), mainChain.end(), pending[0]), pending[0]);

	std::vector<int> jacob = generateJacobsthalSequence(pending.size());

	for (size_t i = 1; i < jacob.size(); ++i)
	{
		if (jacob[i] < (int)pending.size())
		{
			int val = pending[jacob[i]];
			mainChain.insert(std::lower_bound(mainChain.begin(), mainChain.end(), val), val);
		}
	}
	data = mainChain;
}

void PmergeMe::performMergeSortVector(std::vector<int>& vec)
{
	printVector("Before: ");
	auto start = std::chrono::high_resolution_clock::now();
	fordJohnsonVector(vec);
	auto end = std::chrono::high_resolution_clock::now();
	vectorDuration = end - start;
	printVector("After: ");
	std::cout << "Time to process a range of " << vec.size() << " elements with std::vector : " << vectorDuration.count() << " us" << std::endl;
}

void PmergeMe::performMergeSortList(std::list<int>& lst)
{
	auto start = std::chrono::high_resolution_clock::now();
	fordJohnsonList(lst);
	auto end = std::chrono::high_resolution_clock::now();
	listDuration = end - start;
	std::cout << "Time to process a range of " << lst.size() << " elements with std::list : " << listDuration.count() << " us" << std::endl;
}

