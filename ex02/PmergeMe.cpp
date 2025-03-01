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
    if (end - begin <= 1)
        return;
    
    // ペアに分けて、各ペアでソート
    for (int i = begin; i < end - 1; i += 2) {
        if (vec[i] > vec[i + 1])
            std::swap(vec[i], vec[i + 1]);
    }
    
    // 大きいほうの要素でマージソート（ペアの右側）
    int pairs = (end - begin) / 2;
    std::vector<int> largerElements(pairs);
    
    for (int i = 0; i < pairs; ++i) {
        largerElements[i] = vec[begin + i * 2 + 1];
    }
    
    // 再帰的に大きい要素をソート
    fordJohnsonSortVector(largerElements, 0, pairs);
    
    // ソートした大きい要素を元の配列に戻す
    for (int i = 0; i < pairs; ++i) {
        vec[begin + i * 2 + 1] = largerElements[i];
    }
    
    // 小さい方の要素を挿入
    insertSortedVector(vec, begin, end);
}

void PmergeMe::insertSortedVector(std::vector<int>& vec, int begin, int end) {
    // 最初のペアの小さい方（主塊の最初の要素）はすでに正しい位置にある
    std::vector<int> mainChain;
    std::vector<int> pendElements;
    
    // ペアの大きい要素を主塊に、小さい要素を保留要素に追加
    for (int i = begin; i < end; i += 2) {
        if (i + 1 < end) {
            mainChain.push_back(vec[i + 1]);  // 大きい方
            pendElements.push_back(vec[i]);   // 小さい方
        } else {
            // 奇数の場合、最後の要素を主塊に追加
            mainChain.push_back(vec[i]);
        }
    }
    
    // ジャコビアン数列を使用して挿入順序を決定（簡略化のため、定位置挿入を使用）
    std::vector<int> sortedSequence;
    sortedSequence.push_back(pendElements[0]);  // 最初の要素
    
    for (size_t i = 0; i < mainChain.size(); ++i) {
        sortedSequence.push_back(mainChain[i]);
        
        if (i + 1 < pendElements.size()) {
            // 保留要素を二分探索で適切な位置に挿入
            int element = pendElements[i + 1];
            std::vector<int>::iterator pos = std::lower_bound(sortedSequence.begin(), sortedSequence.end(), element);
            sortedSequence.insert(pos, element);
        }
    }
    
    // 結果を元の配列にコピー
    for (size_t i = 0; i < sortedSequence.size(); ++i) {
        vec[begin + i] = sortedSequence[i];
    }
}

void PmergeMe::mergeSortedVector(std::vector<int>& vec, int begin, int mid, int end) {
    std::vector<int> temp(end - begin);
    int i = begin;
    int j = mid;
    int k = 0;
    
    while (i < mid && j < end) {
        if (vec[i] <= vec[j]) {
            temp[k++] = vec[i++];
        } else {
            temp[k++] = vec[j++];
        }
    }
    
    while (i < mid) {
        temp[k++] = vec[i++];
    }
    
    while (j < end) {
        temp[k++] = vec[j++];
    }
    
    for (i = 0; i < k; ++i) {
        vec[begin + i] = temp[i];
    }
}

// ----- Deque実装 -----

void PmergeMe::fordJohnsonSortDeque(std::deque<int>& deq, int begin, int end) {
    if (end - begin <= 1)
        return;
    
    // ペアに分けて、各ペアでソート
    for (int i = begin; i < end - 1; i += 2) {
        if (deq[i] > deq[i + 1])
            std::swap(deq[i], deq[i + 1]);
    }
    
    // 大きいほうの要素でマージソート（ペアの右側）
    int pairs = (end - begin) / 2;
    std::deque<int> largerElements(pairs);
    
    for (int i = 0; i < pairs; ++i) {
        largerElements[i] = deq[begin + i * 2 + 1];
    }
    
    // 再帰的に大きい要素をソート
    fordJohnsonSortDeque(largerElements, 0, pairs);
    
    // ソートした大きい要素を元の配列に戻す
    for (int i = 0; i < pairs; ++i) {
        deq[begin + i * 2 + 1] = largerElements[i];
    }
    
    // 小さい方の要素を挿入
    insertSortedDeque(deq, begin, end);
}

void PmergeMe::insertSortedDeque(std::deque<int>& deq, int begin, int end) {
    // 最初のペアの小さい方（主塊の最初の要素）はすでに正しい位置にある
    std::deque<int> mainChain;
    std::deque<int> pendElements;
    
    // ペアの大きい要素を主塊に、小さい要素を保留要素に追加
    for (int i = begin; i < end; i += 2) {
        if (i + 1 < end) {
            mainChain.push_back(deq[i + 1]);  // 大きい方
            pendElements.push_back(deq[i]);   // 小さい方
        } else {
            // 奇数の場合、最後の要素を主塊に追加
            mainChain.push_back(deq[i]);
        }
    }
    
    // ジャコビアン数列を使用して挿入順序を決定（簡略化のため、定位置挿入を使用）
    std::deque<int> sortedSequence;
    sortedSequence.push_back(pendElements[0]);  // 最初の要素
    
    for (size_t i = 0; i < mainChain.size(); ++i) {
        sortedSequence.push_back(mainChain[i]);
        
        if (i + 1 < pendElements.size()) {
            // 保留要素を二分探索で適切な位置に挿入
            int element = pendElements[i + 1];
            std::deque<int>::iterator pos = std::lower_bound(sortedSequence.begin(), sortedSequence.end(), element);
            sortedSequence.insert(pos, element);
        }
    }
    
    // 結果を元の配列にコピー
    for (size_t i = 0; i < sortedSequence.size(); ++i) {
        deq[begin + i] = sortedSequence[i];
    }
}

void PmergeMe::mergeSortedDeque(std::deque<int>& deq, int begin, int mid, int end) {
    std::deque<int> temp(end - begin);
    int i = begin;
    int j = mid;
    int k = 0;
    
    while (i < mid && j < end) {
        if (deq[i] <= deq[j]) {
            temp[k++] = deq[i++];
        } else {
            temp[k++] = deq[j++];
        }
    }
    
    while (i < mid) {
        temp[k++] = deq[i++];
    }
    
    while (j < end) {
        temp[k++] = deq[j++];
    }
    
    for (i = 0; i < k; ++i) {
        deq[begin + i] = temp[i];
    }
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
    double vecTime = static_cast<double>(vecEnd - vecStart) / CLOCKS_PER_SEC * 1000000;
    double deqTime = static_cast<double>(deqEnd - deqStart) / CLOCKS_PER_SEC * 1000000;
    
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
