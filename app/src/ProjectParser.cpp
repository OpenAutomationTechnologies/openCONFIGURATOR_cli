/************************************************************************
\file ProjectParser.cpp
\author Sree Hari Vignesh, Kalycito Infotech Private Limited.
\brief Specifies the information of XDC/XML of project to library API.
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

#include "OpenConfiguratorCore.h"
#include "ProjectParser.h"
#include "ValidateCLIParameter.h"
#include "Parser.h"
#include "IEC_Datatype.h"
#include "ParameterAccess.h"
#include "Result.h"
#include "LoggingConfiguration.h"

using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;
using namespace IndustrialNetwork::POWERLINK::Core::API;
using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;
using namespace IndustrialNetwork::POWERLINK::Application::openCONFIGURATORCLI;
using namespace xercesc;

ProjectParser& ProjectParser::GetInstance()
{
	static ProjectParser instance;
	return instance;
}

void ProjectParser::UpdateManagingNode()
{
	//To be implemented.
	OpenConfiguratorCore::GetInstance();

}

void ProjectParser::ImportProfileBodyDevicePOWERLINK(DOMDocument* doc, std::uint8_t nodeId)
{
	DOMElement* root = doc->getDocumentElement();
	//macro has to be updated
	DOMXPathResult* result1 = doc->evaluate(
	                              XMLString::transcode("/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/ApplicationProcess/dataTypeList/struct"),
	                              root,
	                              NULL,
	                              DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
	                              NULL);
	if (result1->getNodeValue() == NULL)
	{
		LOG_ERROR() << "There is no result for the provided XPath " << std::endl;
	}
	else
	{
		XMLSize_t count2 = result1->getSnapshotLength();
		std::cout << "Struct Count Value--------------" << count2 << std::endl;
		for (XMLSize_t size = 0; size < count2; size++)
		{
			result1->snapshotItem(size);
			DOMNode* item = result1->getNodeValue();
			char* name = XMLString::transcode(item->getAttributes()->getNamedItem(XMLString::transcode("name"))->getNodeValue());
			char* uniqueID = XMLString::transcode(item->getAttributes()->getNamedItem(XMLString::transcode("uniqueID"))->getNodeValue());
			std::string structName(name);
			std::string structUniqueID(uniqueID);
			XMLString::release(&name);
			XMLString::release(&uniqueID);
			Result res = OpenConfiguratorCore::GetInstance().CreateStructDatatype(ValidateCLIParameter::GetInstance().GetNetworkName(), nodeId, structUniqueID, structName, "", "", 0U);
			if (!res.IsSuccessful())
			{
				LOG_ERROR() << res.GetErrorMessage();
			}
			DOMXPathResult* varDeclaration_result = doc->evaluate(
			        XMLString::transcode("./varDeclaration"),
			        item,
			        NULL,
			        DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
			        NULL);
			if (varDeclaration_result->getNodeValue() == NULL)
			{
				LOG_ERROR() << "There is no result for the provided XPath " << std::endl;
			}
			else
			{
				XMLSize_t countSub = varDeclaration_result->getSnapshotLength();
				std::cout << "varDeclaration Count Value--------------" << countSub << std::endl;
				for (XMLSize_t varSize = 0; varSize < countSub; varSize++)
				{
					varDeclaration_result->snapshotItem(varSize);
					std::cout << "one..." << std::endl;
					DOMNode* itemSub = varDeclaration_result->getNodeValue();
					char* varDeclName =  XMLString::transcode(itemSub->getAttributes()->getNamedItem(XMLString::transcode("name"))->getNodeValue());
					char* varDeclUniqueId =  XMLString::transcode(itemSub->getAttributes()->getNamedItem(XMLString::transcode("uniqueID"))->getNodeValue());
					//char* varDeclSize =  XMLString::transcode(itemSub->getAttributes()->getNamedItem(XMLString::transcode("size"))->getNodeValue());
					std::string structDeclName(varDeclName);
					std::string structDeclUniqueID(varDeclUniqueId);
					XMLString::release(&varDeclName);
					XMLString::release(&varDeclUniqueId);
					Result res = OpenConfiguratorCore::GetInstance().CreateVarDeclaration(ValidateCLIParameter::GetInstance().GetNetworkName(), nodeId, structUniqueID, structDeclUniqueID, structDeclName, IEC_Datatype::BITSTRING, 1U, "", "", "", 0U);
					if (!res.IsSuccessful())
					{
						LOG_ERROR() << res.GetErrorMessage();
					}
				}
			}
			varDeclaration_result->release();
		}
	}

	//Arraydata type
	DOMXPathResult* arrayResult = doc->evaluate(
	                                  XMLString::transcode("/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/ApplicationProcess/dataTypeList/array"),
	                                  root,
	                                  NULL,
	                                  DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
	                                  NULL);//macro has to be updated
	if (arrayResult->getNodeValue() == NULL)
	{
		LOG_ERROR() << "There is no result for the provided XPath " << std::endl;
	}
	else
	{
		XMLSize_t arrayCount = arrayResult->getSnapshotLength();
		std::cout << "array Count Value--------------" << arrayCount << std::endl;
		for (XMLSize_t arraySize = 0; arraySize < arrayCount; arraySize++)
		{
			arrayResult->snapshotItem(arraySize);
			DOMNode* arrayItem = arrayResult->getNodeValue();
			char* name = XMLString::transcode(arrayItem->getAttributes()->getNamedItem(XMLString::transcode("name"))->getNodeValue());

			//To be defined based on the structure of array in XDC
			//Result res=OpenConfiguratorCore::GetInstance().CreateArrayDatatype(ValidateCLIParameter::GetInstance().GetNetworkName(),nodeId,UniqueID,arrayname,upperlimit,lowerlimit,IEC_Datatype::BITSTRING,"","",0U);
		}
	}

	//AddParameter template
	DOMXPathResult* parameterTemplateResult = doc->evaluate(
	            XMLString::transcode("/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/ApplicationProcess/templateList/parameterTemplate"),
	            root,
	            NULL,
	            DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
	            NULL);//macro has to be updated
	if (parameterTemplateResult->getNodeValue() == NULL)
	{
		LOG_ERROR() << "There is no result for the provided XPath " << std::endl;
	}
	else
	{
		XMLSize_t templateCount = parameterTemplateResult->getSnapshotLength();
		LOG_ERROR() << "array Count Value--------------" << templateCount << std::endl;
		for (XMLSize_t tempSize = 0; tempSize < templateCount; tempSize++)
		{
			parameterTemplateResult->snapshotItem(tempSize);
			DOMNode* parameterTemplateItem = parameterTemplateResult->getNodeValue();
			char* uID = XMLString::transcode(parameterTemplateItem->getAttributes()->getNamedItem(XMLString::transcode("uniqueID"))->getNodeValue());
			char* access = XMLString::transcode(parameterTemplateItem->getAttributes()->getNamedItem(XMLString::transcode("access"))->getNodeValue());
			std::string uniqueID = (std::string)uID;
			//The datatype and parameter access has to be updated.
			/*Result res=OpenConfiguratorCore::GetInstance().CreateParameter(ValidateCLIParameter::GetInstance().GetNetworkName(),nodeId,uniqueID,ParameterAccess::read,IEC_Datatype::USINT,true);

			if(!res.IsSuccessful()) {
				LOG_ERROR() << res.GetErrorMessage();
			}*/

			DOMXPathResult* PdataTypeIdRefResult = doc->evaluate(
			        XMLString::transcode("./dataTypeIdRef"),
			        parameterTemplateItem,
			        NULL,
			        DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
			        NULL);
			if (PdataTypeIdRefResult->getNodeValue() == NULL)
			{
				LOG_ERROR() << "There is no result for the provided XPath " << std::endl;
			}
			else
			{
				XMLSize_t dataTypeRefCount = PdataTypeIdRefResult->getSnapshotLength();
				for (XMLSize_t datatypeSize = 0; datatypeSize < dataTypeRefCount; datatypeSize++)
				{
					PdataTypeIdRefResult->snapshotItem(datatypeSize);
					DOMNode* dataTypeIdItem = PdataTypeIdRefResult->getNodeValue();
					char* uID = XMLString::transcode(dataTypeIdItem->getAttributes()->getNamedItem(XMLString::transcode("uniqueID"))->getNodeValue());
					std::string dataTypeuniqueID = (std::string)uID;
					//The datatype and parameter access has to be updated based on datatypeIDRef
					/*Result res=OpenConfiguratorCore::GetInstance().CreateParameter(ValidateCLIParameter::GetInstance().GetNetworkName(),nodeId,uniqueID,dataTypeuniqueID,ParameterAccess::read,true);
					if(!res.IsSuccessful()) {
						LOG_ERROR() << res.GetErrorMessage();
					}*/
				}
			}

