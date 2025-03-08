#include "PmergeMe.hpp"

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe& other) {
    *this = other;
}

PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this != &other) {
        _vec = other._vec;
        _deq = other._deq;
        _originalSequence = other._originalSequence;
    }
    return *this;
}

PmergeMe::~PmergeMe() {}

// 引数チェック＆初期化
bool PmergeMe::parseArgs(int argc, char** argv) {
    if (argc < 2)
        return false;
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        // 数値チェック
        for (size_t j = 0; j < arg.length(); ++j) {
            if (!isdigit(arg[j]))
                return false;
        }
        
        // オーバーフローチェック
        long num = std::atol(arg.c_str());
        if (num <= 0 || num > INT_MAX)
            return false;
        
        _originalSequence.push_back(static_cast<int>(num));
    }
    
    // 両方のコンテナに初期データをコピー
    _vec = _originalSequence;
    _deq = std::deque<int>(_originalSequence.begin(), _originalSequence.end());
    
    return true;
}

// ----- Vector実装 -----

void PmergeMe::fordJohnsonSortVector(std::vector<int>& vec, int begin, int end) {
    if (end - begin <= 1) return;
    
    std::vector<std::pair<int, int> > pairs;
    
    // ペアを作成（大きい方を first, 小さい方を second に）
    for (int i = begin; i < end - 1; i += 2) {
        int a = vec[i];
        int b = vec[i + 1];
        if (a > b)
            pairs.push_back(std::make_pair(a, b));
        else
            pairs.push_back(std::make_pair(b, a));
    }
    // メインチェーンとペンド要素を分離
    std::vector<int> mainChain;
    std::vector<int> pendElements;
    
    if ((end - begin) % 2 != 0) {
        mainChain.push_back(vec[end - 1]);
    }
    for (size_t i = 0; i < pairs.size(); i++) {
        mainChain.push_back(pairs[i].first);     // 大きい方をメインチェーンに
        pendElements.push_back(pairs[i].second);  // 小さい方をペンド要素に
    }

    // メインチェーンを再帰的にソート（要素が1つになるまで）
    if (mainChain.size() > 1) {
        fordJohnsonSortVector(mainChain, 0, mainChain.size());
    }
    
    // 結果を一時的な配列に格納
    std::vector<int> result;
    result.reserve(end - begin);
    result = mainChain;
    
    // ペンド要素を最適化された順序で挿入
    if (!pendElements.empty()) {
        if (pendElements.size() == 1) {
            if (pendElements[0] < result[0]) {
                result.insert(result.begin(), pendElements[0]);
            } else {
                result.insert(result.begin() + 1, pendElements[0]);
            }
        } else {
            // 最初のペンド要素は必ずメインチェーンの最小値より小さいので、先頭に挿入
            result.insert(result.begin(), pendElements[0]);
        }
        // 残りのペンド要素を挿入（探索範囲を2倍ずつ増やす）
        size_t searchLimit = 2;
        for (size_t i = 1; i < pendElements.size(); ++i) {
            std::vector<int>::iterator searchEnd = result.begin() + std::min(searchLimit, result.size());
            std::vector<int>::iterator pos = 
                std::lower_bound(result.begin(), searchEnd, pendElements[i]);
            result.insert(pos, pendElements[i]);
            searchLimit *= 2;
        }
    }
    
    // 結果を元の配列にコピー
    std::copy(result.begin(), result.end(), vec.begin() + begin);
}

// ----- Deque実装 -----

void PmergeMe::fordJohnsonSortDeque(std::deque<int>& deq, int begin, int end) {
    if (end - begin <= 1) return;
    
    std::deque<std::pair<int, int> > pairs;
    
    // ペアを作成（大きい方を first, 小さい方を second に）
    for (int i = begin; i < end - 1; i += 2) {
        int a = deq[i];
        int b = deq[i + 1];
        if (a > b)
            pairs.push_back(std::make_pair(a, b));
        else
            pairs.push_back(std::make_pair(b, a));
    }

    // メインチェーンとペンド要素を分離
    std::deque<int> mainChain;
    std::deque<int> pendElements;
    
    if ((end - begin) % 2 != 0) {
        mainChain.push_back(deq[end - 1]);
    }
    for (size_t i = 0; i < pairs.size(); i++) {
        mainChain.push_back(pairs[i].first);     // 大きい方をメインチェーンに
        pendElements.push_back(pairs[i].second);  // 小さい方をペンド要素に
    }

    // メインチェーンを再帰的にソート
    if (mainChain.size() > 1) {
        fordJohnsonSortDeque(mainChain, 0, mainChain.size());
    }
    
    // 結果を一時的な配列に格納
    std::deque<int> result;
    result = mainChain;
    
    // ペンド要素を最適化された順序で挿入
    if (!pendElements.empty()) {
        if (pendElements.size() == 1) {
            if (pendElements[0] < result[0]) {
                result.insert(result.begin(), pendElements[0]);
            } else {
                result.insert(result.begin() + 1, pendElements[0]);
            }
        } else {
            // 最初のペンド要素は必ずメインチェーンの最小値より小さいので、先頭に挿入
            result.insert(result.begin(), pendElements[0]);
        }
        // 残りのペンド要素を挿入（探索範囲を2倍ずつ増やす）
        size_t searchLimit = 2;
        for (size_t i = 1; i < pendElements.size(); ++i) {
            std::deque<int>::iterator searchEnd = result.begin() + std::min(searchLimit, result.size());
            std::deque<int>::iterator pos = 
                std::lower_bound(result.begin(), searchEnd, pendElements[i]);
            result.insert(pos, pendElements[i]);
            searchLimit *= 2;
        }
    }
    
    // 結果を元の配列にコピー
    std::copy(result.begin(), result.end(), deq.begin() + begin);
}

void PmergeMe::sort() {
    // vectorでソート
    clock_t vecStart = clock();
    fordJohnsonSortVector(_vec, 0, _vec.size());
    clock_t vecEnd = clock();
    
    // dequeでソート
    clock_t deqStart = clock();
    fordJohnsonSortDeque(_deq, 0, _deq.size());
    clock_t deqEnd = clock();
    
    // 時間を計算（マイクロ秒単位）
    double vecTime = static_cast<double>(vecEnd - vecStart) / CLOCKS_PER_SEC * 1000000.0;
    double deqTime = static_cast<double>(deqEnd - deqStart) / CLOCKS_PER_SEC * 1000000.0;
    
    // 結果表示
    displayResults(vecTime, deqTime);
}

void PmergeMe::displayResults(double vecTime, double deqTime) {
    std::cout << "Before: ";
    printOriginalSequence();
    
    std::cout << "After: ";
    printSortedSequence();
    
    std::cout << "Time to process a range of " << _originalSequence.size() 
              << " elements with std::vector : " << std::fixed << std::setprecision(5) 
              << vecTime << " us" << std::endl;
    
    std::cout << "Time to process a range of " << _originalSequence.size() 
              << " elements with std::deque : " << std::fixed << std::setprecision(5) 
              << deqTime << " us" << std::endl;
}

void PmergeMe::printOriginalSequence() {
    for (size_t i = 0; i < _originalSequence.size(); ++i) {
        std::cout << _originalSequence[i] << " ";
    }
    std::cout << std::endl;
}

void PmergeMe::printSortedSequence() {
    for (size_t i = 0; i < _vec.size(); ++i) {
        std::cout << _vec[i] << " ";
    }
    std::cout << std::endl;
}
