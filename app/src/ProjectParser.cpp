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

ProjectParser::ProjectParser()
{
}

ProjectParser::~ProjectParser()
{
}

ProjectParser& ProjectParser::GetInstance()
{
	static ProjectParser instance;

	return instance;
}

CliResult ProjectParser::ParseXmlFile(const std::string xmlPath)
{
	/** Core Library API call to create Network */
	Result res = OpenConfiguratorCore::GetInstance().CreateNetwork(
					OpenConfiguratorCli::GetInstance().networkName);

	if (!res.IsSuccessful())
	{
		return CliLogger::GetInstance().HandleCoreApiFailed("Create Network", res);
	}
	else
	{
		/** Create results for MN, RMN abd CN nodes */
		CliResult clires;

		clires = CreateMnNodeResults(xmlPath);
		if (!clires.IsSuccessful())
		{
			return clires;
		}

		clires = CreateRmnNodeResults(xmlPath);
		if (!clires.IsSuccessful())
		{
			return clires;
		}

		clires = CreateCnNodeResults(xmlPath);
		if (!clires.IsSuccessful())
		{
			return clires;
		}
	}

	return CliResult();
}

CliResult ProjectParser::CreateMnNodeResults(const std::string xmlPath)
{
	try
	{
		ParserElement xmlParserElement;
		ParserResult mnResult;

		CliResult ceres = xmlParserElement.CreateElement(xmlPath);
		if (!ceres.IsSuccessful())
		{
			return ceres;
		}
		else
		{
			CliResult crres;

			crres = mnResult.CreateResult(xmlParserElement, kMnXpathExpression,
											kFormatStrMnXpathExpression);
			if (!crres.IsSuccessful())
			{
				return crres;
			}
			else
			{
				for (std::uint32_t row = 0; row < mnResult.parameters.size(); row++)
				{
					std::uint8_t nodeId = 0;

					if (!mnResult.parameters[row].at(0).empty())
					{
						nodeId = (std::uint8_t)std::stoi(mnResult.parameters[row].at(0).c_str());
					}

					/** Core Library API call to create Node */
					Result res = OpenConfiguratorCore::GetInstance().CreateNode(
										OpenConfiguratorCli::GetInstance().networkName,
										nodeId, 
										mnResult.parameters[row].at(2));	/** name */
					if (!res.IsSuccessful())
					{
						return CliLogger::GetInstance().HandleCoreApiFailed("Create MN Node", res);
					}

					CliResult clires;
					std::vector<std::string> forcedModularNodeObj;		/** Group of forced objects in Node */
					std::vector<std::string> forcedModularNodeSubObj;	/** Group of forced sub objects in Node */

					/** Update the forced objects of node */
					clires = CreateForcedObjects(xmlParserElement, 
													kForcedObjectNodeXpathExpression, 
													nodeId, 
													forcedModularNodeObj, 
													forcedModularNodeSubObj);
					if (!clires.IsSuccessful())
					{
						CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, clires);
					}

					/** Core Library API calls to import the XDD of node */
					clires = UpdateNodeIdCollection(nodeId,
										mnResult.parameters[row].at(1),	/** pathToXDC */
										forcedModularNodeObj, 
										forcedModularNodeSubObj,
										"", "", 0U);
					if (!clires.IsSuccessful())
					{
						return CliLogger::GetInstance().HandleCliApiFailed("Update MN Node Id Collection", clires);
					}

					/** Core Library API calls to update the node assignment of node */
					clires = CreateNodeAssignment(xmlParserElement, kMnXpathExpression, nodeId);
					if (!clires.IsSuccessful())
					{
						CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, clires);
					}
				}
			}
		}
	}
	catch (std::exception& e)
	{
		return CliLogger::GetInstance().HandleExceptionCaught("Parse MN Node", e);
	}

	return CliResult();
}

CliResult ProjectParser::CreateRmnNodeResults(const std::string xmlPath)
{
	try
	{
		ParserElement xmlParserElement;
		ParserResult rmnResult;

		CliResult ceres = xmlParserElement.CreateElement(xmlPath);
		if (!ceres.IsSuccessful())
		{
			return ceres;
		}
		else
		{
			CliResult crres;

			crres = rmnResult.CreateResult(xmlParserElement, kRmnXpathExpression,
											kFormatStrRmnXpathExpression);
			if (!crres.IsSuccessful())
			{
				CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, crres);
			}
			else
			{
				for (std::uint32_t row = 0; row < rmnResult.parameters.size(); row++)
				{
					std::uint8_t nodeId = 0;

					if (!rmnResult.parameters[row].at(0).empty())
					{
						nodeId = (std::uint8_t)std::stoi(rmnResult.parameters[row].at(0).c_str());
					}

					/** Core Library API call to create Node */
					Result res = OpenConfiguratorCore::GetInstance().CreateNode(
										OpenConfiguratorCli::GetInstance().networkName,
										nodeId,
										rmnResult.parameters[row].at(2),		/** name */
										true);
					if (!res.IsSuccessful())
					{
						CliResult CliResult;

						CliResult = CliLogger::GetInstance().HandleCoreApiFailed("Create RMN Node", res);

						CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, CliResult);
					}

					CliResult clires;
					std::vector<std::string> forcedModularNodeObj;		/** Group of forced objects in Node */
					std::vector<std::string> forcedModularNodeSubObj;	/** Group of forced sub objects in Node */

					/** Core Library API calls to import the XDC of node */
					clires = UpdateNodeIdCollection(nodeId,
													rmnResult.parameters[row].at(1),	/** pathToXDC */
													forcedModularNodeObj, 
													forcedModularNodeSubObj, 
													"", "", 0U);
					if (!clires.IsSuccessful())
					{
						CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, clires);
					}

					/** Core Library API calls to update the node assignment of node */
					clires = CreateNodeAssignment(xmlParserElement, 
													kRmnXpathExpression, 
													nodeId);
					if (!clires.IsSuccessful())
					{
						CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, clires);
					}
				}
			}
		}
	}
	catch (std::exception& ex)
	{
		return CliLogger::GetInstance().HandleExceptionCaught("Parse RMN Node", ex);
	}

	return CliResult();
}

