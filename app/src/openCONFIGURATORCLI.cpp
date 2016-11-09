/************************************************************************
\file openCONFIGURATORCLI.cpp
\brief Implementation of the Class Range
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
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include <string>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>


using namespace IndustrialNetwork::POWERLINK::Core::API;
using namespace std;
namespace fs = boost::filesystem;
XERCES_CPP_NAMESPACE_USE


void show_usage(string name)
{
	cout << "Kalycito Infotech Private Limited " << endl;
	cout << "Bernecker + Rainer BR.FS.CdcBuilder 2.x " << endl;
	cout << "Usage: openCONFIGURATOR.exe <ProjectFilePath> [options]" << endl;
	cout << " Options: " << endl;
	cout << " -p (or) --project <ProjectPath>   Project XML path. " << endl;
	cout << " -o (or) --output  <OutputPath>    Output path for generated files. " << endl;
	cout << " -de <German>                      German log messages. " << endl;
	cout << " -d <Debug log>                    Log file. " << endl;
	cout << " -h (or) --help                    Help. " << endl;
	exit(1);
}

string getFileExtension(string& s)
{
	return (fs::extension(s));
}

string getFileName(string& s)
{
	return (fs::basename(s) + fs::extension(s));
}

int main(int parameterCount, char* parameter[])
{
	OpenConfiguratorCore::GetInstance();
	string projectXMLFile = "";
	char* xml = "";
	if (parameterCount < 4)
	{
		cout << "Invalid number of arguments. Expected: 3 Actual: " << parameterCount << endl;
		show_usage("help");
		exit(1);
	}

	if (parameterCount == 1)
	{
		show_usage("help");
		exit(1);
	}

	if (parameterCount == 2)
	{
		cout << "Insufficient command line parameters." << endl;
		show_usage("help");
		exit(1);
	}

	string projectFileparam = string(parameter[1]);

	if ((projectFileparam == "-p") || (projectFileparam == "--project"))
	{

		projectXMLFile = string(parameter[2]);
		xml = parameter[2];
		if (!fs::exists(projectXMLFile))
		{
			cout << "Project file does not exists." << endl;
			exit(1);
		}

		if (getFileExtension(projectXMLFile) != ".xml")
		{
			cout << "Invalid file!" << endl;
			exit(1);
		}
		if (parameter[3] != NULL)
		{
			string outputfileparam = string(parameter[3]);
			if (outputfileparam != "")
			{
				if ((outputfileparam == "-o") || (outputfileparam == "--output"))
				{
					string outputFile = string(parameter[4]);
					if (!fs::exists(outputFile))
					{
						cout << "Output directory does not exists." << endl;
						exit(1);
					}
				}
				else
				{
					cout << "Invalid command line parameter. ";
					show_usage("help");
					exit(1);
				}
			}
		}
	}
	else
	{
		projectXMLFile = string(parameter[1]);
		xml = parameter[1];
		if (!fs::exists(projectXMLFile))
		{
			cout << "Project file does not exists." << endl;
		}
		if (getFileExtension(projectXMLFile) != ".xml")
		{
			cout << "Invalid file!" << endl;
		}
		if (parameter[2] != NULL)
		{

			string outputfileparam = string(parameter[2]);

			if ((outputfileparam == "-o") || (outputfileparam == "--output"))
			{
				string outputFile = string(parameter[3]);
				if (!fs::exists(outputFile))
				{
					cout << "Output directory does not exists." << endl;
					exit(1);
				}
			}
			else
			{
				cout << "Invalid command line parameter. ";
				show_usage("help");
				exit(1);
			}
		}
	}

	return 0;
}



