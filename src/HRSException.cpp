#include "HRSException.h"
#include <sstream>


const char* HRSException::what() const
{
	std::ostringstream oss;

	oss << GetType() << '\n'
		<< GetOrigin() << '\n';

	return oss.str().c_str();
}

const char* HRSException::GetType() const
{
	return "High-Res Screenshot Exception";
}

std::string HRSException::GetOrigin() const
{
	std::ostringstream oss;

	oss << "File: " << GetFile() << '\n'
		<< "Line: " << GetLine();

	buffer = oss.str();
	return buffer;
}