//allowed value update

			DOMXPathResult* allowedValueRefResult = doc->evaluate(
			        XMLString::transcode("./allowedValues"),
			        parameterTemplateItem,
			        NULL,
			        DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
			        NULL);
			if (allowedValueRefResult->getNodeValue() == NULL)
			{
				LOG_ERROR() << "There is no result for the provided XPath " << std::endl;
			}
			else
			{
				XMLSize_t allowedValueCount = allowedValueRefResult->getSnapshotLength();
				for (XMLSize_t allowedValueSize = 0; allowedValueSize < allowedValueCount; allowedValueSize++)
				{
					allowedValueRefResult->snapshotItem(allowedValueSize);
					DOMNode* allowedValueItem = allowedValueRefResult->getNodeValue();
					//The allowed values has to be updated.
					/*Result res=OpenConfiguratorCore::GetInstance().SetParameterAllowedValues(ValidateCLIParameter::GetInstance().GetNetworkName(),nodeId,uniqueID,vector<> allowedValues);

					if(!res.IsSuccessful()) {
						LOG_ERROR() << res.GetErrorMessage();
					}*/
				}
			}

//defaultvalue update
			DOMXPathResult* defaultValueRefResult = doc->evaluate(
			        XMLString::transcode("./defaultValue"),
			        parameterTemplateItem,
			        NULL,
			        DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
			        NULL);
			if (defaultValueRefResult->getNodeValue() == NULL)
			{
				std::cout << "There is no result for the provided XPath " << std::endl;
			}
			else
			{
				DOMNode* defaultValueItem = defaultValueRefResult->getNodeValue();
				char* defaultValue = XMLString::transcode(defaultValueItem->getAttributes()->getNamedItem(XMLString::transcode("value"))->getNodeValue());
				std::string defaultVal = (std::string)defaultValue;

				/*Result res=OpenConfiguratorCore::GetInstance().SetParameterDefaultValue(ValidateCLIParameter::GetInstance().GetNetworkName(),nodeId,uniqueID,defaultVal);

				if(!res.IsSuccessful()) {
					LOG_ERROR() << res.GetErrorMessage();
				}*/
			}
			DOMXPathResult* actualValueRefResult = doc->evaluate(
			        XMLString::transcode("./actualValue"),
			        parameterTemplateItem,
			        NULL,
			        DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
			        NULL);
			if (actualValueRefResult->getNodeValue() == NULL)
			{
				std::cout << "There is no result for the provided XPath " << std::endl;
			}
			else
			{
				DOMNode* actualValueItem = actualValueRefResult->getNodeValue();
				char* actualValue = XMLString::transcode(actualValueItem->getAttributes()->getNamedItem(XMLString::transcode("value"))->getNodeValue());
				std::string actualtVal = (std::string)actualValue;

				/*Result res=OpenConfiguratorCore::GetInstance().SetParameterActualValue(ValidateCLIParameter::GetInstance().GetNetworkName(),nodeId,uniqueID,actualtVal);

				if(!res.IsSuccessful()) {
					LOG_ERROR() << res.GetErrorMessage();
				}*/
			}
		}
	}
	result1->release();
}