CliResult ProjectParser::CreateCnNodeResults(const std::string xmlPath)
{
	try
	{
		ParserElement xmlParserElement;
		ParserResult cnResult;

		CliResult ceres = xmlParserElement.CreateElement(xmlPath);
		if (!ceres.IsSuccessful())
		{
			return ceres;
		}
		else
		{
			CliResult crres;

			crres = cnResult.CreateResult(xmlParserElement, kCnXpathExpression,
											kFormatStrCnXpathExpression);
			if (!crres.IsSuccessful())
			{
				return crres;
			}
			else
			{
				for (std::int32_t row = 0; row < cnResult.parameters.size(); row++)
				{
					std::uint8_t nodeId = 0;

					if (!cnResult.parameters[row].at(0).empty())
					{
						nodeId = (std::uint8_t)std::stoi(cnResult.parameters[row].at(0).c_str());
					}

					ParserResult subCnResult;
					CliResult clires;
					CliResult subclires;

					clires = subCnResult.CreateResult(xmlParserElement, 
														kIntrfcXpathExpression, 
														kFormatStrIntrfcXpathExpression);
					if (!clires.IsSuccessful())
					{
						CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, clires);

						/** Core Library API call to create Node */
						Result res = OpenConfiguratorCore::GetInstance().CreateNode(
									OpenConfiguratorCli::GetInstance().networkName,
									nodeId, 
									cnResult.parameters[row].at(2),		/** name */
									false);
						if (!res.IsSuccessful())
						{
							subclires = CliLogger::GetInstance().HandleCoreApiFailed("Create CN Node", res);

							CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, subclires);
						}

						std::vector<std::string> forcedModularNodeObj;		/** Group of forced objects in Node */
						std::vector<std::string> forcedModularNodeSubObj;	/** Group of forced sub objects in Node */
					
						/** Update the forced objects of modular head node */
						subclires = CreateForcedObjects(xmlParserElement, kForcedObjectNodeXpathExpression, 
														nodeId, forcedModularNodeObj, forcedModularNodeSubObj);
						if (!subclires.IsSuccessful())
						{
							CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, subclires);
						}
						
						/** Core Library API calls to import the XDC of node */
						subclires = UpdateNodeIdCollection(nodeId, 
															cnResult.parameters[row].at(1),				/** pathToXDC */
															forcedModularNodeObj, forcedModularNodeSubObj,
															"", "", 0U);
						if (!subclires.IsSuccessful())
						{
							CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, subclires);
						}
						
						/** Core Library API calls to update the node assignment of node */
						subclires = CreateNodeAssignment(xmlParserElement, kCnXpathExpression, nodeId);
						if (!subclires.IsSuccessful())
						{
							CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, subclires);
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
							subclires = CliLogger::GetInstance().HandleCoreApiFailed("Create CN Node", res);

							CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, subclires);
						}

						std::vector<std::string> forcedModularNodeObj;		/** Group of forced objects in Node */
						std::vector<std::string> forcedModularNodeSubObj;	/** Group of forced sub objects in Node */

						/** Update the forced objects of modular head node */
						subclires = CreateForcedObjects(xmlParserElement, kForcedObjectNodeXpathExpression, 
															nodeId, forcedModularNodeObj, forcedModularNodeSubObj);
						if (!subclires.IsSuccessful())
						{
							CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, subclires);
						}

						/** Core Library API calls to import the XDC of modular head node */
						subclires = UpdateNodeIdCollection(nodeId, cnResult.parameters[row].at(1),	/** pathToXDC */
															forcedModularNodeObj, forcedModularNodeSubObj, 
															"", "", 0U);	
						if (!subclires.IsSuccessful())
						{
							CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, subclires);
						}

						/** Core Library API calls to update the node assignment of modular head node */
						subclires = CreateNodeAssignment(xmlParserElement, kCnXpathExpression, nodeId);
						if (!subclires.IsSuccessful())
						{
							CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, subclires);
						}

						/** Core Library API calls to create interface of modular head node */
						subclires = CreateInterface(nodeId, cnResult.parameters[row].at(1));	/** pathToXDC */
						if (!subclires.IsSuccessful())
						{
							CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, subclires);
						}
						else
						{
							ParserResult moduleResult;
							CliResult clicrres;

							for(std::uint32_t subRow = 0; subRow < subCnResult.parameters.size(); subRow++)
							{
								clicrres = moduleResult.CreateResult(xmlParserElement, 
																				kModuleXpathExpression,
																				kFormatStrModuleXpathExpression);
								if (!clicrres.IsSuccessful())
								{
									CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, clicrres);
								}
								else
								{
									CliResult foRes;
									std::vector<std::string> forcedModularNodeObj;		/** Group of forced objects in Node */
									std::vector<std::string> forcedModularNodeSubObj;	/** Group of forced sub objects in Node */

									for(std::uint32_t moduleRow = 0; moduleRow < moduleResult.parameters.size(); moduleRow++)
									{
										/** Update the forced objects of modular node */
										foRes = CreateForcedObjects(xmlParserElement, kForcedObjectModuleXpathExpression,
																	nodeId, forcedModularNodeObj, forcedModularNodeSubObj);
										if (!foRes.IsSuccessful())
										{
											CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, foRes);
										}

										foRes = CreateModule(nodeId,
															forcedModularNodeObj, forcedModularNodeSubObj,
															subCnResult.parameters[subRow].at(0),				/** Interface Unique ID */
															moduleResult.parameters[moduleRow].at(0),			/** Module name */
															moduleResult.parameters[moduleRow].at(1),			/** Module position */
															moduleResult.parameters[moduleRow].at(2),			/** Module address */
															moduleResult.parameters[moduleRow].at(3),			/** Module pathtoXDC */
															moduleResult.parameters[moduleRow].at(4));			/** Module enabled */
										if (!foRes.IsSuccessful())
										{
											CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, foRes);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	catch (std::exception& ex)
	{
		return CliLogger::GetInstance().HandleExceptionCaught("Parse CN Node", ex);
	}

	return CliResult();
}

IEC_Datatype ProjectParser::GetIecDataType(const xercesc::DOMNode* node)
{
	IEC_Datatype data = IEC_Datatype::UNDEFINED;

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

	return data;
}

CliResult ProjectParser::CreateStructDataType(const ParserElement& element, 
											  const std::uint8_t nodeId, 
											  const std::string interfaceId, 
											  const std::string modId, 
											  const std::uint32_t modPosition)
{
	ParserResult pResult;

	CliResult crres = pResult.CreateResult(element, kStructXpathExpression,
											kFormatStrStructXpathExpression);
	if (!crres.IsSuccessful())
	{
		return crres;
	}
	else
	{
		for (std::int32_t row = 0; row < pResult.parameters.size(); row++)
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
				return CliLogger::GetInstance().HandleCoreApiFailed("Create Struct Datatype", res);
			}

			ParserResult varDecResult;

			CliResult subcrres = varDecResult.CreateResult(element, kStructVarXpathExpression,
										kFormatStrStructVarXpathExpression,
										pResult.node.at(row));
			if (!subcrres.IsSuccessful())
			{
				CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, subcrres);
			}
			else
			{
				for (std::int32_t subrow = 0; subrow < varDecResult.parameters.size(); subrow++)
				{
					IEC_Datatype data = GetIecDataType(varDecResult.node.at(subrow));
					/** Set default value to size if value is empty */
					std::uint32_t varDeclSize = 1U;

					if (!varDecResult.parameters[subrow].at(2).empty())
					{
						varDeclSize = (std::uint32_t)std::stoi(varDecResult.parameters[subrow].at(2).c_str());
					}

					/** Core Library API call to create VarDeclaration */
					Result res = OpenConfiguratorCore::GetInstance().CreateVarDeclaration(
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
						return CliLogger::GetInstance().HandleCoreApiFailed("Create Var Declaration", res);
					}
				}
			}
		}
	}

	return CliResult();
}

CliResult ProjectParser::CreateArrayDataType(const ParserElement& element, 
											 const std::uint8_t nodeId, 
											 const std::string interfaceId, 
											 const std::string modId, 
											 const std::uint32_t modPosition)
{
	ParserResult pResult;

	CliResult crres = pResult.CreateResult(element, kArrayXpathExpression,
											kFormatStrArrayXpathExpression);
	if (!crres.IsSuccessful())
	{
		return crres;
	}
	else
	{
		for (std::int32_t row = 0; row < pResult.parameters.size(); row++)
		{
			ParserResult subpResult;

			IEC_Datatype data = GetIecDataType(pResult.node.at(row));

			CliResult subcrres = subpResult.CreateResult(element, kArraySubRangeXpathExpression,
										kFormatStrArraySubRangeXpathExpression,
										pResult.node.at(row));
			if (!subcrres.IsSuccessful())
			{
				CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, subcrres);
			}
			else
			{
				for (std::int32_t subrow = 0; subrow < subpResult.parameters.size(); subrow++)
				{
					std::uint32_t lowerLimit = 1U;
					std::uint32_t upperLimit = 1U;

					if (!subpResult.parameters[subrow].at(0).empty())
					{
						lowerLimit = (std::uint32_t)std::stoi(subpResult.parameters[subrow].at(0).c_str());
					}

					if (!subpResult.parameters[subrow].at(1).empty())
					{
						upperLimit = (std::uint32_t)std::stoi(subpResult.parameters[subrow].at(1).c_str());
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
						return CliLogger::GetInstance().HandleCoreApiFailed("Create Array Datatype", res);;
					}
				}
			}
		}
	}

	return CliResult();
}

CliResult ProjectParser::CreateDataType(const ParserElement& element, 
										const std::uint8_t nodeId, 
										const std::string interfaceId,
										const std::string modId, 
										const std::uint32_t modposition)
{
	CliResult res;

	/** Creates struct data type of node into the core library */
	res = CreateStructDataType(element, nodeId, interfaceId, modId, modposition);
	if (!res.IsSuccessful())
	{
		CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, res);
	}

	/** Creates array data type of node into the core library */
	res = CreateArrayDataType(element, nodeId, interfaceId, modId, modposition);
	if (!res.IsSuccessful())
	{
		CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, res);
	}

	return CliResult();
}

CliResult ProjectParser::SetParamAllowedValue(const ParserElement & element,
												const std::uint8_t nodeId,
												const xercesc::DOMNode* node,
										 		const std::string uniqueId,
										 		const std::string interfaceId, 
										 		const std::string modId, 
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
	else
	{
		std::vector<std::string> allowedValuelist;
		std::string uniqueIdOutput;

		for (std::int32_t row = 0; row < pResult.parameters.size(); row++)
		{
			allowedValuelist.push_back(pResult.parameters[row].at(0));
		}

		if (!modId.empty())
		{
			uniqueIdOutput = ProjectParser::GetInstance().GetNewParameterId(nodeId, 
															interfaceId, modId, 
															modPosition, uniqueId);
		}

		/** Core Library API call to set allowed parameter values */
		Result res = OpenConfiguratorCore::GetInstance().SetParameterAllowedValues(
									OpenConfiguratorCli::GetInstance().networkName,
									nodeId,
									uniqueIdOutput,
									allowedValuelist);
		if (!res.IsSuccessful())
		{
			return CliLogger::GetInstance().HandleCoreApiFailed("Set Parameter Allowed Values", res);;
		}
	}

	return CliResult();
}

CliResult ProjectParser::SetParamAllowedRange(const ParserElement& element,
												const std::uint8_t nodeId,
												const xercesc::DOMNode* node,
												const std::string uniqueId,
												const std::string interfaceId, 
												const std::string modId, 
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
	else
	{
		for (std::int32_t row = 0; row < pResult.parameters.size(); row++)
		{
			ParserResult minpResult;
			ParserResult maxpResult;
			std::string minValue = "";
			std::string maxValue = "";
			CliResult subcrres;
			std::string uniqueIdOutput;

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
				uniqueIdOutput = ProjectParser::GetInstance().GetNewParameterId(nodeId, 
																	interfaceId, modId, 
																	modPosition, uniqueId);
			}

			/** Core Library API call to set allowed parameter values */
			Result res = OpenConfiguratorCore::GetInstance().SetParameterAllowedRange(
					OpenConfiguratorCli::GetInstance().networkName,
					nodeId, uniqueIdOutput, minValue, maxValue);
			if (!res.IsSuccessful())
			{
				return CliLogger::GetInstance().HandleCoreApiFailed("Set Parameter Allowed Range", res);
			}
		}
	}

	return CliResult();
}

CliResult ProjectParser::SetParamActualValue(const ParserElement& element,
												const std::uint8_t nodeId,
												const xercesc::DOMNode* node,
												const std::string uniqueId,
												const std::string interfaceId, 
												const std::string modId, 
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
	else
	{
		std::string uniqueIdOutput;

		if (!modId.empty())
		{
			uniqueIdOutput = ProjectParser::GetInstance().GetNewParameterId(nodeId, 
																interfaceId, modId, 
																modPosition, uniqueId);
		}

		/** Core Library API call to set actual parameter values */
		Result res = OpenConfiguratorCore::GetInstance().SetParameterActualValue(
								OpenConfiguratorCli::GetInstance().networkName,
								nodeId, uniqueIdOutput, pResult.parameters[0].at(0));
		if (!res.IsSuccessful())
		{
			return CliLogger::GetInstance().HandleCoreApiFailed("Set Parameter Actual Value", res);
		}
	}

	return CliResult();
}

