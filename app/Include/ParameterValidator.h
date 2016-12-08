/**
 * \class ParameterValidator
 *
 * \brief Functionalities for validation of XML file and output path
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 0.1
 *
 */

#ifndef PARAMETER_VALIDATOR_H
#define PARAMETER_VALIDATOR_H

#include "OpenConfiguratorCLI.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Application
		{
			const std::string kXmlSchemaDefinitionFileName = "openCONFIGURATOR.xsd";
			const std::string kXdcSchemaDefinitionFileName = "Powerlink_Main.xsd";

			class ParameterValidator
			{
				public:
					/// Default constructor of the class
					ParameterValidator();

					/// Descriptor of the class
					~ParameterValidator();

					/** \brief Creates single instance
					  * \return Static instance of the class
					  */
					static ParameterValidator& GetInstance();

					/** \brief Validates the XML file existence and extension
					  * \param xmlFileName XML file name to be validated
					  * \return CLIResult
					  */
					CLIResult IsXMLFileValid(std::string xmlFileName);

					/** \brief Validates the output path existence
					  * \param path Output path to be validated
					  * \return CLIResult
					  */
					CLIResult IsPathValid(std::string path);

					/** \brief Validates the schema of XML file
					  * \param xmlFileName XML file name to be validated
					  * \return CLIResult
					  */
					CLIResult IsXMLSchemaValid(std::string xmlFileName);

					/** \brief Validates the schema of XDC file
					  * \param xdcFileName XDC file name to be validated
					  * \return CLIResult
					  */
					CLIResult IsXDCSchemaValid(std::string xdcFileName);
				private:
					/** \brief Validates the schema of given file
					  * \param fileName file to be validated
					  * \param schemaDefFile schema definition file for reference 
					  * \return CLIResult
					  */
					CLIResult IsSchemaValid(std::string fileName, const std::string schemaDefFile);

			}; // end of class ParameterValidator
		} // end of namespace Application
	} // end of namespace POWERLINK
} // end of namespace IndustrialNetwork

#endif // PARAMETER_VALIDATOR_H
