/**
 * \class ProjectParser
 *
 * \brief Implementation of ProjectParser module
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

#include "ProjectParser.h"
#include "ParserResult.h"
#include "ParameterValidator.h"
#include "IEC_Datatype.h"
#include "ParameterAccess.h"
#include "AccessType.h"
#include "PDOMapping.h"

using namespace IndustrialNetwork::POWERLINK::Application::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::API;
using namespace IndustrialNetwork::POWERLINK::Core::Node;
using namespace IndustrialNetwork::POWERLINK::Core::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Core::ObjectDictionary;
using namespace IndustrialNetwork::POWERLINK::Core::CoreConfiguration;
using namespace IndustrialNetwork::POWERLINK::Core::ModularNode;
using namespace IndustrialNetwork::POWERLINK::Application::ErrorHandling;
using namespace IndustrialNetwork::POWERLINK::Application;
using namespace IndustrialNetwork::POWERLINK::Application::CliConstants;

ProjectParser::ProjectParser() :
	iecdataTypeMap(std::map<std::string, IEC_Datatype>()),
	parameterAccessMap(std::map<std::string, ParameterAccess>()),
	accessTypeMap(std::map<std::string, AccessType>()),
	pdoMappingMap(std::map<std::string, PDOMapping>())
{
	iecdataTypeMap.insert(std::pair<std::string, IEC_Datatype>("", IEC_Datatype::UNDEFINED));
	iecdataTypeMap.insert(std::pair<std::string, IEC_Datatype>("UNDEFINED", IEC_Datatype::UNDEFINED));
	iecdataTypeMap.insert(std::pair<std::string, IEC_Datatype>("BITSTRING", IEC_Datatype::BITSTRING));
	iecdataTypeMap.insert(std::pair<std::string, IEC_Datatype>("BOOL", IEC_Datatype::BOOL));
	iecdataTypeMap.insert(std::pair<std::string, IEC_Datatype>("BYTE", IEC_Datatype::BYTE));
	iecdataTypeMap.insert(std::pair<std::string, IEC_Datatype>("CHAR", IEC_Datatype::_CHAR));
	iecdataTypeMap.insert(std::pair<std::string, IEC_Datatype>("WORD", IEC_Datatype::WORD));
	iecdataTypeMap.insert(std::pair<std::string, IEC_Datatype>("DWORD", IEC_Datatype::DWORD));
	iecdataTypeMap.insert(std::pair<std::string, IEC_Datatype>("LWORD", IEC_Datatype::LWORD));
	iecdataTypeMap.insert(std::pair<std::string, IEC_Datatype>("SINT", IEC_Datatype::SINT));
	iecdataTypeMap.insert(std::pair<std::string, IEC_Datatype>("INT", IEC_Datatype::INT));
	iecdataTypeMap.insert(std::pair<std::string, IEC_Datatype>("DINT", IEC_Datatype::DINT));
	iecdataTypeMap.insert(std::pair<std::string, IEC_Datatype>("LINT", IEC_Datatype::LINT));
	iecdataTypeMap.insert(std::pair<std::string, IEC_Datatype>("USINT", IEC_Datatype::USINT));
	iecdataTypeMap.insert(std::pair<std::string, IEC_Datatype>("UINT", IEC_Datatype::UINT));
	iecdataTypeMap.insert(std::pair<std::string, IEC_Datatype>("UDINT", IEC_Datatype::UDINT));
	iecdataTypeMap.insert(std::pair<std::string, IEC_Datatype>("ULINT", IEC_Datatype::ULINT));
	iecdataTypeMap.insert(std::pair<std::string, IEC_Datatype>("REAL", IEC_Datatype::REAL));
	iecdataTypeMap.insert(std::pair<std::string, IEC_Datatype>("LREAL", IEC_Datatype::LREAL));
	iecdataTypeMap.insert(std::pair<std::string, IEC_Datatype>("STRING", IEC_Datatype::STRING));
	iecdataTypeMap.insert(std::pair<std::string, IEC_Datatype>("WSTRING", IEC_Datatype::WSTRING));

	parameterAccessMap.insert(std::pair<std::string, ParameterAccess>("constant", ParameterAccess::constant));
	parameterAccessMap.insert(std::pair<std::string, ParameterAccess>("read", ParameterAccess::read));
	parameterAccessMap.insert(std::pair<std::string, ParameterAccess>("write", ParameterAccess::write));
	parameterAccessMap.insert(std::pair<std::string, ParameterAccess>("readWrite", ParameterAccess::readWrite));
	parameterAccessMap.insert(std::pair<std::string, ParameterAccess>("readWriteInput", ParameterAccess::readWriteInput));
	parameterAccessMap.insert(std::pair<std::string, ParameterAccess>("readWriteOutput", ParameterAccess::readWriteOutput));
	parameterAccessMap.insert(std::pair<std::string, ParameterAccess>("noAccess", ParameterAccess::noAccess));
	parameterAccessMap.insert(std::pair<std::string, ParameterAccess>("undefined", ParameterAccess::undefined));
	parameterAccessMap.insert(std::pair<std::string, ParameterAccess>("", ParameterAccess::undefined));

	accessTypeMap.insert(std::pair<std::string, AccessType>("", AccessType::UNDEFINED));
	accessTypeMap.insert(std::pair<std::string, AccessType>("undefined", AccessType::UNDEFINED));
	accessTypeMap.insert(std::pair<std::string, AccessType>("rw", AccessType::RW));
	accessTypeMap.insert(std::pair<std::string, AccessType>("rws", AccessType::RWS));
	accessTypeMap.insert(std::pair<std::string, AccessType>("wo", AccessType::WO));
	accessTypeMap.insert(std::pair<std::string, AccessType>("wos", AccessType::WOS));
	accessTypeMap.insert(std::pair<std::string, AccessType>("ro", AccessType::RO));
	accessTypeMap.insert(std::pair<std::string, AccessType>("const", AccessType::CONST));
	accessTypeMap.insert(std::pair<std::string, AccessType>("cond", AccessType::COND));

	pdoMappingMap.insert(std::pair<std::string, PDOMapping>("", PDOMapping::UNDEFINED));
	pdoMappingMap.insert(std::pair<std::string, PDOMapping>("undefined", PDOMapping::UNDEFINED));
	pdoMappingMap.insert(std::pair<std::string, PDOMapping>("no", PDOMapping::NO));
	pdoMappingMap.insert(std::pair<std::string, PDOMapping>("default", PDOMapping::DEFAULT));
	pdoMappingMap.insert(std::pair<std::string, PDOMapping>("optional", PDOMapping::OPTIONAL));
	pdoMappingMap.insert(std::pair<std::string, PDOMapping>("TPDO", PDOMapping::TPDO));
	pdoMappingMap.insert(std::pair<std::string, PDOMapping>("RPDO", PDOMapping::RPDO));
}

ProjectParser::~ProjectParser()
{
}

ProjectParser& ProjectParser::GetInstance()
{
	static ProjectParser instance;

	return instance;
}

CliResult ProjectParser::ParseXmlFile(const std::string& xmlPath)
{
	/** Core Library API call to create Network */
	Result res = OpenConfiguratorCore::GetInstance().CreateNetwork(
	                 OpenConfiguratorCli::GetInstance().networkName);

	if (!res.IsSuccessful())
	{
		return CliLogger::GetInstance().GetFailureErrorString(res);
	}

	/** Create results for MN, RMN abd CN nodes */
	CliResult clires;
	CliResult ceres;
	ParserElement xmlParserElement(xmlPath, kXmlSchemaDefinitionFileName, kProjectNamespace);

	ceres = xmlParserElement.CreateElement();
	if (!ceres.IsSuccessful())
	{
		return ceres;
	}

	clires = CreateProjectConfiguration(xmlParserElement);
	if (!clires.IsSuccessful())
	{
		return clires;
	}

	clires = CreateMnNodeResults(xmlParserElement);
	if (!clires.IsSuccessful())
	{
		return clires;
	}

	clires = CreateRmnNodeResults(xmlParserElement);
	if (!clires.IsSuccessful())
	{
		return clires;
	}

	clires = CreateCnNodeResults(xmlParserElement);
	if (!clires.IsSuccessful())
	{
		return clires;
	}

	return CliResult();
}

CliResult ProjectParser::CreateProjectConfiguration(const ParserElement& element)
{
	try
	{
		ParserResult configResult;
		CliResult crres;

		crres = configResult.CreateResult(element, kAutogenXpathExpression,
		                                  kFormatStrAutoGenXpathExpression);
		if (!crres.IsSuccessful())
		{
			return crres;
		}
		else
		{
			for (std::uint32_t row = 0; row < configResult.parameters.size(); row++)
			{
				/** Core Library API call to create Configuration */
				Result res = OpenConfiguratorCore::GetInstance().CreateConfiguration(
				                 OpenConfiguratorCli::GetInstance().networkName,
				                 configResult.parameters[row].at(0));	/** ID */
				if (!res.IsSuccessful())
				{
					return CliLogger::GetInstance().GetFailureErrorString(res);
				}

				ParserResult settingResult;

				crres = settingResult.CreateResult(element, kAutoSettingXpathExpression,
				                                   kFormatStrAutoSetXpathExpression,
				                                   configResult.node.at(row));
				if (!crres.IsSuccessful())
				{
					LOG_WARN() << CliLogger::GetInstance().GetErrorString(crres);
				}

				for (std::uint32_t subRow = 0; subRow < settingResult.parameters.size(); subRow++)
				{
					/** Core Library API call to update settings */
					res = OpenConfiguratorCore::GetInstance().CreateConfigurationSetting(
					          OpenConfiguratorCli::GetInstance().networkName,
					          configResult.parameters[row].at(0),			/** ID */
					          settingResult.parameters[subRow].at(0),		/** Name */
					          settingResult.parameters[subRow].at(1));	/** Value */
					if (!res.IsSuccessful())
					{
						return CliLogger::GetInstance().GetFailureErrorString(res);
					}

					bool enabled = false;

					/** enabled: empty => false; true => true; false => false */
					if (settingResult.parameters[subRow].at(2).compare("true") == 0)
					{
						enabled = true;
					}

					/** Core Library API call to update settings value */
					Result result = OpenConfiguratorCore::GetInstance().SetConfigurationSettingEnabled(
					                    OpenConfiguratorCli::GetInstance().networkName,
					                    configResult.parameters[row].at(0),		/** ID */
					                    settingResult.parameters[subRow].at(0),	/** Name */
					                    enabled);
					if (!result.IsSuccessful())
					{
						return CliLogger::GetInstance().GetFailureErrorString(res);
					}
				}
			}
		}

		ParserResult projectResult;

		crres = projectResult.CreateResult(element, kProjectConfXpathExpression,
		                                   kFormatProjectConfXpathExpression);
		if (!crres.IsSuccessful())
		{
			return crres;
		}

		/** Core Library API call to set active configuration value */
		Result res = OpenConfiguratorCore::GetInstance().SetActiveConfiguration(
		                 OpenConfiguratorCli::GetInstance().networkName,
		                 projectResult.parameters[0].at(0));		/** ID */
		if (!res.IsSuccessful())
		{
			return CliLogger::GetInstance().GetFailureErrorString(res);
		}
	}
	catch (const std::exception& e)
	{
		return CliLogger::GetInstance().GetFailureErrorString(e);
	}

	return CliResult();
}

CliResult ProjectParser::CreateMnNodeResults(const ParserElement& element)
{
	try
	{
		ParserResult mnResult;
		CliResult crres;

		crres = mnResult.CreateResult(element, kMnXpathExpression,
		                              kFormatStrMnXpathExpression);
		if (!crres.IsSuccessful())
		{
			return crres;
		}

		for (std::uint32_t row = 0; row < mnResult.parameters.size(); row++)
		{
			std::uint8_t nodeId = 0;

			if (!mnResult.parameters[row].at(0).empty())
			{
				nodeId = (std::uint8_t)std::stoi(mnResult.parameters[row].at(0));
			}

			/** Core Library API call to create Node */
			Result res = OpenConfiguratorCore::GetInstance().CreateNode(
			                 OpenConfiguratorCli::GetInstance().networkName,
			                 nodeId,
			                 mnResult.parameters[row].at(2));	/** name */
			if (!res.IsSuccessful())
			{
				return CliLogger::GetInstance().GetFailureErrorString(res);
			}

			std::vector<std::string> forcedModularNodeObj;		/** Group of forced objects in Node */
			std::vector<std::string> forcedModularNodeSubObj;	/** Group of forced sub objects in Node */

			/** Update the forced objects of node */
			CliResult clires = CreateForcedObjects(element,
			                                       kForcedObjectNodeXpathExpression,
			                                       forcedModularNodeObj,
			                                       forcedModularNodeSubObj,
			                                       mnResult.node.at(row));
			if (!clires.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(clires);
			}

			/** Core Library API calls to import the XDD of node */
			clires = UpdateNodeIdCollection(nodeId,
			                                mnResult.parameters[row].at(1),	/** pathToXDC */
			                                forcedModularNodeObj,
			                                forcedModularNodeSubObj,
			                                "", "", 0U);
			if (!clires.IsSuccessful())
			{
				return clires;
			}

			/** Core Library API calls to update the node assignment of node */
			clires = CreateNodeAssignment(element, kMnXpathExpression, nodeId);
			if (!clires.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(clires);
			}
		}
	}
	catch (const std::exception& e)
	{
		return CliLogger::GetInstance().GetFailureErrorString(e);
	}

	return CliResult();
}

