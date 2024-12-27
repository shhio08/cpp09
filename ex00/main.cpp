#include "BitcoinExchange.hpp"

int main(int argc, char **argv) {
    // 引数が正しいか確認
    if (argc != 2) {
        std::cerr << "Error: wrong number of arguments." << std::endl;
        return 1;
    }

    try {
        // データベースのファイルパスを指定してBitcoinExchangeインスタンスを作成
        BitcoinExchange exchange("database.csv");

        // 入力ファイルを処理
        exchange.processInputFile(argv[1]);
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}