CliResult ProjectParser::SetParamDefaultValue(const ParserElement& element,
												const std::uint8_t nodeId,
												const xercesc::DOMNode* node,
												const std::string uniqueId,
												const std::string interfaceId, 
												const std::string modId, 
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
	else
	{
		std::string uniqueIdOutput;

		if (!modId.empty())
		{
			uniqueIdOutput = ProjectParser::GetInstance().GetNewParameterId(nodeId, 
																interfaceId, modId, 
																modPosition, uniqueId);
		}

		/** Core Library API call to set actual parameter values */
		Result res = OpenConfiguratorCore::GetInstance().SetParameterDefaultValue(
								OpenConfiguratorCli::GetInstance().networkName,
								nodeId, uniqueIdOutput, pResult.parameters[0].at(0));
		if (!res.IsSuccessful())
		{
			return CliLogger::GetInstance().HandleCoreApiFailed("Set Parameter Default Value", res);
		}
	}

	return CliResult();
}

CliResult ProjectParser::CreateParameterTemplate(const ParserElement& element, 
												 const std::uint8_t nodeId, 
												 const std::string interfaceId, 
												 const std::string modId, 
												 const std::uint32_t modPosition)
{
	ParserResult pResult;
	
	CliResult crres = pResult.CreateResult(element, kParameterTemplateXpathExpression,
								kFormatStrParameterTemplateXpathExpression);
	if (!crres.IsSuccessful())
	{
		CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, crres);
	}
	else
	{
		for (std::int32_t row = 0; row < pResult.parameters.size(); row++)
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
						return CliLogger::GetInstance().HandleCoreApiFailed("Create Parameter", res);
					}
			}
			else
			{
				for (std::int32_t subrow = 0; subrow < subpResult.parameters.size(); subrow++)
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
							return CliLogger::GetInstance().HandleCoreApiFailed("Create parameter", res);
						}
				}
			}
			
			if(!pResult.parameters[row].at(1).empty())							/** Parameter template uniqueID is not empty */
			{
				/** Core Library API call to create Parameter */
				Result res = OpenConfiguratorCore::GetInstance().CreateParameter(
								OpenConfiguratorCli::GetInstance().networkName,
								nodeId, pResult.parameters[row].at(0),
								accessOfParameter,
								pResult.parameters[row].at(1),					/** Parameter template uniqueID */
								interfaceId, modId, modPosition);
				if (!res.IsSuccessful())
				{
					return CliLogger::GetInstance().HandleCoreApiFailed("Create parameter", res);
				}
			
				/** Updates the allowed values of parameter template.*/
				subcrres = SetParamAllowedValue(element, nodeId, pResult.node.at(row), 
												pResult.parameters[row].at(0), interfaceId, 
												modId, modPosition);
				if (!subcrres.IsSuccessful())
				{
					CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, subcrres);
				}

				/** Updates the allowed range of parameter template.*/
				subcrres = SetParamAllowedRange(element, nodeId, pResult.node.at(row), 
												pResult.parameters[row].at(0), interfaceId, 
												modId, modPosition);
				if (!subcrres.IsSuccessful())
				{
					CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, subcrres);
				}

				/** Updates the default value of parameter template.*/
				subcrres = SetParamDefaultValue(element, nodeId, pResult.node.at(row), 
												pResult.parameters[row].at(0), interfaceId, 
												modId, modPosition);
				if (!subcrres.IsSuccessful())
				{
					CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, subcrres);
				}

				/** Updates the actual value of parameter template.*/
				subcrres = SetParamActualValue(element,  nodeId, pResult.node.at(row), 
												pResult.parameters[row].at(0), interfaceId, 
												modId, modPosition);
				if (!subcrres.IsSuccessful())
				{
					CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, subcrres);
				}
			}
		}
	}

	return CliResult();
}

CliResult ProjectParser::CreateChildParameterGroup(const ParserElement& element,
													const std::uint8_t nodeId,
													const xercesc::DOMNode* parameterGroupNode,
													const std::string paramGroupUId, 
													const std::string interfaceId, 
													const std::string modId, 
													const std::uint32_t modPosition)
{
	ParserResult pResult;

	CliResult crres = pResult.CreateResult(element, 
								kChildParameterGroupXpathExpression,
								kFormatStrChildParameterGroupXpathExpression,
								parameterGroupNode);
	if (!crres.IsSuccessful())
	{
		CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, crres);
	}
	else
	{
		for (std::int32_t row = 0; row < pResult.parameters.size(); row++)
		{
			if (pResult.parameters[row].at(1).empty())	/** Is conditionalUniqueIDRef empty */
			{
				std::uint8_t bitoffset = 0;

				if(!pResult.parameters[row].at(3).empty())
				{
					bitoffset = (std::uint8_t)std::stoi(pResult.parameters[row].at(3).c_str());
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
					return CliLogger::GetInstance().HandleCoreApiFailed("Create Parameter Group", res);
				}
			}
			else
			{
				std::uint8_t bitoffset = 0;

				if(!pResult.parameters[row].at(3).empty())
				{
					bitoffset = (std::uint8_t)std::stoi(pResult.parameters[row].at(3).c_str());
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
					return CliLogger::GetInstance().HandleCoreApiFailed("Create Parameter Group", res);
				}
			}

			ParserResult subpResult;

			CliResult subcrres = subpResult.CreateResult(element, 
										kChildParameterRefXpathExpression,
										kFormatStrChildParameterRefXpathExpression,
										pResult.node.at(row));
			if (!subcrres.IsSuccessful())
			{
				CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, subcrres);
			}
			else
			{
				for (std::int32_t subrow = 0; subrow < subpResult.parameters.size(); subrow++)
				{
					if (subpResult.parameters[subrow].at(2).empty())	/** Is actualValue empty */
					{
						std::uint8_t bitoffset = 0;

						if(!subpResult.parameters[subrow].at(3).empty())
						{
							bitoffset = (std::uint8_t)std::stoi(subpResult.parameters[subrow].at(3).c_str());
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
							return CliLogger::GetInstance().HandleCoreApiFailed("Create Parameter References", res);
						}
					}
					else
					{
						std::uint8_t bitoffset = 0;

						if(!subpResult.parameters[subrow].at(3).empty())
						{
							bitoffset = (std::uint8_t)std::stoi(subpResult.parameters[subrow].at(3).c_str());
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
							return CliLogger::GetInstance().HandleCoreApiFailed("Create Parameter References", res);
						}
					}
				}
			}
		}
	}

	return CliResult();
}

CliResult ProjectParser::CreateParameterGroup(const ParserElement& element, 
												const std::uint8_t nodeId, 
												const std::string interfaceId, 
												const std::string modId, 
												const std::uint32_t modPosition)
{
	ParserResult pResult;
	CliResult crres = pResult.CreateResult(element, 
									kParameterGroupXpathExpression,
									kFormatStrParameterGroupXpathExpression);
	if (!crres.IsSuccessful())
	{
		CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, crres);
	}
	else
	{
		for (std::int32_t row = 0; row < pResult.parameters.size(); row++)
		{
			Result res = OpenConfiguratorCore::GetInstance().CreateParameterGroup(
					OpenConfiguratorCli::GetInstance().networkName,
					nodeId,
					pResult.parameters[row].at(0), 				/** uniqueID */
					interfaceId, modId, modPosition);
			if (!res.IsSuccessful())
			{
				return CliLogger::GetInstance().HandleCoreApiFailed("Create Parameter Group", res);
			}

			CliResult subcrres;

			subcrres = CreateChildParameterGroup(element, nodeId, pResult.node.at(row),
												pResult.parameters[row].at(0),interfaceId, 
												modId, modPosition);
			if (!subcrres.IsSuccessful())
			{
				CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, subcrres);
			}

			ParserResult subpResult;

			subcrres = subpResult.CreateResult(element, 
										kChildParameterRefXpathExpression,
										kFormatStrChildParameterRefXpathExpression,
										pResult.node.at(row));
			if (!subcrres.IsSuccessful())
			{
				CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, subcrres);
			}
			else
			{
				for (std::int32_t subrow = 0; subrow < subpResult.parameters.size(); subrow++)
				{
					std::uint8_t bitoffset = 0;

					if(!subpResult.parameters[subrow].at(3).empty())
					{
						bitoffset = (std::uint8_t)std::stoi(subpResult.parameters[subrow].at(3).c_str());
					}

					Result res = OpenConfiguratorCore::GetInstance().CreateParameterReference(
							OpenConfiguratorCli::GetInstance().networkName,
							nodeId,
							pResult.parameters[row].at(0),											/** uniqueID */
							subpResult.parameters[subrow].at(0),									/** uniqueIDRef */
							subpResult.parameters[subrow].at(2),  									/** actualValue */
							bitoffset, 																/** bitOffset */
							interfaceId, modId, modPosition);
					if (!res.IsSuccessful())
					{
						return CliLogger::GetInstance().HandleCoreApiFailed("Create Parameter Reference", res);
					}
				}
			}
		}
	}

	return CliResult();
}

