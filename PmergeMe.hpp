#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <chrono>
#include <algorithm>
#include <set>

#define MAX_LIMIT 3000

class PmergeMe
{
private:
	std::vector<int> vector;
	std::list<int> list;

	std::chrono::duration<double, std::micro> vectorDuration;
	std::chrono::duration<double, std::micro> listDuration;

	// Sorting helpers
	void fordJohnsonVector(std::vector<int>& data);
	void fordJohnsonList(std::list<int>& data);
	std::vector<int> generateJacobsthalSequence(int n);

public:
	PmergeMe();
	PmergeMe(const PmergeMe& other);
	PmergeMe& operator=(const PmergeMe& other);
	~PmergeMe();

	int processInput(int argc, char** argv);

	void performMergeSortVector(std::vector<int>& vec);
	void performMergeSortList(std::list<int>& lst);

	void printVector(std::string word);
	void printList();

	// Setters & Getters
	void setVector(std::vector<int>& vector);
	void setList(std::list<int>& list);
	std::vector<int>& getVector();
	std::list<int>& getList();
};

#endif
