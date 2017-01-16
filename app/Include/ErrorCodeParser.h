/**
 * \file ErrorCodeParser
 *
 * \brief Includes functionalities to load the error codes from XML file
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

#ifndef _ERROR_CODE_PARSER_H_
#define _ERROR_CODE_PARSER_H_

#include "OpenConfiguratorCli.h"
#include "ParserElement.h"
#include "ParserResult.h"
#include "ParameterValidator.h"
#include "Language.h"
#include "ErrorCodeCompType.h"

using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Application
		{
			/** \brief Component type of the error code
			  */
			namespace ComponentType
			{
				const std::string kComponentLibrary = "library";
				const std::string kComponentCli = "cli";
			}

			class ErrorCodeParser
			{
				public:
					/** \brief Default constructor of the class
					  */
					ErrorCodeParser();

					/** \brief Destructor of the class
					  */
					~ErrorCodeParser();

					/** \brief Creates single instance
					  * \return Static instance of the class
					  */
					static ErrorCodeParser& GetInstance();

					/** \brief Loads the error code XML file
					  * \param xmlFilePath		Error code XML file path
					  * \return CliResult
					  */
					CliResult ParseErrorCodeTable(const std::string& xmlFilePath);

					/** \brief Gets the tool code for the original error code
					  * \param compType			Components type
					  * \param originalCode		Original error code
					  * \param toolCode			Tool codeoutput
					  * \return CliResult
					  */
					CliResult GetToolCode(const std::string& compType,
					                      const std::uint32_t& originalCode,
					                      std::uint32_t& toolCode);
				private:
					/** \brief Gets the tool code for the original error code
					  * \param element 		Element that contains handle of XML
					  * \return CliResult
					  */
					CliResult CreateErrorTable(const ParserElement& element);

					/** \brief Unloads the error code XML file
					  * \return Nothing
					  */
					void CloseErrorCodeTable();

					/** Instance where all the loaded error code gets store */
					std::vector<ErrorCodeCompType> errorCodeObject;

			}; // end of class ErrorCodeParser
		} // end of namespace Application
	} // end of namespace POWERLINK
} // end of namespace IndustrialNetwork

#endif // _ERROR_CODE_PARSER_H_
