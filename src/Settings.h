#pragma once
#include <map>
#include <any>
#include <SimpleIni.h>
#include <direct.h>

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
			return;

		for (int i = 0; i < size; i++)
		{
			std::string key = GetAllKeys()[i].name;
			const char* section = GetSection();
			const char* value = ini.GetValue(section, key.c_str());
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
	virtual constexpr std::array<Option, size>      GetAllKeys() const = 0;
	virtual constexpr const char*                   GetSection() const = 0;
	virtual std::string                             GetSetting(std::string key) 
	{ 
		auto found = settingMap.find(key);

		if (found == std::end(settingMap))
		{
			return found->first.defaultVal;
		}

		return found->second; 
	};

private:
	std::map<Option, std::string> settingMap;
	const char*                        file;

};