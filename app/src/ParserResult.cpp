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

bool ParserResult::CreateResult(ParserElement& pElement,
					std::string transcodeString,
					std::string formatString)
{
	return(CreateResult(pElement,
				 transcodeString,
				 formatString,
				 pElement.docElement));
}

bool ParserResult::CreateResult(ParserElement& pElement,
					std::string transcodeString,
					std::string formatString,
					DOMNode * parentNode)
{
	/**< Get result of Managing Node */
	DOMXPathResult* nResult = pElement.docHandle->evaluate(
	                               XMLString::transcode(transcodeString.c_str()),
	                               parentNode,
	                               NULL,
	                               DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
	                               NULL);

	resultNodeValue = nResult->getNodeValue();

	if (resultNodeValue == NULL)
	{
		LOG_INFO() << "There is no result for the Managing Node XPath ";
		return true;
	}
	else
	{
		std::vector<std::string> attributeNames;

		// Interpret the formatString and split in to attribute names
		int commaCounter = 0;
		for (int index = 0; index < formatString.size(); index++) 
		{
			if (formatString[index] == ',') 
			{
				attributeNames.push_back("");
				commaCounter++;
			} 
			else 
			{
				attributeNames.at(commaCounter) += formatString[index];
			}
		}

		XMLSize_t count = nResult->getSnapshotLength();

		// Parse the parameters
		for (XMLSize_t nIndex = 0; nIndex < count; nIndex++)
		{
			nResult->snapshotItem(nIndex);
			node.push_back(nResult->getNodeValue());

			std::vector<std::string> row;

			for (uint32_t attrIndex = 0; attrIndex < attributeNames.size(); attrIndex++)
			{
				row.push_back(GetAttributeValue(node.at(attrIndex), attributeNames.at(attrIndex)));
			}

			parameters.push_back(row);
		}
	}

	nResult->release();

	return true;
}

std::string ParserResult::GetAttributeValue(DOMNode* domNode, std::string attributeName)
{
	if (domNode->hasAttributes())
	{
		DOMNamedNodeMap* attributes = domNode->getAttributes();
		const XMLSize_t attribCount = attributes->getLength();

		for (XMLSize_t attribSize = 0; attribSize < attribCount; attribSize++)
		{
			DOMNode* currentNode = attributes->item(attribSize);
			if (currentNode->getNodeType() == DOMNode::ATTRIBUTE_NODE)
			{
				char* attribute = XMLString::transcode(currentNode->getNodeName());

				if (strcmp(attribute, attributeName.c_str()) == 0)
				{
					char* val = XMLString::transcode(
									domNode->getAttributes()->getNamedItem(
											XMLString::transcode(attributeName.c_str())
									)->getNodeValue()
								);

					std::string value(val);

					XMLString::release(&val);
					XMLString::release(&attribute);

					return value;
				}
				else
				{
					XMLString::release(&attribute);
				}
			}
		}
	}

	return "";
}

