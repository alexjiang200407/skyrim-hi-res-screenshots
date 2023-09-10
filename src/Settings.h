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


		// TODO handle non-existent ini file
		if (rc != SI_OK)
			return;

		for (int i = 0; i < size; i++)
		{
			std::string key = GetAllKeys()[i];
			const char* section = GetSection();
			const char* value = ini.GetValue(section, key.c_str());
			settingMap.insert({ key, value });
		}
	}


public:
	virtual constexpr std::array<std::string, size> GetAllKeys() const = 0;
	virtual constexpr const char*                   GetSection() const = 0;
	virtual std::string                             GetSetting(std::string key) { return settingMap.find(key)->second; };

private:
	std::map<std::string, std::string> settingMap;
	const char*                        file;

};