/************************************************************************
\file Parser.cpp
\author Subash Janarthanan, Kalycito Infotech Private Limited.
\brief Specifies the parser functionalities included in CLI.
************************************************************************/

/*------------------------------------------------------------------------------
Copyright (c) 2016, Kalycito Infotech Private Limited
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

#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include "Parser.h"
#include "ProjectParser.h"
#include "Result.h"
#include "OpenConfiguratorCore.h"
#include "ValidateCLIParameter.h"
#include "LoggingConfiguration.h"

using namespace IndustrialNetwork::POWERLINK::Core::API;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;
using namespace IndustrialNetwork::POWERLINK::Application::openCONFIGURATORCLI;
using namespace xercesc;

Parser& Parser::GetInstance()
{
	static Parser instance;
	return instance;
}

/**
\Parse the project XML file and retrieve the required information.
\param xmlPath Specifies the XML file to be parsed.
*/
int Parser::ParseXMLFile(const std::string xmlPath)
{
	XMLPlatformUtils::Initialize();
	XercesDOMParser domParser;
	domParser.parse(xmlPath.c_str());
	Result res = OpenConfiguratorCore::GetInstance().CreateNetwork(ValidateCLIParameter::GetInstance().GetNetworkName());
	if (!res.IsSuccessful())
	{
		LOG_ERROR() << res.GetErrorMessage();
		return 0;
	}
	DOMDocument* doc = domParser.getDocument();
	DOMElement* root = doc->getDocumentElement();

	//Create ManagingNode
	DOMXPathResult* mnResult = doc->evaluate(
	                               XMLString::transcode("/openCONFIGURATORProject/NetworkConfiguration/NodeCollection/MN"),
	                               root,
	                               NULL,
	                               DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
	                               NULL);//macro has to be used.
	if (mnResult->getNodeValue() == NULL)
	{
		LOG_ERROR() << "There is no result for the provided XPath " << std::endl;
		return 0;
	}
	else
	{
		XMLSize_t count = mnResult->getSnapshotLength();
		for (XMLSize_t mn = 0; mn < count; mn++)
		{
			mnResult->snapshotItem(mn);
			DOMNode* item = mnResult->getNodeValue();
			char* nodeId = XMLString::transcode(item->getAttributes()->getNamedItem(XMLString::transcode("nodeID"))->getNodeValue());
			char* xdc =  XMLString::transcode(item->getAttributes()->getNamedItem(XMLString::transcode("pathToXDC"))->getNodeValue());
			char* name =  XMLString::transcode(item->getAttributes()->getNamedItem(XMLString::transcode("name"))->getNodeValue());
			uint8_t nodeIdvalue = (uint8_t)atoi(nodeId);
			std::string cnXDC = std::string(xdc);
			std::string nodeName = std::string(name);

			//Create Node
			Result res = OpenConfiguratorCore::GetInstance().CreateNode(ValidateCLIParameter::GetInstance().GetNetworkName(), nodeIdvalue, nodeName);
			if (!res.IsSuccessful())
			{
				LOG_ERROR() << res.GetErrorMessage();
				return 0;
			}

			// Library API calls to import the XDD of node
			ProjectParser::GetInstance().UpdateNodeIdCollection(nodeIdvalue, cnXDC);
		}
	}

	//Create Controlled Node
	DOMXPathResult* cnResult = doc->evaluate(
	                               XMLString::transcode("/openCONFIGURATORProject/NetworkConfiguration/NodeCollection/CN"),
	                               root,
	                               NULL,
	                               DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
	                               NULL);//macro has to be updated.
	if (cnResult->getNodeValue() == NULL)
	{
		LOG_ERROR() << "There is no result for the provided XPath " << std::endl;
		return 0;
	}
	else
	{
		XMLSize_t count = cnResult->getSnapshotLength();
		for (XMLSize_t cnCount = 0; cnCount < count; cnCount++)
		{
			cnResult->snapshotItem(cnCount);
			DOMNode* item = cnResult->getNodeValue();
			char* nodeId = XMLString::transcode(item->getAttributes()->getNamedItem(XMLString::transcode("nodeID"))->getNodeValue());
			char* xdc =  XMLString::transcode(item->getAttributes()->getNamedItem(XMLString::transcode("pathToXDC"))->getNodeValue());
			char* name =  XMLString::transcode(item->getAttributes()->getNamedItem(XMLString::transcode("name"))->getNodeValue());
			std::uint8_t nodeIdvalue = (std::uint8_t)atoi(nodeId);
			std::string cnXDC = std::string(xdc);
			std::string nodeName = std::string(name);

			//Create Node
			Result res = OpenConfiguratorCore::GetInstance().CreateNode(ValidateCLIParameter::GetInstance().GetNetworkName(), nodeIdvalue, nodeName, false);
			if (!res.IsSuccessful())
			{
				LOG_ERROR() << res.GetErrorMessage();
				return 0;
			}

			// Library API calls to import the XDD of node
			ProjectParser::GetInstance().UpdateNodeIdCollection(nodeIdvalue, cnXDC);
		}
	}

	mnResult->release();
	cnResult->release();
	return 0;
}