CliResult ProjectParser::CreateRmnNodeResults(const ParserElement& element)
{
	try
	{
		ParserResult rmnResult;
		CliResult crres;

		crres = rmnResult.CreateResult(element, kRmnXpathExpression,
		                               kFormatStrRmnXpathExpression);
		if (!crres.IsSuccessful())
		{
			LOG_WARN() << CliLogger::GetInstance().GetErrorString(crres);
		}
		else
		{
			for (std::uint32_t row = 0; row < rmnResult.parameters.size(); row++)
			{
				std::uint8_t nodeId = 0;

				if (!rmnResult.parameters[row].at(0).empty())
				{
					nodeId = (std::uint8_t)std::stoi(rmnResult.parameters[row].at(0));
				}

				/** Core Library API call to create Node */
				Result res = OpenConfiguratorCore::GetInstance().CreateNode(
				                 OpenConfiguratorCli::GetInstance().networkName,
				                 nodeId,
				                 rmnResult.parameters[row].at(2),		/** name */
				                 true);
				if (!res.IsSuccessful())
				{
					return CliLogger::GetInstance().GetFailureErrorString(res);
				}

				CliResult clires;
				std::vector<std::string> forcedModularNodeObj;		/** Group of forced objects in Node */
				std::vector<std::string> forcedModularNodeSubObj;	/** Group of forced sub objects in Node */

				/** Update the forced objects of node */
				clires = CreateForcedObjects(element,
				                             kForcedObjectNodeXpathExpression,
				                             forcedModularNodeObj,
				                             forcedModularNodeSubObj,
				                             rmnResult.node.at(row));
				if (!clires.IsSuccessful())
				{
					LOG_WARN() << CliLogger::GetInstance().GetErrorString(clires);
				}

				/** Core Library API calls to import the XDC of node */
				clires = UpdateNodeIdCollection(nodeId,
				                                rmnResult.parameters[row].at(1),	/** pathToXDC */
				                                forcedModularNodeObj,
				                                forcedModularNodeSubObj,
				                                "", "", 0U);
				if (!clires.IsSuccessful())
				{
					return clires;
				}

				/** Core Library API calls to update the node assignment of node */
				clires = CreateNodeAssignment(element,
				                              kRmnXpathExpression,
				                              nodeId);
				if (!clires.IsSuccessful())
				{
					LOG_WARN() << CliLogger::GetInstance().GetErrorString(clires);
				}
			}
		}
	}
	catch (const std::exception& e)
	{
		return CliLogger::GetInstance().GetFailureErrorString(e);
	}

	return CliResult();
}

CliResult ProjectParser::CreateCnNodeResults(const ParserElement& element)
{
	try
	{
		ParserResult cnResult;

		CliResult crres;

		crres = cnResult.CreateResult(element, kCnXpathExpression,
		                              kFormatStrCnXpathExpression);
		if (!crres.IsSuccessful())
		{
			LOG_WARN() << CliLogger::GetInstance().GetErrorString(crres);
		}

		for (std::uint32_t row = 0; row < cnResult.parameters.size(); row++)
		{
			std::uint8_t nodeId = 0;
			ParserResult subCnResult;
			CliResult clires;
			CliResult subclires;

			if (!cnResult.parameters[row].at(0).empty())
			{
				nodeId = (std::uint8_t)std::stoi(cnResult.parameters[row].at(0));
			}

			clires = subCnResult.CreateResult(element,
			                                  kIntrfcXpathExpression,
			                                  kFormatStrIntrfcXpathExpression,
			                                  cnResult.node.at(row));
			if (!clires.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(clires);

				/** Core Library API call to create Node */
				Result res = OpenConfiguratorCore::GetInstance().CreateNode(
				                 OpenConfiguratorCli::GetInstance().networkName,
				                 nodeId,
				                 cnResult.parameters[row].at(2),		/** name */
				                 false);
				if (!res.IsSuccessful())
				{
					return CliLogger::GetInstance().GetFailureErrorString(res);
				}

				bool enabled = true;

				/** enabled: empty => true; true => true; false => false */
				if (cnResult.parameters[row].at(3).compare("false") == 0)
				{
					enabled = false;
				}

				/** Core Library API call to create Node */
				res = OpenConfiguratorCore::GetInstance().EnableNode(
				          OpenConfiguratorCli::GetInstance().networkName,
				          nodeId, enabled);
				if (!res.IsSuccessful())
				{
					return CliLogger::GetInstance().GetFailureErrorString(res);
				}

				std::vector<std::string> forcedModularNodeObj;		/** Group of forced objects in Node */
				std::vector<std::string> forcedModularNodeSubObj;	/** Group of forced sub objects in Node */

				/** Update the forced objects of modular head node */
				subclires = CreateForcedObjects(element, kForcedObjectNodeXpathExpression,
				                                forcedModularNodeObj, forcedModularNodeSubObj,
				                                cnResult.node.at(row));
				if (!subclires.IsSuccessful())
				{
					LOG_WARN() << CliLogger::GetInstance().GetErrorString(subclires);
				}

				/** Core Library API calls to import the XDC of node */
				subclires = UpdateNodeIdCollection(nodeId,
				                                   cnResult.parameters[row].at(1),				/** pathToXDC */
				                                   forcedModularNodeObj, forcedModularNodeSubObj,
				                                   "", "", 0U);
				if (!subclires.IsSuccessful())
				{
					return subclires;
				}

				/** Core Library API calls to update the node assignment of node */
				subclires = CreateNodeAssignment(element, kCnXpathExpression, nodeId);
				if (!subclires.IsSuccessful())
				{
					LOG_WARN() << CliLogger::GetInstance().GetErrorString(subclires);
				}
			}
			else
			{
				/** Core Library API call to create Modular Node */
				Result res = OpenConfiguratorCore::GetInstance().CreateModularHeadNode(
				                 OpenConfiguratorCli::GetInstance().networkName,
				                 nodeId,
				                 cnResult.parameters[row].at(2));		/** name */
				if (!res.IsSuccessful())
				{
					return CliLogger::GetInstance().GetFailureErrorString(res);
				}

				std::vector<std::string> forcedModularNodeObj;		/** Group of forced objects in Node */
				std::vector<std::string> forcedModularNodeSubObj;	/** Group of forced sub objects in Node */

				/** Update the forced objects of modular head node */
				subclires = CreateForcedObjects(element, kForcedObjectNodeXpathExpression,
				                                forcedModularNodeObj, forcedModularNodeSubObj,
				                                cnResult.node.at(row));
				if (!subclires.IsSuccessful())
				{
					LOG_WARN() << CliLogger::GetInstance().GetErrorString(subclires);
				}

				/** Core Library API calls to import the XDC of modular head node */
				subclires = UpdateNodeIdCollection(nodeId, cnResult.parameters[row].at(1),	/** pathToXDC */
				                                   forcedModularNodeObj, forcedModularNodeSubObj,
				                                   "", "", 0U);
				if (!subclires.IsSuccessful())
				{
					return subclires;
				}

				/** Core Library API calls to update the node assignment of modular head node */
				subclires = CreateNodeAssignment(element, kCnXpathExpression, nodeId);
				if (!subclires.IsSuccessful())
				{
					LOG_WARN() << CliLogger::GetInstance().GetErrorString(subclires);
				}

				std::uint8_t maxModulePosition = 0;

				/** Core Library API calls to create interface of modular head node */
				subclires = CreateInterface(nodeId, cnResult.parameters[row].at(1),	 /** pathToXDC */
				                            maxModulePosition);
				if (!subclires.IsSuccessful())
				{
					LOG_WARN() << CliLogger::GetInstance().GetErrorString(subclires);
				}
				else
				{
					ParserResult moduleResult;
					CliResult clicrres;

					for (std::uint32_t subRow = 0; subRow < subCnResult.parameters.size(); subRow++)
					{
						clicrres = moduleResult.CreateResult(element,
						                                     kModuleXpathExpression,
						                                     kFormatStrModuleXpathExpression,
						                                     subCnResult.node.at(subRow));
						if (!clicrres.IsSuccessful())
						{
							LOG_WARN() << CliLogger::GetInstance().GetErrorString(clicrres);
						}
						else
						{
							CliResult foRes;

							for (std::uint32_t moduleRow = 0; moduleRow < moduleResult.parameters.size(); moduleRow++)
							{
								std::vector<std::string> forcedModularNodeObjMod;		/** Group of forced objects in Node */
								std::vector<std::string> forcedModularNodeSubObjMod;	/** Group of forced sub objects in Node */

								/** Update the forced objects of modular node */
								foRes = CreateForcedObjects(element, kForcedObjectXpathExpression,
								                            forcedModularNodeObjMod, forcedModularNodeSubObjMod,
								                            subCnResult.node.at(subRow));
								if (!foRes.IsSuccessful())
								{
									LOG_WARN() << CliLogger::GetInstance().GetErrorString(foRes);
								}

								foRes = CreateModule(nodeId,
								                     forcedModularNodeObjMod, forcedModularNodeSubObjMod,
								                     subCnResult.parameters[subRow].at(0),				/** Interface Unique ID */
								                     moduleResult.parameters[moduleRow].at(0),			/** Module name */
								                     moduleResult.parameters[moduleRow].at(1),			/** Module position */
								                     moduleResult.parameters[moduleRow].at(2),			/** Module address */
								                     moduleResult.parameters[moduleRow].at(3),			/** Module pathtoXDC */
								                     maxModulePosition);
								if (!foRes.IsSuccessful())
								{
									return foRes;
								}
							}
						}
					}
				}
			}
		}
	}
	catch (const std::exception& e)
	{
		return CliLogger::GetInstance().GetFailureErrorString(e);
	}

	return CliResult();
}

IEC_Datatype ProjectParser::GetIecDataType(const xercesc::DOMNode* node)
{
	IEC_Datatype data = IEC_Datatype::UNDEFINED;

	try
	{
		if (node->hasChildNodes())
		{
			xercesc::DOMNodeList* childNode = node->getChildNodes();
			const XMLSize_t nodeCount = childNode->getLength();

			for (XMLSize_t index = 0; index < nodeCount; index++)
			{
				xercesc::DOMNode* currentNode = childNode->item(index);

				if ((currentNode->getNodeType()) && (currentNode->getNodeType() == xercesc::DOMNode::ELEMENT_NODE))
				{
					char* subNodeName = xercesc::XMLString::transcode(currentNode->getNodeName());
					std::string childNodeName = subNodeName;

					xercesc::XMLString::release(&subNodeName);

					if ((childNodeName.compare("defaultValue") == 0) ||
					        (childNodeName.compare("allowedValues") == 0) ||
					        (childNodeName.compare("actualValue") == 0) ||
					        (childNodeName.compare("substituteValue") == 0) ||
					        (childNodeName.compare("unit") == 0) ||
					        (childNodeName.compare("property") == 0))
					{
						return data;
					}

					data = GetDataType(childNodeName);
				}
			}
		}
	}
	catch (const std::exception& e)
	{
		CliResult res = CliLogger::GetInstance().GetFailureErrorString(e);

		LOG_ERROR() << res.GetErrorMessage();

		return IEC_Datatype::UNDEFINED;
	}

	return data;
}

CliResult ProjectParser::CreateStructDataType(const ParserElement& element,
        const std::uint8_t nodeId,
        const std::string& interfaceId,
        const std::string& modId,
        const std::uint32_t modPosition)
{
	try
	{
		ParserResult pResult;

		CliResult crres = pResult.CreateResult(element, kStructXpathExpression,
		                                       kFormatStrStructXpathExpression);
		if (!crres.IsSuccessful())
		{
			return crres;
		}

		for (std::uint32_t row = 0; row < pResult.parameters.size(); row++)
		{
			/** Core Library API call to create StructDatatype */
			Result res = OpenConfiguratorCore::GetInstance().CreateStructDatatype(
			                 OpenConfiguratorCli::GetInstance().networkName,
			                 nodeId, pResult.parameters[row].at(0),		/** uniqueID */
			                 pResult.parameters[row].at(1),				/** name */
			                 interfaceId,
			                 modId,
			                 modPosition);
			if (!res.IsSuccessful())
			{
				return CliLogger::GetInstance().GetFailureErrorString(res);
			}

			ParserResult varDecResult;

			CliResult subcrres = varDecResult.CreateResult(element, kStructVarXpathExpression,
			                     kFormatStrStructVarXpathExpression,
			                     pResult.node.at(row));
			if (!subcrres.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(subcrres);
			}
			else
			{
				for (std::uint32_t subrow = 0; subrow < varDecResult.parameters.size(); subrow++)
				{
					IEC_Datatype data = GetIecDataType(varDecResult.node.at(subrow));
					/** Set default value to size if value is empty */
					std::uint32_t varDeclSize = 1U;

					if (!varDecResult.parameters[subrow].at(2).empty())
					{
						varDeclSize = (std::uint32_t)std::stoi(varDecResult.parameters[subrow].at(2));
					}

					/** Core Library API call to create VarDeclaration */
					res = OpenConfiguratorCore::GetInstance().CreateVarDeclaration(
					          OpenConfiguratorCli::GetInstance().networkName,
					          nodeId,
					          pResult.parameters[row].at(0),										/** parent uniqueID */
					          varDecResult.parameters[subrow].at(0),								/** sub item uniqueID */
					          varDecResult.parameters[subrow].at(1), 								/** sub item name */
					          data,
					          varDeclSize,														/** sub item size */
					          "",
					          interfaceId,
					          modId,
					          modPosition);														/** sub item size */
					if (!res.IsSuccessful())
					{
						return CliLogger::GetInstance().GetFailureErrorString(res);
					}
				}
			}
		}
	}
	catch (const std::exception& e)
	{
		return CliLogger::GetInstance().GetFailureErrorString(e);
	}

	return CliResult();
}

