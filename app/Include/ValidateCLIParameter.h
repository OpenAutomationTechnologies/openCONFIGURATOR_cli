/************************************************************************
\file ValidateCLIParameter.h
\author Sree Hari Vignesh, Kalycito Infotech Private Limited.
\brief Validates the parameter involved in CLI.
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
#ifndef VALIDATE_CLI_PARAMETER_H
#define VALIDATE_CLI_PARAMETER_H

#include <fstream>
#include <string>
#include <cstdint>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <boost/filesystem.hpp>

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Application
		{
			namespace openCONFIGURATORCLI
			{
				class ValidateCLIParameter
				{
					public:
						//The project XML file path.
						std::string xmlFile;

						/*
						\Returns the instance of class.
						*/
						static ValidateCLIParameter& GetInstance();

						/*
						\Return the name of the network.
						*/
						std::string GetNetworkName();

						/*
						\Returns the path of project XML.
						*/
						std::string GetProjectpath();

						/*
						\Validates the input command line parameters
						\param paramCount The count of command line parameters.
						\param projectParam The project parameter.
						\param projectXmlpathth The XML file path of project.
						\param outputParam The output parameter.
						\param outputpath The output directory.
						*/
						int ValidateParameter(int paramCount, std::string projectParam, std::string projectXmlpathth, std::string outputParam, std::string outputpath);

						/*
						\Displays the help information.
						*/
						void ShowUsage();
				};
			}
		}
	}
}

#endif // VALIDATE_CLI_PARAMETER_H