CliResult ProjectParser::CreateParameterList(const ParserElement& element, 
												const std::uint8_t nodeId, 
												const std::string interfaceId, 
												const std::string modId, 
												const std::uint32_t modPosition)
{
	ParserResult pResult;

	CliResult crres = pResult.CreateResult(element, 
											kParameterXpathExpression,
											kFormatStrParameterXpathExpression);
	if (!crres.IsSuccessful())
	{
		CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, crres);
	}
	else
	{
		for (std::int32_t row = 0; row < pResult.parameters.size(); row++)
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
					return CliLogger::GetInstance().HandleCoreApiFailed("Create Parameter", res);
				}
			}
			else
			{
				ParserResult subpResult;

				CliResult subcrres = subpResult.CreateResult(element, kParameterDataTypeIdRefXpathExpression,
											kFormatStrParameterDataTypeIdRefXpathExpression,
											pResult.node.at(row));
				if (!subcrres.IsSuccessful())
				{
					LOG_ERROR() << "DataTypeIdRef not available." << std::endl;
						Result res = OpenConfiguratorCore::GetInstance().CreateParameter(
								OpenConfiguratorCli::GetInstance().networkName,
								nodeId,
								pResult.parameters[row].at(0),		/** UniqueID */
								access,	data,
								false, interfaceId,
								modId, modPosition);
						if (!res.IsSuccessful())
						{
							return CliLogger::GetInstance().HandleCoreApiFailed("Create Parameter", res);
						}
				}
				else
				{
					for (std::int32_t subrow = 0; subrow < subpResult.parameters.size(); subrow++)
						{
							ParameterAccess paramAccess = GetParameterAccess(subpResult.parameters[subrow].at(1));

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
								return CliLogger::GetInstance().HandleCoreApiFailed("Create Parameter", res);
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
				CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, subcrres);
			}

			/** Updates the allowed range of parameter.*/
			subcrres = SetParamAllowedRange(element, nodeId, pResult.node.at(row), 
											pResult.parameters[row].at(0), interfaceId, 
											modId, modPosition);
			if (!subcrres.IsSuccessful())
			{
				CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, subcrres);
			}

			/** Updates the default value of parameter.*/
			subcrres = SetParamDefaultValue(element, nodeId, pResult.node.at(row), 
											pResult.parameters[row].at(0), interfaceId, 
											modId, modPosition);
			if (!subcrres.IsSuccessful())
			{
				CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, subcrres);
			}

			/** Updates the actual value of parameter.*/
			subcrres = SetParamActualValue(element, nodeId, pResult.node.at(row), 
											pResult.parameters[row].at(0), interfaceId, 
											modId, modPosition);
			if (!subcrres.IsSuccessful())
			{
				CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, subcrres);
			}
		}
	}

	return CliResult();
}

CliResult ProjectParser::ImportProfileBodyDevice(const ParserElement& element, 
														  const std::uint8_t nodeId, 
														  const std::string interfaceId, 
														  const std::string modId, 
														  const std::uint32_t modPosition)
{
	CliResult subcrres;

	/** Creates datatype in the core library */
	subcrres = CreateDataType(element, nodeId, interfaceId, modId, modPosition);
	if (!subcrres.IsSuccessful())
	{
		CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, subcrres);
	}

	/** Creates parameter template of node into the core library */
	subcrres = CreateParameterTemplate(element, nodeId, interfaceId, modId, modPosition);
	if (!subcrres.IsSuccessful())
	{
		CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, subcrres);
	}

	/** Creates parameter list of node into the core library */
	subcrres = CreateParameterList(element, nodeId, interfaceId, modId, modPosition);
	if (!subcrres.IsSuccessful())
	{
		CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, subcrres);
	}

	/** Creates parameter Group of node into the core library */
	subcrres = CreateParameterGroup(element, nodeId, interfaceId, modId, modPosition);
	if (!subcrres.IsSuccessful())
	{
		CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, subcrres);
	}

	return CliResult();
}

CliResult ProjectParser::CreateObject(const ParserElement& element, 
										const std::uint8_t nodeId,
										const std::vector<std::string> forcedObject, 
										const std::vector<std::string> forcedSubObject)
{
	ParserResult pResult;

	CliResult crres = pResult.CreateResult(element, kObjectXpathExpression,
											kFormatStrObjectXpathExpression);
	if (!crres.IsSuccessful())
	{
		CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, crres);
	}
	else
	{
		for (std::int32_t row = 0; row < pResult.parameters.size(); row++)
		{
			std::uint32_t objId = 0;
			std::uint8_t objIdType = 0;

			if (!pResult.parameters[row].at(0).empty())
			{
				objId = (std::uint32_t)std::strtol(pResult.parameters[row].at(0).c_str(), NULL, 16);
			}
			if (!pResult.parameters[row].at(2).empty())
			{
				objIdType = (std::uint8_t)std::stoi(pResult.parameters[row].at(2).c_str());
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
						pResult.parameters[row].at(3),                  /** defaultValue */
						actualValue);
				if (!res.IsSuccessful())
				{
					return CliLogger::GetInstance().HandleCoreApiFailed("Create Object", res);
				}
				else
				{
					if ((!pResult.parameters[row].at(9).empty()) || (!pResult.parameters[row].at(10).empty()))
					{
						Result res = OpenConfiguratorCore::GetInstance().SetObjectLimits(
								OpenConfiguratorCli::GetInstance().networkName,
								nodeId, objId,
								pResult.parameters[row].at(9),				/** lowLimit */
								pResult.parameters[row].at(10));			/** highLimit */
						if (!res.IsSuccessful())
						{
							return CliLogger::GetInstance().HandleCoreApiFailed("Set Object Limits", res);
						}
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
						pResult.parameters[row].at(3),                  /** defaultValue */
						actualValue);
				if (!res.IsSuccessful())
				{
					return CliLogger::GetInstance().HandleCoreApiFailed("Create Parameter Object", res);
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
						pResult.parameters[row].at(3),                  /** defaultValue */
						actualValue);
				if (!res.IsSuccessful())
				{
					return CliLogger::GetInstance().HandleCoreApiFailed("Create Parameter Object", res);
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
						pResult.parameters[row].at(3),                  /** defaultValue */
						actualValue);
				if (!res.IsSuccessful())
				{
					return CliLogger::GetInstance().HandleCoreApiFailed("Create Object", res);
				}
				else
				{
					if ((!pResult.parameters[row].at(9).empty()) || (!pResult.parameters[row].at(10).empty()))
					{
						Result res = OpenConfiguratorCore::GetInstance().SetObjectLimits(
								OpenConfiguratorCli::GetInstance().networkName,
								nodeId, objId,
								pResult.parameters[row].at(9),			/** lowLimit */
								pResult.parameters[row].at(10));		/** highLimit */
						if (!res.IsSuccessful())
						{
							return CliLogger::GetInstance().HandleCoreApiFailed("Set Object Limits", res);
						}
					}
				}
			}

			ParserResult subpResult;

			CliResult subcrres = subpResult.CreateResult(element, kSubObjectXpathExpression,
											kFormatStrSubObjectXpathExpression,
											pResult.node.at(row));
			if (!subcrres.IsSuccessful())
			{
				CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, subcrres);
			}
			else
			{
				for (std::int32_t subrow = 0; subrow < subpResult.parameters.size(); subrow++)
				{
					std::uint8_t subObjId = 0;
					std::uint8_t subObjIdType = 0;

					if (!subpResult.parameters[subrow].at(0).empty())
					{
						subObjId = (std::uint8_t)std::strtol(subpResult.parameters[subrow].at(0).c_str(), NULL, 16);
					}
					if (!subpResult.parameters[subrow].at(2).empty())
					{
						subObjIdType = (std::uint8_t)std::stoi(subpResult.parameters[subrow].at(2).c_str());
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
							return CliLogger::GetInstance().HandleCoreApiFailed("Create Sub Object", res);
						}
						else
						{
							/** If any one of the sub object lowLimit and highLimit has values */
							if ((!subpResult.parameters[subrow].at(9).empty()) || (!subpResult.parameters[subrow].at(10).empty()))
							{
								Result res = OpenConfiguratorCore::GetInstance().SetSubObjectLimits(
										OpenConfiguratorCli::GetInstance().networkName,
										nodeId, objId, subObjId,
										subpResult.parameters[subrow].at(9),			/** sub object lowLimit */
										subpResult.parameters[subrow].at(10));			/** sub object highLimit */
								if (!res.IsSuccessful())
								{
									return CliLogger::GetInstance().HandleCoreApiFailed("Create Sub Object Limits", res);
								}
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
							return CliLogger::GetInstance().HandleCoreApiFailed("Create Parameter Sub Object", res);
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
							return CliLogger::GetInstance().HandleCoreApiFailed("Create Parameter Sub Object", res);
						}
					}
					CliResult cliRes = 	UpdateForcedObjects(forcedObject, forcedSubObject, nodeId,
														pResult.parameters[row].at(0), 
														subpResult.parameters[subrow].at(0), 
														objId, subObjId, actualValue, 
														subObjActualValue, false, "", "", 0U);
					if (!cliRes.IsSuccessful())
					{
						CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, cliRes);
					}
				}
			}
		}
	}

	return CliResult();
}


