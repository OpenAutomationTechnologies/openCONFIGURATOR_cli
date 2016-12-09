/**
 * \file ParserResult.cpp
 *
 * \brief Implementation of ParserResult module
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 0.1
 *
 */// REVIEW_COMMENT: license

#include "OpenConfiguratorCore.h"
#include "OpenConfiguratorCLI.h"
#include "ParserResult.h"

ParserResult::ParserResult()
{
// REVIEW_COMMENT: line not required
}

ParserResult::~ParserResult()
{
// REVIEW_COMMENT: line not required
}

CLIResult ParserResult::CreateResult(ParserElement& pElement,
					std::string transcodeString,// REVIEW_COMMENT: alignment
					std::string formatString)
{
	return CreateResult(pElement,
				 transcodeString,// REVIEW_COMMENT: alignment
				 formatString,
				 pElement.docElement);
}

CLIResult ParserResult::CreateResult(ParserElement& pElement,
					std::string transcodeString,
					std::string formatString,
					xercesc::DOMNode* parentNode)// REVIEW_COMMENT: const
{
	if (parentNode == NULL)
	{
		boost::format formatter(kMsgNullPtrFound[CLILogger::GetInstance().languageIndex]);// REVIEW_COMMENT: 80 chars
		formatter// REVIEW_COMMENT: in a single line
		% "Create Result";

		return CLIResult(CLIErrorCode::NULL_POINTER_FOUND, formatter.str());
	}

	try
	{
		/**< Get result of Managing Node */
		xercesc::DOMXPathResult* nResult = pElement.docHandle->evaluate(
									   xercesc::XMLString::transcode(transcodeString.c_str()),// REVIEW_COMMENT: remove space
									   parentNode,
									   NULL,
									   xercesc::DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
									   NULL);

		resultNodeValue = nResult->getNodeValue();

		if (resultNodeValue == NULL)
		{
			boost::format formatter(kMsgNoResultForXPath[CLILogger::GetInstance().languageIndex]);
			formatter// REVIEW_COMMENT: single line
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
// REVIEW_COMMENT: remove line
		}

		nResult->release();
	}
	catch (std::exception& ex)// REVIEW_COMMENT: e
	{
		return CLILogger::GetInstance().HandleExceptionCaught("Create Result", ex);// REVIEW_COMMENT: 80
	}

	return CLIResult();
}

std::string ParserResult::GetAttributeValue(xercesc::DOMNode* domNode, std::string attributeName)// REVIEW_COMMENT: const
{
	if (domNode == NULL)
	{
		boost::format formatter(kMsgNullPtrFound[CLILogger::GetInstance().languageIndex]);
		formatter// REVIEW_COMMENT: single line
		% "Get Attribute Value";

		CLIResult res = CLIResult(CLIErrorCode::NULL_POINTER_FOUND, formatter.str());
// REVIEW_COMMENT: remove line
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
				{// REVIEW_COMMENT: intentionally used - comment
					char* attribute = xercesc::XMLString::transcode(currentNode->getNodeName());

					if (attributeName.compare(attribute) == 0)// REVIEW_COMMENT: combine IFs
					{
						char* val = xercesc::XMLString::transcode(
										domNode->getAttributes()->getNamedItem(
												xercesc::XMLString::transcode(attributeName.c_str())
										)->getNodeValue()
									);

						std::string value(val);
// REVIEW_COMMENT: 
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
	catch (std::exception& ex)// REVIEW_COMMENT: e
	{
		CLIResult res = CLILogger::GetInstance().HandleExceptionCaught("Get Attribute Value", ex);// REVIEW_COMMENT: 80
// REVIEW_COMMENT: 
		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);
	}

	return "";
}

