/**
 * \file CliLogger.cpp
 *
 * \brief Implementation of Logger module
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 0.1
 *
 */
/*------------------------------------------------------------------------------
Copyright (c) 2016, Kalycito Infotech Private Limited, INDIA.
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the copyright holders nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
------------------------------------------------------------------------------*/

#include "IResult.h"
#include "OpenConfiguratorCli.h"

CliLogger::CliLogger()
{
	fileLog = false;

	languageGerman = false;
	/** Current language is English by default */
	languageIndex = (std::uint8_t)CliLanguageType::EN;

	/** String descriptions for message types */
	CliMessageTypeString.push_back("INFO");
	CliMessageTypeString.push_back("WARN");
	CliMessageTypeString.push_back("ERROR");
	CliMessageTypeString.push_back("DEBUG");
}

CliLogger::~CliLogger()
{
	if (ofs.is_open())
	{
		/** Close the output file stream that is already opened */
		ofs.close();
	}
}

CliLogger& CliLogger::GetInstance()
{
	static CliLogger instance;

	return instance;
}

void CliLogger::SetFileLog(const bool set, const std::string& path = "")
{
	if (set == true)
	{
		std::ostringstream dateTime;

		/** Prepare log file name to be created under output path */
		boost::posix_time::time_facet* const timeFacet = new boost::posix_time::time_facet("%d%b%Y_%H%M%S");
		dateTime.imbue(std::locale(dateTime.getloc(), timeFacet));
		std::string logPath = path + "/" + kOpenConfiguratorCliLogFileName + dateTime.str() + ".log";

		/** Open the output file stream for logging */
		ofs.open(logPath, std::ofstream::out | std::ofstream::app);
		if (!ofs.is_open())
		{
			boost::format formatter(kMsgUnableToOpenLogFile[languageIndex]);
			formatter % logPath.c_str();
			
			CliLogger::GetInstance().LogMessage(CliMessageType::CLI_ERROR, formatter.str());
		}
		else
		{
			fileLog = true;
		}
	}
	else
	{
		/** Close the output file stream of logging if already opened */
		if (ofs.is_open())
		{
			ofs.close();
		}

		fileLog = false;
	}
}

void CliLogger::SetLanguageToGerman(const bool set)
{
	languageGerman = set;

	if (set == true)
	{
		languageIndex = (std::uint8_t)CliLanguageType::DE;
	}
	else
	{
		languageIndex = (std::uint8_t)CliLanguageType::EN;
	}
}

void CliLogger::LogMessage(const CliMessageType msgType, 
						   const std::string& logDescription)
{
	std::ostringstream message;

	message << "[" << boost::posix_time::second_clock::local_time() << "] ";
	message << "[" << CliMessageTypeString.at((std::uint8_t)msgType).c_str() << "] ";
	message << logDescription.c_str() << "." << std::endl;

	/** Pass the message to output log file if file logging enabled */
	if (fileLog == true)
	{
		ofs << message.str();
	}
	else
	{
		std::cout << message.str();
	}
}

void CliLogger::LogMessage(const CliMessageType msgType, const CliResult& result)
{
	LogMessage(msgType, result.GetErrorMessage());
}

CliResult CliLogger::HandleCliApiFailed(const std::string& apiDescription, 
										const CliResult& result)
{
	/** Log the reason of CLI API failure details */
	LogMessage(CliMessageType::CLI_ERROR, result);

	/** Prepare the failure message with caller API name */
	boost::format formatter(kMsgCliApiFailed[languageIndex]);
	formatter % apiDescription.c_str();

	return CliResult(CliErrorCode::CLI_API_FAILED, formatter.str());
}

CliResult CliLogger::HandleCoreApiFailed(const std::string& apiDescription,
										 const Result& result)
{
	/** Log the reason of CLI API failure details */
	LogMessage(CliMessageType::CLI_ERROR, result.GetErrorMessage());

	/** Prepare the failure message with caller API name */
	boost::format formatter(kMsgCoreApiFailed[languageIndex]);
	formatter % apiDescription.c_str();

	return CliResult(CliErrorCode::CORE_API_FAILED, formatter.str());
}

CliResult CliLogger::HandleExceptionCaught(const std::string& apiDescription,
										   const std::exception& e)
{
	/** Log the reason of CLI API failure details */
	LogMessage(CliMessageType::CLI_ERROR, e.what());

	/** Prepare the failure message with caller API name */
	boost::format formatter(kMsgExceptionCaught[languageIndex]);
	formatter % apiDescription.c_str();

	return CliResult(CliErrorCode::EXCEPTION_CAUGHT, formatter.str());
}
