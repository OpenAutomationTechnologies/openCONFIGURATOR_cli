/**
 * \file OpenConfiguratorCLI.cpp
 *
 * \brief Implementation to receive the command line parameters for
 *        OpenCONFIGURATOR CLI and generate the POWERLINK configuration files
 *        at the output path
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 0.1
 *
 */

#include "OpenConfiguratorCLI.h"
#include "ParameterValidator.h"
#include "ProjectParser.h"

/**************************** Public members *********************************/

OpenConfiguratorCLI::OpenConfiguratorCLI()
{
	xmlFilePath = "";
	outputPath = "";
}

OpenConfiguratorCLI::~OpenConfiguratorCLI()
{

}

OpenConfiguratorCLI& OpenConfiguratorCLI::GetInstance()
{
	static OpenConfiguratorCLI instance;
	return instance;
}

std::string OpenConfiguratorCLI::GetNetworkName()
{
	return boost::filesystem::basename(xmlFilePath);
}

bool OpenConfiguratorCLI::GeneratePOWERLINKConfigurationFiles(std::vector<std::string> paramsList)
{
	const uint8_t kMinimumNumberOfParameters = 3;

	if(GetHelpOption(paramsList))
	{
		ShowUsage();

		return true;
	}

	if (paramsList.size() < kMinimumNumberOfParameters)
	{
		// Show error
		ShowUsage();

		return false;
	}

	if (GetXMLFileName(paramsList))
	{
		if (GetOutputPath(paramsList))
		{
			if (IsLanguageGerman(paramsList))
			{
				//Initiate CLILogger for German language
			}

			if (IsLogVerbose(paramsList))
			{
				//Initiate CLILogger for logging on console
			}

			//Validate the parameters


			//Parse the XML file
			ProjectParser::GetInstance().ParserXMLFile(xmlFilePath);


			//Generate output files


			return true;
		}
		else
		{
			// Show error

			return false;
		}
	}
	else
	{
		// Show error

		return false;
	}
}

/**************************** Private members ********************************/

void OpenConfiguratorCLI::ShowUsage()
{
	const std::string kMsgToolGenerator     = "Kalycito Infotech Private Limited ";
	const std::string kMsgToolVendor        = "B&R Internal" ;
	const std::string kMsgToolUsage         = "Usage: openCONFIGURATOR [options]";
	const std::string kMsgOptions           = "Options: ";
	const std::string kMsgProjectParameter  = " -p,--project <ProjectFile> \t Project XML file. ";
	const std::string kMsgOutputParameter   = " -o,--output <OutputPath> \t Output path for generated files. ";
	const std::string kMsgLanguageParameter = " -de,--german \t\t\t German log messages. Default is English.";
	const std::string kMsgLogMessage        = " -d,--debug \t\t\t Log on file. Default logging is on console";
	const std::string kMsgHelpParameter     = " -h,--help \t\t\t Help. ";

	std::cout << kMsgToolGenerator << std::endl;
	std::cout << kMsgToolVendor << std::endl;
	std::cout << kMsgToolUsage << std::endl;
	std::cout << std::endl;
	std::cout << kMsgOptions << std::endl;
	std::cout << kMsgProjectParameter << std::endl;
	std::cout << kMsgOutputParameter << std::endl;
	std::cout << kMsgLanguageParameter << std::endl;
	std::cout << kMsgLogMessage << std::endl;
	std::cout << kMsgHelpParameter << std::endl;
}

bool OpenConfiguratorCLI::GetXMLFileName(std::vector<std::string> paramsList)
{
	for (uint8_t index = 0; index < paramsList.size(); index++)
	{
		// Search for project file option
		if ((paramsList.at(index) == "-p")  || (paramsList.at(index) == "--project"))
		{
			xmlFilePath = paramsList.at(index + 1);
			break;
		}
	}

	// if options '-p' or '--project' not found, consider the first parameter
	// as the project XML file name
	if (xmlFilePath == "")
	{
		xmlFilePath = paramsList.at(0);
	}

	return true;
}

bool OpenConfiguratorCLI::GetOutputPath(std::vector<std::string> paramsList)
{
	for (uint8_t index = 0; index < paramsList.size(); index++)
	{
		// Search for output path option
		if ((paramsList.at(index) == "-o")  || (paramsList.at(index) == "--output"))
		{
			outputPath = paramsList.at(index + 1);
			return true;
		}
	}

	return false;
}

bool OpenConfiguratorCLI::IsLanguageGerman(std::vector<std::string> paramsList)
{
	for (uint8_t index = 0; index < paramsList.size(); index++)
	{
		// Search for German language option
		if ((paramsList.at(index) == "-de")  || (paramsList.at(index) == "--german"))
		{
			return true;
		}
	}

	return false;
}

bool OpenConfiguratorCLI::IsLogVerbose(std::vector<std::string> paramsList)
{
	for (uint8_t index = 0; index < paramsList.size(); index++)
	{
		// Search for verbose option
		if ((paramsList.at(index) == "-d")  || (paramsList.at(index) == "--debug"))
		{
			return true;
		}
	}

	return false;
}

bool OpenConfiguratorCLI::GetHelpOption(std::vector<std::string> paramsList)
{
	for (uint8_t index = 0; index < paramsList.size(); index++)
	{
		// Search for verbose option
		if ((paramsList.at(index) == "-h")  || (paramsList.at(index) == "--help"))
		{
			return true;
		}
	}

	return false;
}
