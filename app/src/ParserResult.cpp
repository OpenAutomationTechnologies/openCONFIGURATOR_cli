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
#include <map>
#include <boost/algorithm/string.hpp>

using namespace IndustrialNetwork::POWERLINK::Application::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;
using namespace IndustrialNetwork::POWERLINK::Application;

ParserResult::ParserResult() :
	parameters(),
	resultNodeValue(NULL),
	node()
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
		return CliResult(CliErrorCode::NULL_POINTER_FOUND,
		                 kMsgNullPtrFound[CliLogger::GetInstance().languageIndex]);
	}

	try
	{
		/** Get result of Managing Node */
		XMLCh* expression = xercesc::XMLString::transcode(transcodeString.data());
		xercesc::DOMXPathResult* nResult = pElement.domDocument->evaluate(
		                                       expression,
		                                       parentNode,
		                                       NULL,
		                                       xercesc::DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
		                                       NULL);
		xercesc::XMLString::release(&expression);

		resultNodeValue = nResult->getNodeValue();
		if (resultNodeValue == NULL)
		{
			boost::format formatter(kMsgNoResultForXPath[CliLogger::GetInstance().languageIndex]);
			formatter % transcodeString;

			return CliResult(CliErrorCode::NO_RESULT_FOR_XPATH, formatter.str());
		}

		std::vector<std::string> attributeNames;
		boost::split(attributeNames, formatString, boost::is_any_of(","));

		/**  Parse the parameters */
		for (XMLSize_t nIndex = 0; nIndex < nResult->getSnapshotLength(); nIndex++)
		{
			nResult->snapshotItem(nIndex);
			node.push_back(nResult->getNodeValue());
			ParseAttributeValues(nResult->getNodeValue(), attributeNames);
		}
		nResult->release();
	}
	catch (const std::exception& e)
	{
		return CliLogger::GetInstance().GetFailureErrorString(e);
	}
	return CliResult();
}

void ParserResult::ParseAttributeValues(const xercesc::DOMNode* domNode,
                                        const std::vector<std::string>& attributeName)
{
	try
	{
		std::map<std::string, std::string> attributeValues;
		std::vector<std::string> nodeVec;
		if (domNode != NULL)
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
						char* val = xercesc::XMLString::transcode(currentNode->getNodeValue());

						std::pair<std::string, std::string> item;
						item = std::make_pair(std::string(attribute), std::string(val));
						attributeValues.insert(item);

						xercesc::XMLString::release(&attribute);
						xercesc::XMLString::release(&val);
					}
				}

				for (auto& attrName : attributeName)
				{
					if (attributeValues.find(attrName) != attributeValues.end())
						nodeVec.push_back(attributeValues.at(attrName));
					else
						nodeVec.push_back("");
				}
				this->parameters.push_back(nodeVec);
			}
		}
	}
	catch (const std::exception& e)
	{
		LOG_ERROR() << CliLogger::GetInstance().GetFailureErrorString(e).GetErrorMessage();
	}
}