CliResult ProjectParser::CreateArrayDataType(const ParserElement& element,
        const std::uint8_t nodeId,
        const std::string& interfaceId,
        const std::string& modId,
        const std::uint32_t modPosition)
{
	try
	{
		ParserResult pResult;

		CliResult crres = pResult.CreateResult(element, kArrayXpathExpression,
		                                       kFormatStrArrayXpathExpression);
		if (!crres.IsSuccessful())
		{
			return crres;
		}

		for (std::uint32_t row = 0; row < pResult.parameters.size(); row++)
		{
			ParserResult subpResult;

			IEC_Datatype data = GetIecDataType(pResult.node.at(row));

			CliResult subcrres = subpResult.CreateResult(element, kArraySubRangeXpathExpression,
			                     kFormatStrArraySubRangeXpathExpression,
			                     pResult.node.at(row));
			if (!subcrres.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(subcrres);
			}
			else
			{
				for (std::uint32_t subrow = 0; subrow < subpResult.parameters.size(); subrow++)
				{
					std::uint32_t lowerLimit = 1U;
					std::uint32_t upperLimit = 1U;

					if (!subpResult.parameters[subrow].at(0).empty())
					{
						lowerLimit = (std::uint32_t)std::stoi(subpResult.parameters[subrow].at(0));
					}

					if (!subpResult.parameters[subrow].at(1).empty())
					{
						upperLimit = (std::uint32_t)std::stoi(subpResult.parameters[subrow].at(1));
					}

					/** Core Library API call to create ArrayDatatype */
					Result res = OpenConfiguratorCore::GetInstance().CreateArrayDatatype(
					                 OpenConfiguratorCli::GetInstance().networkName,
					                 nodeId,
					                 pResult.parameters[row].at(0),  								/** uniqueID */
					                 pResult.parameters[row].at(1),  								/** name */
					                 lowerLimit,
					                 upperLimit,
					                 data,
					                 interfaceId,
					                 modId,
					                 modPosition);
					if (!res.IsSuccessful())
					{
						return CliLogger::GetInstance().GetFailureErrorString(res);
					}
				}
			}
		}
	}
	catch (const std::exception& e)
	{
		return CliLogger::GetInstance().GetFailureErrorString(e);
	}

	return CliResult();
}

CliResult ProjectParser::CreateDataType(const ParserElement& element,
                                        const std::uint8_t nodeId,
                                        const std::string& interfaceId,
                                        const std::string& modId,
                                        const std::uint32_t modposition)
{
	CliResult res;

	/** Creates struct data type of node into the core library */
	res = CreateStructDataType(element, nodeId, interfaceId, modId, modposition);
	if (!res.IsSuccessful())
	{
		return res;
	}

	/** Creates array data type of node into the core library */
	res = CreateArrayDataType(element, nodeId, interfaceId, modId, modposition);
	if (!res.IsSuccessful())
	{
		return res;
	}

	return CliResult();
}

CliResult ProjectParser::SetParamAllowedValue(const ParserElement& element,
        const std::uint8_t nodeId,
        const xercesc::DOMNode* node,
        const std::string& uniqueId,
        const std::string& interfaceId,
        const std::string& modId,
        const std::uint32_t modPosition)
{
	ParserResult pResult;

	CliResult crres = pResult.CreateResult(element,
	                                       kParameterAllowedValueXpathExpression,
	                                       kFormatStrParameterAllowedValueXpathExpression,
	                                       node);
	if (!crres.IsSuccessful())
	{
		return crres;
	}

	std::vector<std::string> allowedValuelist;
	std::string uniqueIdOutput = uniqueId;

	for (std::uint32_t row = 0; row < pResult.parameters.size(); row++)
	{
		allowedValuelist.push_back(pResult.parameters[row].at(0));
	}

	if (!modId.empty())
	{
		CliResult clires;

		clires = ProjectParser::GetInstance().GetNewParameterId(nodeId,
		         interfaceId, modId,
		         modPosition, uniqueId,
		         uniqueIdOutput);
		if (!clires.IsSuccessful())
		{
			return clires;
		}
	}

	/** Core Library API call to set allowed parameter values */
	Result res = OpenConfiguratorCore::GetInstance().SetParameterAllowedValues(
	                 OpenConfiguratorCli::GetInstance().networkName,
	                 nodeId,
	                 uniqueIdOutput,
	                 allowedValuelist);
	if (!res.IsSuccessful())
	{
		return CliLogger::GetInstance().GetFailureErrorString(res);
	}

	return CliResult();
}

CliResult ProjectParser::SetParamAllowedRange(const ParserElement& element,
        const std::uint8_t nodeId,
        const xercesc::DOMNode* node,
        const std::string& uniqueId,
        const std::string& interfaceId,
        const std::string& modId,
        const std::uint32_t modPosition)
{
	ParserResult pResult;

	CliResult crres = pResult.CreateResult(element,
	                                       kParameterAllowedValueRangeXpathExpression,
	                                       kFormatStrParameterAllowedValueRangeXpathExpression,
	                                       node);
	if (!crres.IsSuccessful())
	{
		return crres;
	}

	for (std::uint32_t row = 0; row < pResult.parameters.size(); row++)
	{
		ParserResult minpResult;
		ParserResult maxpResult;
		CliResult subcrres;
		std::string minValue = "";
		std::string maxValue = "";
		std::string uniqueIdOutput = uniqueId;

		subcrres = minpResult.CreateResult(element,
		                                   kParameterMinValueXpathExpression,
		                                   kFormatStrParameterMinValueXpathExpression,
		                                   pResult.node.at(row));
		if (subcrres.IsSuccessful())
		{
			minValue = minpResult.parameters[0].at(0);
		}

		subcrres = maxpResult.CreateResult(element,
		                                   kParameterMaxValueXpathExpression,
		                                   kFormatStrParameterMaxValueXpathExpression,
		                                   pResult.node.at(row));
		if (subcrres.IsSuccessful())
		{
			maxValue = maxpResult.parameters[0].at(0);
		}

		if (!modId.empty())
		{
			subcrres = ProjectParser::GetInstance().GetNewParameterId(nodeId,
			           interfaceId, modId,
			           modPosition, uniqueId,
			           uniqueIdOutput);
			if (!subcrres.IsSuccessful())
			{
				return subcrres;
			}
		}

		/** Core Library API call to set allowed parameter values */
		Result res = OpenConfiguratorCore::GetInstance().SetParameterAllowedRange(
		                 OpenConfiguratorCli::GetInstance().networkName,
		                 nodeId, uniqueIdOutput, minValue, maxValue);
		if (!res.IsSuccessful())
		{
			return CliLogger::GetInstance().GetFailureErrorString(res);
		}
	}

	return CliResult();
}

CliResult ProjectParser::SetParamActualValue(const ParserElement& element,
        const std::uint8_t nodeId,
        const xercesc::DOMNode* node,
        const std::string& uniqueId,
        const std::string& interfaceId,
        const std::string& modId,
        const std::uint32_t modPosition)
{
	ParserResult pResult;

	CliResult crres = pResult.CreateResult(element,
	                                       kParameterActualValueXpathExpression,
	                                       kFormatStrParameterActualValueXpathExpression,
	                                       node);
	if (!crres.IsSuccessful())
	{
		return crres;
	}

	std::string uniqueIdOutput = uniqueId;

	if (!modId.empty())
	{
		CliResult clires;

		clires = ProjectParser::GetInstance().GetNewParameterId(nodeId,
		         interfaceId, modId,
		         modPosition, uniqueId,
		         uniqueIdOutput);
		if (!clires.IsSuccessful())
		{
			return clires;
		}
	}

	/** Core Library API call to set actual parameter values */
	Result res = OpenConfiguratorCore::GetInstance().SetParameterActualValue(
	                 OpenConfiguratorCli::GetInstance().networkName,
	                 nodeId, uniqueIdOutput, pResult.parameters[0].at(0));
	if (!res.IsSuccessful())
	{
		return CliLogger::GetInstance().GetFailureErrorString(res);
	}

	return CliResult();
}

CliResult ProjectParser::SetParamDefaultValue(const ParserElement& element,
        const std::uint8_t nodeId,
        const xercesc::DOMNode* node,
        const std::string& uniqueId,
        const std::string& interfaceId,
        const std::string& modId,
        const std::uint32_t modPosition)
{
	ParserResult pResult;

	CliResult crres = pResult.CreateResult(element,
	                                       kParameterDefaultValueXpathExpression,
	                                       kFormatStrParameterDefaultValueXpathExpression,
	                                       node);
	if (!crres.IsSuccessful())
	{
		return crres;
	}

	std::string uniqueIdOutput = uniqueId;

	if (!modId.empty())
	{
		CliResult clires = ProjectParser::GetInstance().GetNewParameterId(nodeId,
		                   interfaceId, modId,
		                   modPosition, uniqueId,
		                   uniqueIdOutput);
		if (!clires.IsSuccessful())
		{
			return clires;
		}
	}

	/** Core Library API call to set actual parameter values */
	Result res = OpenConfiguratorCore::GetInstance().SetParameterDefaultValue(
	                 OpenConfiguratorCli::GetInstance().networkName,
	                 nodeId, uniqueIdOutput, pResult.parameters[0].at(0));
	if (!res.IsSuccessful())
	{
		return CliLogger::GetInstance().GetFailureErrorString(res);
	}

	return CliResult();
}

CliResult ProjectParser::CreateParameterTemplate(const ParserElement& element,
        const std::uint8_t nodeId,
        const std::string& interfaceId,
        const std::string& modId,
        const std::uint32_t modPosition)
{
	ParserResult pResult;

	CliResult crres = pResult.CreateResult(element, kParameterTemplateXpathExpression,
	                                       kFormatStrParameterTemplateXpathExpression);
	if (!crres.IsSuccessful())
	{
		LOG_WARN() << crres.GetErrorMessage();
	}
	else
	{
		for (std::uint32_t row = 0; row < pResult.parameters.size(); row++)
		{
			ParameterAccess accessOfParameter = GetParameterAccess(pResult.parameters[row].at(1));
			IEC_Datatype data = GetIecDataType(pResult.node.at(row));

			ParserResult subpResult;

			CliResult subcrres = subpResult.CreateResult(element,
			                     kParameterDataTypeIdRefXpathExpression,
			                     kFormatStrParameterDataTypeIdRefXpathExpression,
			                     pResult.node.at(row));
			if (!subcrres.IsSuccessful())
			{
				/** Core Library API call to create Parameter */
				Result res = OpenConfiguratorCore::GetInstance().CreateParameter(
				                 OpenConfiguratorCli::GetInstance().networkName,
				                 nodeId, pResult.parameters[row].at(0),
				                 accessOfParameter, data, true,
				                 interfaceId, modId, modPosition);
				if (!res.IsSuccessful())
				{
					return CliLogger::GetInstance().GetFailureErrorString(res);
				}
			}
			else
			{
				for (std::uint32_t subrow = 0; subrow < subpResult.parameters.size(); subrow++)
				{
					ParameterAccess paramAccessSubNode = GetParameterAccess(subpResult.parameters[subrow].at(1));

					/** Core Library API call to create Parameter */
					Result res = OpenConfiguratorCore::GetInstance().CreateParameter(
					                 OpenConfiguratorCli::GetInstance().networkName,
					                 nodeId, pResult.parameters[row].at(0),
					                 subpResult.parameters[subrow].at(0),
					                 paramAccessSubNode, true,
					                 interfaceId, modId, modPosition);
					if (!res.IsSuccessful())
					{
						return CliLogger::GetInstance().GetFailureErrorString(res);
					}
				}
			}

			/** Updates the allowed values of parameter template.*/
			subcrres = SetParamAllowedValue(element, nodeId, pResult.node.at(row),
			                                pResult.parameters[row].at(0), interfaceId,
			                                modId, modPosition);
			if (!subcrres.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(subcrres);
			}

			/** Updates the allowed range of parameter template.*/
			subcrres = SetParamAllowedRange(element, nodeId, pResult.node.at(row),
			                                pResult.parameters[row].at(0), interfaceId,
			                                modId, modPosition);
			if (!subcrres.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(subcrres);
			}

			/** Updates the default value of parameter template.*/
			subcrres = SetParamDefaultValue(element, nodeId, pResult.node.at(row),
			                                pResult.parameters[row].at(0), interfaceId,
			                                modId, modPosition);
			if (!subcrres.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(subcrres);
			}

			/** Updates the actual value of parameter template.*/
			subcrres = SetParamActualValue(element,  nodeId, pResult.node.at(row),
			                               pResult.parameters[row].at(0), interfaceId,
			                               modId, modPosition);
			if (!subcrres.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(subcrres);
			}
		}
	}

	return CliResult();
}

