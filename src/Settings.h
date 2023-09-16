#pragma once
#include <map>
#include <any>
#include <SimpleIni.h>
#include <direct.h>
#include "logger.h"

template <size_t size>
class Settings
{
public:
	Settings() = delete;
	Settings(const char* filename)
		:
		file(filename)
	{}

	void ReadAllSettings()
	{
		CSimpleIniA ini;
		ini.SetUnicode();

		SI_Error rc = ini.LoadFile(file);

		if (rc != SI_OK)
		{
			logger::warn("Ini file doesn't exist!");
			for (int i = 0; i < size; i++)
			{
				Option      option = GetAllOptions()[i];
				std::string key = option.name;
				std::string defaultVal = option.defaultVal;

				settingMap.insert({ key, defaultVal });
			}

			return;
		}

		logger::info("Reading all settings for {}", GetSection());

		for (int i = 0; i < size; i++)
		{
			Option      option = GetAllOptions()[i];
			std::string key = option.name;
			std::string defaultVal = option.defaultVal;

			logger::info("Searching for key {}", key);

			const char* value = ini.GetValue(GetSection(), key.c_str(), defaultVal.c_str());
			
			logger::info("Value is {}", value);			
			settingMap.insert({ key, value });
		}
	}

public:
	class Option
	{
	public:
		constexpr Option(const char* name, const char* defaultVal)
			:
			name(name), defaultVal(defaultVal)
		{}

		constexpr Option(std::string name)
			:
			name(name)
		{}

		constexpr Option(const char* name)
			:
			name(name)
		{}

		bool operator==(const Option& rhs) const
		{
			return this->name == rhs.name;
		}

		bool operator!=(const Option& rhs) const
		{
			return !(*this == rhs);
		}

		bool operator<(const Option& rhs) const
		{
			return this->name < rhs.name;
		}

	public:
		std::string const name{};
		std::string const defaultVal{};
	};

public:
	virtual constexpr std::array<Option, size>      GetAllOptions() const = 0;
	virtual constexpr const char*                   GetSection() const = 0;
	virtual std::string                             GetSetting(std::string key) 
	{ 
		auto found = settingMap.find(key);

		//if (found == std::end(settingMap))
		//{
		//	return found->first.defaultVal;
		//}

		return found->second; 
	};

private:
	std::map<Option, std::string> settingMap;
	const char*                        file;

};