CliResult ProjectParser::ImportProfileBodyCommn(const ParserElement& element, 
												const std::uint8_t nodeId, 
												const std::vector<std::string> forcedObject, 
												const std::vector<std::string> forcedSubObject, 
												const std::string interfaceId, 
												const std::string modId, 
												const std::uint32_t modPosition)
{
	CliResult subcrres;

	/** Creates objest in the core library */
	if(modId.empty())
	{
		subcrres = CreateObject(element, nodeId, forcedObject, 
								forcedSubObject);
		if (!subcrres.IsSuccessful())
		{
			CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, subcrres);
		}
	}
	else
	{
		subcrres = CreateModuleObject(element, nodeId, forcedObject, forcedSubObject, 
										interfaceId, modId, modPosition);
		if (!subcrres.IsSuccessful())
		{
			CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, subcrres);
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
	else
	{
		for (std::int32_t row = 0; row < pResult.parameters.size(); row++)
		{
			for (std::int32_t param = 0; param < pResult.parameters[row].size(); param++)
			{
				Result res = OpenConfiguratorCore::GetInstance().SetFeatureValue(
									OpenConfiguratorCli::GetInstance().networkName,
									nodeId,
									((GeneralFeatureEnum) param),
									pResult.parameters[row].at(param).c_str());
				if (!res.IsSuccessful())
				{
					return CliLogger::GetInstance().HandleCoreApiFailed("Set Feature Value", res);
				}
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
	else
	{
		for (std::int32_t row = 0; row < pResult.parameters.size(); row++)
		{
			for (std::int32_t param = 0; param < pResult.parameters[row].size(); param++)
			{
				Result res = OpenConfiguratorCore::GetInstance().SetFeatureValue(
					OpenConfiguratorCli::GetInstance().networkName,
					nodeId,
					(MNFeatureEnum) (param + (std::int32_t) MNFeatureEnum::DLLErrMNMultipleMN),
					pResult.parameters[row].at(param).c_str());
				if (!res.IsSuccessful())
				{
					return CliLogger::GetInstance().HandleCoreApiFailed("Set Feature Value", res);
				}
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
	else
	{
		for (std::int32_t row = 0; row < pResult.parameters.size(); row++)
		{
			for (std::int32_t param = 0; param < pResult.parameters[row].size(); param++)
			{
				Result res = OpenConfiguratorCore::GetInstance().SetFeatureValue(
					OpenConfiguratorCli::GetInstance().networkName,
					nodeId,
					(CNFeatureEnum) (param + (std::int32_t) CNFeatureEnum::DLLCNFeatureMultiplex),
					pResult.parameters[row].at(param).c_str());
				if (!res.IsSuccessful())
				{
					return CliLogger::GetInstance().HandleCoreApiFailed("Set Feature Value", res);
				}
			}
		}
	}

	return CliResult();
}

CliResult ProjectParser::CreateInterface(const std::uint8_t nodeId, 
										 const std::string& cnXdc)
{
	std::string indexString = "/" + cnXdc;
	std::string initialPath = OpenConfiguratorCli::GetInstance().xmlFilePath;
	std::string nextInitialPath = initialPath.substr(0, initialPath.find_last_of("\\/"));
	std::string fullPath = nextInitialPath.append(indexString);
	CliResult res;

	res = ParameterValidator::GetInstance().IsXdcSchemaValid(nextInitialPath);
	if (!res.IsSuccessful())
	{
		/** XDC file schema is not valid */
		return res;
	}

	ParserElement element;
	CliResult ceres;

	ceres = element.CreateElement(nextInitialPath);
	if (!ceres.IsSuccessful())
	{
		return ceres;
	}
	else
	{
		ParserResult pResult;

		CliResult res = pResult.CreateResult(element, kInterfaceXdcXpathExpression,
												kFormatStrInterfaceXdcXpathExpression);
		if (!res.IsSuccessful())
		{
			return res;
		}
		else
		{
			for (std::int32_t row = 0; row < pResult.parameters.size(); row++)
			{
				std::uint32_t maxModules = 0;

				ModuleAddressing modAddress = GetModuleAddressing(pResult.parameters[row].at(2));					/** Module addressing */
				if (!pResult.parameters[row].at(3).empty())
				{
					maxModules = (std::uint32_t)std::strtol(pResult.parameters[row].at(3).c_str(), NULL, 16);		/** Max Modules */	
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
				Result res = OpenConfiguratorCore::GetInstance().CreateInterface(
								OpenConfiguratorCli::GetInstance().networkName,
								nodeId, 
								pResult.parameters[row].at(0),			/** Interface uniqueId */
								pResult.parameters[row].at(1),			/** Interface type */
								modAddress,
								maxModules,
								unusedSlot,
								multipleMod);
				if (!res.IsSuccessful())
				{
					return CliLogger::GetInstance().HandleCoreApiFailed("Create Interface", res);
				}

				ParserResult subPresult;

				CliResult cliRes = subPresult.CreateResult(element, kAppLayerInterfaceXpathExpression,
											kFormatStrAppLayerInterfaceXpathExpression);
				if (!cliRes.IsSuccessful())
				{
					CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, cliRes);
				}
				else
				{
					for(std::uint32_t row = 0; row < subPresult.parameters.size(); row++)
					{
						ParserResult subRangeResult;

						CliResult rescli = subRangeResult.CreateResult(element, kInterfaceRangeXpathExpression,
																			kFormatStrInterfaceRangeXpathExpression);
						if (!rescli.IsSuccessful())
						{
							CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, rescli);
						}
						else
						{
							for(std::uint32_t subRow = 0 ; subRow < subRangeResult.parameters.size(); subRow++)
							{
								std::uint32_t baseIndex = 0;
								std::uint32_t maxIndex = 0;
								std::uint32_t maxSubIndex = 0;
								std::uint32_t rangeSortStep = 0;

								if (!subRangeResult.parameters[subRow].at(1).empty())
								{
									baseIndex = (std::uint32_t)std::strtol(subRangeResult.parameters[subRow].at(1).c_str(), NULL, 16);			/** Base index value */
								}
								if (!subRangeResult.parameters[subRow].at(2).empty())
								{
									maxIndex = (std::uint32_t)std::strtol(subRangeResult.parameters[subRow].at(2).c_str(), NULL, 16);			/** Max index value */
								}
								if (!subRangeResult.parameters[subRow].at(3).empty())
								{
									maxSubIndex = (std::uint32_t)std::strtol(subRangeResult.parameters[subRow].at(3).c_str(), NULL, 16);		/** Max Sub index value */
								}
								SortMode sortmodRange = GetSortMode(subRangeResult.parameters[subRow].at(4));									/** Sort mode value */
								SortNumber sortRangeNumber = GetSortNumber(subRangeResult.parameters[subRow].at(5));							/** Sort number value */
								PDOMapping mapping = GetPdoMapping(subRangeResult.parameters[subRow].at(6));									/** PDO mapping value */
								if (!subRangeResult.parameters[subRow].at(7).empty())
								{
									rangeSortStep = (std::uint32_t)std::strtol(subRangeResult.parameters[subRow].at(7).c_str(), NULL, 16);		/** Sort step value */
								}

								Result res = OpenConfiguratorCore::GetInstance().CreateRange(
														OpenConfiguratorCli::GetInstance().networkName,
														nodeId, 
														subPresult.parameters[row].at(0),					/** Interface uniqueId */			
														subRangeResult.parameters[subRow].at(0),			/** Range name */	
														baseIndex, 
														maxIndex, 
														maxSubIndex, 
														rangeSortStep, 
														sortmodRange, 
														sortRangeNumber,
														mapping);
								if (!res.IsSuccessful())
								{
									return CliLogger::GetInstance().HandleCoreApiFailed("Create Range", res);
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

CliResult ProjectParser::UpdateNodeIdCollection(const std::uint8_t nodeId, 
												const std::string cnXdc,
												const std::vector<std::string> forcedObject,
												const std::vector<std::string> forcedSubObject, 
												const std::string interfaceId, 
												const std::string modId, 
												const std::uint32_t modPosition)
{
	std::string indexString = "/" + cnXdc;
	std::string initialPath = OpenConfiguratorCli::GetInstance().xmlFilePath;
	std::string nextInitialPath = initialPath.substr(0, initialPath.find_last_of("\\/"));
	std::string fullPath = nextInitialPath.append(indexString);
	CliResult res;

	res = ParameterValidator::GetInstance().IsXdcSchemaValid(nextInitialPath);
	if (!res.IsSuccessful())
	{
		/** XDC file schema is not valid */
		return res;
	}

	ParserElement element;
	CliResult ceres;

	ceres = element.CreateElement(nextInitialPath);
	if (!ceres.IsSuccessful())
	{
		return ceres;
	}
	else
	{
		res = ImportProfileBodyDevice(element, nodeId, interfaceId, modId, modPosition);
		if (!res.IsSuccessful())
		{
			CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, res);
		}

		res = ImportProfileBodyCommn(element, nodeId, forcedObject, forcedSubObject, 
										interfaceId, modId, modPosition);
		if (!res.IsSuccessful())
		{
			CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, res);
		}

		if(modId.empty())
		{
			res = CreateNwMgtGeneralFeatures(element, nodeId);
			if (!res.IsSuccessful())
			{
				CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, res);
			}

			res = CreateNwMgtMnFeatures(element, nodeId);
			if (!res.IsSuccessful())
			{
				CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, res);
			}

			res = CreateNwMgtCnFeatures(element, nodeId);
			if (!res.IsSuccessful())
			{
				CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, res);
			}
		}
	}

	return CliResult();
}

CliResult ProjectParser::CreateNodeAssignment(const ParserElement& pElement, 
											  const std::string xPathExpression, 
											  const std::uint8_t nodeId)
{
	ParserResult pResult;

	CliResult crres = pResult.CreateResult(pElement, xPathExpression.c_str(),
											kFormatStrxPathExpression);
	if (!crres.IsSuccessful())
	{
		return crres;
	}
	else
	{
		if(nodeId == MN_DEFAULT_NODE_ID)
		{
			CliResult res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_MN_PRES, 
												nodeId, 
												pResult.parameters[0].at(0));	/** TaransmitPres value */
			if (!res.IsSuccessful())
			{
				CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, res);
			}
		}
		else
		{
			if(nodeId > MN_DEFAULT_NODE_ID)
			{
				CliResult res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_NODE_IS_CN, nodeId, "true");
				if (!res.IsSuccessful())
				{
					CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, res);
				}
			}
			else
			{
				CliResult res;

				res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_NODE_IS_CN, nodeId, "true");
				if (!res.IsSuccessful())
				{
					CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, res);
				}

				res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_MANDATORY_CN, 
										nodeId, pResult.parameters[0].at(1));		/** isMandatory value */
				if (!res.IsSuccessful())
				{
					CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, res);
				}

				res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_START_CN, 
										nodeId, pResult.parameters[0].at(2));			/** autostartNode value */
				if (!res.IsSuccessful())
				{
					CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, res);
				}

				res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_KEEPALIVE, 
										nodeId, pResult.parameters[0].at(3));			/** resetInOperational value */
				if (!res.IsSuccessful())
				{
					CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, res);
				}

				res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_SWVERSIONCHECK, 
										nodeId, pResult.parameters[0].at(4));	/** verifyAppSwVersion value */
				if (!res.IsSuccessful())
				{
					CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, res);
				}

				res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_SWUPDATE, 
										nodeId, pResult.parameters[0].at(5));				/** autoAppSwUpdateAllowed value */
				if (!res.IsSuccessful())
				{
					CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, res);
				}

				res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_ASYNCONLY_NODE, 
										nodeId, pResult.parameters[0].at(7));				/** isAsyncOnly value */
				if (!res.IsSuccessful())
				{
					CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, res);
				}

				res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_NODE_EXISTS, nodeId, "true");
				if (!res.IsSuccessful())
				{
					CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, res);
				}

				res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_RT1, 
										nodeId, pResult.parameters[0].at(8));				/** isType1Router value */
				if (!res.IsSuccessful())
				{
					CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, res);
				}

				res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_RT2, 
										nodeId, pResult.parameters[0].at(9));				/** isType2Router value */
				if (!res.IsSuccessful())
				{
					CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, res);
				}

				if(pResult.parameters[0].at(6).compare("true") == 0)						/** isChained value */
				{
					Result res = OpenConfiguratorCore::GetInstance().SetOperationModeChained(
										OpenConfiguratorCli::GetInstance().networkName, nodeId);
					if (!res.IsSuccessful())
					{
						return CliLogger::GetInstance().HandleCoreApiFailed("Set Operation Mode Chained", res);
					}
				}
			}
		}
	}

	return CliResult();
}

