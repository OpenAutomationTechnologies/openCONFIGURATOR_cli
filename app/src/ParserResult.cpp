/**
 * \file ParserResult.cpp
 *
 * \brief Implementation of ParserResult module
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

#include "OpenConfiguratorCore.h"
#include "OpenConfiguratorCli.h"
#include "ParserResult.h"

ParserResult::ParserResult()
{
}

ParserResult::~ParserResult()
{
}

CliResult ParserResult::CreateResult(const ParserElement& pElement,
										const std::string& transcodeString,
										const std::string& formatString)
{
	return CreateResult(pElement,
						transcodeString,
						formatString,
						pElement.domElement);
}

CliResult ParserResult::CreateResult(const ParserElement& pElement,
										const std::string& transcodeString,
										const std::string& formatString,
										const xercesc::DOMNode* parentNode)
{
	if (parentNode == NULL)
	{
		boost::format formatter(kMsgNullPtrFound[CliLogger::GetInstance().languageIndex]);
		formatter % "Create Result";

		return CliResult(CliErrorCode::NULL_POINTER_FOUND, formatter.str());
	}

	try
	{
		/** Get result of Managing Node */
		xercesc::DOMXPathResult* nResult = pElement.domDocument->evaluate(
											xercesc::XMLString::transcode(transcodeString.c_str()),
											parentNode,
											NULL,
											xercesc::DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
											NULL);

		resultNodeValue = nResult->getNodeValue();

		if (resultNodeValue == NULL)
		{
			boost::format formatter(kMsgNoResultForXPath[CliLogger::GetInstance().languageIndex]);
			formatter % transcodeString.c_str();

			return CliResult(CliErrorCode::NO_RESULT_FOR_XPATH, formatter.str());
		}
		else
		{
			std::vector<std::string> attributeNames;
			std::string subString;

			/**  Convert the string into string stream */
			std::stringstream splitString(formatString);

			/**  Interpret the formatString and split in to attribute names */
			while(std::getline(splitString, subString, ','))
			{
				attributeNames.push_back(subString);
			}

			XMLSize_t count = nResult->getSnapshotLength();

			/**  Parse the parameters */
			for (XMLSize_t nIndex = 0; nIndex < count; nIndex++)
			{
				nResult->snapshotItem(nIndex);
				node.push_back(nResult->getNodeValue());

				std::vector<std::string> row;
				for (std::uint32_t attrIndex = 0; attrIndex < attributeNames.size(); attrIndex++)
				{
					row.push_back(GetAttributeValue(node.at(nIndex), 
									attributeNames.at(attrIndex)));
				}

				parameters.push_back(row);
			}
		}

		nResult->release();
	}
	catch (const std::exception& e)
	{
		return CliLogger::GetInstance().HandleExceptionCaught("Create Result", e);
	}

	return CliResult();
}

std::string ParserResult::GetAttributeValue(const xercesc::DOMNode* domNode, 
											const std::string& attributeName)
{
	if (domNode == NULL)
	{
		boost::format formatter(kMsgNullPtrFound[CliLogger::GetInstance().languageIndex]);
		formatter % "Get Attribute Value";

		LOG_WARN() << formatter.str();

		return "";
	}

	try
	{
		if (domNode->hasAttributes())
		{
			xercesc::DOMNamedNodeMap* attributes = domNode->getAttributes();
			const XMLSize_t attribCount = attributes->getLength();

			for (XMLSize_t attribSize = 0; attribSize < attribCount; attribSize++)
			{
				xercesc::DOMNode* currentNode = attributes->item(attribSize);
				if (currentNode->getNodeType() == xercesc::DOMNode::ATTRIBUTE_NODE)
				{
					char* attribute = xercesc::XMLString::transcode(currentNode->getNodeName());

					if (attributeName.compare(attribute) == 0)
					{
						char* val = xercesc::XMLString::transcode(
										domNode->getAttributes()->getNamedItem(
												xercesc::XMLString::transcode(attributeName.c_str())
										)->getNodeValue()
									);

						std::string value(val);
						xercesc::XMLString::release(&val);
						xercesc::XMLString::release(&attribute);

						return value;
					}
					else
					{
						xercesc::XMLString::release(&attribute);
					}
				}
			}
		}
	}
	catch (const std::exception& e)
	{
		CliResult res = CliLogger::GetInstance().HandleExceptionCaught("Get Attribute Value", e);

		LOG_WARN() << res.GetErrorMessage();
	}

	return "";
}
