/**
 * \class ParameterValidator
 *
 * \brief Functionalities for validation of XML file and output path
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

#ifndef _PARAMETER_VALIDATOR_H_
#define _PARAMETER_VALIDATOR_H_

#include "OpenConfiguratorCli.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Application
		{
			const std::string kXmlSchemaDefinitionFileName = "./resources/OC_ProjectFile/openCONFIGURATOR.xsd";
			const std::string kXdcSchemaDefinitionFileName = "./resources/xddschema/Powerlink_Main.xsd";

			class ParameterValidator
			{
				public:
					/** \brief Default constructor of the class
					  */
					ParameterValidator();

					/** \brief Destructor of the class
					  */
					~ParameterValidator();

					/** \brief Creates single instance
					  * \return Static instance of the class
					  */
					static ParameterValidator& GetInstance();

					/** \brief Validates the XML file existence and extension
					  * \param xmlFileName		XML file name to be validated
					  * \return CliResult
					  */
					CliResult IsXmlFileValid(const std::string& xmlFileName);

					/** \brief Validates the XDC file existence and extension
					  * \param xdcFileName		XDC file name to be validated
					  * \return CliResult
					  */
					CliResult IsXdcFileValid(const std::string& xdcFileName);

					/** \brief Validates the output path existence
					  * \param path				Output path to be validated
					  * \return CliResult
					  */
					CliResult IsPathValid(const std::string& path);

					/** \brief Validates the schema of XML file
					  * \param domParserXml		DOM parser handle of XML file
					  * \return CliResult
					  */
					CliResult IsXmlSchemaValid(xercesc::XercesDOMParser* domParserXml);

					/** \brief Validates the schema of XDC file
					  * \param domParserXdc		DOM parser handle of XDC file
					  * \return CliResult
					  */
					CliResult IsXdcSchemaValid(xercesc::XercesDOMParser* domParserXdc);
				private:
					/** \brief Validates file existence and extension
					  * \param fileName			File name to be validated
					  * \param fileExtn			File extention expected
					  * \return CliResult
					  */
					CliResult IsFileValid(const std::string& fileName,
									const std::string& fileExtn);

					/** \brief Validates the schema of given file
					  * \param domParser		DOM parser handle of the file to be validated
					  * \param schemaDefFile	Schema definition file for reference 
					  * \return CliResult
					  */
					CliResult IsSchemaValid(xercesc::XercesDOMParser* domParser, 
									const std::string& schemaDefFile);

			}; // end of class ParameterValidator
		} // end of namespace Application
	} // end of namespace POWERLINK
} // end of namespace IndustrialNetwork

#endif // _PARAMETER_VALIDATOR_H_
