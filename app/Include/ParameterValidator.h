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
			class ParameterValidator
			{
				public:
				/// Default constructor of the class
				ParameterValidator();

				/// Descriptor of the class
				~ParameterValidator();

				/** \brief Validates the XML file existence and extension
				  * \param xmlFileName XML file name
				  * \return true if XML file is valid; false otherwise
				  */
				bool IsXMLFileValid(std::string xmlFileName);

				/** \brief Validates the output path existence
				  * \param path Output path
				  * \return true if path is valid; false otherwise
				  */
				bool IsPathValid(std::string path);

				/** \brief Validates the schema of XML file
				  * \param xmlFileName XML file name
				  * \return true if XML file is valid; false otherwise
				  */
				bool IsXMLSchemaValid(std::string xmlFileName);

				/** \brief Validates the schema of XDC file
				  * \param xdcFileName XDC file name
				  * \return true if XDC file is valid; false otherwise
				  */
				bool IsXDCSchemaValid(std::string xdcFileName);

			}; // end of class ParameterValidator
		} // end of namespace Application
	} // end of namespace POWERLINK
} // end of namespace IndustrialNetwork

#endif // PARAMETER_VALIDATOR_H
