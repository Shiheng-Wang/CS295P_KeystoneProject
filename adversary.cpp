#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <vector>
#include <unordered_map>

std::unordered_map<std::string, double> earnings;
double profit = 0.0;

bool myCompare(std::pair<std::string, float> a, std::pair<std::string, float> b)
{
    return a.second > b.second;
}

void getNameAndShare(std::string line) {
    int shares;
    std::string name;
    size_t spaceIdx;
    for (int i = 0; i < 6; i++) {
        spaceIdx = line.find(' ');
        if (i == 3) {
            shares = std::stoi(line.substr(0, spaceIdx));
        }
        line = line.substr(spaceIdx + 1);
    }
    name = line;
    profit += earnings[name] * shares;
}

int main(int argc, char *argv[])
{
    std::ifstream inFile;
    std::string line;


    // Read the open prices at the beginning of the month
    // and the close prices at the end of the month
    inFile.open("openClose1112.txt");
    size_t commaIdx1, commaIdx2;
    std::string name;
    double openPrice, closePrice;
    while (std::getline(inFile, line)) {
        commaIdx1 = line.find(',');
        commaIdx2 = line.find(',', commaIdx1 + 1);
        name = line.substr(0, commaIdx1);
        openPrice = std::stod(line.substr(commaIdx1 + 1, commaIdx2 - commaIdx1 - 1));
        closePrice = std::stod(line.substr(commaIdx2 + 1));
        earnings.insert(std::pair<std::string, double>(name, openPrice - closePrice));
    }
    inFile.close();

    inFile.open("output12.txt");
    while (std::getline(inFile, line)) {
        // std::cout << line << "\n";
        getNameAndShare(line);
    }
    inFile.close();

    std::cout << "The total profit is " << profit << "\n";
    std::cout << "The difference of GSPC is " << earnings["^GSPC"] << "\n";
    std::cout << "The difference of our earnings and GSPC is " << profit - earnings["^GSPC"] << "\n";

    return 0;
}