#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <limits>
#include <iomanip>
#include <algorithm>
#include <chrono>
#include <unordered_map>
#include <unordered_set>

const std::string filePath = "input.txt";
const std::string outputFilePath = "output.txt";
int main() {


    auto startTime = std::chrono::high_resolution_clock::now();

    std::ifstream file(filePath);

    std::unordered_map<std::string, double> citySumMap;
    std::unordered_map<std::string, std::unordered_map<std::string, double>> cityProductMinPriceMap;

    double minTotalPrice = std::numeric_limits<double>::max();
    std::string cityWithMinPrice;

    std::vector<std::tuple<std::string, std::string, double>> allProducts;
    std::vector<std::string> printedProducts;

    std::string line;

while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string city, product;
        double price = 0.0;
        std::getline(iss, city, ',');
        std::getline(iss, product, ',');
        if (iss >> price) {
            citySumMap[city] += price;

            double& minPrice = cityProductMinPriceMap[city][product];
            minPrice = (minPrice == 0.0) ? price : std::min(minPrice, price);

            allProducts.emplace_back(city, product, price);
        }
    }

    file.close();

    std::ofstream outputFile(outputFilePath);

    for (const auto& citySumPair : citySumMap) {
        const std::string& city = citySumPair.first;
        double sum = citySumPair.second;

        if (sum < minTotalPrice) {
            minTotalPrice = sum;
            cityWithMinPrice = city;
        }
    }

    if (!cityWithMinPrice.empty()) {
        outputFile << cityWithMinPrice << " " << std::fixed << std::setprecision(2) << minTotalPrice << std::endl;

        std::sort(allProducts.begin(), allProducts.end(),
                  [](const auto& a, const auto& b) {
                      return std::get<2>(a) < std::get<2>(b) ||
                             (std::get<2>(a) == std::get<2>(b) && std::get<1>(a) < std::get<1>(b));
                  });

        int count = 0;
        for (const auto& productInfo : allProducts) {
            if (std::get<0>(productInfo) != cityWithMinPrice ||
                count >= 5 ||
                std::find(printedProducts.begin(), printedProducts.end(), std::get<1>(productInfo)) != printedProducts.end()) {
                continue;
            }

            outputFile << std::get<1>(productInfo) << " " << std::fixed << std::setprecision(2) << cityProductMinPriceMap[cityWithMinPrice][std::get<1>(productInfo)] << std::endl;
            printedProducts.push_back(std::get<1>(productInfo));
            ++count;
        }
    }

    outputFile.close();

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    std::cout << "Execution time: " << duration << " milliseconds" << std::endl;

    return 0;
}
