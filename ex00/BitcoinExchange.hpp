#pragma once

#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <stdexcept>


class BitcoinExchange {
private:
	//mapに格納されたデータは、自動的にキー(この場合は日付)の昇順にソートされる
	std::map<std::string, double> _btcRates;
	void loadDatabase(const std::string &filename);
public:
	BitcoinExchange();
	BitcoinExchange(const std::string &filename);
	~BitcoinExchange();
	BitcoinExchange(const BitcoinExchange &other);
	BitcoinExchange &operator=(const BitcoinExchange &other);

	double getRate(const std::string &date) const;
	void processInputFile(const std::string &filename) const;

	bool validateDate(const std::string &date) const;
	bool validateRate(const double &rate) const;
};