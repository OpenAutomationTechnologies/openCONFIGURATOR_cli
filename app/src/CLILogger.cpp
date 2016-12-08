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

#include "IResult.h"
#include "OpenConfiguratorCLI.h"

CLILogger::CLILogger()
{
	fileLog = false;

	languageGerman = false;
	/**< Current language is English by default */
	languageIndex = (std::uint8_t) CLILanguageType::EN;

	/**< String descriptions for message types */
	CLIMessageTypeString.push_back("INFO");
	CLIMessageTypeString.push_back("WARN");
	CLIMessageTypeString.push_back("ERROR");
	CLIMessageTypeString.push_back("DEBUG");
}

CLILogger::~CLILogger()
{
	if (ofs.is_open())
	{
		/**< Close the output file stream that is already opened */
		ofs.close();
	}
}

CLILogger& CLILogger::GetInstance()
{
	static CLILogger instance;
	return instance;
}

void CLILogger::SetFileLog(bool set, std::string path = "")
{
	if (set == true)
	{
		std::string logPath = path + "/" + kOpenConfiguratorCLILogFileName;

		/**< Open the output file stream for logging */
		ofs.open(logPath, std::ofstream::out | std::ofstream::app);
		if (!ofs.is_open())
		{
			LOG_WARN() << "Unable to open log file" << logPath << std::endl;
		}
		else
		{
			fileLog = true;
		}
	}
	else
	{
		/**< Close the output file stream of logging if already opened */
		if (ofs.is_open())
		{
			ofs.close();
		}

		fileLog = false;
	}
}

void CLILogger::SetLanguageToGerman(bool set)
{
	languageGerman = set;

	languageIndex = (std::uint8_t) CLILanguageType::DE;
}

void CLILogger::LogMessage(CLIMessageType msgType, std::string logDescription)
{
	std::ostringstream message;

	message << "[" << boost::posix_time::second_clock::local_time()				<< "] ";
	message << "[" << CLIMessageTypeString.at((std::uint8_t)msgType).c_str()	<< "] ";
	message << logDescription.c_str() << std::endl;

	/**< Pass the message to output log file if file logging enabled */
	if (fileLog == true)
	{
		ofs << message.str();
	}
	else
	{
		std::cout << message.str();
	}
}

void CLILogger::LogMessage(CLIMessageType msgType, CLIResult& result)
{
	std::ostringstream message;

	message << "[" << boost::posix_time::second_clock::local_time()				<< "] ";
	message << "[" << CLIMessageTypeString.at((std::uint8_t)msgType).c_str()	<< "] ";
	message << result.GetErrorMessage() << std::endl;

	/**< Pass the message to output log file if file logging enabled */
	if (fileLog == true)
	{
		ofs << message.str();
	}
	else
	{
		std::cout << message.str();
	}
}

CLIResult CLILogger::HandleCliApiFailed(std::string apiDescription, CLIResult& result)
{
	/**< Log the reason of CLI API failure details */
	LogMessage(CLIMessageType::CLI_ERROR, result);

	/**< Prepare the failure message with caller API name */
	boost::format formatter(kMsgCliApiFailed[languageIndex]);
	formatter
	% apiDescription.c_str();

	return CLIResult(CLIErrorCode::CLI_API_FAILED, formatter.str());
}

CLIResult CLILogger::HandleCoreApiFailed(std::string apiDescription, Result& result)
{
	/**< Log the reason of CLI API failure details */
	LogMessage(CLIMessageType::CLI_ERROR, result.GetErrorMessage());

	/**< Prepare the failure message with caller API name */
	boost::format formatter(kMsgCoreApiFailed[languageIndex]);
	formatter
	% apiDescription.c_str();

	return CLIResult(CLIErrorCode::CORE_API_FAILED, formatter.str());
}

CLIResult CLILogger::HandleExceptionCaught(std::string apiDescription, std::exception& e)
{
	/**< Log the reason of CLI API failure details */
	LogMessage(CLIMessageType::CLI_ERROR, e.what());

	/**< Prepare the failure message with caller API name */
	boost::format formatter(kMsgExceptionCaught[languageIndex]);
	formatter
	% apiDescription.c_str();

	return CLIResult(CLIErrorCode::EXCEPTION_CAUGHT, formatter.str());
}
