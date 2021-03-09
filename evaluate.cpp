#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <vector>
#include <unordered_map>

float budget;

bool myCompare(std::pair<std::string, float> a, std::pair<std::string, float> b)
{
    return a.second > b.second;
}

int main(int argc, char *argv[])
{
    std::string filename = argv[1];
    std::cout << "Enter your budget: ";
    std::cin >> budget;
    std::string line;

    std::ifstream myFile;
    float curY, curPrice;
    size_t commaIndex1, commaIndex2;
    std::string name;

    // Get open price for the next month
    std::cout << "Reading the prices\n";
    std::unordered_map<std::string, std::pair<float, float> > opens;
    myFile.open("openClose1111.txt");
    while (std::getline(myFile, line))
    {
        commaIndex1 = line.find(',');
        commaIndex2 = line.find(',', commaIndex1 + 1);
        name = line.substr(0, commaIndex1);
        std::pair<float, float> curPrices;
        curPrices.first = std::stof(line.substr(commaIndex1 + 1, commaIndex2));
        curPrices.second = std::stof(line.substr(commaIndex2 + 1));
        opens[name] = curPrices;
    }
    myFile.close();
    std::cout << "Done\n";

    // Get output from the model
    std::cout << "Reading the model output\n";
    std::vector<std::pair<std::string, float> > modelOut;
    myFile.open(filename);
    std::getline(myFile, line);

    while (std::getline(myFile, line))
    {
        commaIndex1 = line.find(',');
        commaIndex2 = line.find(',', commaIndex1 + 1);
        curY = std::stof(line.substr(commaIndex2 + 1));
        name = line.substr(0, commaIndex1);
        std::pair<std::string, float> cur;
        cur.first = name;
        cur.second = curY;
        modelOut.push_back(cur);
    }

    std::sort(modelOut.begin(), modelOut.end(), myCompare);
    myFile.close();
    std::cout << "Done\n";

    // Pick the top 10 stocks to buy

    std::cout << "Picking stocks\n";
    int numOfCurStock;
    std::vector<std::pair<std::string, int> > result;
    int counter = 0, index = 0;
    while (counter < 10)
    {
        std::pair<std::string, int> cur;
        name = modelOut[index].first;
        if (opens.count(name) == 0) {
            index++;
            continue;
        }
        numOfCurStock = (int)((budget / 10) / opens[name].first);
        // std::cout << name << " " << numOfCurStock << "\n";
        cur.first = name;
        cur.second = numOfCurStock;
        result.push_back(cur);
        index++;
        counter++;
    }

    // Print out the result
    float profit = 0;
    for (int i = 0; i < result.size(); i++)
    {
        std::cout << "Buy " << result[i].second << " shares of " << result[i].first << "\n";
        profit += (opens[result[i].first].second - opens[result[i].first].first) * result[i].second;
    }

    std::cout << "The total profit is " << profit << ", which is " << profit / budget << " of the budget." << std::endl;

    return 0;
}