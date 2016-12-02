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
#include "CLILogger.h"

CLILogger::CLILogger()
{
	fileLog = false;

	languageGerman = false;
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
	fileLog = set;

	if (fileLog == true)
	{
		ofs.open ("OpenCONFIGURATOR_CLI.log", std::ofstream::out | std::ofstream::app);
	}
}

void CLILogger::SetLanguageToGerman(bool set)
{
	languageGerman = set;
}

void CLILogger::LogMessage(uint32_t messageId)
{
	if (fileLog == true)
	{
		//ofs << logMessage->at(messageId);
	}
	else
	{
		//LOG_ERROR(logMessage->at(messageId));
	}
}