CliResult ProjectParser::SetNodeAssignment(const NodeAssignment nodeassign, 
										   const std::uint8_t nodeId, 
										   const std::string value)
{
	if (value.compare("true") == 0)
	{
		Result res = OpenConfiguratorCore::GetInstance().AddNodeAssignment(
							OpenConfiguratorCli::GetInstance().networkName, 
							nodeId, nodeassign);
		if (!res.IsSuccessful())
		{
			return CliLogger::GetInstance().HandleCoreApiFailed("Add Node Assignment", res);
		}
	}
	else
	{
		Result res = OpenConfiguratorCore::GetInstance().RemoveNodeAssignment(
							OpenConfiguratorCli::GetInstance().networkName, 
							nodeId, nodeassign);
		if (!res.IsSuccessful())
		{
			return CliLogger::GetInstance().HandleCoreApiFailed("Remove Node Assignment", res);
		}
	}

	return CliResult();
}

ModuleAddressing ProjectParser::GetModuleAddressing(const std::string moduleAddressing)
{
	if (moduleAddressing == "manual")
	{
		return ModuleAddressing::MANUAL;
	}
	else if (moduleAddressing == "position")
	{
		return ModuleAddressing::POSITION;
	}
	else if (moduleAddressing == "next")
	{
		return ModuleAddressing::NEXT;
	}

	return ModuleAddressing::NEXT;
}

SortNumber ProjectParser::GetSortNumber(const std::string sortNumber)
{
	if (sortNumber == "continuous")
	{
		return SortNumber::CONTINUOUS;
	}
	else if (sortNumber == "address")
	{
		return SortNumber::ADDRESS;
	}

	return SortNumber::CONTINUOUS;
}

SortMode ProjectParser::GetSortMode(const std::string sortMode)
{
	if (sortMode == "index")
	{
		return SortMode::INDEX;
	}
	else if (sortMode == "subindex")
	{
		return SortMode::SUBINDEX;
	}

	return SortMode::INDEX;
}

CliResult ProjectParser::CreateModule(const std::uint8_t nodeId, 
										const std::vector<std::string> forcedObject, 
										const std::vector<std::string> forcedSubObject, 
										const std::string interfacecUId, 
										const std::string moduleName, 
										const std::string modulePosition, 
										const std::string moduleAddress, 
										const std::string modulePathToXDC, 
										const std::string moduleEnabled)
{
	std::string indexString = "/" + modulePathToXDC;
	std::string initialPath = OpenConfiguratorCli::GetInstance().xmlFilePath;
	std::string nextInitialPath = initialPath.substr(0, initialPath.find_last_of("\\/"));
	std::string fullPath = nextInitialPath.append(indexString);
	CliResult res;

	res = ParameterValidator::GetInstance().IsXdcSchemaValid(nextInitialPath);
	if (!res.IsSuccessful())
	{
		/** XDC file schema is not valid */
		return res;
	}

	ParserElement element;
	CliResult ceres;

	ceres = element.CreateElement(nextInitialPath);
	if (!ceres.IsSuccessful())
	{
		return ceres;
	}
	else
	{
		ParserResult pResult;

		res = pResult.CreateResult(element, kModuleNodeXpathExpression.c_str(),
									kFormatStrModuleNodeXpathExpression);
		if (!res.IsSuccessful())
		{
			return res;
		}
		else
		{
			for(std::int32_t moduleSubRow = 0; moduleSubRow < pResult.parameters.size(); moduleSubRow++)
			{
				std::uint8_t minPosition = 0;
				std::uint8_t maxPosition = 0;
				std::uint8_t minAddress = 0;
				std::uint8_t maxAddress = 0;
				std::uint8_t maxCount = 0;
				std::uint32_t modpos = 0;
				std::uint32_t moduleAdrs = 0;

				ModuleAddressing modAddressing = GetModuleAddressing(pResult.parameters[moduleSubRow].at(2));	/** Module addressing */
				if (!pResult.parameters[moduleSubRow].at(3).empty())
				{
					minPosition = (std::uint8_t)std::stoi(pResult.parameters[moduleSubRow].at(3).c_str());		/** Module min position */
				}
				if (!pResult.parameters[moduleSubRow].at(4).empty())
				{
					maxPosition = (std::uint8_t)std::stoi(pResult.parameters[moduleSubRow].at(4).c_str());		/** Module max position */
				}
				if (!pResult.parameters[moduleSubRow].at(5).empty())
				{
					minAddress = (std::uint8_t)std::stoi(pResult.parameters[moduleSubRow].at(5).c_str());		/** Module min adress */
				}
				if (!pResult.parameters[moduleSubRow].at(6).empty())
				{
					maxAddress = (std::uint8_t)std::stoi(pResult.parameters[moduleSubRow].at(6).c_str());		/** Module max address */
				}
				if (!pResult.parameters[moduleSubRow].at(7).empty())
				{
					maxCount = (std::uint8_t)std::stoi(pResult.parameters[moduleSubRow].at(7).c_str());			/** Module max count */
				}
				if (!modulePosition.empty())
				{
					modpos = (std::uint32_t)std::strtol(modulePosition.c_str(), NULL, 16);
				}
				if (!moduleAddress.empty())
				{
					moduleAdrs = (std::uint32_t)std::strtol(moduleAddress.c_str(), NULL, 16);
				}

				Result res = OpenConfiguratorCore::GetInstance().CreateModule(OpenConfiguratorCli::GetInstance().networkName, 
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
				if (!res.IsSuccessful())
				{
					return CliLogger::GetInstance().HandleCoreApiFailed("Create Module", res);
				}

				CliResult clires = UpdateNodeIdCollection(nodeId, modulePathToXDC, forcedObject, 
														forcedSubObject , interfacecUId, 
														pResult.parameters[moduleSubRow].at(0), 
														modpos);
				if (!clires.IsSuccessful())
				{
					CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, clires);
				}
			}
		}
	}

	return CliResult();
}

