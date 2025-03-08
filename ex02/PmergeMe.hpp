#pragma once

#include <vector>
#include <deque>
#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <cstdlib>
#include <climits>

class PmergeMe {
private:
    std::vector<int> _vec;
    std::deque<int> _deq;
    std::vector<int> _originalSequence;

    // vectorを使用したFord-Johnsonソート
    void fordJohnsonSortVector(std::vector<int>& vec, int begin, int end);
    
    // dequeを使用したFord-Johnsonソート
    void fordJohnsonSortDeque(std::deque<int>& deq, int begin, int end);

public:
    PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();

    // 引数を解析し、整数シーケンスを取得
    bool parseArgs(int argc, char** argv);
    
    // シーケンスをソート
    void sort();
    
    // 結果を表示
    void displayResults(double vecTime, double deqTime);
    
    // シーケンスの表示
    void printOriginalSequence();
    void printSortedSequence();

    // ヤコブスタール数列を生成する関数
    std::vector<size_t> generateJacobsthal(size_t n);

    // 挿入順序を生成する関数
    std::vector<size_t> getInsertionOrder(size_t size);

};
