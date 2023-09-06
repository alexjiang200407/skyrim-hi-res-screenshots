#include <map>
#include <any>
#include "SimpleIni.h"
#include <direct.h>

template <size_t size>
class Settings
{
public:
	Settings()
	{
		CSimpleIniA ini;
		ini.SetUnicode();

		SI_Error rc = ini.LoadFile("./HiResScreenshots.ini");

		if (rc != SI_OK)
			return;

		printf(_getcwd(NULL, 0));


		for (int i = 0; i < size; i++)
		{
			const char* value = ini.GetValue(GetSection(), GetAllKeys()[i].c_str());
			settingMap.insert({ GetAllKeys()[i], value});
		}
	}

	virtual constexpr std::array<std::string, size> GetAllKeys() const;

	virtual constexpr const char* GetSection() const;

	std::string GetSetting(std::string key)
	{
		return settingMap.find(key)->first;
	};

private:
	std::map<std::string, std::string> settingMap;

};