CliResult ProjectParser::CreateChildParameterGroup(const ParserElement& element,
        const std::uint8_t nodeId,
        const xercesc::DOMNode* parameterGroupNode,
        const std::string& paramGroupUId,
        const std::string& interfaceId,
        const std::string& modId,
        const std::uint32_t modPosition)
{
	try
	{
		ParserResult pResult;

		CliResult crres = pResult.CreateResult(element,
		                                       kChildParameterGroupXpathExpression,
		                                       kFormatStrChildParameterGroupXpathExpression,
		                                       parameterGroupNode);
		if (!crres.IsSuccessful())
		{
			LOG_WARN() << crres.GetErrorMessage();
		}
		else
		{
			for (std::uint32_t row = 0; row < pResult.parameters.size(); row++)
			{
				if (pResult.parameters[row].at(1).empty())	/** Is conditionalUniqueIDRef empty */
				{
					std::uint8_t bitoffset = 0;

					if (!pResult.parameters[row].at(3).empty())
					{
						bitoffset = (std::uint8_t)std::stoi(pResult.parameters[row].at(3));
					}

					Result res = OpenConfiguratorCore::GetInstance().CreateParameterGroup(
					                 OpenConfiguratorCli::GetInstance().networkName,
					                 nodeId,
					                 pResult.parameters[row].at(0), 									/** uniqueID */
					                 paramGroupUId,
					                 bitoffset,														/** bitOffset */
					                 interfaceId, modId, modPosition);
					if (!res.IsSuccessful())
					{
						return CliLogger::GetInstance().GetFailureErrorString(res);
					}
				}
				else
				{
					std::uint8_t bitoffset = 0;

					if (!pResult.parameters[row].at(3).empty())
					{
						bitoffset = (std::uint8_t)std::stoi(pResult.parameters[row].at(3));
					}

					Result res = OpenConfiguratorCore::GetInstance().CreateParameterGroup(
					                 OpenConfiguratorCli::GetInstance().networkName,
					                 nodeId,
					                 pResult.parameters[row].at(0), 									/** uniqueID */
					                 paramGroupUId,
					                 pResult.parameters[row].at(1), 									/** conditionalUniqueIDRef */
					                 pResult.parameters[row].at(2), 									/** conditionalValue */
					                 bitoffset,														/** bitOffset */
					                 interfaceId, modId, modPosition);
					if (!res.IsSuccessful())
					{
						return CliLogger::GetInstance().GetFailureErrorString(res);
					}
				}

				ParserResult subpResult;

				CliResult subcrres = subpResult.CreateResult(element,
				                     kChildParameterRefXpathExpression,
				                     kFormatStrChildParameterRefXpathExpression,
				                     pResult.node.at(row));
				if (!subcrres.IsSuccessful())
				{
					LOG_WARN() << CliLogger::GetInstance().GetErrorString(subcrres);
				}
				else
				{
					for (std::uint32_t subrow = 0; subrow < subpResult.parameters.size(); subrow++)
					{
						if (subpResult.parameters[subrow].at(2).empty())	/** Is actualValue empty */
						{
							std::uint8_t bitoffset = 0;

							if (!subpResult.parameters[subrow].at(3).empty())
							{
								bitoffset = (std::uint8_t)std::stoi(subpResult.parameters[subrow].at(3));
							}

							Result res = OpenConfiguratorCore::GetInstance().CreateParameterReference(
							                 OpenConfiguratorCli::GetInstance().networkName,
							                 nodeId, paramGroupUId,
							                 subpResult.parameters[subrow].at(0),									/** uniqueIDRef */
							                 "",
							                 bitoffset, 																/** bitOffset */
							                 interfaceId, modId, modPosition);
							if (!res.IsSuccessful())
							{
								return CliLogger::GetInstance().GetFailureErrorString(res);
							}
						}
						else
						{
							std::uint8_t bitoffset = 0;

							if (!subpResult.parameters[subrow].at(3).empty())
							{
								bitoffset = (std::uint8_t)std::stoi(subpResult.parameters[subrow].at(3));
							}

							Result res = OpenConfiguratorCore::GetInstance().CreateParameterReference(
							                 OpenConfiguratorCli::GetInstance().networkName,
							                 nodeId,
							                 pResult.parameters[row].at(0),											/** uniqueID */
							                 subpResult.parameters[subrow].at(0),									/** uniqueIDRef */
							                 subpResult.parameters[subrow].at(2),  									/** actualValue */
							                 bitoffset,																/** bitOffset */
							                 interfaceId, modId, modPosition);
							if (!res.IsSuccessful())
							{
								return CliLogger::GetInstance().GetFailureErrorString(res);
							}
						}
					}
				}
			}
		}
	}
	catch (const std::exception& e)
	{
		return CliLogger::GetInstance().GetFailureErrorString(e);
	}

	return CliResult();
}

CliResult ProjectParser::CreateParameterGroup(const ParserElement& element,
        const std::uint8_t nodeId,
        const std::string& interfaceId,
        const std::string& modId,
        const std::uint32_t modPosition)
{
	try
	{
		ParserResult pResult;
		CliResult crres = pResult.CreateResult(element,
		                                       kParameterGroupXpathExpression,
		                                       kFormatStrParameterGroupXpathExpression);
		if (!crres.IsSuccessful())
		{
			LOG_WARN() << crres.GetErrorMessage();
		}
		else
		{
			for (std::uint32_t row = 0; row < pResult.parameters.size(); row++)
			{
				Result res = OpenConfiguratorCore::GetInstance().CreateParameterGroup(
				                 OpenConfiguratorCli::GetInstance().networkName,
				                 nodeId,
				                 pResult.parameters[row].at(0), 				/** uniqueID */
				                 interfaceId, modId, modPosition);
				if (!res.IsSuccessful())
				{
					return CliLogger::GetInstance().GetFailureErrorString(res);
				}

				CliResult subcrres;

				subcrres = CreateChildParameterGroup(element, nodeId, pResult.node.at(row),
				                                     pResult.parameters[row].at(0), interfaceId,
				                                     modId, modPosition);
				if (!subcrres.IsSuccessful())
				{
					LOG_WARN() << CliLogger::GetInstance().GetErrorString(subcrres);
				}

				ParserResult subpResult;

				subcrres = subpResult.CreateResult(element,
				                                   kChildParameterRefXpathExpression,
				                                   kFormatStrChildParameterRefXpathExpression,
				                                   pResult.node.at(row));
				if (!subcrres.IsSuccessful())
				{
					LOG_WARN() << CliLogger::GetInstance().GetErrorString(subcrres);
				}
				else
				{
					for (std::uint32_t subrow = 0; subrow < subpResult.parameters.size(); subrow++)
					{
						std::uint8_t bitoffset = 0;

						if (!subpResult.parameters[subrow].at(3).empty())
						{
							bitoffset = (std::uint8_t)std::stoi(subpResult.parameters[subrow].at(3));
						}

						res = OpenConfiguratorCore::GetInstance().CreateParameterReference(
						          OpenConfiguratorCli::GetInstance().networkName,
						          nodeId,
						          pResult.parameters[row].at(0),											/** uniqueID */
						          subpResult.parameters[subrow].at(0),									/** uniqueIDRef */
						          subpResult.parameters[subrow].at(2),  									/** actualValue */
						          bitoffset, 																/** bitOffset */
						          interfaceId, modId, modPosition);
						if (!res.IsSuccessful())
						{
							return CliLogger::GetInstance().GetFailureErrorString(res);
						}
					}
				}
			}
		}
	}
	catch (const std::exception& e)
	{
		return CliLogger::GetInstance().GetFailureErrorString(e);
	}

	return CliResult();
}

CliResult ProjectParser::CreateParameterList(const ParserElement& element,
        const std::uint8_t nodeId,
        const std::string& interfaceId,
        const std::string& modId,
        const std::uint32_t modPosition)
{
	ParserResult pResult;

	CliResult crres = pResult.CreateResult(element,
	                                       kParameterXpathExpression,
	                                       kFormatStrParameterXpathExpression);
	if (!crres.IsSuccessful())
	{
		LOG_WARN() << CliLogger::GetInstance().GetErrorString(crres);
	}
	else
	{
		for (std::uint32_t row = 0; row < pResult.parameters.size(); row++)
		{
			ParameterAccess access = GetParameterAccess(pResult.parameters[row].at(1));
			IEC_Datatype data = GetIecDataType(pResult.node.at(row));

			if (!pResult.parameters[row].at(2).empty())		/** Is templateIDRef not empty */
			{
				Result res = OpenConfiguratorCore::GetInstance().CreateParameter(
				                 OpenConfiguratorCli::GetInstance().networkName,
				                 nodeId,
				                 pResult.parameters[row].at(0), 		/** uniqueID */
				                 access,
				                 pResult.parameters[row].at(2),		/** templateIDRef */
				                 interfaceId,
				                 modId,
				                 modPosition);
				if (!res.IsSuccessful())
				{
					return CliLogger::GetInstance().GetFailureErrorString(res);
				}
			}
			else
			{
				ParserResult subpResult;

				CliResult subcrres = subpResult.CreateResult(element, kParameterDataTypeIdRefXpathExpression,
				                     kFormatStrAppLayerInterfaceXpathExpression,
				                     pResult.node.at(row));
				if (!subcrres.IsSuccessful())
				{
					/** DataTypeIdRef not available */
					Result res = OpenConfiguratorCore::GetInstance().CreateParameter(
					                 OpenConfiguratorCli::GetInstance().networkName,
					                 nodeId,
					                 pResult.parameters[row].at(0),		/** UniqueID */
					                 access,	data,
					                 false, interfaceId,
					                 modId, modPosition);
					if (!res.IsSuccessful())
					{
						return CliLogger::GetInstance().GetFailureErrorString(res);
					}
				}
				else
				{
					for (std::uint32_t subrow = 0; subrow < subpResult.parameters.size(); subrow++)
					{
						ParameterAccess paramAccess = GetParameterAccess(pResult.parameters[row].at(1));

						Result res = OpenConfiguratorCore::GetInstance().CreateParameter(
						                 OpenConfiguratorCli::GetInstance().networkName,
						                 nodeId,
						                 pResult.parameters[row].at(0),					/** uniqueID */
						                 subpResult.parameters[subrow].at(0),			/** uniqueID of subresult */
						                 paramAccess,
						                 false,
						                 interfaceId,
						                 modId,
						                 modPosition);
						if (!res.IsSuccessful())
						{
							return CliLogger::GetInstance().GetFailureErrorString(res);
						}
					}
				}
			}

			CliResult subcrres;

			/** Updates the allowed values of parameter.*/
			subcrres = SetParamAllowedValue(element, nodeId, pResult.node.at(row),
			                                pResult.parameters[row].at(0), interfaceId,
			                                modId, modPosition);
			if (!subcrres.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(subcrres);
			}

			/** Updates the allowed range of parameter.*/
			subcrres = SetParamAllowedRange(element, nodeId, pResult.node.at(row),
			                                pResult.parameters[row].at(0), interfaceId,
			                                modId, modPosition);
			if (!subcrres.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(subcrres);
			}

			/** Updates the default value of parameter.*/
			subcrres = SetParamDefaultValue(element, nodeId, pResult.node.at(row),
			                                pResult.parameters[row].at(0), interfaceId,
			                                modId, modPosition);
			if (!subcrres.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(subcrres);
			}

			/** Updates the actual value of parameter.*/
			subcrres = SetParamActualValue(element, nodeId, pResult.node.at(row),
			                               pResult.parameters[row].at(0), interfaceId,
			                               modId, modPosition);
			if (!subcrres.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(subcrres);
			}
		}
	}

	return CliResult();
}

CliResult ProjectParser::ImportProfileBodyDevice(const ParserElement& element,
        const std::uint8_t nodeId,
        const std::string& interfaceId,
        const std::string& modId,
        const std::uint32_t modPosition)
{
	CliResult subcrres;

	/** Creates datatype in the core library */
	subcrres = CreateDataType(element, nodeId, interfaceId, modId, modPosition);
	if (!subcrres.IsSuccessful())
	{
		LOG_WARN() << CliLogger::GetInstance().GetErrorString(subcrres);
	}

	/** Creates parameter template of node into the core library */
	subcrres = CreateParameterTemplate(element, nodeId, interfaceId, modId, modPosition);
	if (!subcrres.IsSuccessful())
	{
		LOG_WARN() << CliLogger::GetInstance().GetErrorString(subcrres);
	}

	/** Creates parameter list of node into the core library */
	subcrres = CreateParameterList(element, nodeId, interfaceId, modId, modPosition);
	if (!subcrres.IsSuccessful())
	{
		LOG_WARN() << CliLogger::GetInstance().GetErrorString(subcrres);
	}

	/** Creates parameter Group of node into the core library */
	subcrres = CreateParameterGroup(element, nodeId, interfaceId, modId, modPosition);
	if (!subcrres.IsSuccessful())
	{
		LOG_WARN() << CliLogger::GetInstance().GetErrorString(subcrres);
	}

	return CliResult();
}

CliResult ProjectParser::CreateDynamicChannels(const ParserElement& element,
        const std::uint8_t nodeId)
{
	ParserResult pResult;

	CliResult crres = pResult.CreateResult(element, kDynamicChnlXpathExpression,
	                                       kFormatStrDynamicChnlXpathExpressionModule);
	if (!crres.IsSuccessful())
	{
		LOG_WARN() << CliLogger::GetInstance().GetErrorString(crres);
	}
	else
	{
		for (std::uint32_t row = 0; row < pResult.parameters.size(); row++)
		{
			std::string dataType = pResult.parameters[row].at(0);
			std::string accessType = pResult.parameters[row].at(1);
			std::string startIndex = pResult.parameters[row].at(2);
			std::string endIndex = pResult.parameters[row].at(3);
			std::string maxNumber = pResult.parameters[row].at(4);
			std::string addressOfset = pResult.parameters[row].at(5);
			std::string bitAlignment = pResult.parameters[row].at(6);

			std::uint32_t start = 0;
			std::uint32_t end = 0;
			std::uint32_t mxNumber = 0;
			std::uint32_t ofset = 0;
			std::uint8_t bitLign = 0;

			if (!startIndex.empty())
			{
				start = (std::uint32_t)std::stol(startIndex, NULL, 16);
			}
			if (!endIndex.empty())
			{
				end = (std::uint32_t)std::stol(endIndex, NULL, 16);
			}
			if (!maxNumber.empty())
			{
				mxNumber = (std::uint32_t)std::stol(maxNumber, NULL, 16);
			}
			if (!addressOfset.empty())
			{
				ofset = (std::uint32_t)std::stol(addressOfset, NULL, 16);
			}
			if (!bitAlignment.empty())
			{
				bitLign = (std::uint8_t)std::stol(bitAlignment, NULL, 16);
			}

			Result res = OpenConfiguratorCore::GetInstance().CreateDynamicChannel(
			                 OpenConfiguratorCli::GetInstance().networkName,
			                 nodeId,
			                 GetPlkDataType(dataType),
			                 GetDynamicChannelAccessType(accessType),
			                 start,
			                 end,
			                 mxNumber,
			                 ofset,
			                 bitLign);
			if (!res.IsSuccessful())
			{
				return CliLogger::GetInstance().GetFailureErrorString(res);
			}
		}
	}

	return CliResult();
}

