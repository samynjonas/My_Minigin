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

	class ScoreSaver final
	{
	public:
		ScoreSaver(const std::string& scoreFile);
		~ScoreSaver() = default;

		ScoreSaver(const ScoreSaver& other) = delete;
		ScoreSaver(ScoreSaver&& other) = delete;
		ScoreSaver& operator=(const ScoreSaver& other) = delete;
		ScoreSaver& operator=(ScoreSaver&& other) = delete;

		void AddScore(int score, const std::string& name);

		std::vector<playerScoreInfo> GetHighScores() const;

	private:
		const std::string m_ScoreFile;

	};
}

