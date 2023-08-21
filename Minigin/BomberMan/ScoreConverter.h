#pragma once
#include <string>
#include <vector>

namespace dae
{
	struct playerScoreInfo
	{
		std::string name;
		int score;
	};

	class ScoreConverter final
	{
	public:
		ScoreConverter(const std::string& scoreFile);
		~ScoreConverter() = default;

		ScoreConverter(const ScoreConverter& other) = delete;
		ScoreConverter(ScoreConverter&& other) = delete;
		ScoreConverter& operator=(const ScoreConverter& other) = delete;
		ScoreConverter& operator=(ScoreConverter&& other) = delete;

		void AddScore(int score, const std::string& name);

		std::vector<playerScoreInfo> GetHighScores() const;

	private:
		const std::string m_ScoreFile;
	};
}