CliResult ProjectParser::CreateObject(const ParserElement& element,
                                      const std::uint8_t nodeId,
                                      const std::vector<std::string>& forcedObject,
                                      const std::vector<std::string>& forcedSubObject)
{
	try
	{
		ParserResult pResult;

		CliResult crres = pResult.CreateResult(element, kObjectXpathExpression,
		                                       kFormatStrObjectXpathExpression);
		if (!crres.IsSuccessful())
		{
			LOG_WARN() << CliLogger::GetInstance().GetErrorString(crres);
		}
		else
		{
			for (std::uint32_t row = 0; row < pResult.parameters.size(); row++)
			{
				std::uint32_t objId = 0;
				std::uint8_t objIdType = 0;

				if (!pResult.parameters[row].at(0).empty())
				{
					objId = (std::uint32_t)std::stol(pResult.parameters[row].at(0), NULL, 16);
				}
				if (!pResult.parameters[row].at(2).empty())
				{
					objIdType = (std::uint8_t)std::stoi(pResult.parameters[row].at(2));
				}
				std::string accessType = pResult.parameters[row].at(5);
				std::string dataType = pResult.parameters[row].at(4);
				std::string uniqueIdRef = pResult.parameters[row].at(8);
				std::string actualValue = pResult.parameters[row].at(7);

				if ((accessType.compare("ro") == 0) ||
				        (accessType.compare("const") == 0))
				{
					actualValue = "";
				}

				if ((!dataType.empty()) && (uniqueIdRef.empty()))
				{
					Result res = OpenConfiguratorCore::GetInstance().CreateObject(
					                 OpenConfiguratorCli::GetInstance().networkName,
					                 nodeId, objId,
					                 GetObjectType(objIdType),
					                 pResult.parameters[row].at(1),					/** name */
					                 GetPlkDataType(dataType),
					                 GetObjAccessType(accessType),
					                 GetPdoMapping(pResult.parameters[row].at(6)), 	/** PDOmapping */
					                 pResult.parameters[row].at(3),					/** defaultValue */
					                 actualValue);
					if (!res.IsSuccessful())
					{
						return CliLogger::GetInstance().GetFailureErrorString(res);
					}

					if ((!pResult.parameters[row].at(9).empty()) || (!pResult.parameters[row].at(10).empty()))
					{
						res = OpenConfiguratorCore::GetInstance().SetObjectLimits(
						          OpenConfiguratorCli::GetInstance().networkName,
						          nodeId, objId,
						          pResult.parameters[row].at(9),				/** lowLimit */
						          pResult.parameters[row].at(10));			/** highLimit */
						if (!res.IsSuccessful())
						{
							return CliLogger::GetInstance().GetFailureErrorString(res);
						}
					}
				}
				else if ((!dataType.empty()) && (!uniqueIdRef.empty()))
				{
					Result res = OpenConfiguratorCore::GetInstance().CreateParameterObject(
					                 OpenConfiguratorCli::GetInstance().networkName,
					                 nodeId, objId, GetObjectType(objIdType),
					                 pResult.parameters[row].at(1),					/** name */
					                 GetPlkDataType(dataType),
					                 GetObjAccessType(accessType),
					                 GetPdoMapping(pResult.parameters[row].at(6)), 	/** PDOmapping */
					                 uniqueIdRef,
					                 pResult.parameters[row].at(3),					/** defaultValue */
					                 actualValue);
					if (!res.IsSuccessful())
					{
						return CliLogger::GetInstance().GetFailureErrorString(res);
					}
				}
				else if ((dataType.empty()) && (!uniqueIdRef.empty()))
				{
					Result res = OpenConfiguratorCore::GetInstance().CreateParameterObject(
					                 OpenConfiguratorCli::GetInstance().networkName,
					                 nodeId, objId,
					                 GetObjectType(objIdType),
					                 pResult.parameters[row].at(1),					/** name */
					                 PlkDataType::UNDEFINED,
					                 GetObjAccessType(accessType),
					                 GetPdoMapping(pResult.parameters[row].at(6)), 	/** PDOmapping */
					                 uniqueIdRef,
					                 pResult.parameters[row].at(3),					/** defaultValue */
					                 actualValue);
					if (!res.IsSuccessful())
					{
						return CliLogger::GetInstance().GetFailureErrorString(res);
					}

				}
				else if ((dataType.empty()) && (uniqueIdRef.empty()))
				{
					Result res = OpenConfiguratorCore::GetInstance().CreateObject(
					                 OpenConfiguratorCli::GetInstance().networkName,
					                 nodeId, objId, GetObjectType(objIdType),
					                 pResult.parameters[row].at(1),					/** name */
					                 GetPlkDataType(dataType),
					                 GetObjAccessType(accessType),
					                 GetPdoMapping(pResult.parameters[row].at(6)), 	/** PDOmapping */
					                 pResult.parameters[row].at(3),					/** defaultValue */
					                 actualValue);
					if (!res.IsSuccessful())
					{
						return CliLogger::GetInstance().GetFailureErrorString(res);
					}

					if ((!pResult.parameters[row].at(9).empty()) || (!pResult.parameters[row].at(10).empty()))
					{
						res = OpenConfiguratorCore::GetInstance().SetObjectLimits(
						          OpenConfiguratorCli::GetInstance().networkName,
						          nodeId, objId,
						          pResult.parameters[row].at(9),			/** lowLimit */
						          pResult.parameters[row].at(10));		/** highLimit */
						if (!res.IsSuccessful())
						{
							return CliLogger::GetInstance().GetFailureErrorString(res);
						}
					}
				}

				ParserResult subpResult;

				CliResult subcrres = subpResult.CreateResult(element, kSubObjectXpathExpression,
				                     kFormatStrSubObjectXpathExpression,
				                     pResult.node.at(row));
				if (!subcrres.IsSuccessful())
				{
					LOG_WARN() << CliLogger::GetInstance().GetErrorString(subcrres);
				}
				else
				{
					for (std::uint32_t subrow = 0; subrow < subpResult.parameters.size(); subrow++)
					{
						std::uint8_t subObjId = 0;
						std::uint8_t subObjIdType = 0;

						if (!subpResult.parameters[subrow].at(0).empty())
						{
							subObjId = (std::uint8_t)std::stol(subpResult.parameters[subrow].at(0), NULL, 16);
						}
						if (!subpResult.parameters[subrow].at(2).empty())
						{
							subObjIdType = (std::uint8_t)std::stoi(subpResult.parameters[subrow].at(2));
						}
						std::string subObjaccessType = subpResult.parameters[subrow].at(5);
						std::string subObjActualValue = subpResult.parameters[subrow].at(7);
						std::string subObjuniqueIdRef = subpResult.parameters[subrow].at(8);
						std::string subObjdataType = subpResult.parameters[subrow].at(4);

						if ((subObjaccessType.compare("ro") == 0) ||
						        (subObjaccessType.compare("const") == 0))
						{
							subObjActualValue = "";
						}

						if (subpResult.parameters[subrow].at(8).empty())						/** Is uniqueIdRef empty */
						{
							Result res = OpenConfiguratorCore::GetInstance().CreateSubObject(
							                 OpenConfiguratorCli::GetInstance().networkName,
							                 nodeId, objId, subObjId, GetObjectType(subObjIdType),
							                 subpResult.parameters[subrow].at(1),					/** sub object name */
							                 GetPlkDataType(subpResult.parameters[subrow].at(4)), 	/** sub object dataType */
							                 GetObjAccessType(subObjaccessType),
							                 GetPdoMapping(subpResult.parameters[subrow].at(6)),		/** sub object PDOmapping */
							                 subpResult.parameters[subrow].at(3),					/** sub object defaultValue */
							                 subObjActualValue);
							if (!res.IsSuccessful())
							{
								return CliLogger::GetInstance().GetFailureErrorString(res);
							}

							/** If any one of the sub object lowLimit and highLimit has values */
							if ((!subpResult.parameters[subrow].at(9).empty()) || (!subpResult.parameters[subrow].at(10).empty()))
							{
								res = OpenConfiguratorCore::GetInstance().SetSubObjectLimits(
								          OpenConfiguratorCli::GetInstance().networkName,
								          nodeId, objId, subObjId,
								          subpResult.parameters[subrow].at(9),			/** sub object lowLimit */
								          subpResult.parameters[subrow].at(10));			/** sub object highLimit */
								if (!res.IsSuccessful())
								{
									return CliLogger::GetInstance().GetFailureErrorString(res);
								}
							}
						}
						else if ((!subObjuniqueIdRef.empty()) && (!subObjdataType.empty()))
						{
							/** Both sub object uniqueIdRef and dataType has values */
							Result res = OpenConfiguratorCore::GetInstance().CreateParameterSubObject(
							                 OpenConfiguratorCli::GetInstance().networkName,
							                 nodeId, objId, subObjId,
							                 GetObjectType(subObjIdType),
							                 subpResult.parameters[subrow].at(1),					/** sub object name */
							                 GetPlkDataType(subObjdataType),
							                 GetObjAccessType(subObjaccessType),
							                 GetPdoMapping(subpResult.parameters[subrow].at(6)),		/** sub object PDOmapping */
							                 subObjuniqueIdRef,
							                 subpResult.parameters[subrow].at(3),					/** sub object defaultValue */
							                 subObjActualValue);
							if (!res.IsSuccessful())
							{
								return CliLogger::GetInstance().GetFailureErrorString(res);
							}
						}
						else if ((!subObjuniqueIdRef.empty()) && (subObjdataType.empty()))
						{
							/** Both sub object uniqueIdRef and dataType has values */
							Result res = OpenConfiguratorCore::GetInstance().CreateParameterSubObject(
							                 OpenConfiguratorCli::GetInstance().networkName,
							                 nodeId, objId, subObjId, GetObjectType(subObjIdType),
							                 subpResult.parameters[subrow].at(1),					/** sub object name */
							                 PlkDataType::UNDEFINED,
							                 GetObjAccessType(subObjaccessType),
							                 GetPdoMapping(subpResult.parameters[subrow].at(6)),		/** sub object PDOmapping */
							                 subObjuniqueIdRef,
							                 subpResult.parameters[subrow].at(3),					/** sub object defaultValue */
							                 subObjActualValue);
							if (!res.IsSuccessful())
							{
								return CliLogger::GetInstance().GetFailureErrorString(res);
							}
						}

						CliResult cliRes = 	UpdateForcedObjects(forcedObject, forcedSubObject, nodeId,
						                                        pResult.parameters[row].at(0),
						                                        subpResult.parameters[subrow].at(0),
						                                        objId, subObjId, actualValue,
						                                        subObjActualValue, false, "", "", 0U);
						if (!cliRes.IsSuccessful())
						{
							LOG_WARN() << CliLogger::GetInstance().GetErrorString(cliRes);
						}
					}
				}
			}
		}
	}
	catch (const std::exception& e)
	{
		return CliLogger::GetInstance().GetFailureErrorString(e);
	}

	return CliResult();
}

CliResult ProjectParser::ImportProfileBodyCommn(const ParserElement& element,
        const std::uint8_t nodeId,
        const std::vector<std::string>& forcedObject,
        const std::vector<std::string>& forcedSubObject,
        const std::string& interfaceId,
        const std::string& modId,
        const std::uint32_t modPosition)
{
	CliResult subcrres;

	/** Creates objest in the core library */
	if (modId.empty())
	{
		subcrres = CreateObject(element, nodeId, forcedObject,
		                        forcedSubObject);
		if (!subcrres.IsSuccessful())
		{
			LOG_WARN() << CliLogger::GetInstance().GetErrorString(subcrres);
		}
	}
	else
	{
		subcrres = CreateModuleObject(element, nodeId, forcedObject, forcedSubObject,
		                              interfaceId, modId, modPosition);
		if (!subcrres.IsSuccessful())
		{
			LOG_WARN() << CliLogger::GetInstance().GetErrorString(subcrres);
		}
	}

	return CliResult();
}

CliResult ProjectParser::CreateNwMgtGeneralFeatures(const ParserElement& element,
        const std::uint8_t nodeId)
{
	ParserResult pResult;

	CliResult crres = pResult.CreateResult(element,
	                                       kGeneralFeatureXpathExpression,
	                                       kFormatStrGeneralFeatureXpathExpression);
	if (!crres.IsSuccessful())
	{
		return crres;
	}

	for (std::uint32_t row = 0; row < pResult.parameters.size(); row++)
	{
		for (std::uint32_t param = 0; param < pResult.parameters[row].size(); param++)
		{
			Result res = OpenConfiguratorCore::GetInstance().SetFeatureValue(
			                 OpenConfiguratorCli::GetInstance().networkName,
			                 nodeId,
			                 ((GeneralFeatureEnum) param),
			                 pResult.parameters[row].at(param));
			if (!res.IsSuccessful())
			{
				return CliLogger::GetInstance().GetFailureErrorString(res);
			}
		}
	}

	return CliResult();
}

CliResult ProjectParser::CreateNwMgtMnFeatures(const ParserElement& element,
        const std::uint8_t nodeId)
{
	ParserResult pResult;

	CliResult crres = pResult.CreateResult(element,
	                                       kMnFeatureXpathExpression,
	                                       kFormatStrMnFeatureXpathExpression);
	if (!crres.IsSuccessful())
	{
		return crres;
	}

	for (std::uint32_t row = 0; row < pResult.parameters.size(); row++)
	{
		for (std::uint32_t param = 0; param < pResult.parameters[row].size(); param++)
		{
			Result res = OpenConfiguratorCore::GetInstance().SetFeatureValue(
			                 OpenConfiguratorCli::GetInstance().networkName,
			                 nodeId,
			                 (MNFeatureEnum)(param + (std::int32_t) MNFeatureEnum::DLLErrMNMultipleMN),
			                 pResult.parameters[row].at(param));
			if (!res.IsSuccessful())
			{
				return CliLogger::GetInstance().GetFailureErrorString(res);
			}
		}
	}

	return CliResult();
}

