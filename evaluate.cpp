#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <vector>

int budget;

bool myCompare(std::pair<std::string, float> a, std::pair<std::string, float> b) {
    return a.second > b.second;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Enter year and month as command line arguments.\n";
        exit(0);
    }

    std::cout << "Enter your budget: ";
    std::cin >> budget;

    float actualY = std::numeric_limits<float>::min();
    std::string maxName, line, month = argv[2], year = argv[1];
    std::vector<std::pair<std::string, float> > actualRanking;

    std::string filename = "Yvalue";
    filename += year + month + ".csv";

    std::ifstream myFile;
    myFile.open(filename);
    float curY;
    size_t commaIndex;
    std::string name;
    while (std::getline(myFile, line)) {
        commaIndex = line.find(',');
        curY = std::stof(line.substr(commaIndex + 1));
        name = line.substr(0, commaIndex);
        if (curY > actualY) {
            actualY = curY;
            maxName = name;
        }
        std::pair<std::string, float> curPair(name, curY);
        actualRanking.push_back(curPair);
    }

    actualRanking.sort(actualRanking.begin(), actualRanking.end(), myCompare);

    myFile.close();

    std::cout << "Best stock this month is " << maxName << ", Y value = " << actualY << "\n";

    // Get output from the model
    std::vector<std::pair<std::string, float> > modelOut;
    myFile.open("resutl.txt");
    
    while (std::getline(myFile, line)) {
        commaIndex = line.find(',');
        curY = std::stof(line.substr(commaIndex + 1));
        name = line.substr(0, commaIndex);
        std::pair<std::string, float> curPair(name, curY);
        modelOut.push_back(curPair);
    }

    modelOut.sort(modelOut.begin(), modelOut.end(), myCompare);
    myFile.close();

    // Pick the top stocks to buy
    
    int numOfCurStock;
    std::vector<std::pair<std::string, int> > result;
    for (int i = 0; i < modelOut.size(); i++) {
        numOfCurStock = budget / modelOut[i].second;
        budget -= numOfCurStock * modelOut[i].second;
        std::pair<std::string, int> curStock;
        curStock.first = modelOut[i].first;
        curStock.second = numOfCurStock;
        result.push_back(curStock);
    }

    // Print out the result
    for (int i = 0; i < result.size(); i++) {
        std::cout << "Buy " << result[i].second << " shares of " << result[i].first << "\n";
    }

    return 0;
}