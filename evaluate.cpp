#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <vector>

int budget;

struct Stock{
    std::string name;
    float Yvalue;
    float openPrice;
};

bool myCompare(Stock a, Stock b) {
    return a.Yvalue > b.Yvalue;
}

int main(int argc, char* argv[]) {

    std::cout << "Enter your budget: ";
    std::cin >> budget;
    std::string line;

    std::ifstream myFile;
    float curY, curPrice;
    size_t commaIndex;
    std::string name;

    // Get output from the model
    std::vector<Stock> modelOut;
    myFile.open("sample_output.csv");
    std::getline(myFile, line);
    
    while (std::getline(myFile, line)) {
        commaIndex = line.find(',');
        curY = std::stof(line.substr(commaIndex + 1));
        name = line.substr(0, commaIndex);
        Stock cur;
        cur.name = name;
        cur.Yvalue = curY;
        cur.openPrice = curPrice;
        modelOut.push_back(cur);
    }

    std::sort(modelOut.begin(), modelOut.end(), myCompare);
    myFile.close();

    // Pick the top stocks to buy
    
    int numOfCurStock;
    std::vector<std::pair<std::string, int> > result;
    for (int i = 0; i < modelOut.size(); i++) {
        numOfCurStock = budget / modelOut[i].openPrice;
        if (numOfCurStock == 0) continue;
        budget -= numOfCurStock * modelOut[i].openPrice;
        std::pair<std::string, int> curStock;
        curStock.first = modelOut[i].name;
        curStock.second = numOfCurStock;
        result.push_back(curStock);
    }

    // Print out the result
    for (int i = 0; i < result.size(); i++) {
        std::cout << "Buy " << result[i].second << " shares of " << result[i].first << "\n";
    }

    return 0;
}