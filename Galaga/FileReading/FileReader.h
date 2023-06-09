#pragma once
#include "Singleton.h"
#include <string>
#include <vector>
namespace dae
{

	class FileReader final : public Singleton<FileReader>
	{
	public:
		void Init(const std::string& dataPath);

		void TestRead(const std::string& filePath);

		std::vector<int> ReadHighscores(const std::string& filePath);
		void WriteHighscores(const std::string& filePath, std::vector<int> scores);

	private: 
		friend class Singleton<FileReader>;

		FileReader() = default;

		~FileReader() = default;

		FileReader(const FileReader& other) = delete;
		FileReader(FileReader&& other) = delete;
		FileReader& operator=(const FileReader& other) = delete;
		FileReader& operator=(FileReader&& other) = delete;

		std::string m_DataPath{};
	};

}
