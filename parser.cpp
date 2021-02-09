#include <iostream>
#include <fstream>
#include <string>
#include <map>

std::map<std::string, std::pair<std::string, std::string> > datas;

void removeLeadingSpace(std::string &str)
{
    int index = 0;
    while (str[index] == ' ' || str[index] == 9)
    {
        index++;
    }
    // std::cout << "index = " << index << "\n";
    str = str.substr(index);
}

int main()
{
    std::ifstream inputFile;
    inputFile.open("./12/03/close");
    std::string line;
    std::getline(inputFile, line);
    int spaceIndex;
    std::string name, openPrice;
    while (std::getline(inputFile, line))
    {
        removeLeadingSpace(line);
        // std::cout << line << "\n";
        spaceIndex = line.find(' ');
        name = line.substr(0, spaceIndex);
        if (name.length() > 3 && name.substr(name.length()-3) == ".TO") continue;
        // std::cout << name << "\n";

        for (int i = 0; i < 6; i++)
        {
            spaceIndex = line.find(' ');
            line = line.substr(spaceIndex);
            removeLeadingSpace(line);
        }
        // std::cout << line << "\n";

        spaceIndex = line.find(' ');
        openPrice = line.substr(0, spaceIndex);
        // std::cout << "openPrice = " << openPrice << "\n";
        std::pair<std::string, std::string> curPair(openPrice, "0");
        datas[name] = curPair;
    }
    inputFile.close();

    inputFile.open("./12/31/close");
    std::getline(inputFile, line);
    std::string closePrice;
    while (std::getline(inputFile, line)) {
        removeLeadingSpace(line);
        // std::cout << line << "\n";
        spaceIndex = line.find(' ');
        name = line.substr(0, spaceIndex);
        if (name.length() > 3 && name.substr(name.length()-3) == ".TO") continue;
        // std::cout << name << "\n";

        for (int i = 0; i < 2; i++) {
            spaceIndex = line.find(' ');
            line = line.substr(spaceIndex);
            removeLeadingSpace(line);
        }

        spaceIndex = line.find(' ');
        closePrice = line.substr(0, spaceIndex);
        if (datas.count(name) > 0)
            datas[name].second = closePrice;
        else {
            std::pair<std::string, std::string> curPair("0", closePrice);
            datas[name] = curPair;
        }
    }

    inputFile.close();

    std::string result = "";
    for (std::map<std::string, std::pair<std::string, std::string> >::iterator it = datas.begin(); it != datas.end(); it++)
    {
        std::cout << it->first << "," << it->second.first << "," << it->second.second << "\n";
        result += it->first + "," + it->second.first + "," + it->second.second + "\n";
    }

    std::ofstream outputFile;
    outputFile.open("openClose12.txt");
    outputFile << result;
    outputFile.close();

    return 0;
}