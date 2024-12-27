#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() {}

// CSVのDBを読み込み、_btcRatesに格納
BitcoinExchange::BitcoinExchange(const std::string &filename) {
	loadDatabase(filename);
}

BitcoinExchange::~BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) {
	*this = other;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other) {
	if (this != &other)
	{
		_btcRates = other._btcRates;
	}
	return *this;
}

// DBの読み込み
void BitcoinExchange::loadDatabase(const std::string &filename) {
	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cerr << "Error: Could not open file." << std::endl;
		return;
	}
	std::string line;
	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::string date;
		double rate;

		if (std::getline(iss, date, ',') && iss >> rate)
		{
			_btcRates[date] = rate;
		}
	}
	file.close();
}

double BitcoinExchange::getRate(const std::string &date) const {
	std::map<std::string,double>::const_iterator it =_btcRates.lower_bound(date);
	// データが空の場合
	if (_btcRates.empty())
		throw std::runtime_error("Error: No data available.");

	// lower_boundがDBの終端を指している場合（指定日付がDB内の全ての日付より大きい場合）
	if (it == _btcRates.end())
		--it;

	// lower_boundが指定日付よりも大きい日付を指している場合、1つ前に移動して最も近い過去の日付を取得
	if (it != _btcRates.begin() && it->first > date)
		--it;

	return it->second;
}

void BitcoinExchange::processInputFile(const std::string &filename) const {
	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cerr << "Error: Could not open file." << std::endl;
		return;
	}

	std::string line;
	std::getline(file, line); // skip the first line
	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::string date, valueStr;

		if (std::getline(iss, date, '|') && std::getline(iss, valueStr))
		{
			try
			{
				date.erase(remove(date.begin(), date.end(), ' '), date.end());
				double value = std::stod(valueStr);

				if (validateRate(value) == false)
				{
					continue;
				}
				if (validateDate(date) == false)
				{
					std::cerr << "Error: bad input => " << line << std::endl;
					continue;
				}

				double rate = getRate(date);
				std::cout << date << " => " << value << " = " << value * rate << std::endl;
			}
			catch(const std::exception& e)
			{
				std::cerr << "Error: bad input => " << line << std::endl;
			}
		}
		else
		{
			std::cerr << "Error: bad input => " << line << std::endl;
		}
	}
}

bool BitcoinExchange::validateDate(const std::string &date) const {
	// yyyy-mm-dd
	if (date.size() != 10)
		return false;
	if (date[4] != '-' || date[7] != '-')
		return false;

	int year = std::stoi(date.substr(0, 4));
	int month = std::stoi(date.substr(5, 2));
	int day = std::stoi(date.substr(8, 2));

	if (year < 0)
		return false;
	if (month < 1 || month > 12)
		return false;

	static const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if (day < 1 || day > daysInMonth[month - 1])
		return false;

	return true;
}

bool BitcoinExchange::validateRate(const double &rate) const {
	if (rate < 0)
	{
		std::cerr << "Error: not a positive number." << std::endl;
		return false;
	}
	else if (rate > 1000)
	{
		std::cerr << "Error: too large a number." << std::endl;
		return false;
	}
	return true;
}