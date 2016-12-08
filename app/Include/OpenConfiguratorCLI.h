/**
 * \class OpenConfiguratorCLI
 *
 * \brief Main engine that calls functions for validation, parsing and configuration generation
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 0.1
 *
 */

#ifndef OPEN_CONFIGURATOR_CLI_H
#define OPEN_CONFIGURATOR_CLI_H

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
#include "CLIErrorCode.h"
#include "Result.h"
#include "CLIResult.h"
#include "CLILogger.h"
#include "CLIConstants.h"

using namespace IndustrialNetwork::POWERLINK::Core::API;
using namespace IndustrialNetwork::POWERLINK::Core::Node;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;
using namespace IndustrialNetwork::POWERLINK::Core::ModularNode;
using namespace IndustrialNetwork::POWERLINK::Application::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Application;
using namespace IndustrialNetwork::POWERLINK::Application::CLIConstants;

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Application
		{
			class OpenConfiguratorCLI
			{
				public:
					/// Default constructor of the class
					OpenConfiguratorCLI();

					/// Descriptor of the class
					~OpenConfiguratorCLI();

					/** \brief Creates single instance
					  * \return Static instance of the class
					  */
					static OpenConfiguratorCLI& GetInstance();

					/** \brief Gets the network name of the XML file
					  * \return Network name as string; "" in case of failure
					  */
					std::string GetNetworkName();

					/** \brief Generates the configuration output files
					  * \param paramsList List of command line parameters
					  * \return CLIResult
					  */
					CLIResult GeneratePOWERLINKConfigurationFiles(std::vector<std::string> paramsList);

					std::string xmlFilePath;	///< The input XML file with path

					std::string outputPath;		///< The output directory path

					std::string networkName;	///< Network name of the project file

				private:
					/** \brief Prints the command line usage syntax of the application
					  * \return Nothing
					  */
					void ShowUsage();

					/** \brief Searches for options '-p' or '--project' followed by
					  *        the XML file path from command line parameters list
					  * \param paramsList List of command line parameters
					  * \return true if option is found; false otherwise
					  */
					bool GetXMLFileName(std::vector<std::string> paramsList);

					/** \brief Searches for options '-o' or '--output' followed by
					  *        the output path from command line parameters list
					  * \param paramsList List of command line parameters
					  * \return true if option is found; false otherwise
					  */
					bool GetOutputPath(std::vector<std::string> paramsList);

					/** \brief Searches for options '-de' or '--german'
					  *        from command line parameters list
					  * \param paramsList List of command line parameters
					  * \return true if option is found; false otherwise
					  */
					bool IsLanguageGerman(std::vector<std::string> paramsList);

					/** \brief Searches for options '-v' or '--verbose'
					  *        from command line parameters list
					  * \param paramsList List of command line parameters
					  * \return true if option is found; false otherwise
					  */
					bool IsLogDebug(std::vector<std::string> paramsList);

					/** \brief Searches for options '-h' or '--help'
					  *        from command line parameters list
					  * \param paramsList List of command line parameters
					  * \return true if option is found; false otherwise
					  */
					bool GetHelpOption(std::vector<std::string> paramsList);

			}; // end of class OpenConfiguratorCLI
		} // end of namespace Application
	} // end of namespace POWERLINK
} // end of namespace IndustrialNetwork

#endif // OPEN_CONFIGURATOR_CLI_H
