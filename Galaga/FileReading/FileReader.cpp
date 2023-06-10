#include "FileReader.h"

#include <fstream>
#include <iostream>
#include <filesystem>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/IStreamWrapper.h>
#include <rapidjson/StringBuffer.h>
#include <rapidjson/Writer.h>

void dae::FileReader::Init(const std::string& dataPath)
{
	m_DataPath = dataPath;
}

void dae::FileReader::TestRead(const std::string& filePath)
{
	auto totalPath = m_DataPath + filePath;
	if (std::ifstream is{ totalPath })
	{
		using namespace rapidjson;
		IStreamWrapper isw{ is };

		Document scenedoc;
		scenedoc.ParseStream(isw);
		auto test = scenedoc["Test"].GetString();

		std::cout << test;
	}
}

std::vector<int> dae::FileReader::ReadHighscores(const std::string& filePath)
{
	std::vector<int> scores{ };
	auto totalPath = m_DataPath + filePath;

	if (!std::filesystem::exists(totalPath))
		return scores;

	if (std::ifstream is{ totalPath })
	{
		using namespace rapidjson;
		IStreamWrapper isw{ is };
		Document document;
		document.ParseStream(isw);

		assert(document.IsArray() && "The highscores json needs to be an array of ints");

		const rapidjson::Value& scoresJson = document;

		for (rapidjson::SizeType i = 0; i < scoresJson.Size(); i++) {
			assert(scoresJson[i].IsInt() && "The highscores json needs to be an array of ints");
			scores.push_back(scoresJson[i].GetInt());
		}
	}

	return scores;
}

void dae::FileReader::WriteHighscores(const std::string& filePath, std::vector<int> scores)
{
	auto totalPath = m_DataPath + filePath;

	if (std::ofstream os{ totalPath })
	{
		using namespace rapidjson;
		Document document;
		document.SetArray();


		for (const int num : scores) {
			rapidjson::Value value;
			value.SetInt(num);
			document.PushBack(value, document.GetAllocator());
		}

		StringBuffer buffer;
		Writer<StringBuffer> writer(buffer);
		document.Accept(writer);


		std::string jsonString = buffer.GetString();

		os << jsonString;

	}
}

std::vector<dae::EnemyFileData> dae::FileReader::ReadEnemies(const std::string& filePath)
{

	std::vector<EnemyFileData> enemies{};
	auto totalPath = m_DataPath + filePath;

	bool isBossRead{ false };

	assert(std::filesystem::exists(totalPath) && "Level json file could not be found");

	if (std::ifstream is{ totalPath })
	{
		using namespace rapidjson;
		IStreamWrapper isw{ is };
		Document document;
		document.ParseStream(isw);

		assert(document.IsArray() && "Level files is an array of { \"type\": 0,\"x\" : 240,\"y\" : 120} objects");

		for (SizeType i = 0; i < document.Size(); i++) {
			const Value& object = document[i];

			assert(object.IsObject() && "Level files is an array of { \"type\": 0,\"x\" : 240,\"y\" : 120} objects");

			assert(object.HasMember("type") && object.HasMember("x") && object.HasMember("y") &&
				"Level files is an array of { \"type\": 0,\"x\" : 240,\"y\" : 120} objects");


			EnemyFileData enemy;
			enemy.x = object["x"].GetFloat();
			enemy.y = object["y"].GetFloat();
			enemy.enemyType = object["type"].GetInt();


			assert(!(isBossRead && enemy.enemyType != 2) && "Enemies with id 2 need to be last in the list");

			if (enemy.enemyType == 2)
				isBossRead = true;

			enemies.push_back(enemy);

		}



	}
	return enemies;
}
