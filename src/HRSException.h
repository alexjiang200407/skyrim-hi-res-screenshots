#pragma once

#include <exception>
#include <string>

class HRSException
	:
	public std::exception
{
public:
	HRSException(int line, const char* file) 
		:
		line(line), file(file)
	{};

	const char* what() const override;
	virtual const char* GetType() const;
	std::string GetFile() const { return file; };
	int GetLine() const { return line; };
	std::string GetOrigin() const;

private:
	int line;
	std::string file;

protected:
	mutable std::string buffer;
};