CliResult ProjectParser::CreateNwMgtCnFeatures(const ParserElement& element,
        const std::uint8_t nodeId)
{
	ParserResult pResult;

	CliResult crres = pResult.CreateResult(element,
	                                       kCnFeatureXpathExpression,
	                                       kFormatStrCnFeatureXpathExpression);
	if (!crres.IsSuccessful())
	{
		return crres;
	}

	for (std::uint32_t row = 0; row < pResult.parameters.size(); row++)
	{
		for (std::uint32_t param = 0; param < pResult.parameters[row].size(); param++)
		{
			Result res = OpenConfiguratorCore::GetInstance().SetFeatureValue(
			                 OpenConfiguratorCli::GetInstance().networkName,
			                 nodeId,
			                 (CNFeatureEnum)(param + (std::int32_t) CNFeatureEnum::DLLCNFeatureMultiplex),
			                 pResult.parameters[row].at(param));
			if (!res.IsSuccessful())
			{
				return CliLogger::GetInstance().GetFailureErrorString(res);
			}
		}
	}

	return CliResult();
}

CliResult ProjectParser::CreateInterface(const std::uint8_t nodeId,
        const std::string& cnXdc,
        std::uint8_t& maxModulePosition)
{
	std::string indexString = kPathSeparator + cnXdc;
	std::string initialPath = OpenConfiguratorCli::GetInstance().xmlFilePath;
	std::string nextInitialPath = initialPath.substr(0, initialPath.find_last_of(kPathSeparator));
	std::string xdcPath = nextInitialPath.append(indexString);
	CliResult res;

	maxModulePosition = 0;

	/** Validate the XDC file */
	res = ParameterValidator::GetInstance().IsXdcFileValid(xdcPath);
	if (!res.IsSuccessful())
	{
		/** XDC file is not exists or invalid */
		return res;
	}

	ParserElement element(xdcPath, kXdcSchemaDefinitionFileName, kXDDNamespace);
	CliResult ceres;

	ceres = element.CreateElement();
	if (!ceres.IsSuccessful())
	{
		return ceres;
	}
	else
	{
		ParserResult pResult;

		res = pResult.CreateResult(element, kInterfaceXdcXpathExpression,
		                           kFormatStrInterfaceXdcXpathExpression);
		if (!res.IsSuccessful())
		{
			return res;
		}

		for (std::uint32_t row = 0; row < pResult.parameters.size(); row++)
		{
			std::uint32_t maxModules = 0;

			ModuleAddressing modAddress = GetModuleAddressing(pResult.parameters[row].at(2));		/** Module addressing */
			if (!pResult.parameters[row].at(3).empty())
			{
				maxModules = (std::uint32_t)std::stol(pResult.parameters[row].at(3), NULL, 16);		/** Max Modules */
				maxModulePosition = (std::uint8_t)maxModules;
			}

			bool unusedSlot = false;
			if (pResult.parameters[row].at(4).compare("true") == 0)
			{
				unusedSlot = true;
			}
			else
			{
				unusedSlot = false;
			}

			bool multipleMod = false;
			if (pResult.parameters[row].at(5).compare("true") == 0)
			{
				multipleMod = true;
			}
			else
			{
				multipleMod = false;
			}

			/** Core Library API call to create Interface */
			Result result = OpenConfiguratorCore::GetInstance().CreateInterface(
			                    OpenConfiguratorCli::GetInstance().networkName,
			                    nodeId,
			                    pResult.parameters[row].at(0),			/** Interface uniqueId */
			                    pResult.parameters[row].at(1),			/** Interface type */
			                    modAddress,
			                    maxModules,
			                    unusedSlot,
			                    multipleMod);
			if (!result.IsSuccessful())
			{
				return CliLogger::GetInstance().GetFailureErrorString(result);
			}

			ParserResult subPresult;

			CliResult cliRes = subPresult.CreateResult(element, kAppLayerInterfaceXpathExpression,
			                   kFormatStrAppLayerInterfaceXpathExpression);
			if (!cliRes.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(cliRes);
			}
			else
			{
				for (std::uint32_t line = 0; line < subPresult.parameters.size(); line++)
				{
					ParserResult subRangeResult;

					CliResult rescli = subRangeResult.CreateResult(element, kInterfaceRangeXpathExpression,
					                   kFormatStrInterfaceRangeXpathExpression,
					                   subPresult.node.at(line));
					if (!rescli.IsSuccessful())
					{
						LOG_WARN() << CliLogger::GetInstance().GetErrorString(rescli);
					}
					else
					{
						for (std::uint32_t subRow = 0 ; subRow < subRangeResult.parameters.size(); subRow++)
						{
							std::uint32_t baseIndex = 0;
							std::uint32_t maxIndex = 0;
							std::uint32_t maxSubIndex = 0;
							std::uint32_t rangeSortStep = 0;

							if (!subRangeResult.parameters[subRow].at(1).empty())
							{
								baseIndex = (std::uint32_t)std::stol(subRangeResult.parameters[subRow].at(1), NULL, 16);			/** Base index value */
							}
							if (!subRangeResult.parameters[subRow].at(2).empty())
							{
								maxIndex = (std::uint32_t)std::stol(subRangeResult.parameters[subRow].at(2), NULL, 16);			/** Max index value */
							}
							if (!subRangeResult.parameters[subRow].at(3).empty())
							{
								maxSubIndex = (std::uint32_t)std::stol(subRangeResult.parameters[subRow].at(3), NULL, 16);		/** Max Sub index value */
							}
							SortMode sortmodRange = GetSortMode(subRangeResult.parameters[subRow].at(4));									/** Sort mode value */
							SortNumber sortRangeNumber = GetSortNumber(subRangeResult.parameters[subRow].at(5));							/** Sort number value */
							PDOMapping mapping = GetPdoMapping(subRangeResult.parameters[subRow].at(6));									/** PDO mapping value */
							if (!subRangeResult.parameters[subRow].at(7).empty())
							{
								rangeSortStep = (std::uint32_t)std::stol(subRangeResult.parameters[subRow].at(7), NULL, 16);		/** Sort step value */
							}

							result = OpenConfiguratorCore::GetInstance().CreateRange(
							             OpenConfiguratorCli::GetInstance().networkName,
							             nodeId,
							             subPresult.parameters[line].at(0),					/** Interface uniqueId */
							             subRangeResult.parameters[subRow].at(0),			/** Range name */
							             baseIndex,
							             maxIndex,
							             maxSubIndex,
							             rangeSortStep,
							             sortmodRange,
							             sortRangeNumber,
							             mapping);
							if (!result.IsSuccessful())
							{
								return CliLogger::GetInstance().GetFailureErrorString(result);
							}
						}
					}
				}
			}
		}
	}

	return CliResult();
}

CliResult ProjectParser::UpdateNodeIdCollection(const std::uint8_t nodeId,
        const std::string& cnXdc,
        const std::vector<std::string>& forcedObject,
        const std::vector<std::string>& forcedSubObject,
        const std::string& interfaceId,
        const std::string& modId,
        const std::uint32_t modPosition)
{
	std::string indexString = kPathSeparator + cnXdc;
	std::string initialPath = OpenConfiguratorCli::GetInstance().xmlFilePath;
	std::string nextInitialPath = initialPath.substr(0, initialPath.find_last_of(kPathSeparator));
	std::string xdcPath = nextInitialPath.append(indexString);
	CliResult res;

	/** Validate the XDC file */
	res = ParameterValidator::GetInstance().IsXdcFileValid(xdcPath);
	if (!res.IsSuccessful())
	{
		/** XDC file is not exists or invalid */
		return res;
	}

	ParserElement element(xdcPath, kXdcSchemaDefinitionFileName, kXDDNamespace);
	CliResult ceres;

	ceres = element.CreateElement();
	if (!ceres.IsSuccessful())
	{
		return ceres;
	}
	else
	{
		res = ImportProfileBodyDevice(element, nodeId, interfaceId, modId, modPosition);
		if (!res.IsSuccessful())
		{
			LOG_WARN() << CliLogger::GetInstance().GetErrorString(res);
		}

		res = ImportProfileBodyCommn(element, nodeId, forcedObject, forcedSubObject,
		                             interfaceId, modId, modPosition);
		if (!res.IsSuccessful())
		{
			LOG_WARN() << CliLogger::GetInstance().GetErrorString(res);
		}

		if (nodeId > MAX_CN_NODE_ID)
		{
			res = CreateDynamicChannels(element, nodeId);
			if (!res.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(res);
			}
		}

		if (modId.empty())
		{
			res = CreateNwMgtGeneralFeatures(element, nodeId);
			if (!res.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(res);
			}

			res = CreateNwMgtMnFeatures(element, nodeId);
			if (!res.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(res);
			}

			res = CreateNwMgtCnFeatures(element, nodeId);
			if (!res.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(res);
			}
		}
	}

	return CliResult();
}

CliResult ProjectParser::CreateNodeAssignment(const ParserElement& pElement,
        const std::string& xPathExpression,
        const std::uint8_t nodeId)
{
	ParserResult pResult;

	CliResult crres = pResult.CreateResult(pElement, xPathExpression,
	                                       kFormatStrxPathExpression);
	if (!crres.IsSuccessful())
	{
		return crres;
	}

	std::uint32_t row = 0;
	for (row = 0; row < pResult.parameters.size(); row++)
	{
		if (pResult.parameters[row].at(10) == std::to_string(nodeId))
			break;
	}

	if (nodeId == MN_DEFAULT_NODE_ID)
	{
		CliResult res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_MN_PRES,
		                                  nodeId,
		                                  pResult.parameters[0].at(0));	/** TaransmitPres value */
		if (!res.IsSuccessful())
		{
			LOG_WARN() << CliLogger::GetInstance().GetErrorString(res);
		}

		res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_NODE_EXISTS, nodeId, "true");
		if (!res.IsSuccessful())
		{
			LOG_WARN() << CliLogger::GetInstance().GetErrorString(res);
		}

		res = SetNodeAssignment(NodeAssignment::MNT_NODEASSIGN_VALID, nodeId, "true");
		if (!res.IsSuccessful())
		{
			LOG_WARN() << CliLogger::GetInstance().GetErrorString(res);
		}
	}
	else
	{
		if (nodeId > MN_DEFAULT_NODE_ID)
		{
			CliResult res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_NODE_IS_CN, nodeId, "true");
			if (!res.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(res);
			}

			res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_NODE_EXISTS, nodeId, "true");
			if (!res.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(res);
			}
		}
		else
		{
			CliResult res;

			res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_NODE_IS_CN, nodeId, "true");
			if (!res.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(res);
			}

			res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_MANDATORY_CN,
			                        nodeId, pResult.parameters[row].at(1));			/** isMandatory value */
			if (!res.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(res);
			}

			res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_START_CN,
			                        nodeId, pResult.parameters[row].at(2));			/** autostartNode value */
			if (!res.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(res);
			}

			res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_KEEPALIVE,
			                        nodeId, pResult.parameters[row].at(3));			/** resetInOperational value */
			if (!res.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(res);
			}

			res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_SWVERSIONCHECK,
			                        nodeId, pResult.parameters[row].at(4));			/** verifyAppSwVersion value */
			if (!res.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(res);
			}

			res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_SWUPDATE,
			                        nodeId, pResult.parameters[row].at(5));			/** autoAppSwUpdateAllowed value */
			if (!res.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(res);
			}

			res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_ASYNCONLY_NODE,
			                        nodeId, pResult.parameters[row].at(7));			/** isAsyncOnly value */
			if (!res.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(res);
			}

			res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_NODE_EXISTS, nodeId, "true");
			if (!res.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(res);
			}

			res = SetNodeAssignment(NodeAssignment::MNT_NODEASSIGN_VALID, nodeId, "true");
			if (!res.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(res);
			}

			res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_RT1,
			                        nodeId, pResult.parameters[row].at(8));			/** isType1Router value */
			if (!res.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(res);
			}

			res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_RT2,
			                        nodeId, pResult.parameters[row].at(9));			/** isType2Router value */
			if (!res.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(res);
			}

			if (pResult.parameters[row].at(6).compare("true") == 0)					/** isChained value */
			{
				Result result = OpenConfiguratorCore::GetInstance().SetOperationModeChained(
				                    OpenConfiguratorCli::GetInstance().networkName, nodeId);
				if (!result.IsSuccessful())
				{
					return CliLogger::GetInstance().GetFailureErrorString(result);
				}
			}
		}
	}

	return CliResult();
}

CliResult ProjectParser::SetNodeAssignment(const NodeAssignment nodeassign,
        const std::uint8_t nodeId,
        const std::string& value)
{
	if (value.compare("true") == 0)
	{
		Result res = OpenConfiguratorCore::GetInstance().AddNodeAssignment(
		                 OpenConfiguratorCli::GetInstance().networkName,
		                 nodeId, nodeassign);
		if (!res.IsSuccessful())
		{
			return CliLogger::GetInstance().GetFailureErrorString(res);
		}
	}
	else
	{
		Result res = OpenConfiguratorCore::GetInstance().RemoveNodeAssignment(
		                 OpenConfiguratorCli::GetInstance().networkName,
		                 nodeId, nodeassign);
		if (!res.IsSuccessful())
		{
			return CliLogger::GetInstance().GetFailureErrorString(res);
		}
	}

	return CliResult();
}

