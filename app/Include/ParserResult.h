/**
 * \class ParserResult
 *
 * \brief Creates results after parsing files
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

#ifndef _PARSER_RESULT_H_
#define _PARSER_RESULT_H_

#include "OpenConfiguratorCli.h"
#include "ParserElement.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Application
		{
			class ParserResult
			{
				public:
					/** \brief Default constructor of the class 
					  */
					ParserResult();

					/** \brief Destructor of the class 
					  */
					~ParserResult();

					/** \brief Parses and produces attributes from root
					  * \param pElement 		File element to be parsed
					  * \param transcodeString 	Transcode input string for parsing
					  * \param formatString 	Attribute names as string separated by comma
					  * \return CliResult
					  */
					CliResult CreateResult(const ParserElement& pElement,
									const std::string transcodeString,
									const std::string formatString);

					/** \brief Parses and produces attributes by considering
					  *        an sub node element as parent
					  * \param pElement 		File element to be parsed
					  * \param transcodeString 	Transcode input string for parsing
					  * \param formatString 	Attribute names as string separated by comma
					  * \param parentNode 		Sub node to be considered as parent
					  * \return CliResult
					  */
					CliResult CreateResult(const ParserElement& pElement,
									const std::string transcodeString,
									const std::string formatString,
									const xercesc::DOMNode* parentNode);

					/** Parsed parameters */
					std::vector<std::vector<std::string> > parameters;		

					/** Parent node */
					xercesc::DOMNode* resultNodeValue;

					/** Parsed node */
					std::vector<xercesc::DOMNode*> node;

				private:
					/** \brief Gets the value of attribute available
					  * \param domNode 			Instance of DOMNode
					  * \param attributeName 	Name of the attribute
					  * \return Attribute value
					  */
					std::string GetAttributeValue(const xercesc::DOMNode* domNode,
									const std::string attributeName); 

			}; // end of class ParserResult
		} // end of namespace Application
	} // end of namespace POWERLINK
} // end of namespace IndustrialNetwork

#endif // _PARSER_RESULT_H_
