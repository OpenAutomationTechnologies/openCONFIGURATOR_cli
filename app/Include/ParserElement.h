/**
 * \class ParserElement
 *
 * \brief Set of handles of a file to be parsed
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

#ifndef PARSER_ELEMENT_H_
#define PARSER_ELEMENT_H_

#include "OpenConfiguratorCli.h"
#include "ParserErrorHandler.h"
#include <xercesc/sax/SAXParseException.hpp>

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Application
		{
			class ParserElement
			{
				public:

					/** \brief Constructor of the class with file
					  * \param file			Name of XML ot XDC file to parse
					  * \param schemaFile	Name of the schema file for validation
					  */
					ParserElement(const std::string& file, const std::string& schemaFile, const std::string& xmlNamespace);

					/** \brief Destructor of the class
					  */
					~ParserElement();

					/** \brief Creates element for parsing
					  * \return CliResult
					  */
					IndustrialNetwork::POWERLINK::Application::ErrorHandling::CliResult CreateElement();

					/** Document handle */
					xercesc::DOMDocument* domDocument;

					/** Document element handle */
					xercesc::DOMElement* domElement;

					/** DOM parser handle */
					xercesc::XercesDOMParser* domParser;

				private:
					/** \brief private copy constructor, no definition (declaration-only) */
					ParserElement(const ParserElement&);

					/** \brief private copy assign, no definition (declaration-only) */
					ParserElement& operator=(const ParserElement&);

					/** \brief Validates against schema file and parses
					  * \return CliResult
					  */
					IndustrialNetwork::POWERLINK::Application::ErrorHandling::CliResult parseFile();

					/** file that has to be parsed */
					std::string filePath;

					/** schema file for validation */
					std::string schemaFilePath;

					/** namespace for validation */
					std::string xmlNamespace;

			}; // end of class ParserElement
		} // end of namespace Application
	} // end of namespace POWERLINK
} // end of namespace IndustrialNetwork

#endif // _PARSER_ELEMENT_H_
