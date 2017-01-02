/**
 * \class OpenConfiguratorCli
 *
 * \brief Main engine that calls functions for validation, parsing and configuration generation
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 1.0
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

#ifndef _OPEN_CONFIGURATOR_CLI_H_
#define _OPEN_CONFIGURATOR_CLI_H_

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
#include <boost/filesystem/fstream.hpp>

#include "OpenConfiguratorCore.h"
#include "LoggingConfiguration.h"
#include "CliErrorCode.h"
#include "Result.h"
#include "CliResult.h"
#include "Language.h"
#include "CliLogger.h"
#include "CliConstants.h"

using namespace IndustrialNetwork::POWERLINK::Core::API;
using namespace IndustrialNetwork::POWERLINK::Core::Node;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;
using namespace IndustrialNetwork::POWERLINK::Core::ModularNode;
using namespace IndustrialNetwork::POWERLINK::Application::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Application;
using namespace IndustrialNetwork::POWERLINK::Application::CliConstants;

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Application
		{
			/** Boost init file for file and console log settings */
			const std::string kLogConfigurationFileName = "boost_log_settings.ini";

			/* Ouput configuration and process image file names */
			const std::string kMnobdTxtFileName = "mnobd.txt";
			const std::string kMnobdCdcFileName = "mnobd.cdc";
			const std::string kMnobdHexTxtFileName = "mnobd_char.txt";
			const std::string kXapHFileName = "xap.h";
			const std::string kXapXmlFileName = "xap.xml";
			const std::string kProcessImageFileName = "ProcessImage.cs";

			/** Schema file names for validation */
			const std::string kXmlSchemaDefinitionFileName = "./resources/OC_ProjectFile/openCONFIGURATOR.xsd";
			const std::string kXdcSchemaDefinitionFileName = "./resources/xddschema/Powerlink_Main.xsd";
			const std::string kErrCodeXmlSchemaDefinitionFileName = "./resources/ErrorCodesXMLSchema/ocErrorCodes.xsd";

			/** Error code XML file from where the error code table gets populate */
			const std::string kErrorCodeXmlFile = "./resources/error_codes.xml";

			/** Maximum number of paramater allowed */
			const std::uint8_t kMaxNumberOfParameters = 6;

			class OpenConfiguratorCli
			{
				public:
					/** \brief Default constructor of the class
					  */
					OpenConfiguratorCli();

					/** \brief Destructor of the class
					  */
					~OpenConfiguratorCli();

					/** \brief Creates single instance
					  * \return Static instance of the class
					  */
					static OpenConfiguratorCli& GetInstance();

					/** \brief Generates the configuration output files
					  * \param paramsList		List of command line parameters
					  * \return CliResult
					  */
					CliResult GenerateConfigurationFiles(const std::vector<std::string>& paramsList);

					/** The input XML file with path */
					std::string xmlFilePath;

					/** The output directory path */
					std::string outputPath;

					/** Network name of the project file */
					std::string networkName;

				private:
					/** \brief Prints the command line usage syntax of the application
					  * \return Nothing
					  */
					void ShowUsage();

					/** \brief Searches for options '-p' or '--project' followed by
					  *        the XML file path from command line parameters list
					  * \param paramsList		List of command line parameters
					  * \return true if option is found; false otherwise
					  */
					bool GetXmlFileName(const std::vector<std::string>& paramsList);

					/** \brief Searches for options '-o' or '--output' followed by
					  *        the output path from command line parameters list
					  * \param paramsList		List of command line parameters
					  * \return true if option is found; false otherwise
					  */
					bool GetOutputPath(const std::vector<std::string>& paramsList);

					/** \brief Searches for options '-de' or '--german'
					  *        from command line parameters list
					  * \param paramsList		List of command line parameters
					  * \return true if option is found; false otherwise
					  */
					bool IsLanguageGerman(const std::vector<std::string>& paramsList);

					/** \brief Searches for options '-v' or '--verbose'
					  *        from command line parameters list
					  * \param paramsList		List of command line parameters
					  * \return true if option is found; false otherwise
					  */
					bool IsLogDebug(const std::vector<std::string>& paramsList);

					/** \brief Searches for options '-h' or '--help'
					  *        from command line parameters list
					  * \param paramsList		List of command line parameters
					  * \return true if option is found; false otherwise
					  */
					bool GetHelpOption(const std::vector<std::string>& paramsList);

			}; // end of class OpenConfiguratorCli
		} // end of namespace Application
	} // end of namespace POWERLINK
} // end of namespace IndustrialNetwork

#endif // _OPEN_CONFIGURATOR_CLI_H_
