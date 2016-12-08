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
#include "ConfigurationGenerator.h"

OpenConfiguratorCLI::OpenConfiguratorCLI()
{
	xmlFilePath = "";
	outputPath = "";
	networkName = "";
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
	networkName = boost::filesystem::basename(xmlFilePath);
	return networkName;
}

CLIResult OpenConfiguratorCLI::GeneratePOWERLINKConfigurationFiles(std::vector<std::string> paramsList)
{
	const std::uint8_t kMinimumNumberOfParameters = 3;
	CLIResult res;

	if(GetHelpOption(paramsList))
	{
		ShowUsage();

		return CLIResult();
	}

	if (paramsList.size() < kMinimumNumberOfParameters)
	{
		ShowUsage();

		return CLIResult(CLIErrorCode::LESS_NO_OF_PARAMS, 
							kMsgLessNoOfParams[CLILogger::GetInstance().languageIndex]);
	}

	if (GetXMLFileName(paramsList))
	{
		if (GetOutputPath(paramsList))
		{
			if (IsLanguageGerman(paramsList))
			{
				/**< Initiate CLILogger for German language */
				CLILogger::GetInstance().SetLanguageToGerman(true);
			}

			if (IsLogDebug(paramsList))
			{
				/**< Initiate CLILogger for logging on console */
				CLILogger::GetInstance().SetFileLog(true, outputPath);
			}

			GetNetworkName();

			/**< Validate the parameters */
			res = ParameterValidator::GetInstance().IsXMLFileValid(xmlFilePath);
			if (!res.IsSuccessful())
			{
				return res;
			}

			res = ParameterValidator::GetInstance().IsPathValid(outputPath);
			if (!res.IsSuccessful())
			{
				CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);

				/**< Create the output path as it doesnt exists */
				boost::filesystem::path dir(outputPath);
				if(!boost::filesystem::create_directory(dir))
				{
					/**< Failed to create the output path */
					return res;
				}
			}

//			res = ParameterValidator::GetInstance().IsXMLSchemaValid(xmlFilePath);
			//if (!res.IsSuccessful())
//			{
				/**< XML file schema is not valid */
//				return res;
//			}

			/**< Parse and Generate configuration output */
			res = ConfigurationGenerator::GetInstance().GenerateConfigurationFiles(xmlFilePath, outputPath);
			if (!res.IsSuccessful())
			{
				/**< Unable to parse XML or generate configuration files */
				return res;
			}

			return CLIResult();
		}
		else
		{
			return CLIResult(CLIErrorCode::OUTPUT_PATH_NOT_FOUND, 
							kMsgOutputPathNotFound[CLILogger::GetInstance().languageIndex]);
		}
	}
	else
	{
		return CLIResult(CLIErrorCode::XML_FILE_NOT_FOUND, 
							kMsgXMLFileNotFound[CLILogger::GetInstance().languageIndex]);
	}
}

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
	for (std::uint8_t index = 0; index < paramsList.size(); index++)
	{
		/**< Search for project file option */
		if ((paramsList.at(index) == "-p")  || (paramsList.at(index) == "--project"))
		{
			xmlFilePath = paramsList.at(index + 1);
			break;
		}
	}

	/**< if options '-p' or '--project' not found, consider the first parameter
	     as the project XML file name */
	if (xmlFilePath.empty())
	{
		xmlFilePath = paramsList.at(0);
	}

	return true;
}

bool OpenConfiguratorCLI::GetOutputPath(std::vector<std::string> paramsList)
{
	for (std::uint8_t index = 0; index < paramsList.size(); index++)
	{
		/**< Search for output path option */
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
	for (std::uint8_t index = 0; index < paramsList.size(); index++)
	{
		/**< Search for German language option */
		if ((paramsList.at(index) == "-de")  || (paramsList.at(index) == "--german"))
		{
			return true;
		}
	}

	return false;
}

bool OpenConfiguratorCLI::IsLogDebug(std::vector<std::string> paramsList)
{
	for (std::uint8_t index = 0; index < paramsList.size(); index++)
	{
		/**< Search for verbose option */
		if ((paramsList.at(index) == "-d")  || (paramsList.at(index) == "--debug"))
		{
			return true;
		}
	}

	return false;
}

bool OpenConfiguratorCLI::GetHelpOption(std::vector<std::string> paramsList)
{
	for (std::uint8_t index = 0; index < paramsList.size(); index++)
	{
		/**< Search for verbose option */
		if ((paramsList.at(index) == "-h")  || (paramsList.at(index) == "--help"))
		{
			return true;
		}
	}

	return false;
}
