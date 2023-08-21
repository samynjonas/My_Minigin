#include "ScoreConverter.h"
#include <iostream>
#include <fstream>

#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/stringbuffer.h"

#include "../rapidjson/rapidjson.h"
#include "../rapidjson/stream.h"
#include "../rapidjson/filereadstream.h"
#include "../rapidjson/istreamwrapper.h"

#include "servicelocator.h"
#include "Logger.h"

using namespace rapidjson;
dae::ScoreConverter::ScoreConverter(const std::string& scoreFile)
	: m_ScoreFile{ scoreFile }
{

}

void dae::ScoreConverter::AddScore(int score, const std::string& playerName)
{
	std::ifstream ifs(m_ScoreFile);
	if (!ifs.is_open())
	{
		servicelocator<Logger>::get_serviceLocator().LogError({ "Failed to open " + m_ScoreFile });
		return;
	}

	std::string jsonStr((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	ifs.close();


	Document document;
	if (jsonStr.empty() == false)
	{
		if (document.Parse(jsonStr.c_str()).HasParseError())
		{
			servicelocator<Logger>::get_serviceLocator().LogError({ "Failed to parse " + m_ScoreFile });
			return;
		}
	}


	if (document.IsArray() == false)
	{
		document.SetArray();
	}

	Document::AllocatorType& allocator = document.GetAllocator();

	Value newObject(kObjectType);
	Value playerNameValue(playerName.c_str(), allocator);
	newObject.AddMember("name", playerNameValue, allocator);
	newObject.AddMember("score", score, allocator);

	document.PushBack(newObject, allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	document.Accept(writer);

	std::ofstream ofs(m_ScoreFile);
	if (!ofs.is_open())
	{
		servicelocator<Logger>::get_serviceLocator().LogError({ "Failed to open " + m_ScoreFile + " for writing" });
		return;
	}

	ofs << buffer.GetString();
	ofs.close();

	servicelocator<Logger>::get_serviceLocator().Log({ m_ScoreFile + " Updated succesfully" });
	return;

}

std::vector<dae::playerScoreInfo> dae::ScoreConverter::GetHighScores() const
{
	std::vector<dae::playerScoreInfo> vecPlayers;

	using rapidjson::Document;
	Document jsonDoc;

	if (std::ifstream is{ m_ScoreFile })
	{
		rapidjson::IStreamWrapper isw{ is };
		jsonDoc.ParseStream(isw);

		using rapidjson::Value;

		for (Value::ConstValueIterator itr = jsonDoc.Begin(); itr != jsonDoc.End(); ++itr)
		{
			const Value& player = *itr;

			const Value& valName = player.GetObj()["name"];
			const Value& valScore = player.GetObj()["score"];

			vecPlayers.push_back({ valName.GetString(), valScore.GetInt() });
		}
	}
	else
	{
		servicelocator<Logger>::get_serviceLocator().LogError({ "Failed to open " + m_ScoreFile });
	}

	return vecPlayers;
}