CliResult ProjectParser::CreateModuleObject(const ParserElement& element, 
											const std::uint8_t nodeId, 
											const std::vector<std::string> forcedObject, 
											const std::vector<std::string> forcedSubObject, 
											const std::string interfaceId, 
											const std::string modId, 
											const std::uint32_t modPosition)
{
	ParserResult pResult;
	CliResult clires = pResult.CreateResult(element, kObjectXpathExpression.c_str(),
								kFormatStrObjectXpathExpressionModule);

	if (!clires.IsSuccessful())
	{
		return clires;
	}
	else
	{
		for (std::uint32_t row = 0; row < pResult.parameters.size(); row++)
		{
			std::uint32_t objId = 0;

			if(!pResult.parameters[row].at(0).empty())						/** Object Index */
			{
				objId = (std::uint32_t)std::strtol(pResult.parameters[row].at(0).c_str(), NULL, 16);
			}

			std::uint8_t objIdType = 0;

			if (!pResult.parameters[row].at(2).empty())
			{
				objIdType = (std::uint8_t)std::stoi(pResult.parameters[row].at(2).c_str());
			}
			std::string accessType = pResult.parameters[row].at(5);			/** Access Type */
			std::string dataType = pResult.parameters[row].at(4);			/** data Type */
			std::string uniqueIdRef = pResult.parameters[row].at(8);		/** UniqueId ref */
			std::string actualValue = pResult.parameters[row].at(7);		/** Actual value */
			std:: string rangeSelector = pResult.parameters[row].at(11);	/** Range selector */

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
					CliResult clires = CliLogger::GetInstance().HandleCoreApiFailed("Create Module Object", res);

					CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, clires);
				}
				else
				{
					if ((!pResult.parameters[row].at(9).empty()) || (!pResult.parameters[row].at(10).empty()))
					{
						Result res = OpenConfiguratorCore::GetInstance().SetObjectLimits(
								OpenConfiguratorCli::GetInstance().networkName,
								nodeId, 
								GetNewObjectIndex(nodeId, interfaceId, modId, modPosition, objId),
								pResult.parameters[row].at(9),				/** lowLimit */
								pResult.parameters[row].at(10));			/** highLimit */
						if (!res.IsSuccessful())
						{
							CliResult clires = CliLogger::GetInstance().HandleCoreApiFailed("Set Object Limits", res);

							CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, clires);
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
					CliResult clires = CliLogger::GetInstance().HandleCoreApiFailed("Create Module Parameter Object", res);

					CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, clires);
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
								actualValue,rangeSelector);
				if (!res.IsSuccessful())
				{
					CliResult clires = CliLogger::GetInstance().HandleCoreApiFailed("Create Module Parameter Object", res);

					CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, clires);
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
					CliResult clires = CliLogger::GetInstance().HandleCoreApiFailed("Create Module Object", res);

					CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, clires);
				}
				else
				{
					if ((!pResult.parameters[row].at(9).empty()) || (!pResult.parameters[row].at(10).empty()))
					{
						Result res = OpenConfiguratorCore::GetInstance().SetObjectLimits(
								OpenConfiguratorCli::GetInstance().networkName,
								nodeId, 
								GetNewObjectIndex(nodeId, interfaceId, modId, modPosition, objId),
								pResult.parameters[row].at(9),				/** lowLimit */
								pResult.parameters[row].at(10));			/** highLimit */
						if (!res.IsSuccessful())
						{
							CliResult clires = CliLogger::GetInstance().HandleCoreApiFailed("Set Object Limits", res);

							CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, clires);
						}
					}
				}
			}

			ParserResult subpResult;

			CliResult res = subpResult.CreateResult(element, kSubObjectXpathExpression.c_str(),
													kFormatStrSubObjectXpathExpression,
													pResult.node.at(row));
			if (!res.IsSuccessful())
			{
				CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, res);;
			}
			else
			{
				for (std::uint32_t subrow = 0; subrow < subpResult.parameters.size(); subrow++)
				{
					std::uint8_t subObjId = 0;
					std::uint8_t subObjIdType = 0;

					if (!subpResult.parameters[subrow].at(0).empty())
					{
						subObjId = (std::uint8_t)std::strtol(subpResult.parameters[subrow].at(0).c_str(), NULL, 16);
					}
					if (!subpResult.parameters[subrow].at(2).empty())
					{
						subObjIdType = (std::uint8_t)std::stoi(subpResult.parameters[subrow].at(2).c_str());
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
						Result res = OpenConfiguratorCore::GetInstance().CreateModuleSubObject(
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
						if (!res.IsSuccessful())
						{
							CliResult clires = CliLogger::GetInstance().HandleCoreApiFailed("Create Module Sub Object", res);

							CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, clires);
						}
						else
						{
							/** If any one of the sub object lowLimit and highLimit has values */
							if ((!subpResult.parameters[subrow].at(9).empty()) || 
								(!subpResult.parameters[subrow].at(10).empty()))
							{
								Result res = OpenConfiguratorCore::GetInstance().SetSubObjectLimits(
										OpenConfiguratorCli::GetInstance().networkName,
										nodeId, 
										GetNewObjectIndex(nodeId, interfaceId, modId, modPosition, objId),
										(std::uint8_t)GetNewSubObjectIndex(nodeId, interfaceId, 
																			modId, modPosition, 
																			GetNewObjectIndex(nodeId, 
																				interfaceId, modId, 
																				modPosition, objId), 
																			subObjId),
										subpResult.parameters[subrow].at(9),			/** sub object lowLimit */
										subpResult.parameters[subrow].at(10));			/** sub object highLimit */
								if (!res.IsSuccessful())
								{
									CliResult clires = CliLogger::GetInstance().HandleCoreApiFailed("Set Sub Object Limits", res);

									CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, clires);
								}
							}
						}
					}
					else if ((!subObjuniqueIdRef.empty()) && (!subObjdataType.empty()))
					{
						/** Both sub object uniqueIdRef and dataType has values */
						Result res = OpenConfiguratorCore::GetInstance().CreateModuleParameterSubObject(
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
						if (!res.IsSuccessful())
						{
							CliResult clires = CliLogger::GetInstance().HandleCoreApiFailed("Create Module Parameter Sub Object", res);

							CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, clires);
						}
					}
					else if ((!subObjuniqueIdRef.empty()) && (subObjdataType.empty()))
					{
						/** Both sub object uniqueIdRef and dataType has values */
						Result res = OpenConfiguratorCore::GetInstance().CreateModuleParameterSubObject(
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
						if (!res.IsSuccessful())
						{
							CliResult clires = CliLogger::GetInstance().HandleCoreApiFailed("Create Module Parameter Sub Object", res);

							CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, clires);
						}
					}
					
					CliResult result = UpdateForcedObjects(forcedObject, forcedSubObject, nodeId,
															pResult.parameters[row].at(0), subpResult.parameters[subrow].at(0), 
															objId, subObjId, actualValue, subObjActualValue, true,
															interfaceId, modId, modPosition);
					if (!result.IsSuccessful())
					{
						CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, result);
					}
				}
			}
		}
	}

	return CliResult();
}

std::uint32_t ProjectParser::GetNewObjectIndex(const std::uint8_t nodeId, 
											   const std::string interfaceId, 
											   const std::string modId, 
											   const std::uint32_t modposition,
											   const std::uint32_t objId) 
{
	std::uint32_t index = 0;
	std::int32_t subIndex = 0;

	Result res = OpenConfiguratorCore::GetInstance().GetModuleObjectCurrentIndex(
								OpenConfiguratorCli::GetInstance().networkName,
								nodeId, interfaceId, modId, 
								modposition, objId, -1, index, subIndex);
	if(!res.IsSuccessful())
	{
		CliResult clires = CliLogger::GetInstance().HandleCoreApiFailed("Get Module Object Current Index", res);

		CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, clires);
	}

	return index;
}

std::int32_t ProjectParser::GetNewSubObjectIndex(const std::uint8_t nodeId, 
												 const std::string interfaceId, 
												 const std::string modId, 
												 const std::uint32_t modPosition,
												 const std::uint32_t objId, 
												 const std::uint8_t subObjId) 
{
	std::uint32_t index = 0;
	std::int32_t subIndex = 0;

	Result res = OpenConfiguratorCore::GetInstance().GetModuleObjectCurrentIndex(
								OpenConfiguratorCli::GetInstance().networkName,
								nodeId, interfaceId, modId, 
								modPosition, 0, subObjId, index, subIndex);
	if(!res.IsSuccessful())
	{
		CliResult clires = CliLogger::GetInstance().HandleCoreApiFailed("Get Module Object Current Sub Index", res);

		CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, clires);
	}

	return subIndex;
}

std::string ProjectParser::GetNewParameterId(const std::uint8_t nodeId, 
											 const std::string interfaceId, 
											 const std::string modId, 
											 const std::uint32_t modPosition, 
											 const std::string uniqueID)
{
	std::string parameterName;

	Result res = OpenConfiguratorCore::GetInstance().GetModuleParameterCurrentName(
									OpenConfiguratorCli::GetInstance().networkName,
									nodeId, interfaceId, modId, 
									modPosition, uniqueID, parameterName);
	if (!res.IsSuccessful())
	{
		CliResult clires = CliLogger::GetInstance().HandleCoreApiFailed("Get Module Parameter Current Name", res);

		CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, clires);
	}

	return parameterName;
}