/**
/Updates the Library with the information extracted from the node XDC.
/param nodeid Specifies the ID of the Node.
/param cnXDC Specifies the XDC path of Node.
*/
void ProjectParser::UpdateNodeIdCollection(std::uint8_t nodeId, const std::string& cnXDC)
{
	std::string indexString1 = "/" + cnXDC;
	std::string initialPath = ValidateCLIParameter::GetInstance().GetProjectpath();
	std::string nextInitialPath = initialPath.substr(0, initialPath.find_last_of("\\/"));
	std::string fullPath = nextInitialPath.append(indexString1);

	// Initialize Xerces infrastructure
	XMLPlatformUtils::Initialize();

	// Creating a DOMParser Object
	XercesDOMParser domParser;
	domParser.parse(nextInitialPath.c_str());
	DOMDocument* doc = domParser.getDocument();
	ProjectParser::GetInstance().ImportProfileBodyDevicePOWERLINK(doc, nodeId);
	XMLPlatformUtils::Terminate();

	/*DOMElement* root = doc->getDocumentElement();

			DOMXPathResult* result1 = doc->evaluate(
			                              XMLString::transcode("/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/ApplicationLayers/ObjectList/Object"),
			                              root,
			                              NULL,
			                              DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
			                              NULL);
			if (result1->getNodeValue() == NULL)
			{
				std::cout << "There is no result for the provided XPath " << std::endl;
			}
			else
			{
				XMLSize_t count2 = result1->getSnapshotLength();
				std::cout << "Object Count Value--------------" << count2 << std::endl;
				for (XMLSize_t i = 0; i < count2; i++)
				{
					result1->snapshotItem(i);
					DOMNode* item = result1->getNodeValue();
					char* index = XMLString::transcode(item->getAttributes()->getNamedItem(XMLString::transcode("index"))->getNodeValue());
					std::string indexString(index);
					XMLString::release(&index);
					std::cout << "Object_index: " << TranscodeToStr(item->getAttributes()->getNamedItem(XMLString::transcode("index"))->getNodeValue(), "ascii").str() << " ";
					std::cout <<"\t"<<"Object_Name: " << TranscodeToStr(item->getAttributes()->getNamedItem(XMLString::transcode("name"))->getNodeValue(), "ascii").str() << " ";
					std::cout <<"\t"<<"Object_Type: " << TranscodeToStr(item->getAttributes()->getNamedItem(XMLString::transcode("objectType"))->getNodeValue(), "ascii").str() << std::endl;
					//cout << TranscodeToStr(item->getAttributes()->getNamedItem(XMLString::transcode("defaultValue"))->getNodeValue(), "ascii").str() << std::endl;
					//cout << TranscodeToStr(item->getAttributes()->getNamedItem(XMLString::transcode("dataType"))->getNodeValue(), "ascii").str() << std::endl;
					//cout << TranscodeToStr(item->getAttributes()->getNamedItem(XMLString::transcode("accessType"))->getNodeValue(), "ascii").str() << std::endl;
					//cout << TranscodeToStr(item->getAttributes()->getNamedItem(XMLString::transcode("PDOmapping"))->getNodeValue(), "ascii").str() << std::endl;
					//cout << TranscodeToStr(item->getAttributes()->getNamedItem(XMLString::transcode("defaultValue"))->getNodeValue(), "ascii").str() << std::endl;
					std::stringstream stream;
					uint32_t indexVal = 0;
					stream << std::hex << indexString;
					stream >> indexVal;
					DOMXPathResult* subObject_result = doc->evaluate(
					                                       XMLString::transcode("./SubObject"),
					                                       item,
					                                       NULL,
					                                       DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
					                                       NULL);
					if (subObject_result->getNodeValue() != NULL)
					{
						XMLSize_t countSub = subObject_result->getSnapshotLength();
						std::cout << "SubObject Count Value--------------" << countSub << std::endl;
						for (XMLSize_t j = 0; j < countSub; j++)
						{
							subObject_result->snapshotItem(j);
							DOMNode* itemSub = subObject_result->getNodeValue();
							std::cout<<"\t";
							std::cout << "SubObject_subIndex: " <<  TranscodeToStr(itemSub->getAttributes()->getNamedItem(XMLString::transcode("subIndex"))->getNodeValue(), "ascii").str() << " ";
							std::cout <<"\t"<<"SubObject_Name: " <<  TranscodeToStr(itemSub->getAttributes()->getNamedItem(XMLString::transcode("name"))->getNodeValue(), "ascii").str() << " ";
							std::cout <<"\t"<<"SubObject_Type: " <<  TranscodeToStr(itemSub->getAttributes()->getNamedItem(XMLString::transcode("objectType"))->getNodeValue(), "ascii").str() << std::endl;
							//cout << TranscodeToStr(itemSub->getAttributes()->getNamedItem(XMLString::transcode("dataType"))->getNodeValue(), "ascii").str() << std::endl;
							//cout << TranscodeToStr(itemSub->getAttributes()->getNamedItem(XMLString::transcode("defaultValue"))->getNodeValue(), "ascii").str() << std::endl;
							//cout << TranscodeToStr(itemSub->getAttributes()->getNamedItem(XMLString::transcode("PDOmapping"))->getNodeValue(), "ascii").str() << std::endl;
							//cout << TranscodeToStr(itemSub->getAttributes()->getNamedItem(XMLString::transcode("lowLimit"))->getNodeValue(), "ascii").str() << std::endl;
							//cout << TranscodeToStr(itemSub->getAttributes()->getNamedItem(XMLString::transcode("highLimit"))->getNodeValue(), "ascii").str() << std::endl;
							//cout << TranscodeToStr(itemSub->getAttributes()->getNamedItem(XMLString::transcode("actualValue"))->getNodeValue(), "ascii").str() << std::endl;
						}
						subObject_result->release();
					}
				}
				result1->release();
			}*/
}

void ProjectParser::UpdateControlledNode()
{
	//To be implemented
}
void ProjectParser::UpdateModules()
{
	//To be implemented
}
void ProjectParser::UpdateParameters()
{
	//To be implemented
}
void ProjectParser::UpdateObjects()
{
	//To be implemented
}
