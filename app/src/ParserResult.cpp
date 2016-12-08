/**
 * \file ParserResult.cpp
 *
 * \brief Implementation of ParserResult module
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 0.1
 *
 */

#include "OpenConfiguratorCore.h"
#include "OpenConfiguratorCLI.h"
#include "ParserResult.h"

ParserResult::ParserResult()
{

}

ParserResult::~ParserResult()
{

}

CLIResult ParserResult::CreateResult(ParserElement& pElement,
					std::string transcodeString,
					std::string formatString)
{
	return CreateResult(pElement,
				 transcodeString,
				 formatString,
				 pElement.docElement);
}

CLIResult ParserResult::CreateResult(ParserElement& pElement,
					std::string transcodeString,
					std::string formatString,
					xercesc::DOMNode* parentNode)
{
	if (parentNode == NULL)
	{
		boost::format formatter(kMsgNullPtrFound[CLILogger::GetInstance().languageIndex]);
		formatter
		% "Create Result";

		return CLIResult(CLIErrorCode::NULL_POINTER_FOUND, formatter.str());
	}

	try
	{
		/**< Get result of Managing Node */
		xercesc::DOMXPathResult* nResult = pElement.docHandle->evaluate(
									   xercesc::XMLString::transcode(transcodeString.c_str()),
									   parentNode,
									   NULL,
									   xercesc::DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
									   NULL);

		resultNodeValue = nResult->getNodeValue();

		if (resultNodeValue == NULL)
		{
			boost::format formatter(kMsgNoResultForXPath[CLILogger::GetInstance().languageIndex]);
			formatter
			% transcodeString.c_str();

			return CLIResult(CLIErrorCode::NO_RESULT_FOR_XPATH, formatter.str());
		}
		else
		{
			std::vector<std::string> attributeNames;
			std::string subString;

			/**<  Convert the string into string stream */
			std::stringstream splitString(formatString);

			/**<  Interpret the formatString and split in to attribute names */
			while(std::getline(splitString, subString, ','))
			{
				attributeNames.push_back(subString);
			}

			XMLSize_t count = nResult->getSnapshotLength();

			/**<  Parse the parameters */
			for (XMLSize_t nIndex = 0; nIndex < count; nIndex++)
			{
				nResult->snapshotItem(nIndex);
				node.push_back(nResult->getNodeValue());

				std::vector<std::string> row;
				for (std::uint32_t attrIndex = 0; attrIndex < attributeNames.size(); attrIndex++)
				{
					row.push_back(GetAttributeValue(node.at(nIndex), attributeNames.at(attrIndex)));
				}

				parameters.push_back(row);
			}

		}

		nResult->release();
	}
	catch (std::exception& ex)
	{
		return CLILogger::GetInstance().HandleExceptionCaught("Create Result", ex);
	}

	return CLIResult();
}

std::string ParserResult::GetAttributeValue(xercesc::DOMNode* domNode, std::string attributeName)
{
	if (domNode == NULL)
	{
		boost::format formatter(kMsgNullPtrFound[CLILogger::GetInstance().languageIndex]);
		formatter
		% "Get Attribute Value";

		CLIResult res = CLIResult(CLIErrorCode::NULL_POINTER_FOUND, formatter.str());

		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);
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
	catch (std::exception& ex)
	{
		CLIResult res = CLILogger::GetInstance().HandleExceptionCaught("Get Attribute Value", ex);

		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);
	}

	return "";
}

