#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "nlohmann/json.hpp"
#include "../lib/httplib.h"

using json = nlohmann::json;

class CryptoInfo {
public:
    CryptoInfo(const std::string& currency) : currency_(currency) {}
    CryptoInfo(const std::string& api_key, const std::string& currency) : api_key_(api_key), currency_(currency) {}

    std::string GetKey() const noexcept {
        return api_key_;
    }

    std::string GetValue() const noexcept {
        return currency_;
    }

    json GetFile() const noexcept {
        return last_data_;
    }

    void setValue(const std::string& currency) noexcept {
        currency_ = currency;
    }

    void printInfoWithUpd() noexcept {
        while(true) {
            try {
                printInfo();
            } catch (const std::exception& e) {
                std::cerr << e.what() << '\n';
            }
            std::this_thread::sleep_for(std::chrono::seconds(15));
        }
    }

private:
    std::string api_key_;
    std::string currency_;
    double old_price_ = 0;
    double new_price_;
    json last_data_;

    void parseJson() {
        httplib::SSLClient client("api.coingecko.com", 443);
        client.set_follow_location(true);
        std::string path = "/api/v3/simple/price?ids=" + currency_ + "&vs_currencies=usd";

        auto response = client.Get(path.c_str());

        if(response && response->status == 200) {
            last_data_ = json::parse(response->body);
        } else {
            throw std::runtime_error(" Ошибка при получении данных\n");
        }
    }


    void checkDiff(double change) const noexcept {
        double percent = (change / old_price_) * 100.0;
        if (change > 0) {
            std::cout << "\033[32m (+" << percent << "% ↑)\033[0m\n";
        } else if (change < 0) {
            std::cout << "\033[31m (" << percent << "% ↓)\033[0m\n";
        }
    }

    void printInfo() {
        parseJson();

        if (!last_data_.contains(currency_) || last_data_[currency_]["usd"].is_null()) {
            std::cerr << "Ошибка: данные недоступны для " << currency_ << "\n";
            return;
        }

        new_price_ = last_data_[currency_]["usd"];
        std::cout << currency_ << ": $" << new_price_;

        if (old_price_ != 0) {
            double change = new_price_ - old_price_;
            if (change != 0) {
                checkDiff(change);
            } else {
                std::cout << " (без изменений)\n";
            }
        } else {
            std::cout << " (без изменений)\n";
        }

        old_price_ = new_price_;
    }
};