CliResult ProjectParser::CreateModule(const std::uint8_t nodeId,
                                      const std::vector<std::string>& forcedObject,
                                      const std::vector<std::string>& forcedSubObject,
                                      const std::string& interfacecUId,
                                      const std::string& moduleName,
                                      const std::string& modulePosition,
                                      const std::string& moduleAddress,
                                      const std::string& modulePathToXDC,
                                      const std::uint8_t maxModulePosition)
{
	try
	{
		std::string indexString = kPathSeparator + modulePathToXDC;
		std::string initialPath = OpenConfiguratorCli::GetInstance().xmlFilePath;
		std::string nextInitialPath = initialPath.substr(0, initialPath.find_last_of(kPathSeparator));
		std::string xdcPath = nextInitialPath.append(indexString);
		CliResult res;

		/** Validate the XDC file */
		res = ParameterValidator::GetInstance().IsXdcFileValid(xdcPath);
		if (!res.IsSuccessful())
		{
			/** XDC file is not exists or invalid */
			return res;
		}

		ParserElement element(xdcPath, kXdcSchemaDefinitionFileName, kXDDNamespace);
		CliResult ceres;

		ceres = element.CreateElement();
		if (!ceres.IsSuccessful())
		{
			return ceres;
		}

		ParserResult pResult;

		res = pResult.CreateResult(element, kModuleNodeXpathExpression,
		                           kFormatStrModuleNodeXpathExpression);
		if (!res.IsSuccessful())
		{
			return res;
		}

		for (std::uint32_t moduleSubRow = 0; moduleSubRow < pResult.parameters.size(); moduleSubRow++)
		{
			std::uint8_t minPosition = 0;
			std::uint8_t maxPosition = maxModulePosition;
			std::uint8_t minAddress = 0;
			std::uint8_t maxAddress = maxModulePosition;
			std::uint8_t maxCount = maxModulePosition;
			std::uint32_t modpos = 0;
			std::uint32_t moduleAdrs = 0;

			ModuleAddressing modAddressing = GetModuleAddressing(pResult.parameters[moduleSubRow].at(2));	/** Module addressing */
			if (!pResult.parameters[moduleSubRow].at(3).empty())
			{
				minPosition = (std::uint8_t)std::stoi(pResult.parameters[moduleSubRow].at(3));		/** Module min position */
			}
			if (!pResult.parameters[moduleSubRow].at(4).empty())
			{
				maxPosition = (std::uint8_t)std::stoi(pResult.parameters[moduleSubRow].at(4));		/** Module max position */
			}
			if (!pResult.parameters[moduleSubRow].at(5).empty())
			{
				minAddress = (std::uint8_t)std::stoi(pResult.parameters[moduleSubRow].at(5));		/** Module min adress */
			}
			if (!pResult.parameters[moduleSubRow].at(6).empty())
			{
				maxAddress = (std::uint8_t)std::stoi(pResult.parameters[moduleSubRow].at(6));		/** Module max address */
			}
			if (!pResult.parameters[moduleSubRow].at(7).empty())
			{
				maxCount = (std::uint8_t)std::stoi(pResult.parameters[moduleSubRow].at(7));			/** Module max count */
			}
			if (!modulePosition.empty())
			{
				modpos = (std::uint32_t)std::stol(modulePosition, NULL, 16);
			}
			if (!moduleAddress.empty())
			{
				moduleAdrs = (std::uint32_t)std::stol(moduleAddress, NULL, 16);
			}

			Result result = OpenConfiguratorCore::GetInstance().CreateModule(OpenConfiguratorCli::GetInstance().networkName,
			                nodeId,
			                interfacecUId,
			                pResult.parameters[moduleSubRow].at(0),		/** Module Child ID */
			                modpos,
			                moduleAdrs,
			                pResult.parameters[moduleSubRow].at(1),		/** Module type */
			                moduleName,
			                modAddressing,
			                minPosition,
			                maxPosition,
			                minAddress,
			                maxAddress,
			                maxCount);
			if (!result.IsSuccessful())
			{
				return CliLogger::GetInstance().GetFailureErrorString(result);
			}

			CliResult clires = UpdateNodeIdCollection(nodeId, modulePathToXDC, forcedObject,
			                   forcedSubObject, interfacecUId,
			                   pResult.parameters[moduleSubRow].at(0),
			                   modpos);
			if (!clires.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(clires);
			}
		}
	}
	catch (const std::exception& e)
	{
		return CliLogger::GetInstance().GetFailureErrorString(e);
	}

	return CliResult();
}

CliResult ProjectParser::CreateModuleObject(const ParserElement& element,
        const std::uint8_t nodeId,
        const std::vector<std::string>& forcedObject,
        const std::vector<std::string>& forcedSubObject,
        const std::string& interfaceId,
        const std::string& modId,
        const std::uint32_t modPosition)
{
	try
	{
		ParserResult pResult;
		CliResult clires = pResult.CreateResult(element, kObjectXpathExpression,
		                                        kFormatStrObjectXpathExpressionModule);

		if (!clires.IsSuccessful())
		{
			return clires;
		}

		for (std::uint32_t row = 0; row < pResult.parameters.size(); row++)
		{
			std::uint32_t objId = 0;

			if (!pResult.parameters[row].at(0).empty())						/** Object Index */
			{
				objId = (std::uint32_t)std::stol(pResult.parameters[row].at(0), NULL, 16);
			}

			std::uint8_t objIdType = 0;

			if (!pResult.parameters[row].at(2).empty())
			{
				objIdType = (std::uint8_t)std::stoi(pResult.parameters[row].at(2));
			}

			std::string accessType = pResult.parameters[row].at(5);			/** Access Type */
			std::string dataType = pResult.parameters[row].at(4);			/** data Type */
			std::string uniqueIdRef = pResult.parameters[row].at(8);		/** UniqueId ref */
			std::string actualValue = pResult.parameters[row].at(7);		/** Actual value */
			std::string rangeSelector = pResult.parameters[row].at(11);	/** Range selector */

			if ((accessType.compare("ro") == 0) ||
			        (accessType.compare("const") == 0))
			{
				actualValue = "";
			}

			if ((!dataType.empty()) && (uniqueIdRef.empty()))
			{
				Result res = OpenConfiguratorCore::GetInstance().CreateModuleObject(
				                 OpenConfiguratorCli::GetInstance().networkName,
				                 nodeId, interfaceId,
				                 modId, modPosition,
				                 objId, GetObjectType(objIdType),
				                 pResult.parameters[row].at(1),						/** name */
				                 GetPlkDataType(dataType),
				                 GetObjAccessType(accessType),
				                 GetPdoMapping(pResult.parameters[row].at(6)), 		/** PDOmapping */
				                 pResult.parameters[row].at(3),						/** defaultValue */
				                 actualValue, rangeSelector);
				if (!res.IsSuccessful())
				{
					clires = CliLogger::GetInstance().GetFailureErrorString(res);

					LOG_WARN() << CliLogger::GetInstance().GetErrorString(clires);
				}
				else
				{
					if ((!pResult.parameters[row].at(9).empty()) || (!pResult.parameters[row].at(10).empty()))
					{
						CliResult oiclires;
						std::uint32_t objIndex = 0;

						oiclires = GetNewObjectIndex(nodeId, interfaceId, modId,
						                             modPosition, objId, objIndex);
						if (!oiclires.IsSuccessful())
						{
							LOG_WARN() << CliLogger::GetInstance().GetErrorString(oiclires);
						}
						else
						{
							res = OpenConfiguratorCore::GetInstance().SetObjectLimits(
							          OpenConfiguratorCli::GetInstance().networkName,
							          nodeId,
							          objIndex,
							          pResult.parameters[row].at(9),				/** lowLimit */
							          pResult.parameters[row].at(10));			/** highLimit */
							if (!res.IsSuccessful())
							{
								CliResult soclires = CliLogger::GetInstance().GetFailureErrorString(res);

								LOG_WARN() << CliLogger::GetInstance().GetErrorString(soclires);
							}
						}
					}
				}
			}
			else if ((!dataType.empty()) && (!uniqueIdRef.empty()))
			{
				Result res = OpenConfiguratorCore::GetInstance().CreateModuleParameterObject(
				                 OpenConfiguratorCli::GetInstance().networkName,
				                 nodeId, interfaceId,
				                 modId, modPosition,
				                 objId, GetObjectType(objIdType),
				                 pResult.parameters[row].at(1),						/** name */
				                 GetPlkDataType(dataType),
				                 GetObjAccessType(accessType),
				                 GetPdoMapping(pResult.parameters[row].at(6)), 		/** PDOmapping */
				                 uniqueIdRef,
				                 pResult.parameters[row].at(3),						/** defaultValue */
				                 actualValue, rangeSelector);
				if (!res.IsSuccessful())
				{
					clires = CliLogger::GetInstance().GetFailureErrorString(res);

					LOG_WARN() << CliLogger::GetInstance().GetErrorString(clires);
				}
			}
			else if ((dataType.empty()) && (!uniqueIdRef.empty()))
			{
				Result res = OpenConfiguratorCore::GetInstance().CreateModuleParameterObject(
				                 OpenConfiguratorCli::GetInstance().networkName,
				                 nodeId, interfaceId,
				                 modId, modPosition,
				                 objId,
				                 GetObjectType(objIdType),
				                 pResult.parameters[row].at(1),						/** name */
				                 PlkDataType::UNDEFINED,
				                 GetObjAccessType(accessType),
				                 GetPdoMapping(pResult.parameters[row].at(6)), 		/** PDOmapping */
				                 uniqueIdRef,
				                 pResult.parameters[row].at(3),						/** defaultValue */
				                 actualValue, rangeSelector);
				if (!res.IsSuccessful())
				{
					clires = CliLogger::GetInstance().GetFailureErrorString(res);

					LOG_WARN() << CliLogger::GetInstance().GetErrorString(clires);
				}

			}
			else if ((dataType.empty()) && (uniqueIdRef.empty()))
			{
				Result res = OpenConfiguratorCore::GetInstance().CreateModuleObject(
				                 OpenConfiguratorCli::GetInstance().networkName,
				                 nodeId, interfaceId,
				                 modId, modPosition,
				                 objId, GetObjectType(objIdType),
				                 pResult.parameters[row].at(1),						/** name */
				                 GetPlkDataType(dataType),
				                 GetObjAccessType(accessType),
				                 GetPdoMapping(pResult.parameters[row].at(6)), 		/** PDOmapping */
				                 pResult.parameters[row].at(3),						/** defaultValue */
				                 actualValue, rangeSelector);
				if (!res.IsSuccessful())
				{
					clires = CliLogger::GetInstance().GetFailureErrorString(res);

					LOG_WARN() << CliLogger::GetInstance().GetErrorString(clires);
				}
				else
				{
					if ((!pResult.parameters[row].at(9).empty()) || (!pResult.parameters[row].at(10).empty()))
					{
						CliResult oiclires;
						std::uint32_t objIndex = 0;

						oiclires = GetNewObjectIndex(nodeId, interfaceId, modId,
						                             modPosition, objId, objIndex);
						if (!oiclires.IsSuccessful())
						{
							LOG_WARN() << CliLogger::GetInstance().GetErrorString(oiclires);
						}
						else
						{
							res = OpenConfiguratorCore::GetInstance().SetObjectLimits(
							          OpenConfiguratorCli::GetInstance().networkName,
							          nodeId,
							          objIndex,
							          pResult.parameters[row].at(9),				/** lowLimit */
							          pResult.parameters[row].at(10));			/** highLimit */
							if (!res.IsSuccessful())
							{
								clires = CliLogger::GetInstance().GetFailureErrorString(res);

								LOG_WARN() << CliLogger::GetInstance().GetErrorString(clires);
							}
						}
					}
				}
			}

			ParserResult subpResult;

			CliResult res = subpResult.CreateResult(element, kSubObjectXpathExpression,
			                                        kFormatStrSubObjectXpathExpression,
			                                        pResult.node.at(row));
			if (!res.IsSuccessful())
			{
				LOG_WARN() << CliLogger::GetInstance().GetErrorString(res);
			}
			else
			{
				for (std::uint32_t subrow = 0; subrow < subpResult.parameters.size(); subrow++)
				{
					std::uint8_t subObjId = 0;
					std::uint8_t subObjIdType = 0;

					if (!subpResult.parameters[subrow].at(0).empty())
					{
						subObjId = (std::uint8_t)std::stol(subpResult.parameters[subrow].at(0), NULL, 16);
					}
					if (!subpResult.parameters[subrow].at(2).empty())
					{
						subObjIdType = (std::uint8_t)std::stoi(subpResult.parameters[subrow].at(2));
					}
					std::string subObjaccessType = subpResult.parameters[subrow].at(5);		/** Access type */
					std::string subObjActualValue = subpResult.parameters[subrow].at(7);	/** Actual value */
					std::string subObjuniqueIdRef = subpResult.parameters[subrow].at(8);	/** UniqueID ref */
					std::string subObjdataType = subpResult.parameters[subrow].at(4);		/** Data type */

					if ((subObjaccessType.compare("ro") == 0) ||
					        (subObjaccessType.compare("const") == 0))
					{
						subObjActualValue = "";
					}

					if (subpResult.parameters[subrow].at(8).empty())						/** Is uniqueIdRef empty */
					{
						Result result = OpenConfiguratorCore::GetInstance().CreateModuleSubObject(
						                    OpenConfiguratorCli::GetInstance().networkName,
						                    nodeId, interfaceId,
						                    modId, modPosition, objId, subObjId,
						                    GetObjectType(subObjIdType),
						                    subpResult.parameters[subrow].at(1),						/** sub object name */
						                    GetPlkDataType(subpResult.parameters[subrow].at(4)), 		/** sub object dataType */
						                    GetObjAccessType(subObjaccessType),
						                    GetPdoMapping(subpResult.parameters[subrow].at(6)),			/** sub object PDOmapping */
						                    subpResult.parameters[subrow].at(3),						/** sub object defaultValue */
						                    subObjActualValue);
						if (!result.IsSuccessful())
						{
							clires = CliLogger::GetInstance().GetFailureErrorString(result);

							LOG_WARN() << CliLogger::GetInstance().GetErrorString(clires);
						}
						else
						{
							/** If any one of the sub object lowLimit and highLimit has values */
							if ((!subpResult.parameters[subrow].at(9).empty()) ||
							        (!subpResult.parameters[subrow].at(10).empty()))
							{
								CliResult oiclires;
								std::uint32_t objIndex = 0;
								std::uint32_t subObjIndex = 0;

								oiclires = GetNewObjectIndex(nodeId, interfaceId, modId,
								                             modPosition, objId, objIndex);
								if (!oiclires.IsSuccessful())
								{
									LOG_WARN() << CliLogger::GetInstance().GetErrorString(oiclires);
								}
								else
								{
									CliResult soiclires = GetNewSubObjectIndex(nodeId, interfaceId,
									                      modId, modPosition,
									                      subObjId, subObjIndex);
									if (!soiclires.IsSuccessful())
									{
										LOG_WARN() << CliLogger::GetInstance().GetErrorString(soiclires);
									}
									else
									{
										result = OpenConfiguratorCore::GetInstance().SetSubObjectLimits(
										             OpenConfiguratorCli::GetInstance().networkName,
										             nodeId,
										             objIndex,
										             (std::uint8_t)subObjIndex,
										             subpResult.parameters[subrow].at(9),			/** sub object lowLimit */
										             subpResult.parameters[subrow].at(10));			/** sub object highLimit */
										if (!result.IsSuccessful())
										{
											clires = CliLogger::GetInstance().GetFailureErrorString(result);

											LOG_WARN() << CliLogger::GetInstance().GetErrorString(clires);
										}
									}
								}
							}
						}
					}
					else if ((!subObjuniqueIdRef.empty()) && (!subObjdataType.empty()))
					{
						/** Both sub object uniqueIdRef and dataType has values */
						Result result = OpenConfiguratorCore::GetInstance().CreateModuleParameterSubObject(
						                    OpenConfiguratorCli::GetInstance().networkName,
						                    nodeId, interfaceId,
						                    modId, modPosition, objId, subObjId,
						                    GetObjectType(subObjIdType),
						                    subpResult.parameters[subrow].at(1),					/** sub object name */
						                    GetPlkDataType(subObjdataType),
						                    GetObjAccessType(subObjaccessType),
						                    GetPdoMapping(subpResult.parameters[subrow].at(6)),		/** sub object PDOmapping */
						                    subObjuniqueIdRef,
						                    subpResult.parameters[subrow].at(3),					/** sub object defaultValue */
						                    subObjActualValue);
						if (!result.IsSuccessful())
						{
							clires = CliLogger::GetInstance().GetFailureErrorString(result);

							LOG_WARN() << CliLogger::GetInstance().GetErrorString(clires);
						}
					}
					else if ((!subObjuniqueIdRef.empty()) && (subObjdataType.empty()))
					{
						/** Both sub object uniqueIdRef and dataType has values */
						Result result = OpenConfiguratorCore::GetInstance().CreateModuleParameterSubObject(
						                    OpenConfiguratorCli::GetInstance().networkName,
						                    nodeId, interfaceId,
						                    modId, modPosition, objId, subObjId, GetObjectType(subObjIdType),
						                    subpResult.parameters[subrow].at(1),					/** sub object name */
						                    PlkDataType::UNDEFINED,
						                    GetObjAccessType(subObjaccessType),
						                    GetPdoMapping(subpResult.parameters[subrow].at(6)),		/** sub object PDOmapping */
						                    subObjuniqueIdRef,
						                    subpResult.parameters[subrow].at(3),					/** sub object defaultValue */
						                    subObjActualValue);
						if (!result.IsSuccessful())
						{
							clires = CliLogger::GetInstance().GetFailureErrorString(result);

							LOG_WARN() << CliLogger::GetInstance().GetErrorString(clires);
						}
					}

					CliResult result = UpdateForcedObjects(forcedObject, forcedSubObject, nodeId,
					                                       pResult.parameters[row].at(0), subpResult.parameters[subrow].at(0),
					                                       objId, subObjId, actualValue, subObjActualValue, true,
					                                       interfaceId, modId, modPosition);
					if (!result.IsSuccessful())
					{
						LOG_WARN() << CliLogger::GetInstance().GetErrorString(result);
					}
				}
			}
		}
	}
	catch (const std::exception& e)
	{
		return CliLogger::GetInstance().GetFailureErrorString(e);
	}

	return CliResult();
}