CliResult ProjectParser::CreateForcedObjects(const ParserElement& pElement, 
												const std::string xPathExpression,
												const std::uint8_t nodeId, 
												std::vector<std::string>& forcedObj, 
												std::vector<std::string>& forcedSubObj)
{
	ParserResult pResult;
	CliResult cliRes;

	cliRes = pResult.CreateResult(pElement, xPathExpression.c_str(),
										kFormatStrxPathExpressionObject);
	if (!cliRes.IsSuccessful())
	{
		return CliResult();
	}
	else
	{
		for (std::int32_t row = 0; row < pResult.parameters.size(); row++)
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

CliResult ProjectParser::UpdateForcedObjects(const std::vector<std::string> forcedObject, 
											 const std::vector<std::string> forcedSubObject, 
											 const std::uint8_t nodeId, 
											 const std::string objectIndex, 
											 const std::string SubobjectIndex, 
											 const std::uint32_t objectId, 
											 const std::uint8_t subObjectId, 
											 const std::string objectValue, 
											 const std::string subobjectValue, 
											 const bool value,
											 const std::string interfaceId, 
											 const std::string modId, 
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
						std::uint8_t subObjectIdOutput = 0; 
						Result res;

						if (value)
						{
							objectIdOutput = GetNewObjectIndex(nodeId, interfaceId, modId, 
															modPosition, objectId);
							subObjectIdOutput = (std::uint8_t)(GetNewSubObjectIndex(nodeId, 
															interfaceId, 
															modId, modPosition,	
															objectId, subObjectId));

							res = OpenConfiguratorCore::GetInstance().SetSubObjectActualValue(
											OpenConfiguratorCli::GetInstance().networkName,
											nodeId, objectIdOutput, subObjectIdOutput, 
											subobjectValue, true, false);
						}
						else
						{
							res = OpenConfiguratorCore::GetInstance().SetSubObjectActualValue(
											OpenConfiguratorCli::GetInstance().networkName,
											nodeId, objectId, subObjectId, 
											subobjectValue, true, false);
						}

						if (!res.IsSuccessful())
						{
							CliResult clires = CliLogger::GetInstance().HandleCoreApiFailed("Set Sub Object Actual Value", res);

							CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, clires);
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
								objectIdOutput = GetNewObjectIndex(nodeId, interfaceId, modId, modPosition, objectId);

								res = OpenConfiguratorCore::GetInstance().SetObjectActualValue(
													OpenConfiguratorCli::GetInstance().networkName,
													nodeId, objectIdOutput, objectValue, true, false);
							}
							else
							{
								res = OpenConfiguratorCore::GetInstance().SetObjectActualValue(
													OpenConfiguratorCli::GetInstance().networkName,
													nodeId, objectId, objectValue, true, false);
							}

							if (!res.IsSuccessful())
							{
								CliResult clires = CliLogger::GetInstance().HandleCoreApiFailed("Set Object Actual Value", res);

								CliLogger::GetInstance().LogMessage(CliMessageType::CLI_WARN, clires);
							}
						}
					}
				}
			}
		}
	}

	return CliResult();
}

IEC_Datatype ProjectParser::GetDataType(const std::string dataType)
{
	if (dataType == "UNDEFINED")
	{
		return IEC_Datatype::UNDEFINED;
	}
	else if (dataType == "BITSTRING")
	{
		return IEC_Datatype::BITSTRING;
	}
	else if (dataType == "BOOL")
	{
		return IEC_Datatype::BOOL;
	}
	else if (dataType == "BYTE")
	{
		return IEC_Datatype::BYTE;
	}
	else if (dataType == "_CHAR")
	{
		return IEC_Datatype::_CHAR;
	}
	else if (dataType == "WORD")
	{
		return IEC_Datatype::WORD;
	}
	else if (dataType == "DWORD")
	{
		return IEC_Datatype::DWORD;
	}
	else if (dataType == "LWORD")
	{
		return IEC_Datatype::LWORD;
	}
	else if (dataType == "SINT")
	{
		return IEC_Datatype::SINT;
	}
	else if (dataType == "INT")
	{
		return IEC_Datatype::INT;
	}
	else if (dataType == "DINT")
	{
		return IEC_Datatype::DINT;
	}
	else if (dataType == "LINT")
	{
		return IEC_Datatype::LINT;
	}
	else if (dataType == "USINT")
	{
		return IEC_Datatype::USINT;
	}
	else if (dataType == "UINT")
	{
		return IEC_Datatype::UINT;
	}
	else if (dataType == "UDINT")
	{
		return IEC_Datatype::UDINT;
	}
	else if (dataType == "ULINT")
	{
		return IEC_Datatype::ULINT;
	}
	else if (dataType == "REAL")
	{
		return IEC_Datatype::REAL;
	}
	else  if (dataType == "LREAL")
	{
		return IEC_Datatype::LREAL;
	}
	else  if (dataType == "STRING")
	{
		return IEC_Datatype::STRING;
	}
	else if (dataType == "WSTRING")
	{
		return IEC_Datatype::WSTRING;
	}

	return IEC_Datatype::UNDEFINED;
}

ParameterAccess ProjectParser::GetParameterAccess(const std::string access)
{
	if (access == "constant")
	{
		return ParameterAccess::constant;
	}
	else if (access == "read")
	{
		return ParameterAccess::read;
	}
	else if (access == "write")
	{
		return ParameterAccess::write;
	}
	else if (access == "readWrite")
	{
		return ParameterAccess::readWrite;
	}
	else if (access == "readWriteInput")
	{
		return ParameterAccess::readWriteInput;
	}
	else if (access == "readWriteOutput")
	{
		return ParameterAccess::readWriteOutput;
	}
	else if (access == "noAccess")
	{
		return ParameterAccess::noAccess;
	}
	else if (access == "undefined")
	{
		return ParameterAccess::undefined;
	}

	return ParameterAccess::undefined;
}

ObjectType ProjectParser::GetObjectType(const std::uint8_t objType)
{
	switch (objType)
	{
		case 0:
			return ObjectType::INVALID;
		case 5:
			return ObjectType::DEFTYPE;
		case 6:
			return ObjectType::DEFSTRUCT;
		case 7:
			return ObjectType::VAR;
		case 8:
			return ObjectType::ARRAY;
		case 9:
			return ObjectType::RECORD;
		default:
			return ObjectType::INVALID;
	}
}

PlkDataType ProjectParser::GetPlkDataType(const std::string plkDataType)
{
	if (plkDataType == "0000")
	{
		return PlkDataType::UNDEFINED;
	}
	else if (plkDataType == "0001")
	{
		return PlkDataType::BOOLEAN;
	}
	else if (plkDataType == "0002")
	{
		return PlkDataType::INTEGER8;
	}
	else if (plkDataType == "0003")
	{
		return PlkDataType::INTEGER16;
	}
	else if (plkDataType == "0004")
	{
		return PlkDataType::INTEGER32;
	}
	else if (plkDataType == "0005")
	{
		return PlkDataType::UNSIGNED8;
	}
	else if (plkDataType == "0006")
	{
		return PlkDataType::UNSIGNED16;
	}
	else if (plkDataType == "0007")
	{
		return PlkDataType::UNSIGNED32;
	}
	else if (plkDataType == "0008")
	{
		return PlkDataType::REAL32;
	}
	else if (plkDataType == "0009")
	{
		return PlkDataType::VISIBLE_STRING;
	}
	else if (plkDataType == "000A")
	{
		return PlkDataType::OCTET_STRING;
	}
	else if (plkDataType == "000B")
	{
		return PlkDataType::UNICODE_STRING;
	}
	else if (plkDataType == "000C")
	{
		return PlkDataType::TIME_OF_DAY;
	}
	else if (plkDataType == "000D")
	{
		return PlkDataType::TIME_DIFF;
	}
	else if (plkDataType == "000F")
	{
		return PlkDataType::Domain;
	}
	else if (plkDataType == "0010")
	{
		return PlkDataType::INTEGER24;
	}
	else if (plkDataType == "0011")
	{
		return PlkDataType::REAL64;
	}
	else if (plkDataType == "0012")
	{
		return PlkDataType::INTEGER40;
	}
	else if (plkDataType == "0013")
	{
		return PlkDataType::INTEGER48;
	}
	else if (plkDataType == "0014")
	{
		return PlkDataType::INTEGER56;
	}
	else if (plkDataType == "0015")
	{
		return PlkDataType::INTEGER64;
	}
	else if (plkDataType == "0016")
	{
		return PlkDataType::UNSIGNED24;
	}
	else if (plkDataType == "0018")
	{
		return PlkDataType::UNSIGNED40;
	}
	else if (plkDataType == "0019")
	{
		return PlkDataType::UNSIGNED48;
	}
	else if (plkDataType == "001A")
	{
		return PlkDataType::UNSIGNED56;
	}
	else if (plkDataType == "001B")
	{
		return PlkDataType::UNSIGNED64;
	}
	else if (plkDataType == "0401")
	{
		return PlkDataType::MAC_ADDRESS;
	}
	else if (plkDataType == "0402")
	{
		return PlkDataType::IP_ADDRESS;
	}
	else if (plkDataType == "0403")
	{
		return PlkDataType::NETTIME;
	}

	return PlkDataType::UNDEFINED;
}

AccessType ProjectParser::GetObjAccessType(const std::string accessType)
{
	if (accessType == "undefined")
	{
		return AccessType::UNDEFINED;
	}
	else if (accessType == "rw")
	{
		return AccessType::RW;
	}
	else if (accessType == "rws")
	{
		return AccessType::RWS;
	}
	else if (accessType == "wo")
	{
		return AccessType::WO;
	}
	else if (accessType == "wos")
	{
		return AccessType::WOS;
	}
	else if (accessType == "ro")
	{
		return AccessType::RO;
	}
	else if (accessType == "const")
	{
		return AccessType::CONST;
	}
	else if (accessType == "cond")
	{
		return AccessType::COND;
	}

	return AccessType::UNDEFINED;
}

PDOMapping ProjectParser::GetPdoMapping(const std::string pdoMapp)
{
	if (pdoMapp == "undefined")
	{
		return PDOMapping::UNDEFINED;
	}
	else  if (pdoMapp == "no")
	{
		return PDOMapping::NO;
	}
	else if (pdoMapp == "default")
	{
		return PDOMapping::DEFAULT;
	}
	else if (pdoMapp == "optional")
	{
		return PDOMapping::OPTIONAL;
	}
	else if (pdoMapp == "TPDO")
	{
		return PDOMapping::TPDO;
	}
	else if (pdoMapp == "RPDO")
	{
		return PDOMapping::RPDO;
	}

	return PDOMapping::UNDEFINED;
}

