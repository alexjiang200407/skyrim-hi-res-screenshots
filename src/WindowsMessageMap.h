#pragma once

#include "WinAPI.h"
#include <unordered_map>

class WindowsMessageMap
{
public:
	WindowsMessageMap();
	std::string Decode(DWORD msg) const;
private:
	std::unordered_map<DWORD, std::string> map;
};