CliResult ProjectParser::GetNewObjectIndex(const std::uint8_t nodeId,
        const std::string& interfaceId,
        const std::string& modId,
        const std::uint32_t modposition,
        const std::uint32_t objId,
        std::uint32_t& index)
{
	std::int32_t subIndex = 0;

	Result res = OpenConfiguratorCore::GetInstance().GetModuleObjectCurrentIndex(
	                 OpenConfiguratorCli::GetInstance().networkName,
	                 nodeId, interfaceId, modId,
	                 modposition, objId, -1, index, subIndex);
	if (!res.IsSuccessful())
	{
		index = 0;

		return CliLogger::GetInstance().GetFailureErrorString(res);
	}

	return CliResult();
}

CliResult ProjectParser::GetNewSubObjectIndex(const std::uint8_t nodeId,
        const std::string& interfaceId,
        const std::string& modId,
        const std::uint32_t modPosition,
        const std::uint8_t subObjId,
        std::uint32_t& subIndex)
{
	std::uint32_t index = 0;
	std::int32_t subObjIndex = 0;

	Result res = OpenConfiguratorCore::GetInstance().GetModuleObjectCurrentIndex(
	                 OpenConfiguratorCli::GetInstance().networkName,
	                 nodeId, interfaceId, modId,
	                 modPosition, 0, subObjId, index, subObjIndex);
	if (!res.IsSuccessful())
	{
		subIndex = 0;

		return CliLogger::GetInstance().GetFailureErrorString(res);
	}

	subIndex = (uint32_t)subObjIndex;

	return CliResult();
}

CliResult ProjectParser::GetNewParameterId(const std::uint8_t nodeId,
        const std::string& interfaceId,
        const std::string& modId,
        const std::uint32_t modPosition,
        const std::string& uniqueID,
        std::string& paramId)
{
	Result res = OpenConfiguratorCore::GetInstance().GetModuleParameterCurrentName(
	                 OpenConfiguratorCli::GetInstance().networkName,
	                 nodeId, interfaceId, modId,
	                 modPosition, uniqueID, paramId);
	if (!res.IsSuccessful())
	{
		paramId = "";

		return CliLogger::GetInstance().GetFailureErrorString(res);
	}

	return CliResult();
}

CliResult ProjectParser::CreateForcedObjects(const ParserElement& pElement,
        const std::string& xPathExpression,
        std::vector<std::string>& forcedObj,
        std::vector<std::string>& forcedSubObj,
        const xercesc::DOMNode* parentNode)
{
	ParserResult pResult;
	CliResult cliRes;

	cliRes = pResult.CreateResult(pElement, xPathExpression,
	                              kFormatStrxPathExpressionObject,
	                              parentNode);
	if (!cliRes.IsSuccessful())
	{
		return CliResult();
	}
	else
	{
		for (std::uint32_t row = 0; row < pResult.parameters.size(); row++)
		{
			/** Updates the vector based on existing object */
			std::string index = pResult.parameters[row].at(0);
			forcedObj.push_back(index);

			/** Updates the vector based on existing sub-object */
			std::string subIndex = pResult.parameters[row].at(1);
			forcedSubObj.push_back(subIndex);
		}
	}

	return CliResult();
}

CliResult ProjectParser::UpdateForcedObjects(const std::vector<std::string>& forcedObject,
        const std::vector<std::string>& forcedSubObject,
        const std::uint8_t nodeId,
        const std::string& objectIndex,
        const std::string& SubobjectIndex,
        const std::uint32_t objectId,
        const std::uint8_t subObjectId,
        const std::string& objectValue,
        const std::string& subobjectValue,
        const bool value,
        const std::string& interfaceId,
        const std::string& modId,
        const std::uint32_t modPosition)
{
	if (!forcedObject.empty())
	{
		for (std::string subObjectIdOfVector : forcedSubObject)
		{
			if (subObjectIdOfVector.compare(SubobjectIndex) == 0)
			{
				for (std::string objectIdOfVector : forcedObject)
				{
					if (objectIdOfVector.compare(objectIndex) == 0)
					{
						std::uint32_t objectIdOutput = 0;
						std::uint32_t subObjectIdOutput = 0;
						Result res;

						if (value)
						{
							CliResult oiclires;

							oiclires = GetNewObjectIndex(nodeId, interfaceId, modId,
							                             modPosition, objectId,
							                             objectIdOutput);
							if (!oiclires.IsSuccessful())
							{
								LOG_WARN() << CliLogger::GetInstance().GetErrorString(oiclires);
							}
							else
							{
								CliResult soiclires;

								soiclires = GetNewSubObjectIndex(nodeId,
								                                 interfaceId,
								                                 modId, modPosition,
								                                 subObjectId,
								                                 subObjectIdOutput);
								if (!soiclires.IsSuccessful())
								{
									LOG_WARN() << CliLogger::GetInstance().GetErrorString(soiclires);
								}
								else
								{
									res = OpenConfiguratorCore::GetInstance().SetSubObjectActualValue(
									          OpenConfiguratorCli::GetInstance().networkName,
									          nodeId, objectIdOutput,
									          (std::uint8_t)subObjectIdOutput,
									          subobjectValue, true, false);
									if (!res.IsSuccessful())
									{
										return CliLogger::GetInstance().GetFailureErrorString(res);
									}
								}
							}
						}
						else
						{
							res = OpenConfiguratorCore::GetInstance().SetSubObjectActualValue(
							          OpenConfiguratorCli::GetInstance().networkName,
							          nodeId, objectId, subObjectId,
							          subobjectValue, true, false);
							if (!res.IsSuccessful())
							{
								return CliLogger::GetInstance().GetFailureErrorString(res);
							}
						}
					}
				}
			}
			else
			{
				if (forcedSubObject.empty())
				{
					for (std::string object : forcedObject)
					{
						if (objectIndex.compare(object) == 0)
						{
							std::uint32_t objectIdOutput = 0;
							Result res;

							if (value)
							{
								CliResult oiclires;

								oiclires = GetNewObjectIndex(nodeId, interfaceId, modId,
								                             modPosition, objectId,
								                             objectIdOutput);
								if (!oiclires.IsSuccessful())
								{
									LOG_WARN() << CliLogger::GetInstance().GetErrorString(oiclires);
								}
								else
								{
									res = OpenConfiguratorCore::GetInstance().SetObjectActualValue(
									          OpenConfiguratorCli::GetInstance().networkName,
									          nodeId, objectIdOutput, objectValue, true, false);
									if (!res.IsSuccessful())
									{
										return CliLogger::GetInstance().GetFailureErrorString(res);
									}
								}
							}
							else
							{
								res = OpenConfiguratorCore::GetInstance().SetObjectActualValue(
								          OpenConfiguratorCli::GetInstance().networkName,
								          nodeId, objectId, objectValue, true, false);
								if (!res.IsSuccessful())
								{
									return CliLogger::GetInstance().GetFailureErrorString(res);
								}
							}
						}
					}
				}
			}
		}
	}

	return CliResult();
}

IEC_Datatype ProjectParser::GetDataType(const std::string& dataType)
{
	if (iecdataTypeMap.find(dataType) != iecdataTypeMap.end())
		return iecdataTypeMap.at(dataType);
	return IEC_Datatype::UNDEFINED;
}

ParameterAccess ProjectParser::GetParameterAccess(const std::string& access)
{
	if (parameterAccessMap.find(access) != parameterAccessMap.end())
		return parameterAccessMap.at(access);
	return ParameterAccess::undefined;
}

ObjectType ProjectParser::GetObjectType(const std::uint8_t objType)
{
	return ObjectType(objType);
}

PlkDataType ProjectParser::GetPlkDataType(const std::string& plkDataType)
{
	if (plkDataType.empty())
		return PlkDataType::UNDEFINED;
	return (PlkDataType)std::stoi(plkDataType, NULL, 16);
}

AccessType ProjectParser::GetObjAccessType(const std::string& accessType)
{
	if (accessTypeMap.find(accessType) != accessTypeMap.end())
		return accessTypeMap.at(accessType);
	return AccessType::UNDEFINED;
}

PDOMapping ProjectParser::GetPdoMapping(const std::string& pdoMapp)
{
	return pdoMappingMap.at(pdoMapp);
}

DynamicChannelAccessType ProjectParser::GetDynamicChannelAccessType(const std::string& accessType)
{
	if (accessType.compare("readOnly") == 0)
	{
		return DynamicChannelAccessType::readOnly;
	}
	else if (accessType.compare("writeOnly") == 0)
	{
		return DynamicChannelAccessType::writeOnly;
	}
	else if (accessType.compare("readWriteOutput") == 0)
	{
		return DynamicChannelAccessType::readWriteOutput;
	}

	return DynamicChannelAccessType::UNDEFINED;
}

ModuleAddressing ProjectParser::GetModuleAddressing(const std::string& moduleAddressing)
{
	if (moduleAddressing.compare("manual") == 0)
	{
		return ModuleAddressing::MANUAL;
	}
	else if (moduleAddressing.compare("position") == 0)
	{
		return ModuleAddressing::POSITION;
	}
	else if (moduleAddressing.compare("next") == 0)
	{
		return ModuleAddressing::NEXT;
	}

	return ModuleAddressing::NEXT;
}

SortNumber ProjectParser::GetSortNumber(const std::string& sortNumber)
{
	if (sortNumber.compare("continuous") == 0)
	{
		return SortNumber::CONTINUOUS;
	}
	else if (sortNumber.compare("address") == 0)
	{
		return SortNumber::ADDRESS;
	}

	return SortNumber::CONTINUOUS;
}

SortMode ProjectParser::GetSortMode(const std::string& sortMode)
{
	if (sortMode.compare("index") == 0)
	{
		return SortMode::INDEX;
	}
	else if (sortMode.compare("subindex") == 0)
	{
		return SortMode::SUBINDEX;
	}

	return SortMode::INDEX;
}
