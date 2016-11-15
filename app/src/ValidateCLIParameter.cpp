/************************************************************************
\file ValidateCLIParameter.cpp
\author Sree Hari Vignesh, Kalycito Infotech Private Limited.
\brief Implementation of the Class to validate the input parameters in command line.
************************************************************************/

/*------------------------------------------------------------------------------
Copyright (c) 2016, Kalycito Infotech Private Limited
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

#include "OpenConfiguratorCore.h"
#include "ValidateCLIParameter.h"
#include "CLIResult.h"
#include "LoggingConfiguration.h"
#include "Parser.h"

using namespace IndustrialNetwork::POWERLINK::Core::API;
using namespace IndustrialNetwork::POWERLINK::Application::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;
using namespace IndustrialNetwork::POWERLINK::Application::openCONFIGURATORCLI;

void ValidateCLIParameter::ShowUsage()
{
	LOG_INFO() << kMsgToolGenerator;
	LOG_INFO() << kMsgToolVendor;
	LOG_INFO() << kMsgToolUsage;
	LOG_INFO() << kMsgOptions;
	LOG_INFO() << kMsgProjectParameter;
	LOG_INFO() << kMsgOutputParameter;
	LOG_INFO() << kMsgLanguageParameter;
	LOG_INFO() << kMsgLogMessage;
	LOG_INFO() << kMsgHelpParameter;
	exit(1);
}

ValidateCLIParameter& ValidateCLIParameter::GetInstance()
{
	static ValidateCLIParameter instance;
	return instance;
}

std::string ValidateCLIParameter::GetProjectpath()
{
	return std::string(ValidateCLIParameter::xmlFile);
}

std::string ValidateCLIParameter::GetNetworkName()
{
	return boost::filesystem::basename(ValidateCLIParameter::xmlFile);
}

int ValidateCLIParameter::ValidateParameter(int parameterCount, std::string projectParam, std::string projectPath, std::string outputParam, std::string outputPath)
{
	std::string projectXMLFile = "";

	if (parameterCount < 4)
	{
		LOG_ERROR() << "Invalid number of arguments. Expected: 3 Actual: " << parameterCount;
		ShowUsage();
		exit(1);
	}
	if (parameterCount == 1)
	{
		ShowUsage();
		exit(1);
	}
	if (parameterCount == 2)
	{
		LOG_ERROR() << kMsgInsufficientParameters;
		ShowUsage();
		exit(1);
	}

	std::string projectFileparam = projectParam;

	if ((projectFileparam == "-p") || (projectFileparam == "--project"))
	{
		std::cout << "Section left";
		projectXMLFile = projectPath;
		std::cout << "Section left";
		ValidateCLIParameter::xmlFile = projectPath;
		if (!boost::filesystem::exists(projectXMLFile))
		{
			LOG_ERROR() << kMsgProjectFileExists;
			exit(1);
		}

		if (boost::filesystem::extension(projectXMLFile) != ".xml")
		{
			LOG_ERROR() << kMsgInvalidParameter;
			exit(1);
		}
		const std::string& outputfileparam = outputParam;
		if (!outputfileparam.empty())
		{
			if ((outputfileparam == "-o") || (outputfileparam == "--output"))
			{
				std::string outputFile = outputPath;
				try
				{
					if (!boost::filesystem::exists(outputFile))
					{
						LOG_ERROR() << kMsgOutputDirectoryExists;
						exit(1);
					}
				}
				catch (std::exception exc)
				{
					exc.what();
				}
			}
			else
			{
				LOG_ERROR() << kMsgInvalidParameter;
				ShowUsage();
				exit(1);
			}
		}
	}
	else
	{
		projectXMLFile = projectPath;
		ValidateCLIParameter::xmlFile = projectPath;
		try
		{
			if (!boost::filesystem::exists(projectXMLFile))
			{
				LOG_ERROR() << kMsgProjectFileExists;
			}
		}
		catch (std::exception exc)
		{
			exc.what();
		}
		if (boost::filesystem::extension(projectXMLFile) != ".xml")
		{
			LOG_ERROR() << kMsgInvalidFile;
		}
		std::string outputfileparam = outputParam;

		if ((outputfileparam == "-o") || (outputfileparam == "--output"))
		{
			std::string outputFile = outputPath;
			if (!boost::filesystem::exists(outputFile))
			{
				LOG_ERROR() << kMsgOutputDirectoryExists;
				exit(1);
			}
		}
		else
		{
			LOG_ERROR() << kMsgInvalidParameter;
			ShowUsage();
			exit(1);
		}
	}
	//Parse the project XML file
	Parser::GetInstance().ParseXMLFile(ValidateCLIParameter::xmlFile);
	return 0;
}

