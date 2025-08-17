#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <sstream>
#include <iterator>
#include <chrono>
#include <set>
#include <algorithm>

#define MAX_LIMIT 3000

class PmergeMe {
private:
	std::vector<int> vector;
	std::list<int> list;
	std::chrono::duration<double, std::micro> vectorDuration;
	std::chrono::duration<double, std::micro> listDuration;

	void insertionSortVector(std::vector<int>& vec, int start, int end);
	void insertionSortList(std::list<int>::iterator begin, std::list<int>::iterator end);

	void mergeVector(std::vector<int>& vec, int start, int mid, int end);
	void mergeList(std::list<int>::iterator start, std::list<int>::iterator mid, std::list<int>::iterator end);

	void mergeInsertionSortVector(std::vector<int>& vec, int start, int end);
	void mergeInsertionSortList(std::list<int>::iterator start, std::list<int>::iterator end);

public:
	PmergeMe();
	PmergeMe(const PmergeMe& other);
	PmergeMe& operator=(const PmergeMe& other);
	~PmergeMe();

	int processInput(int argc, char** argv);

	void performMergeSortVector(std::vector<int>& vec);
	void performMergeSortList(std::list<int>& list);

	void printVector(const std::string& label);
	void printList();
};

#endif
