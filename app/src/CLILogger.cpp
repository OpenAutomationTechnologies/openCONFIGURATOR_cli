/**
 * \file CLILogger.cpp
 *
 * \brief Implementation of Logger module
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 0.1
 *
 */

#include "OpenConfiguratorCLI.h"

CLILogger::CLILogger()
{
	fileLog = false;

	languageGerman = false;

	CLIMessageTypeString.push_back("INFO");
	CLIMessageTypeString.push_back("WARN");
	CLIMessageTypeString.push_back("ERROR");
	CLIMessageTypeString.push_back("DEBUG");
}

CLILogger::~CLILogger()
{
	if (fileLog == true)
	{
		ofs.close();
	}
}

CLILogger& CLILogger::GetInstance()
{
	static CLILogger instance;
	return instance;
}

void CLILogger::SetFileLog(bool set)
{
	if (set == true)
	{
		ofs.open ("OpenCONFIGURATOR_CLI.log", std::ofstream::out | std::ofstream::app);
	}
	else
	{
		if (fileLog == true)
		{
			ofs.close();
		}
	}

	fileLog = set;
}

void CLILogger::SetLanguageToGerman(bool set)
{
	languageGerman = set;
}

/*
void CLILogger::LogMessage(CLIMessageType msgType, CLIResult& result)
{
	std::ostringstream message;

	message << "[" << CLIMessageTypeString.at((uint8_t)msgType).c_str()	<< "] ";
	message << "[" << boost::posix_time::second_clock::local_time()		<< "] ";
	message << result.GetErrorMessage() << std::endl;
			
	if (fileLog == true)
	{
		ofs << message.str();
	}
	else
	{
		std::cout << message.str();
	}
}
*/
