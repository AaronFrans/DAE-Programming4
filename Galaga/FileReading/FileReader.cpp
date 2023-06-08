#include "FileReader.h"

#include <fstream>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/IStreamWrapper.h>
#include <iostream>

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
