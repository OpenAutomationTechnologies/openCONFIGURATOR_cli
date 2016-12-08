/**
 * \class ConfigurationGenerator
 *
 * \brief Generates the configuration file by getting byte stream from Core library
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 0.1
 *
 */

#ifndef CONFIGURATION_GENERATOR_CLI_H
#define CONFIGURATION_GENERATOR_CLI_H

#include "OpenConfiguratorCLI.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Application
		{
			const std::string kMnobdTxtFileName = "mnobd.txt";
			const std::string kMnobdCdcFileName = "mnobd.cdc";
			const std::string kMnobdHexTxtFileName = "mnobd_char.txt";
			const std::string kXapHFileName = "xap.h";
			const std::string kXapXmlFileName = "mnobd_char.txt";
			const std::string kProcessImageFileName = "mnobd_char.txt";

			class ConfigurationGenerator
			{
				public:
					/// Default constructor of the class
					ConfigurationGenerator();

					/// Descriptor of the class
					~ConfigurationGenerator();

					/** \brief Creates single instance
					  * \return Static instance of the class
					  */
					static ConfigurationGenerator& GetInstance();

					/** \brief Parses and builds configuration output files
					  * \param xmlPath XML file name with path to be parsed
					  * \param outputPath Output path of the configuraiton files
					  * \return CLIResult
					  */
					CLIResult GenerateConfigurationFiles(std::string xmlPath, std::string outputPath);
				private:
					/** \brief Creates Mnobd configuration files
					  * \param outputPath Output path of the configuraiton files
					  * \return CLIResult
					  */
					CLIResult BuildConciseDeviceConfiguration(std::string outputPath);

					/** \brief Creates process images
					  * \param outputPath Output path of the image files
					  * \return CLIResult
					  */
					CLIResult BuildProcessImageDescriptions(std::string outputPath);

					/** \brief Creates mnobd.txt
					  * \param outputPath Output path of the image files
					  * \param configuration Configuration output to be copied
					  * \return CLIResult
					  */
					CLIResult CreateMnobdTxt(std::string outputPath, std::string configuration);

					/** \brief Creates mnobd.cdc
					  * \param outputPath Output path of the image files
					  * \param buffer Buffer output to be coiped
					  * \return CLIResult
					  */
					CLIResult CreateMnobdCdc(std::string outputPath, std::ostringstream& buffer);

					/** \brief Creates mnobdhex.txt
					  * \param outputPath Output path of the image files
					  * \param buffer Buffer output to be coiped
					  * \return CLIResult
					  */
					CLIResult CreateMnobdHexTxt(std::string outputPath, std::ostringstream& buffer);

					/** \brief Creates CProcess image file
					  * \param nodeId Node for which the process to be created
					  * \param outputPath Output path of the image files
					  * \return CLIResult
					  */
					CLIResult CreateCProcessImage(std::uint8_t nodeId, std::string outputPath);

					/** \brief Creates XML process image file
					  * \param nodeId Node for which the process to be created
					  * \param outputPath Output path of the image files
					  * \return CLIResult
					  */
					CLIResult CreateXMLProcessImage(std::uint8_t nodeId, std::string outputPath);

					/** \brief Creates CSharp process image file
					  * \param nodeId Node for which the process to be created
					  * \param outputPath Output path of the image files
					  * \return CLIResult
					  */
					CLIResult CreateCSharpProcessImage(std::uint8_t nodeId, std::string outputPath);

			}; // end of class ConfigurationGenerator
		} // end of namespace Application
	} // end of namespace POWERLINK
} // end of namespace IndustrialNetwork

#endif // CONFIGURATION_GENERATOR_CLI_H
