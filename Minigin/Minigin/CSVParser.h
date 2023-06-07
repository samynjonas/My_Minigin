#pragma once
#include <vector>
#include <string>

namespace dae
{
	class CSVParser
	{
	public:
		CSVParser() = default;
		~CSVParser() = default;

		CSVParser(const CSVParser& other) = delete;
		CSVParser(CSVParser&& other) = delete;
		CSVParser& operator=(const CSVParser& other) = delete;
		CSVParser& operator=(CSVParser&& other) = delete;
		
		//The parse will return a vector(collums) filled with a vector for each collum that represents the rows in int
		std::vector<std::vector<int>> ParseCSV(const std::string& CVSmapFile) const;

	private:


	};
}

