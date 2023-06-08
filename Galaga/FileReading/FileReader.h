#pragma once
#include "Singleton.h"
#include <string>
namespace dae
{

	class FileReader final : public Singleton<FileReader>
	{
	public:
		void Init(const std::string& dataPath);

		void TestRead(const std::string& filePath);

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
