/**
 * \file CliLogger.cpp
 *
 * \brief Implementation of Logger module
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 0.1
 *
 */ // REVIEW_COMMENT: copyright and License
//  REVIEW_COMMENT: Class name shall be CliLogger
#include "IResult.h"
#include "OpenConfiguratorCli.h"

CliLogger::CliLogger()
{
	fileLog = false;

	languageGerman = false;
	/** Current language is English by default */
	languageIndex = (std::uint8_t) CLILanguageType::EN;

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

void CliLogger::SetFileLog(bool set, std::string path = "") // REVIEW_COMMENT: CHeck the usage of bool
{
	if (set == true)
	{// REVIEW_COMMENT: Add code comment
	// REVIEW_COMMENT: Name of the configuration shall be with time stamp
		std::string logPath = path + "/" + kOpenConfiguratorCliLogFileName;

		/** Open the output file stream for logging */
		ofs.open(logPath, std::ofstream::out | std::ofstream::app);
		if (!ofs.is_open())
		{
			LOG_WARN() << "Unable to open log file" << logPath << std::endl; // REVIEW_COMMENT: Use LogMessage
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

void CliLogger::SetLanguageToGerman(bool set)// REVIEW_COMMENT: CHeck the usage of bool
{
	languageGerman = set;

	languageIndex = (std::uint8_t) CLILanguageType::DE;// REVIEW_COMMENT: If set = true then set to German otherwise English
	// REVIEW_COMMENT: no space after type conversion
}

void CliLogger::LogMessage(CliMessageType msgType, std::string logDescription)
{
	std::ostringstream message;

	message << "[" << boost::posix_time::second_clock::local_time()				<< "] ";// REVIEW_COMMENT: avoid extra tabs
	message << "[" << CliMessageTypeString.at((std::uint8_t)msgType).c_str()	<< "] ";// REVIEW_COMMENT: avoid extra tabs
	message << logDescription.c_str() << "." << std::endl;

	/** Pass the message to output log file if file logging enabled */
	if (fileLog == true)
	{
		ofs << message.str();
	}
	else
	{
		std::cout << message.str(); // REVIEW_COMMENT: cout used to accomodate file logging as well
	}
}

void CliLogger::LogMessage(CliMessageType msgType, CliResult& result) // REVIEW_COMMENT: This function can call the previous function to avoid code repeateability
{
	std::ostringstream message;

	message << "[" << boost::posix_time::second_clock::local_time()				<< "] ";// REVIEW_COMMENT: avoid extra tabs
	message << "[" << CliMessageTypeString.at((std::uint8_t)msgType).c_str()	<< "] ";// REVIEW_COMMENT: avoid extra tabs
	message << result.GetErrorMessage() << "." << std::endl;

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
// REVIEW_COMMENT: 80 characters limit per line
CliResult CliLogger::HandleCliApiFailed(std::string apiDescription, CliResult& result) // REVIEW_COMMENT: use const for input param
{
	/** Log the reason of CLI API failure details */
	LogMessage(CliMessageType::CLI_ERROR, result);

	/** Prepare the failure message with caller API name */
	boost::format formatter(kMsgCliApiFailed[languageIndex]);
	formatter// REVIEW_COMMENT: keep it in a single line
	% apiDescription.c_str();

	return CliResult(CliErrorCode::CLI_API_FAILED, formatter.str());
}

CliResult CliLogger::HandleCoreApiFailed(std::string apiDescription, Result& result)
{
	/** Log the reason of CLI API failure details */
	LogMessage(CliMessageType::CLI_ERROR, result.GetErrorMessage());

	/** Prepare the failure message with caller API name */
	boost::format formatter(kMsgCoreApiFailed[languageIndex]);
	formatter// REVIEW_COMMENT: keep it in a single line
	% apiDescription.c_str();

	return CliResult(CliErrorCode::CORE_API_FAILED, formatter.str());
}

CliResult CliLogger::HandleExceptionCaught(std::string apiDescription, std::exception& e)// REVIEW_COMMENT: 80 characters limit per line; avoid single letter variable name
{
	/** Log the reason of CLI API failure details */
	LogMessage(CliMessageType::CLI_ERROR, e.what());

	/** Prepare the failure message with caller API name */
	boost::format formatter(kMsgExceptionCaught[languageIndex]);
	formatter// REVIEW_COMMENT: keep it in a single line
	% apiDescription.c_str();

	return CliResult(CliErrorCode::EXCEPTION_CAUGHT, formatter.str());
}
