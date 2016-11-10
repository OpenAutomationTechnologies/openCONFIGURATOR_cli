/************************************************************************
\file ValidateCLIParameter.cpp
\brief Implementation of the Class to validate the input parameters in command line.
\author Sree Hari Vignesh, Kalycito Infotech Private Limited.
*
* @copyright (c) 2016, Kalycito Infotech Private Limited
*                    All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the
*     documentation and/or other materials provided with the distribution.
*   * Neither the name of the copyright holders nor the
*     names of its contributors may be used to endorse or promote products
*     derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
------------------------------------------------------------------------------*/

#include "OpenConfiguratorCore.h"
#include "ValidateCLIParameter.h"
#include "CLIResult.h"
#include "LoggingConfiguration.h"

using namespace IndustrialNetwork::POWERLINK::Core::API;
using namespace IndustrialNetwork::POWERLINK::Application::ErrorHandling;

char* xmlFile = "";

void ValidateCLIParameter::Show_usage() {
	boost::format formatter(kMsgToolGenerator);
	
		//LOG_ERROR() << formatter.str();
		std::cout<< "Kalycito Infotech Private Limited "<< std::endl;
		std::cout<< "B&R Internal" << std::endl;
		std::cout << "Usage: CdcBuilder.exe <ProjectFilePath> [options]"<< std::endl;
		std::cout<< " Options: "<<std::endl;
		std::cout<< " -p,--project <ProjectFile>   Project XML file. "<<std::endl;
		std::cout<< " -o,--output <OutputPath>     Output path for generated files. "<<std::endl;
		std::cout<< " -de,--german                 German log messages. Default is English."<<std::endl;
		std::cout<< " -v,--verbose                 Log file. "<<std::endl;
		std::cout<< " -h,--help <Help>             Help. "<<std::endl;
		exit(1);
}


ValidateCLIParameter& ValidateCLIParameter::GetInstance()
{
	static ValidateCLIParameter instance;
	return instance;
}

std::string ValidateCLIParameter::GetNetworkName() {
	return boost::filesystem::basename(xmlFile);
}

int ValidateCLIParameter::ValidateParameter(int parameterCount, char* parameter[])
{
	OpenConfiguratorCore::GetInstance();
	std::string projectXMLFile = "";
	
	if(parameterCount < 4) {
		boost::format formatter(kMsgInvalidArguments);
		std::cout << "Invalid number of arguments. Expected: 3 Actual: "<< parameterCount << std::endl;
		Show_usage();
		exit(1);
	}

	if(parameterCount == 1) {
	Show_usage();
	exit(1);
	}

	if(parameterCount == 2) {
		std::cout << "Insufficient command line parameters." << std::endl;
		Show_usage();
		exit(1);
	}

	std::string projectFileparam = std::string(parameter[1]);

	if((projectFileparam == "-p") || (projectFileparam == "--project"))  {

		projectXMLFile = std::string(parameter[2]);
		xmlFile = parameter[2];
		if(!boost::filesystem::exists(projectXMLFile)) {
			std::cout << "Project file does not exists." << std::endl;
			exit(1);
		}
		
		if(boost::filesystem::extension(projectXMLFile) != ".xml") {
			std::cout << "Invalid file!" << std::endl;
			exit(1);
		}
		if(parameter[3] != NULL) {
		std::string outputfileparam = std::string(parameter[3]);
		if(outputfileparam != "") {
			if((outputfileparam == "-o") || (outputfileparam == "--output")) {
			std::string outputFile = std::string(parameter[4]);
			if(!boost::filesystem::exists(outputFile)) {
				std::cout << "Output directory does not exists." << std::endl;
				exit(1);
			}
		}else {
			std::cout << "Invalid command line parameter. ";
			Show_usage();
			exit(1);
		}
		}
		}
	}else {
		projectXMLFile = std::string(parameter[1]);
		xmlFile = parameter[1];
		if(!boost::filesystem::exists(projectXMLFile)) {
			std::cout << "Project file does not exists." << std::endl;
		}
		if(boost::filesystem::extension(projectXMLFile) != ".xml") {
			std::cout << "Invalid file!" << std::endl;
		}
		if(parameter[2] != NULL) {

		std::string outputfileparam = std::string(parameter[2]);

		if((outputfileparam == "-o") || (outputfileparam == "--output")) {
			std::string outputFile = std::string(parameter[3]);
			if(!boost::filesystem::exists(outputFile)) {
				std::cout << "Output directory does not exists." << std::endl;
				exit(1);
			}
		}else {
			std::cout << "Invalid command line parameter. ";
			Show_usage();
			exit(1);
		}
		}
	}

	//ParseXMLXDC(XmlFile);
	return 0;
}	
