#include "PmergeMe.hpp"

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe& other) { *this = other; }

PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
	if (this != &other) {
		this->vector = other.vector;
		this->list = other.list;
	}
	return *this;
}

PmergeMe::~PmergeMe() {}

int PmergeMe::processInput(int argc, char** argv)
{
	if (argc < 2) {
		std::cerr << "Usage: ./PmergeMe [positive numbers...]" << std::endl;
		return 1;
	}

	std::set<int> seen;

	for (int i = 1; i < argc; ++i) {
		std::string str(argv[i]);
		if (!std::all_of(str.begin(), str.end(), ::isdigit)) {
			std::cerr << "Error: Invalid input '" << str << "'" << std::endl;
			return 1;
		}
		int num = std::stoi(str);
		if (num < 0) {
			std::cerr << "Error: Negative number not allowed" << std::endl;
			return 1;
		}
		if (!seen.insert(num).second) {
			std::cerr << "Error: Duplicate number '" << num << "'" << std::endl;
			return 1;
		}
		vector.push_back(num);
		list.push_back(num);
	}

	if (vector.size() > MAX_LIMIT) {
		std::cerr << "Error: Too many numbers (max " << MAX_LIMIT << ")" << std::endl;
		return 1;
	}

	return 0;
}

void PmergeMe::insertionSortVector(std::vector<int>& vec, int start, int end)
{
	for (int i = start + 1; i <= end; ++i) {
		int key = vec[i];
		int j = i - 1;
		while (j >= start && vec[j] > key) {
			vec[j + 1] = vec[j];
			--j;
		}
		vec[j + 1] = key;
	}
}

void PmergeMe::insertionSortList(std::list<int>::iterator begin, std::list<int>::iterator end)
{
	for (auto it = std::next(begin); it != std::next(end); ++it) {
		auto key = *it;
		auto j = it;
		while (j != begin && *std::prev(j) > key) {
			*j = *std::prev(j);
			--j;
		}
		*j = key;
	}
}

void PmergeMe::mergeVector(std::vector<int>& vec, int start, int mid, int end)
{
	std::vector<int> temp;
	int i = start, j = mid + 1;
	while (i <= mid && j <= end) {
		if (vec[i] <= vec[j]) temp.push_back(vec[i++]);
		else temp.push_back(vec[j++]);
	}
	while (i <= mid) temp.push_back(vec[i++]);
	while (j <= end) temp.push_back(vec[j++]);
	for (int k = 0; k < temp.size(); ++k)
		vec[start + k] = temp[k];
}

void PmergeMe::mergeList(std::list<int>::iterator start, std::list<int>::iterator mid, std::list<int>::iterator end)
{
	std::list<int> temp;
	auto i = start;
	auto j = std::next(mid);
	while (i != std::next(mid) && j != std::next(end)) {
		if (*i <= *j) temp.push_back(*i++);
		else temp.push_back(*j++);
	}
	while (i != std::next(mid)) temp.push_back(*i++);
	while (j != std::next(end)) temp.push_back(*j++);

	auto it = temp.begin();
	for (auto k = start; k != std::next(end); ++k, ++it)
		*k = *it;
}

void PmergeMe::mergeInsertionSortVector(std::vector<int>& vec, int start, int end)
{
	if (end - start < 16) {
		insertionSortVector(vec, start, end);
		return;
	}
	int mid = (start + end) / 2;
	mergeInsertionSortVector(vec, start, mid);
	mergeInsertionSortVector(vec, mid + 1, end);
	mergeVector(vec, start, mid, end);
}

void PmergeMe::mergeInsertionSortList(std::list<int>::iterator start, std::list<int>::iterator end)
{
	if (std::distance(start, end) < 16) {
		insertionSortList(start, end);
		return;
	}
	auto mid = std::next(start, std::distance(start, end) / 2);
	mergeInsertionSortList(start, mid);
	mergeInsertionSortList(std::next(mid), end);
	mergeList(start, mid, end);
}

void PmergeMe::performMergeSortVector(std::vector<int>& vec)
{
	printVector("Before: ");
	auto start = std::chrono::high_resolution_clock::now();
	mergeInsertionSortVector(vec, 0, vec.size() - 1);
	auto end = std::chrono::high_resolution_clock::now();
	vectorDuration = end - start;
	printVector("After: ");
	std::cout << "Time to process a range of " << vec.size() << " elements with std::vector : " << vectorDuration.count() << " us" << std::endl;
}

void PmergeMe::performMergeSortList(std::list<int>& lst)
{
	auto start = std::chrono::high_resolution_clock::now();
	mergeInsertionSortList(lst.begin(), std::prev(lst.end()));
	auto end = std::chrono::high_resolution_clock::now();
	listDuration = end - start;
	std::cout << "Time to process a range of " << lst.size() << " elements with std::list : " << listDuration.count() << " us" << std::endl;
}

void PmergeMe::printVector(const std::string& label)
{
	std::cout << label;
	for (std::vector<int>::iterator it = vector.begin(); it != vector.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl;
}

void PmergeMe::printList()
{
	for (std::list<int>::iterator it = list.begin(); it != list.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl;
}

std::vector<int>& PmergeMe::getVector() { return vector; }
std::list<int>& PmergeMe::getList() { return list; }
