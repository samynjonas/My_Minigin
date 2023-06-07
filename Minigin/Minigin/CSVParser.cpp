#include "CSVParser.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include "servicelocator.h"
#include "Logger.h"

std::vector<std::vector<int>> dae::CSVParser::ParseCSV(const std::string& CVSmapFile) const
{
    std::vector<std::vector<int>> parsedVector;

    std::ifstream file(CVSmapFile);
    if (!file.is_open())
    {
        servicelocator<Logger>::get_serviceLocator().LogError({ "Failed to open " + CVSmapFile });
        return parsedVector;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string field;

        std::vector<int> vecRow;
        while (std::getline(iss, field, ','))
        {
            int value = std::stoi(field);
            vecRow.push_back(value);
        }
        parsedVector.push_back(vecRow);
    }

    file.close();

    return parsedVector;
}