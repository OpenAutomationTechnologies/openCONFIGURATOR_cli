/**
 * \class ProjectParser
 *
 * \brief Implementation of ProjectParser module
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 0.1
 *
 */

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

CLIResult ProjectParser::ParserXMLFile(std::string xmlPath)
{
	try
	{
		/**< Core Library API call to create Network */
		Result res = OpenConfiguratorCore::GetInstance().CreateNetwork(
						OpenConfiguratorCLI::GetInstance().networkName);

		ParserElement xmlParserElement;
		ParserResult mnResult, cnResult;

		CLIResult ceres = xmlParserElement.CreateElement(xmlPath);
		if (!ceres.IsSuccessful())
		{
			return ceres;
		}
		else
		{
			CLIResult crres;

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
					std::uint8_t nodeId = (std::uint8_t)std::stoi(mnResult.parameters[row].at(0).c_str());

					/**< Core Library API call to create Node */
					Result res = OpenConfiguratorCore::GetInstance().CreateNode(
										OpenConfiguratorCLI::GetInstance().networkName,
										nodeId,
										mnResult.parameters[row].at(2));	/**< name */
					if (!res.IsSuccessful())
					{
						return CLILogger::GetInstance().HandleCoreApiFailed("Create MN Node", res);
					}

					CLIResult clires;
					std::vector<std::string> forcedModularNodeObj;		/**< Specifies the group of forced objects in Node */
					std::vector<std::string> forcedModularNodeSubObj;	/**< Specifies the group of forced sub objects in Node */

					/**< Update the forced objects of node */
					clires = CreateForcedObjects(xmlParserElement, kForcedObjectNodeXpathExpression, 
													nodeId, forcedModularNodeObj, forcedModularNodeSubObj);
					if (!clires.IsSuccessful())
					{
						CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, clires);
					}

					/**< Core Library API calls to import the XDD of node */
					clires = UpdateNodeIdCollection(nodeId,
										mnResult.parameters[row].at(1),	/**< pathToXDC */
										forcedModularNodeObj, forcedModularNodeSubObj, 
										"","",0U);
					if (!clires.IsSuccessful())
					{
						return CLILogger::GetInstance().HandleCliApiFailed("Update MN Node Id Collection", clires);
					}

					/**< Core Library API calls to update the node assignment of node */
					clires = CreateNodeAssignment(xmlParserElement, kMnXpathExpression, nodeId);
					if (!clires.IsSuccessful())
					{
						CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, clires);
					}
				}
			}

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
					std::uint8_t nodeId = (std::uint8_t)std::stoi(cnResult.parameters[row].at(0).c_str());

					ParserResult subCnResult;
					CLIResult clires;
					CLIResult cliresult;

					clires = subCnResult.CreateResult(xmlParserElement, 
														kIntrfcXpathExpression, "id");
					if (!clires.IsSuccessful())
					{
						CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, clires);

						/**< Core Library API call to create Node */
						Result res = OpenConfiguratorCore::GetInstance().CreateNode(
									OpenConfiguratorCLI::GetInstance().networkName,
									nodeId, 
									cnResult.parameters[row].at(2),		/* name */
									false);
						if (!res.IsSuccessful())
						{
							cliresult = CLILogger::GetInstance().HandleCoreApiFailed("Create CN Node", res);

							CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, cliresult);
						}
					
						/**< Update the forced objects of modular head node */
						cliresult = CreateForcedObjects(xmlParserElement, kForcedObjectNodeXpathExpression, 
														nodeId, forcedNodeObj, forcedNodeSubObj);
						if (!cliresult.IsSuccessful())
						{
							CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, cliresult);
						}
						
						/**< Core Library API calls to import the XDD of node */
						cliresult = UpdateNodeIdCollection(nodeId, 
															cnResult.parameters[row].at(1),				/* pathToXDC */
															forcedNodeObj, forcedNodeSubObj,
															"", "", 0U);
						if (!cliresult.IsSuccessful())
						{
							CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, cliresult);
						}
						
						/**< Core Library API calls to update the node assignment of node */
						cliresult = CreateNodeAssignment(xmlParserElement, kCnXpathExpression, nodeId);
						if (!cliresult.IsSuccessful())
						{
							CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, cliresult);
						}
					}
					else
					{
						/**< Core Library API call to create Modular Node */
						Result res = OpenConfiguratorCore::GetInstance().CreateModularHeadNode(
											OpenConfiguratorCLI::GetInstance().networkName,
											nodeId, 
											cnResult.parameters[row].at(2));		/* name */
						if (!res.IsSuccessful())
						{
							cliresult = CLILogger::GetInstance().HandleCoreApiFailed("Create CN Node", res);

							CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, cliresult);
						}

						/**< Update the forced objects of modular head node */
						cliresult = CreateForcedObjects(xmlParserElement, kForcedObjectNodeXpathExpression, 
															nodeId, forcedNodeObj, forcedNodeSubObj);
						if (!cliresult.IsSuccessful())
						{
							CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, cliresult);
						}

						/**< Core Library API calls to import the XDD of modular head node */
						cliresult = UpdateNodeIdCollection(nodeId, cnResult.parameters[row].at(1),	/* pathToXDC */
															forcedNodeObj, forcedNodeSubObj, 
															"", "", 0U);	
						if (!cliresult.IsSuccessful())
						{
							CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, cliresult);
						}

						/**< Core Library API calls to update the node assignment of modular head node */
						cliresult = CreateNodeAssignment(xmlParserElement, kCnXpathExpression, nodeId);
						if (!cliresult.IsSuccessful())
						{
							CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, cliresult);
						}

						/**< Core Library API calls to create interface of modular head node */
						cliresult = CreateInterface(nodeId, cnResult.parameters[row].at(1));	/* pathToXDC */
						if (!cliresult.IsSuccessful())
						{
							CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, cliresult);
						}
						else
						{
							ParserResult moduleResult;

							for(std::uint32_t subRow = 0; subRow < subCnResult.parameters.size(); subRow++)
							{

								CLIResult clicrres = moduleResult.CreateResult(xmlParserElement, kModuleXpathExpression,
																				kFormatStrModuleXpathExpression);
								if (!clicrres.IsSuccessful())
								{
									CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, clicrres);
								}
								else
								{
									CLIResult res;
									for(std::uint32_t moduleRow = 0; moduleRow < moduleResult.parameters.size(); moduleRow++)
									{
										/**< Update the forced objects of modular node */
										res = CreateForcedObjects(xmlParserElement, kForcedObjectModuleXpathExpression, nodeId, 
																	forcedModularObj, forcedModularSubObj);
										if (!res.IsSuccessful())
										{
											CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);
										}

										res = CreateModule(nodeId,
															subCnResult.parameters[subRow].at(0),				/* Interface Unique ID */
															moduleResult.parameters[moduleRow].at(0),			/* Module name */
															moduleResult.parameters[moduleRow].at(1),			/* Module position */
															moduleResult.parameters[moduleRow].at(2),			/* Module address */
															moduleResult.parameters[moduleRow].at(3),			/* Module pathtoXDC */
															moduleResult.parameters[moduleRow].at(4));			/* Module enabled */
										if (!res.IsSuccessful())
										{
											CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);
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
		return CLILogger::GetInstance().HandleExceptionCaught("Parser XML File", ex);
	}

	return CLIResult();
}

IEC_Datatype ProjectParser::GetIECDataType(xercesc::DOMNode* node)
{
	IEC_Datatype data = IEC_Datatype::UNDEFINED;

	if (node->hasChildNodes())
	{
		xercesc::DOMNodeList* childNode = node-> getChildNodes();
		const XMLSize_t nodeCount = childNode->getLength();

		for (XMLSize_t index = 0; index < nodeCount; index++)
		{
			xercesc::DOMNode* currentNode = childNode->item(index);

			if ((currentNode->getNodeType()) && (currentNode->getNodeType() == xercesc::DOMNode::ELEMENT_NODE))
			{
				char* subNodeName = xercesc::XMLString::transcode(currentNode->getNodeName());
				std::string childNodeName = subNodeName;
				if ((childNodeName == "defaultValue") ||
					(childNodeName == "allowedValues") || 
					(childNodeName == "actualValue") || 
					(childNodeName == "substituteValue") || 
					(childNodeName == "unit") || 
					(childNodeName == "property"))
				{
					return data;
				}
				data = GetDataType(childNodeName);
			}
		}
	}

	return data;
}

CLIResult ProjectParser::CreateStructDataType(ParserElement& element, std::uint8_t nodeId, 
											  std::string interfaceId, std::string modId, 
											  std::uint32_t modposition)
{
	ParserResult pResult;

	CLIResult crres = pResult.CreateResult(element, kStructXpathExpression,
											kFormatStrStructXpathExpression);
	if (!crres.IsSuccessful())
	{
		return crres;
	}
	else
	{
		for (std::int32_t row = 0; row < pResult.parameters.size(); row++)
		{
			/**< Core Library API call to create StructDatatype */
			Result res = OpenConfiguratorCore::GetInstance().CreateStructDatatype(
					OpenConfiguratorCLI::GetInstance().networkName,
					nodeId, pResult.parameters[row].at(0),
					pResult.parameters[row].at(1), 
					interfaceId,
					modId,
					modposition);
			if (!res.IsSuccessful())
			{
				return CLILogger::GetInstance().HandleCoreApiFailed("Create Struct Datatype", res);
			}

			ParserResult varDecResult;

			CLIResult subcrres = varDecResult.CreateResult(element, kStructVarXpathExpression,
										kFormatStrStructVarXpathExpression,
										pResult.node.at(row));
			if (!subcrres.IsSuccessful())
			{
				CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
			}
			else
			{
				for (std::int32_t subrow = 0; subrow < varDecResult.parameters.size(); subrow++)
				{
					IEC_Datatype data = GetIECDataType(varDecResult.node.at(subrow));

					std::uint32_t varDeclSize;
					if (varDecResult.parameters[subrow].at(2).empty())
					{
						/**< Set default value to size if value is empty */
						varDeclSize = 1U;
					}
					else
					{
						varDeclSize = (std::uint32_t) std::stoi(varDecResult.parameters[subrow].at(2).c_str());
					}

					/**< Core Library API call to create VarDeclaration */
					Result res = OpenConfiguratorCore::GetInstance().CreateVarDeclaration(
							OpenConfiguratorCLI::GetInstance().networkName,
							nodeId,
							pResult.parameters[row].at(0),										/**< parent uniqueID */
							varDecResult.parameters[subrow].at(0),								/**< sub item uniqueID */
							varDecResult.parameters[subrow].at(1), 								/**< sub item name */
							data,
							varDeclSize,														/* sub item size */
							"",
							interfaceId,
							modId,
							modposition);														/**< sub item size */
					if (!res.IsSuccessful())
					{
						return CLILogger::GetInstance().HandleCoreApiFailed("Create Var Declaration", res);
					}
				}
			}
		}
	}

	return CLIResult();
}

CLIResult ProjectParser::CreateArrayDataType(ParserElement& element, std::uint8_t nodeId, 
											 std::string interfaceId, std::string modId, 
											 std::uint32_t modposition)
{
	ParserResult pResult;

	CLIResult crres = pResult.CreateResult(element, kArrayXpathExpression,
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

			IEC_Datatype data = GetIECDataType(pResult.node.at(row));

			CLIResult subcrres = subpResult.CreateResult(element, kArraySubRangeXpathExpression,
										kFormatStrArraySubRangeXpathExpression,
										pResult.node.at(row));
			if (!subcrres.IsSuccessful())
			{
				CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
			}
			else
			{
				for (std::int32_t subrow = 0; subrow < subpResult.parameters.size(); subrow++)
				{
					std::uint32_t lowerLimit;
					std::uint32_t upperLimit;

					if (subpResult.parameters[subrow].at(0).empty())
					{
						/**< Defalut value to lowerLimit in case it is empty */
						lowerLimit = 1U;
					}
					else
					{
						lowerLimit = (std::uint32_t)std::stoi(subpResult.parameters[subrow].at(0).c_str());
					}

					if (subpResult.parameters[subrow].at(1).empty())
					{
						/**< Defalut value to upperLimit in case it is empty */
						upperLimit = 1U;
					}
					else
					{
						upperLimit = (std::uint32_t)std::stoi(subpResult.parameters[subrow].at(1).c_str());
					}

					/**< Core Library API call to create ArrayDatatype */
					Result res = OpenConfiguratorCore::GetInstance().CreateArrayDatatype(
							OpenConfiguratorCLI::GetInstance().networkName,
							nodeId,
							pResult.parameters[row].at(0),  								/**< uniqueID */
							pResult.parameters[row].at(1),  								/**< name */
							lowerLimit,
							upperLimit,
							data,
							interfaceId,
							modId,
							modposition);
					if (!res.IsSuccessful())
					{
						return CLILogger::GetInstance().HandleCoreApiFailed("Create Array Datatype", res);;
					}
				}
			}
		}
	}

	return CLIResult();
}

CLIResult ProjectParser::CreateDataType(ParserElement& element, std::uint8_t nodeId, 
										std::string interfaceId, std::string modId, 
										std::uint32_t modposition)
{
	CLIResult res;

	/**< Creates struct data type of node into the core library */
	res = CreateStructDataType(element, nodeId, interfaceId, modId, modposition);
	if (!res.IsSuccessful())
	{
		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);
	}

	/**< Creates array data type of node into the core library */
	res = CreateArrayDataType(element, nodeId, interfaceId, modId, modposition);
	if (!res.IsSuccessful())
	{
		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);
	}

	return CLIResult();
}

CLIResult ProjectParser::SetParamAllowedValue(ParserElement & element,
										 std::uint8_t nodeId,
										 xercesc::DOMNode* node,
										 std::string uniqueId,
										 std::string interfaceId, 
										 std::string modId, 
										 std::uint32_t modposition)
{
	ParserResult pResult;

	CLIResult crres = pResult.CreateResult(element,
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

		for (std::int32_t row = 0; row < pResult.parameters.size(); row++)
		{
			allowedValuelist.push_back(pResult.parameters[row].at(0));
		}

		if (!modId.empty())
		{
			uniqueId = ProjectParser::GetInstance().GetNewParameterId(nodeId, interfaceId, modId, modposition, uniqueId);
		}

		/**< Core Library API call to set allowed parameter values */
		Result res = OpenConfiguratorCore::GetInstance().SetParameterAllowedValues(
				OpenConfiguratorCLI::GetInstance().networkName,
				nodeId,
				uniqueId,
				allowedValuelist);
		if (!res.IsSuccessful())
		{
			return CLILogger::GetInstance().HandleCoreApiFailed("Set Parameter Allowed Values", res);;
		}
	}

	return CLIResult();
}

CLIResult ProjectParser::SetParamAllowedRange(ParserElement& element,
										std::uint8_t nodeId,
										xercesc::DOMNode* node,
										std::string uniqueId,
										std::string interfaceId, 
										std::string modId, 
										std::uint32_t modposition)
{
	ParserResult pResult;

	CLIResult crres = pResult.CreateResult(element,
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
			CLIResult subcrres;

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
				uniqueId = ProjectParser::GetInstance().GetNewParameterId(nodeId, interfaceId, modId, modposition, uniqueId);
			}

			/**< Core Library API call to set allowed parameter values */
			Result res = OpenConfiguratorCore::GetInstance().SetParameterAllowedRange(
					OpenConfiguratorCLI::GetInstance().networkName,
					nodeId, uniqueId, minValue, maxValue);
			if (!res.IsSuccessful())
			{
				return CLILogger::GetInstance().HandleCoreApiFailed("Set Parameter Allowed Range", res);
			}
		}
	}

	return CLIResult();
}

CLIResult ProjectParser::SetParamActualValue(ParserElement& element,
										std::uint8_t nodeId,
										xercesc::DOMNode* node,
										std::string uniqueId,
										std::string interfaceId, 
										std::string modId, 
										std::uint32_t modposition)
{
	ParserResult pResult;

	CLIResult crres = pResult.CreateResult(element,
								kParameterActualValueXpathExpression,
								kFormatStrParameterActualValueXpathExpression,
								node);
	if (!crres.IsSuccessful())
	{
		return crres;
	}
	else
	{
		if (!modId.empty())
		{
			uniqueId = ProjectParser::GetInstance().GetNewParameterId(nodeId, interfaceId, modId, modposition, uniqueId);
		}
		/**< Core Library API call to set actual parameter values */
		Result res = OpenConfiguratorCore::GetInstance().SetParameterActualValue(
				OpenConfiguratorCLI::GetInstance().networkName,
				nodeId, uniqueId, pResult.parameters[0].at(0));
		if (!res.IsSuccessful())
		{
			return CLILogger::GetInstance().HandleCoreApiFailed("Set Parameter Actual Value", res);
		}
	}

	return CLIResult();
}

CLIResult ProjectParser::SetParamDefaultValue(ParserElement& element,
										std::uint8_t nodeId,
										xercesc::DOMNode* node,
										std::string uniqueId,
										std::string interfaceId, 
										std::string modId, 
										std::uint32_t modposition)
{
	ParserResult pResult;

	CLIResult crres = pResult.CreateResult(element,
								kParameterDefaultValueXpathExpression,
								kFormatStrParameterDefaultValueXpathExpression,
								node);
	if (!crres.IsSuccessful())
	{
		return crres;
	}
	else
	{
		if (!modId.empty())
		{
			uniqueId = ProjectParser::GetInstance().GetNewParameterId(nodeId, interfaceId, modId, modposition, uniqueId);
		}
		/**< Core Library API call to set actual parameter values */
		Result res = OpenConfiguratorCore::GetInstance().SetParameterDefaultValue(
				OpenConfiguratorCLI::GetInstance().networkName,
				nodeId, uniqueId, pResult.parameters[0].at(0));
		if (!res.IsSuccessful())
		{
			return CLILogger::GetInstance().HandleCoreApiFailed("Set Parameter Default Value", res);
		}
	}

	return CLIResult();
}

CLIResult ProjectParser::CreateParameterTemplate(ParserElement& element, std::uint8_t nodeId, 
												 std::string interfaceId, std::string modId, 
												 std::uint32_t modposition)
{
	ParserResult pResult;
	LOG_DEBUG() << "Template Entered!";
	CLIResult crres = pResult.CreateResult(element, kParameterTemplateXpathExpression,
								kFormatStrParameterTemplateXpathExpression);
	if (!crres.IsSuccessful())
	{
		LOG_DEBUG() << "Template not available!";
		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, crres);
	}
	else
	{
		for (std::int32_t row = 0; row < pResult.parameters.size(); row++)
		{
			ParameterAccess accessOfParameter = GetParameterAccess(pResult.parameters[row].at(1));
			IEC_Datatype data = GetIECDataType(pResult.node.at(row));

			ParserResult subpResult;
			LOG_DEBUG() << "Template  available!";
			CLIResult subcrres = subpResult.CreateResult(element, kParameterDataTypeIdRefXpathExpression,
										kFormatStrParameterDataTypeIdRefXpathExpression,
										pResult.node.at(row));
			if (!subcrres.IsSuccessful())
			{ LOG_DEBUG() << "Data type not  available!";
					/**< Core Library API call to create Parameter */
					Result res = OpenConfiguratorCore::GetInstance().CreateParameter(
							OpenConfiguratorCLI::GetInstance().networkName,
							nodeId, pResult.parameters[row].at(0),
							accessOfParameter, data, true,
							interfaceId, modId, modposition);
					if (!res.IsSuccessful())
					{
						return CLILogger::GetInstance().HandleCoreApiFailed("Create Parameter", res);
					}
			}
			else
			{
				LOG_INFO() << "DataTypeIdRef not available.";
					for (std::int32_t subrow = 0; subrow < subpResult.parameters.size(); subrow++)
					{
						ParameterAccess paramAccessSubNode = GetParameterAccess(subpResult.parameters[subrow].at(1));

						/**< Core Library API call to create Parameter */
						Result res = OpenConfiguratorCore::GetInstance().CreateParameter(
								OpenConfiguratorCLI::GetInstance().networkName,
								nodeId, pResult.parameters[row].at(0),
								subpResult.parameters[subrow].at(0),
								paramAccessSubNode, true,
								interfaceId, modId, modposition);
						if (!res.IsSuccessful())
						{
							return CLILogger::GetInstance().HandleCoreApiFailed("Create parameter", res);
						}
					}
				}
			//TODO: 8122016 Implement check for templateid ref
			/**< Updates the allowed values of parameter template.*/
			//subcrres = SetParamAllowedValue(element, nodeId, pResult.node.at(row), pResult.parameters[row].at(0), interfaceId, modId, modposition);
			//if (!subcrres.IsSuccessful())
			//{
			//	CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
			//}

			///**< Updates the allowed range of parameter template.*/
			//subcrres = SetParamAllowedRange(element, nodeId, pResult.node.at(row), pResult.parameters[row].at(0), interfaceId, modId, modposition);
			//if (!subcrres.IsSuccessful())
			//{
			//	CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
			//}

			///**< Updates the default value of parameter template.*/
			//subcrres = SetParamDefaultValue(element, nodeId, pResult.node.at(row), pResult.parameters[row].at(0), interfaceId, modId, modposition);
			//if (!subcrres.IsSuccessful())
			//{
			//	CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
			//}

			///**< Updates the actual value of parameter template.*/
			//subcrres = SetParamActualValue(element,  nodeId, pResult.node.at(row), pResult.parameters[row].at(0), interfaceId, modId, modposition);
			//if (!subcrres.IsSuccessful())
			//{
			//	CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
			//}
		}
	}

	return CLIResult();
}

CLIResult ProjectParser::CreateChildParameterGroup(ParserElement& element,
												std::uint8_t nodeId,
												xercesc::DOMNode* parameterGroupNode,
												std::string paramGroupUId, 
												std::string interfaceId, 
												std::string modId, 
												std::uint32_t modposition)
{
	ParserResult pResult;

	CLIResult crres = pResult.CreateResult(element, kChildParameterGroupXpathExpression,
								kFormatStrChildParameterGroupXpathExpression,
								parameterGroupNode);
	if (!crres.IsSuccessful())
	{
		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, crres);
	}
	else
	{
		for (std::int32_t row = 0; row < pResult.parameters.size(); row++)
		{
			if (pResult.parameters[row].at(1).empty())	/**< Is conditionalUniqueIDRef empty */
			{
				Result res = OpenConfiguratorCore::GetInstance().CreateParameterGroup(
						OpenConfiguratorCLI::GetInstance().networkName,
						nodeId,
						pResult.parameters[row].at(0), 									/**< uniqueID */
						paramGroupUId,
						(std::uint8_t)std::stoi(pResult.parameters[row].at(3).c_str()),	/**< bitOffset */
						interfaceId, modId, modposition);
				if (!res.IsSuccessful())
				{
					return CLILogger::GetInstance().HandleCoreApiFailed("Create Parameter Group", res);
				}
			}
			else
			{
				Result res = OpenConfiguratorCore::GetInstance().CreateParameterGroup(
						OpenConfiguratorCLI::GetInstance().networkName,
						nodeId,
						pResult.parameters[row].at(0), 									/**< uniqueID */
						paramGroupUId,
						pResult.parameters[row].at(1), 									/**< conditionalUniqueIDRef */
						pResult.parameters[row].at(2), 									/**< conditionalValue */
						(std::uint8_t)std::stoi(pResult.parameters[row].at(3).c_str()),	/**< bitOffset */
						interfaceId, modId, modposition);
				if (!res.IsSuccessful())
				{
					return CLILogger::GetInstance().HandleCoreApiFailed("Create Parameter Group", res);
				}
			}

			ParserResult subpResult;

			CLIResult subcrres = subpResult.CreateResult(element, kChildParameterRefXpathExpression,
										kFormatStrChildParameterRefXpathExpression,
										pResult.node.at(row));
			if (!subcrres.IsSuccessful())
			{
				CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
			}
			else
			{
				for (std::int32_t subrow = 0; subrow < subpResult.parameters.size(); subrow++)
				{
					if (subpResult.parameters[subrow].at(2).empty())	/**< Is actualValue empty */
					{
						Result res = OpenConfiguratorCore::GetInstance().CreateParameterReference(
										OpenConfiguratorCLI::GetInstance().networkName,
										nodeId, paramGroupUId,
										subpResult.parameters[subrow].at(0),									/**< uniqueIDRef */
										"",
										(std::uint8_t)std::stoi(subpResult.parameters[subrow].at(3).c_str()), 	/**< bitOffset */
										interfaceId, modId, modposition);
						if (!res.IsSuccessful())
						{
							return CLILogger::GetInstance().HandleCoreApiFailed("Create Parameter References", res);
						}
					}
					else
					{
						Result res = OpenConfiguratorCore::GetInstance().CreateParameterReference(
										OpenConfiguratorCLI::GetInstance().networkName,
										nodeId,
										pResult.parameters[row].at(0),											/**< uniqueID */
										subpResult.parameters[subrow].at(0),									/**< uniqueIDRef */
										subpResult.parameters[subrow].at(2),  									/**< actualValue */
										(std::uint8_t)std::stoi(subpResult.parameters[subrow].at(3).c_str()), 	/**< bitOffset */
										interfaceId, modId, modposition);
						if (!res.IsSuccessful())
						{
							return CLILogger::GetInstance().HandleCoreApiFailed("Create Parameter References", res);
						}
					}
				}
			}
		}
	}

	return CLIResult();
}

CLIResult ProjectParser::CreateParameterGroup(ParserElement& element, std::uint8_t nodeId, 
												std::string interfaceId, 
												std::string modId, 
												std::uint32_t modposition)
{
	ParserResult pResult;
	LOG_DEBUG() << "Parameter group available";
	CLIResult crres = pResult.CreateResult(element, kParameterGroupXpathExpression,
											kFormatStrParameterGroupXpathExpression);
	if (!crres.IsSuccessful())
	{
		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, crres);
	}
	else
	{
		for (std::int32_t row = 0; row < pResult.parameters.size(); row++)
		{
			LOG_DEBUG() << "Parameter group Entered";
			Result res = OpenConfiguratorCore::GetInstance().CreateParameterGroup(
					OpenConfiguratorCLI::GetInstance().networkName,
					nodeId,
					pResult.parameters[row].at(0), 				/**< uniqueID */
					interfaceId, modId, modposition);
			if (!res.IsSuccessful())
			{
				return CLILogger::GetInstance().HandleCoreApiFailed("Create Parameter Group", res);
			}

			CLIResult subcrres;
			subcrres = CreateChildParameterGroup(element, nodeId, pResult.node.at(row), pResult.parameters[row].at(0),interfaceId, modId, modposition);
			if (!subcrres.IsSuccessful())
			{
				CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
			}

			ParserResult subpResult;

			subcrres = subpResult.CreateResult(element, kChildParameterRefXpathExpression,
										kFormatStrChildParameterRefXpathExpression,
										pResult.node.at(row));
			if (!subcrres.IsSuccessful())
			{
				CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
			}
			else
			{
				for (std::int32_t subrow = 0; subrow < subpResult.parameters.size(); subrow++)
				{
					LOG_DEBUG() << "Parameter Reference Entered";
					Result res = OpenConfiguratorCore::GetInstance().CreateParameterReference(
							OpenConfiguratorCLI::GetInstance().networkName,
							nodeId,
							pResult.parameters[row].at(0),											/**< uniqueID */
							subpResult.parameters[subrow].at(0),									/**< uniqueIDRef */
							subpResult.parameters[subrow].at(2),  									/**< actualValue */
							(std::uint8_t)std::stoi(subpResult.parameters[subrow].at(3).c_str()), 	/**< bitOffset */
							interfaceId, modId, modposition);
					LOG_DEBUG() << "Parameter Reference left";
					if (!res.IsSuccessful())
					{
						return CLILogger::GetInstance().HandleCoreApiFailed("Create Parameter Reference", res);
					}
				}
			}
		}
	}

	return CLIResult();
}

CLIResult ProjectParser::CreateParameterList(ParserElement& element, std::uint8_t nodeId, 
												std::string interfaceId, 
												std::string modId, 
												std::uint32_t modposition)
{
	ParserResult pResult;

	CLIResult crres = pResult.CreateResult(element, kParameterXpathExpression,
											kFormatStrParameterXpathExpression);
	if (!crres.IsSuccessful())
	{
		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, crres);
	}
	else
	{
		for (std::int32_t row = 0; row < pResult.parameters.size(); row++)
		{
			ParameterAccess access = GetParameterAccess(pResult.parameters[row].at(1));
			IEC_Datatype data = GetIECDataType(pResult.node.at(row));

			if (!pResult.parameters[row].at(2).empty())		/**< Is templateIDRef not empty */
			{
				Result res = OpenConfiguratorCore::GetInstance().CreateParameter(
						OpenConfiguratorCLI::GetInstance().networkName,
						nodeId,
						pResult.parameters[row].at(0), 		/**< uniqueID */
						access,
						pResult.parameters[row].at(2),		/* templateIDRef */
						interfaceId,
						modId,
						modposition);
				if (!res.IsSuccessful())
				{
					return CLILogger::GetInstance().HandleCoreApiFailed("Create Parameter", res);
				}
			}
			else
			{
				ParserResult subpResult;

				CLIResult subcrres = subpResult.CreateResult(element, kParameterDataTypeIdRefXpathExpression,
											kFormatStrParameterDataTypeIdRefXpathExpression,
											pResult.node.at(row));
				if (!subcrres.IsSuccessful())
				{
					LOG_ERROR() << "DataTypeIdRef not available." << std::endl;
						Result res = OpenConfiguratorCore::GetInstance().CreateParameter(
								OpenConfiguratorCLI::GetInstance().networkName,
								nodeId,
								pResult.parameters[row].at(0),		/**< UniqueID */
								access,	data,
								false, interfaceId,
								modId, modposition);
						if (!res.IsSuccessful())
						{
							return CLILogger::GetInstance().HandleCoreApiFailed("Create Parameter", res);
						}
				}
				else
				{
					for (std::int32_t subrow = 0; subrow < subpResult.parameters.size(); subrow++)
						{
							ParameterAccess paramAccess = GetParameterAccess(subpResult.parameters[subrow].at(1));

							Result res = OpenConfiguratorCore::GetInstance().CreateParameter(
									OpenConfiguratorCLI::GetInstance().networkName,
									nodeId,
									pResult.parameters[row].at(0),					/**< uniqueID */
									subpResult.parameters[subrow].at(0),			/**< uniqueID of subresult */
									paramAccess,
									false,
									interfaceId,
									modId,
									modposition);
							if (!res.IsSuccessful())
							{
								return CLILogger::GetInstance().HandleCoreApiFailed("Create Parameter", res);
							}
						}
					
				}
			}

			CLIResult subcrres;

			/**< Updates the allowed values of parameter.*/
			subcrres = SetParamAllowedValue(element, nodeId, pResult.node.at(row), pResult.parameters[row].at(0), interfaceId, modId, modposition);
			if (!subcrres.IsSuccessful())
			{
				CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
			}

			/**< Updates the allowed range of parameter.*/
			subcrres = SetParamAllowedRange(element, nodeId, pResult.node.at(row), pResult.parameters[row].at(0), interfaceId, modId, modposition);
			if (!subcrres.IsSuccessful())
			{
				CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
			}

			/**< Updates the default value of parameter.*/
			subcrres = SetParamDefaultValue(element, nodeId, pResult.node.at(row), pResult.parameters[row].at(0), interfaceId, modId, modposition);
			if (!subcrres.IsSuccessful())
			{
				CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
			}

			/**< Updates the actual value of parameter.*/
			subcrres = SetParamActualValue(element, nodeId, pResult.node.at(row), pResult.parameters[row].at(0), interfaceId, modId, modposition);
			if (!subcrres.IsSuccessful())
			{
				CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
			}
		}
	}

	return CLIResult();
}

CLIResult ProjectParser::ImportProfileBodyDevicePOWERLINK(ParserElement& element, std::uint8_t nodeId, 
														  std::string interfaceId, std::string modId, 
														  std::uint32_t modposition)
{
	CLIResult subcrres;

	/**< Creates datatype in the core library */
	subcrres = CreateDataType(element, nodeId, interfaceId, modId, modposition);
	if (!subcrres.IsSuccessful())
	{
		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
	}

	/**< Creates parameter template of node into the core library */
	subcrres = CreateParameterTemplate(element, nodeId, interfaceId, modId, modposition);
	if (!subcrres.IsSuccessful())
	{
		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
	}

	/**< Creates parameter list of node into the core library */
	subcrres = CreateParameterList(element, nodeId, interfaceId, modId, modposition);
	if (!subcrres.IsSuccessful())
	{
		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
	}

	/**< Creates parameter Group of node into the core library */
	subcrres = CreateParameterGroup(element, nodeId, interfaceId, modId, modposition);
	if (!subcrres.IsSuccessful())
	{
		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
	}

	return CLIResult();
}

CLIResult ProjectParser::CreateObject(ParserElement& element, std::uint8_t nodeId,
									  std::vector<std::string> forcedObject, 
									  std::vector<std::string> forcedSubObject)
{
	ParserResult pResult;

	CLIResult crres = pResult.CreateResult(element, kObjectXpathExpression,
											kFormatStrObjectXpathExpression);
	if (!crres.IsSuccessful())
	{
		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, crres);
	}
	else
	{
		for (std::int32_t row = 0; row < pResult.parameters.size(); row++)
		{
			std::uint32_t objId = (std::uint32_t)std::strtol(pResult.parameters[row].at(0).c_str(), NULL, 16);
			std::uint8_t objIdType = (std::uint8_t)std::stoi(pResult.parameters[row].at(2).c_str());
			std::string accessType = pResult.parameters[row].at(5);
			std::string dataType = pResult.parameters[row].at(4);
			std::string uniqueIdRef = pResult.parameters[row].at(8);
			std::string actualValue = pResult.parameters[row].at(7);

			if ((accessType == "ro") || (accessType == "const"))
			{
				actualValue = "";
			}

			if ((!dataType.empty()) && (uniqueIdRef.empty()))
			{
				Result res = OpenConfiguratorCore::GetInstance().CreateObject(
						OpenConfiguratorCLI::GetInstance().networkName,
						nodeId, objId,
						GetObjectType(objIdType),
						pResult.parameters[row].at(1),					/**< name */
						GetPLKDataType(dataType),
						GetObjAccessType(accessType),
						GetPDOMapping(pResult.parameters[row].at(6)), 	/**< PDOmapping */
						pResult.parameters[row].at(3),                  /**< defaultValue */
						actualValue);
				if (!res.IsSuccessful())
				{
					return CLILogger::GetInstance().HandleCoreApiFailed("Create Object", res);
				}
				else
				{
					if ((!pResult.parameters[row].at(9).empty()) || (!pResult.parameters[row].at(10).empty()))
					{
						Result res = OpenConfiguratorCore::GetInstance().SetObjectLimits(
								OpenConfiguratorCLI::GetInstance().networkName,
								nodeId, objId,
								pResult.parameters[row].at(9),				/**< lowLimit */
								pResult.parameters[row].at(10));			/**< highLimit */
						if (!res.IsSuccessful())
						{
							return CLILogger::GetInstance().HandleCoreApiFailed("Set Object Limits", res);
						}
					}
				}
			}
			else if ((!dataType.empty()) && (!uniqueIdRef.empty()))
			{
				Result res = OpenConfiguratorCore::GetInstance().CreateParameterObject(
						OpenConfiguratorCLI::GetInstance().networkName,
						nodeId, objId, GetObjectType(objIdType),
						pResult.parameters[row].at(1),					/**< name */
						GetPLKDataType(dataType),
						GetObjAccessType(accessType),
						GetPDOMapping(pResult.parameters[row].at(6)), 	/**< PDOmapping */
						uniqueIdRef,
						pResult.parameters[row].at(3),                  /**< defaultValue */
						actualValue);
				if (!res.IsSuccessful())
				{
					return CLILogger::GetInstance().HandleCoreApiFailed("Create Parameter Object", res);
				}
			}
			else if ((dataType.empty()) && (!uniqueIdRef.empty()))
			{
				Result res = OpenConfiguratorCore::GetInstance().CreateParameterObject(
						OpenConfiguratorCLI::GetInstance().networkName,
						nodeId, objId,
						GetObjectType(objIdType),
						pResult.parameters[row].at(1),					/**< name */
						PlkDataType::UNDEFINED,
						GetObjAccessType(accessType),
						GetPDOMapping(pResult.parameters[row].at(6)), 	/**< PDOmapping */
						uniqueIdRef,
						pResult.parameters[row].at(3),                  /**< defaultValue */
						actualValue);
				if (!res.IsSuccessful())
				{
					return CLILogger::GetInstance().HandleCoreApiFailed("Create Parameter Object", res);
				}

			}
			else if ((dataType.empty()) && (uniqueIdRef.empty()))
			{
				Result res = OpenConfiguratorCore::GetInstance().CreateObject(
						OpenConfiguratorCLI::GetInstance().networkName,
						nodeId, objId, GetObjectType(objIdType),
						pResult.parameters[row].at(1),					/**< name */
						GetPLKDataType(dataType),
						GetObjAccessType(accessType),
						GetPDOMapping(pResult.parameters[row].at(6)), 	/**< PDOmapping */
						pResult.parameters[row].at(3),                  /**< defaultValue */
						actualValue);
				if (!res.IsSuccessful())
				{
					return CLILogger::GetInstance().HandleCoreApiFailed("Create Object", res);
				}
				else
				{
					if ((!pResult.parameters[row].at(9).empty()) || (!pResult.parameters[row].at(10).empty()))
					{
						Result res = OpenConfiguratorCore::GetInstance().SetObjectLimits(
								OpenConfiguratorCLI::GetInstance().networkName,
								nodeId, objId,
								pResult.parameters[row].at(9),			/**< lowLimit */
								pResult.parameters[row].at(10));		/**< highLimit */
						if (!res.IsSuccessful())
						{
							return CLILogger::GetInstance().HandleCoreApiFailed("Set Object Limits", res);
						}
					}
				}
			}

			ParserResult subpResult;

			CLIResult subcrres = subpResult.CreateResult(element, kSubObjectXpathExpression,
											kFormatStrSubObjectXpathExpression,
											pResult.node.at(row));
			if (!subcrres.IsSuccessful())
			{
				CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
			}
			else
			{
				for (std::int32_t subrow = 0; subrow < subpResult.parameters.size(); subrow++)
				{
					std::uint8_t subObjId = (std::uint8_t)std::strtol(subpResult.parameters[subrow].at(0).c_str(), NULL, 16);
					std::uint8_t subObjIdType = (std::uint8_t)std::stoi(subpResult.parameters[subrow].at(2).c_str());
					std::string subObjaccessType = subpResult.parameters[subrow].at(5);
					std::string subObjActualValue = subpResult.parameters[subrow].at(7);
					std::string subObjuniqueIdRef = subpResult.parameters[subrow].at(8);
					std::string subObjdataType = subpResult.parameters[subrow].at(4);

					if ((subObjaccessType == "ro") || (subObjaccessType == "const"))
					{
						subObjActualValue = "";
					}

					if (subpResult.parameters[subrow].at(8).empty())						/**< Is uniqueIdRef empty */
					{
						Result res = OpenConfiguratorCore::GetInstance().CreateSubObject(
								OpenConfiguratorCLI::GetInstance().networkName,
								nodeId, objId, subObjId, GetObjectType(subObjIdType),
								subpResult.parameters[subrow].at(1),					/**< sub object name */
								GetPLKDataType(subpResult.parameters[subrow].at(4)), 	/**< sub object dataType */
								GetObjAccessType(subObjaccessType),
								GetPDOMapping(subpResult.parameters[subrow].at(6)),		/**< sub object PDOmapping */
								subpResult.parameters[subrow].at(3),					/**< sub object defaultValue */
								subObjActualValue);
						if (!res.IsSuccessful())
						{
							return CLILogger::GetInstance().HandleCoreApiFailed("Create Sub Object", res);
						}
						else
						{
							/**< If any one of the sub object lowLimit and highLimit has values */
							if ((!subpResult.parameters[subrow].at(9).empty()) || (!subpResult.parameters[subrow].at(10).empty()))
							{
								Result res = OpenConfiguratorCore::GetInstance().SetSubObjectLimits(
										OpenConfiguratorCLI::GetInstance().networkName,
										nodeId, objId, subObjId,
										subpResult.parameters[subrow].at(9),			/**< sub object lowLimit */
										subpResult.parameters[subrow].at(10));			/**< sub object highLimit */
								if (!res.IsSuccessful())
								{
									return CLILogger::GetInstance().HandleCoreApiFailed("Create Sub Object Limits", res);
								}
							}
						}
					}
					else if ((!subObjuniqueIdRef.empty()) && (!subObjdataType.empty()))
					{
						/**< Both sub object uniqueIdRef and dataType has values */
						Result res = OpenConfiguratorCore::GetInstance().CreateParameterSubObject(
								OpenConfiguratorCLI::GetInstance().networkName,
								nodeId, objId, subObjId,
								GetObjectType(subObjIdType),
								subpResult.parameters[subrow].at(1),					/**< sub object name */
								GetPLKDataType(subObjdataType),
								GetObjAccessType(subObjaccessType),
								GetPDOMapping(subpResult.parameters[subrow].at(6)),		/**< sub object PDOmapping */
								subObjuniqueIdRef,
								subpResult.parameters[subrow].at(3),					/**< sub object defaultValue */
								subObjActualValue);
						if (!res.IsSuccessful())
						{
							return CLILogger::GetInstance().HandleCoreApiFailed("Create Parameter Sub Object", res);
						}
					}
					else if ((!subObjuniqueIdRef.empty()) && (subObjdataType.empty()))
					{
						/**< Both sub object uniqueIdRef and dataType has values */
						Result res = OpenConfiguratorCore::GetInstance().CreateParameterSubObject(
								OpenConfiguratorCLI::GetInstance().networkName,
								nodeId, objId, subObjId, GetObjectType(subObjIdType),
								subpResult.parameters[subrow].at(1),					/**< sub object name */
								PlkDataType::UNDEFINED,
								GetObjAccessType(subObjaccessType),
								GetPDOMapping(subpResult.parameters[subrow].at(6)),		/**< sub object PDOmapping */
								subObjuniqueIdRef,
								subpResult.parameters[subrow].at(3),					/**< sub object defaultValue */
								subObjActualValue);
						if (!res.IsSuccessful())
						{
							return CLILogger::GetInstance().HandleCoreApiFailed("Create Parameter Sub Object", res);
						}
					}
					CLIResult cliRes = 	UpdateForcedObjects(forcedObject, forcedSubObject, nodeId,
														pResult.parameters[row].at(0), subpResult.parameters[subrow].at(0), 
														objId, subObjId, actualValue, subObjActualValue,false,"","",0U);
					if (!cliRes.IsSuccessful())
					{
						CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, cliRes);
					}
				}
			}
		}
	}

	return CLIResult();
}


CLIResult ProjectParser::ImportProfileBodyCommunicationPOWERLINK(ParserElement& element, std::uint8_t nodeId, 
																 std::vector<std::string> forcedObject, 
																 std::vector<std::string> forcedSubObject, 
																 std::string interfaceId, std::string modId, 
																 std::uint32_t modposition)
{
	CLIResult subcrres;

	/**< Creates objest in the core library */
	if(modId.empty())
	{
		subcrres = CreateObject(element, nodeId, forcedObject, 
								forcedSubObject);
		if (!subcrres.IsSuccessful())
		{
			CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
		}
	}
	else
	{
		subcrres = CreateModuleObject(element, nodeId, forcedObject, forcedSubObject, interfaceId, modId, modposition);
		if (!subcrres.IsSuccessful())
		{
			CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
		}
	}

	return CLIResult();
}

CLIResult ProjectParser::CreateNetworkManagementGeneralFeatures(ParserElement& element, std::uint8_t nodeId)
{
	ParserResult pResult;

	CLIResult crres = pResult.CreateResult(element,
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
					OpenConfiguratorCLI::GetInstance().networkName,
					nodeId,
					((GeneralFeatureEnum) param),
					pResult.parameters[row].at(param).c_str());

				if (!res.IsSuccessful())
				{
					return CLILogger::GetInstance().HandleCoreApiFailed("Set Feature Value", res);
				}
			}
		}
	}

	return CLIResult();
}

CLIResult ProjectParser::CreateNetworkManagementMnFeatures(ParserElement& element, std::uint8_t nodeId)
{
	ParserResult pResult;

	CLIResult crres = pResult.CreateResult(element,
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
					OpenConfiguratorCLI::GetInstance().networkName,
					nodeId,
					(MNFeatureEnum) (param + (std::int32_t) MNFeatureEnum::DLLErrMNMultipleMN),
					pResult.parameters[row].at(param).c_str());
				if (!res.IsSuccessful())
				{
					return CLILogger::GetInstance().HandleCoreApiFailed("Set Feature Value", res);
				}
			}
		}
	}

	return CLIResult();
}

CLIResult ProjectParser::CreateNetworkManagementCnFeatures(ParserElement& element, std::uint8_t nodeId)
{
	ParserResult pResult;

	CLIResult crres = pResult.CreateResult(element,
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
					OpenConfiguratorCLI::GetInstance().networkName,
					nodeId,
					(CNFeatureEnum) (param + (std::int32_t) CNFeatureEnum::DLLCNFeatureMultiplex),
					pResult.parameters[row].at(param).c_str());
				if (!res.IsSuccessful())
				{
					return CLILogger::GetInstance().HandleCoreApiFailed("Set Feature Value", res);
				}
			}
		}
	}

	return CLIResult();
}

CLIResult ProjectParser::CreateInterface(std::uint8_t nodeId, const std::string& cnXDC)
{
	std::string indexString1 = "/" + cnXDC;
	std::string initialPath = OpenConfiguratorCLI::GetInstance().xmlFilePath;
	std::string nextInitialPath = initialPath.substr(0, initialPath.find_last_of("\\/"));
	std::string fullPath = nextInitialPath.append(indexString1);

	CLIResult res;

	res = ParameterValidator::GetInstance().IsXDCSchemaValid(nextInitialPath);
	if (!res.IsSuccessful())
	{
		/**< XDC file schema is not valid */
		return res;
	}

	ParserElement element;
	CLIResult ceres;

	ceres = element.CreateElement(nextInitialPath);
	if (!ceres.IsSuccessful())
	{
		return ceres;
	}
	else
	{
		ParserResult pResult;

		CLIResult res = pResult.CreateResult(element, kInterfaceXDCxpathExpression.c_str(),
												kFormatStrInterfaceXDCxpathExpression);
		if (!res.IsSuccessful())
		{
			return res;
		}
		else
		{
			for (std::int32_t row = 0; row < pResult.parameters.size(); row++)
			{
					ModuleAddressing modAddress = GetModuleAddressing(pResult.parameters[row].at(2));							/*< Module addressing */
					std::uint32_t maxModules = (std::uint32_t)std::strtol(pResult.parameters[row].at(3).c_str(), NULL, 16);		/*< Max Modules */	

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

						/**< Core Library API call to create Interface */
						Result res = OpenConfiguratorCore::GetInstance().CreateInterface(
									OpenConfiguratorCLI::GetInstance().GetNetworkName(),
									nodeId, 
									pResult.parameters[row].at(0),			/**< Interface uniqueId */
									pResult.parameters[row].at(1),			/**< Interface type */
									modAddress,
									maxModules,
									unusedSlot,
									multipleMod);

						if (!res.IsSuccessful())
						{
							return CLILogger::GetInstance().HandleCoreApiFailed("Create Interface", res);
						}
										
						ParserResult subPresult;

						CLIResult cliRes = subPresult.CreateResult(element,
												kAppLayerInterfaceXpathExpression,
												"uniqueIDRef");

						if (!cliRes.IsSuccessful())
						{
							return cliRes;
						}
						else
						{
							for(std::uint32_t row = 0; row < subPresult.parameters.size(); row++)
							{
								ParserResult subRangeResult;

								CLIResult rescli = subRangeResult.CreateResult(element,kInterfaceRangeXpathExpression,
																				kFormatStrInterfaceRangeXpathExpression);
								if (!rescli.IsSuccessful())
								{
									return rescli;
								}
								else
								{
									for(std::uint32_t subRow = 0 ; subRow < subRangeResult.parameters.size(); subRow++)
									{
										std::uint32_t baseIndex = (std::uint32_t)std::strtol(subRangeResult.parameters[subRow].at(1).c_str(), NULL, 16);			/**< Base index value */
										std::uint32_t maxIndex = (std::uint32_t)std::strtol(subRangeResult.parameters[subRow].at(2).c_str(), NULL, 16);				/**< Max index value */
										std::uint32_t maxSubIndex = (std::uint32_t)std::strtol(subRangeResult.parameters[subRow].at(3).c_str(), NULL, 16);			/**< Max Sub index value */
										SortMode sortmodRange = GetSortMode(subRangeResult.parameters[subRow].at(4));												/**< Sort mode value */
										SortNumber sortRangeNumber = GetSortNumber(subRangeResult.parameters[subRow].at(5));										/**< Sort number value */
										PDOMapping maping = GetPDOMapping(subRangeResult.parameters[subRow].at(6));													/**< PDO mapping value */
										std::uint32_t rangeSortStep = (std::uint32_t)std::strtol(subRangeResult.parameters[subRow].at(7).c_str(), NULL, 16);		/**< Sort step value */

										Result res = OpenConfiguratorCore::GetInstance().CreateRange(
															OpenConfiguratorCLI::GetInstance().GetNetworkName(),
															nodeId, 
															subPresult.parameters[row].at(0),					/**< Interface uniqueId */			
															subRangeResult.parameters[subRow].at(0),			/**< Range name */	
															baseIndex, 
															maxIndex, 
															maxSubIndex, 
															rangeSortStep, 
															sortmodRange, 
															sortRangeNumber,
															maping);
										if (!res.IsSuccessful())
										{
											return CLILogger::GetInstance().HandleCoreApiFailed("Create Range", res);
										}
									}
								}
							}
						}
					}
				}
			}
	return CLIResult();
}

CLIResult ProjectParser::UpdateNodeIdCollection(std::uint8_t nodeId, const std::string& cnXDC, 
												std::vector<std::string> forcedObject, 
												std::vector<std::string> forcedSubObject, 
												std::string interfaceId, std::string modId, 
												std::uint32_t modposition)
{
	std::string indexString1 = "/" + cnXDC;
	std::string initialPath = OpenConfiguratorCLI::GetInstance().xmlFilePath;
	std::string nextInitialPath = initialPath.substr(0, initialPath.find_last_of("\\/"));
	std::string fullPath = nextInitialPath.append(indexString1);

	CLIResult res;

	res = ParameterValidator::GetInstance().IsXDCSchemaValid(nextInitialPath);
	if (!res.IsSuccessful())
	{
		/**< XDC file schema is not valid */
		return res;
	}

	ParserElement element;
	CLIResult ceres;

	ceres = element.CreateElement(nextInitialPath);
	if (!ceres.IsSuccessful())
	{
		return ceres;
	}
	else
	{
		res = ImportProfileBodyDevicePOWERLINK(element, nodeId, interfaceId, modId, modposition);
		if (!res.IsSuccessful())
		{
			CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);
		}

		res = ImportProfileBodyCommunicationPOWERLINK(element, nodeId, forcedObject, forcedSubObject, interfaceId, modId, modposition);
		if (!res.IsSuccessful())
		{
			CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);
		}

		if(modId.empty())
		{
			res = CreateNetworkManagementGeneralFeatures(element, nodeId);
			if (!res.IsSuccessful())
			{
				CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);
			}

			res = CreateNetworkManagementMnFeatures(element, nodeId);
			if (!res.IsSuccessful())
			{
				CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);
			}

			res = CreateNetworkManagementCnFeatures(element, nodeId);
			if (!res.IsSuccessful())
			{
				CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);
			}
		}
	}

	return CLIResult();
}

CLIResult ProjectParser::CreateNodeAssignment(ParserElement& pElement, std::string xPathExpression, std::uint8_t nodeId)
{
	ParserResult pResult;

	CLIResult crres = pResult.CreateResult(pElement, xPathExpression.c_str(),
											kFormatStrxPathExpression);

	if (!crres.IsSuccessful())
	{
		return crres;
	}
	else
	{
		if(nodeId == MN_DEFAULT_NODE_ID)
		{
			CLIResult res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_MN_PRES, 
												nodeId, 
												pResult.parameters[0].at(0));	/** TaransmitPres value **/
			if (!res.IsSuccessful())
			{
				CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);
			}
		}
		else
		{
			if(nodeId > MN_DEFAULT_NODE_ID)
			{
				CLIResult res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_NODE_IS_CN, nodeId, "true");
				if (!res.IsSuccessful())
				{
					CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);
				}
			}
			else
			{
				CLIResult res;

				res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_NODE_IS_CN, nodeId, "true");
				if (!res.IsSuccessful())
				{
					CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);
				}

				res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_MANDATORY_CN, nodeId, pResult.parameters[0].at(1));		/** isMandatory value **/
				if (!res.IsSuccessful())
				{
					CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);
				}

				res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_START_CN, nodeId, pResult.parameters[0].at(2));			/** autostartNode value **/
				if (!res.IsSuccessful())
				{
					CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);
				}

				res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_KEEPALIVE, nodeId, pResult.parameters[0].at(3));			/** resetInOperational value **/
				if (!res.IsSuccessful())
				{
					CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);
				}

				res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_SWVERSIONCHECK, nodeId, pResult.parameters[0].at(4));	/** verifyAppSwVersion value **/
				if (!res.IsSuccessful())
				{
					CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);
				}

				res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_SWUPDATE, nodeId, pResult.parameters[0].at(5));			/** autoAppSwUpdateAllowed value **/
				if (!res.IsSuccessful())
				{
					CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);
				}

				res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_ASYNCONLY_NODE, nodeId, pResult.parameters[0].at(7));	/** isAsyncOnly value ***/
				if (!res.IsSuccessful())
				{
					CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);
				}

				res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_NODE_EXISTS, nodeId, "true");
				if (!res.IsSuccessful())
				{
					CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);
				}

				res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_RT1, nodeId, pResult.parameters[0].at(8));				/** isType1Router value **/
				if (!res.IsSuccessful())
				{
					CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);
				}

				res = SetNodeAssignment(NodeAssignment::NMT_NODEASSIGN_RT2, nodeId, pResult.parameters[0].at(9));				/* isType2Router value **/
				if (!res.IsSuccessful())
				{
					CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);
				}

				if(pResult.parameters[0].at(6).compare("true") == 0)															/** isChained value **/
				{
					Result res = OpenConfiguratorCore::GetInstance().SetOperationModeChained(
										OpenConfiguratorCLI::GetInstance().networkName, nodeId);
					if (!res.IsSuccessful())
					{
						return CLILogger::GetInstance().HandleCoreApiFailed("Set Operation Mode Chained", res);
					}
				}
			}
		}
	}

	return CLIResult();
}

CLIResult ProjectParser::SetNodeAssignment(NodeAssignment nodeassign, std::uint8_t nodeId, std::string value)
{
	if (value.compare("true") == 0)
	{
		Result res = OpenConfiguratorCore::GetInstance().AddNodeAssignment(
							OpenConfiguratorCLI::GetInstance().networkName, nodeId, nodeassign);
		if (!res.IsSuccessful())
		{
			return CLILogger::GetInstance().HandleCoreApiFailed("Add Node Assignment", res);
		}
	}
	else
	{
		Result res = OpenConfiguratorCore::GetInstance().RemoveNodeAssignment(
							OpenConfiguratorCLI::GetInstance().networkName, nodeId, nodeassign);
		if (!res.IsSuccessful())
		{
			return CLILogger::GetInstance().HandleCoreApiFailed("Remove Node Assignment", res);
		}
	}

	return CLIResult();
}

ModuleAddressing ProjectParser::GetModuleAddressing(std::string moduleAddressing)
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

SortNumber ProjectParser::GetSortNumber(std::string sortNumber)
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

SortMode ProjectParser::GetSortMode(std::string sortMode)
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

CLIResult ProjectParser::CreateModule(std::uint8_t nodeId, std::string interfacecUId, 
									  std::string moduleName, std::string modulePosition, 
									  std::string moduleAddress, std::string modulePathToXDC, 
									  std::string moduleEnabled)
{
	std::string indexString1 = "/" + modulePathToXDC;
	std::string initialPath = OpenConfiguratorCLI::GetInstance().xmlFilePath;
	std::string nextInitialPath = initialPath.substr(0, initialPath.find_last_of("\\/"));
	std::string fullPath = nextInitialPath.append(indexString1);

	CLIResult res;

	res = ParameterValidator::GetInstance().IsXDCSchemaValid(nextInitialPath);
	if (!res.IsSuccessful())
	{
		/**< XDC file schema is not valid */
		return res;
	}

	ParserElement element;
	CLIResult ceres;

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
				ModuleAddressing modAddressing = GetModuleAddressing(pResult.parameters[moduleSubRow].at(2));			/** Module addressing **/
				std::uint8_t minPosition = (std::uint8_t)std::stoi(pResult.parameters[moduleSubRow].at(3).c_str());		/** Module min position **/
				std::uint8_t maxPosition = (std::uint8_t)std::stoi(pResult.parameters[moduleSubRow].at(4).c_str());		/** Module max position **/
				std::uint8_t minAddress = (std::uint8_t)std::stoi(pResult.parameters[moduleSubRow].at(5).c_str());		/** Module min adress **/
				std::uint8_t maxAddress = (std::uint8_t)std::stoi(pResult.parameters[moduleSubRow].at(6).c_str());		/** Module max address **/
				std::uint8_t maxCount = (std::uint8_t)std::stoi(pResult.parameters[moduleSubRow].at(7).c_str());		/** Module max count **/
				std::uint32_t moduleposition = (std::uint32_t)std::strtol(modulePosition.c_str(), NULL, 16);
				std::uint32_t moduleAdrs = (std::uint32_t)std::strtol(moduleAddress.c_str(), NULL, 16);

				Result res = OpenConfiguratorCore::GetInstance().CreateModule(OpenConfiguratorCLI::GetInstance().networkName, 
																			nodeId, 
																			interfacecUId, 
																			pResult.parameters[moduleSubRow].at(0),		/** Module Child ID **/ 
																			moduleposition, 
																			moduleAdrs, 
																			pResult.parameters[moduleSubRow].at(1),		/** Module type **/
																			moduleName, 
																			modAddressing, 
																			minPosition, 
																			maxPosition, 
																			minAddress, 
																			maxAddress, 
																			maxCount);
				if (!res.IsSuccessful())
				{
					return CLILogger::GetInstance().HandleCoreApiFailed("Create Module", res);
				}

				CLIResult clires = UpdateNodeIdCollection(nodeId, modulePathToXDC, forcedModularObj, 
														forcedModularSubObj , interfacecUId, 
														pResult.parameters[moduleSubRow].at(0), 
														moduleposition);
				if (!clires.IsSuccessful())
				{
					CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, clires);
				}
			}
		}
	}

	return CLIResult();
}

CLIResult ProjectParser::CreateModuleObject(ParserElement & element, std::uint8_t nodeId, 
											std::vector<std::string> forcedObject, 
											std::vector<std::string> forcedSubObject, 
											std::string interfaceId, std::string modId, 
											std::uint32_t modposition)
{
	ParserResult pResult;
	
	CLIResult clires = pResult.CreateResult(element, kObjectXpathExpression.c_str(),
								kFormatStrObjectXpathExpressionModule);

	if (!clires.IsSuccessful())
	{
		return clires;
	}
	else
	{
		for (std::uint32_t row = 0; row < pResult.parameters.size(); row++)
		{
			std::uint32_t objId;

			if(pResult.parameters[row].at(0).empty())						/**< Object Index */
			{
				objId = 0;
			}
			else
			{
				objId = (std::uint32_t)std::strtol(pResult.parameters[row].at(0).c_str(), NULL, 16);
			}

			std::uint8_t objIdType = (std::uint8_t)std::stoi(pResult.parameters[row].at(2).c_str());
			std::string accessType = pResult.parameters[row].at(5);			/**< Access Type */
			std::string dataType = pResult.parameters[row].at(4);			/**< data Type */
			std::string uniqueIdRef = pResult.parameters[row].at(8);		/**< UniqueId ref */
			std::string actualValue = pResult.parameters[row].at(7);		/**< Actual value */
			std:: string rangeSelector = pResult.parameters[row].at(11);	/**< Range selector */

			if ((accessType == "ro") || (accessType == "const"))
			{
				actualValue = "";
			}

			if ((!dataType.empty()) && (uniqueIdRef.empty()))
			{
				Result res = OpenConfiguratorCore::GetInstance().CreateModuleObject(
						OpenConfiguratorCLI::GetInstance().networkName,
						nodeId, interfaceId,
						modId, modposition,
						objId, GetObjectType(objIdType),
						pResult.parameters[row].at(1),						/**< name */
						GetPLKDataType(dataType),
						GetObjAccessType(accessType),
						GetPDOMapping(pResult.parameters[row].at(6)), 		/**< PDOmapping */
						pResult.parameters[row].at(3),						/**< defaultValue */
						actualValue, rangeSelector);
				if (!res.IsSuccessful())
				{
					CLIResult clires = CLILogger::GetInstance().HandleCoreApiFailed("Create Module Object", res);

					CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, clires);
				}
				else
				{
					if ((!pResult.parameters[row].at(9).empty()) || (!pResult.parameters[row].at(10).empty()))
					{
						Result res = OpenConfiguratorCore::GetInstance().SetObjectLimits(
								OpenConfiguratorCLI::GetInstance().networkName,
								nodeId, GetNewObjectIndex(nodeId, interfaceId, modId, modposition, objId),
								pResult.parameters[row].at(9),				/**< lowLimit */
								pResult.parameters[row].at(10));			/**< highLimit */
						if (!res.IsSuccessful())
						{
							CLIResult clires = CLILogger::GetInstance().HandleCoreApiFailed("Set Object Limits", res);

							CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, clires);
						}
					}
				}
			}
			else if ((!dataType.empty()) && (!uniqueIdRef.empty()))
			{
				Result res = OpenConfiguratorCore::GetInstance().CreateModuleParameterObject(
						OpenConfiguratorCLI::GetInstance().networkName,
						nodeId, interfaceId,
						modId, modposition,
						objId, GetObjectType(objIdType),
						pResult.parameters[row].at(1),						/**< name */ 
						GetPLKDataType(dataType),
						GetObjAccessType(accessType),
						GetPDOMapping(pResult.parameters[row].at(6)), 		/**< PDOmapping */
						uniqueIdRef,
						pResult.parameters[row].at(3),						/**< defaultValue */
						actualValue, rangeSelector);
				if (!res.IsSuccessful())
				{
					CLIResult clires = CLILogger::GetInstance().HandleCoreApiFailed("Create Module Parameter Object", res);

					CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, clires);
				}
			}
			else if ((dataType.empty()) && (!uniqueIdRef.empty()))
			{
				Result res = OpenConfiguratorCore::GetInstance().CreateModuleParameterObject(
						OpenConfiguratorCLI::GetInstance().networkName,
						nodeId, interfaceId,
						modId, modposition,
						objId,
						GetObjectType(objIdType),
						pResult.parameters[row].at(1),						/**< name */
						PlkDataType::UNDEFINED,
						GetObjAccessType(accessType),
						GetPDOMapping(pResult.parameters[row].at(6)), 		/**< PDOmapping */
						uniqueIdRef,
						pResult.parameters[row].at(3),						/**< defaultValue */
						actualValue,rangeSelector);
				if (!res.IsSuccessful())
				{
					CLIResult clires = CLILogger::GetInstance().HandleCoreApiFailed("Create Module Parameter Object", res);

					CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, clires);
				}

			}
			else if ((dataType.empty()) && (uniqueIdRef.empty()))
			{
				Result res = OpenConfiguratorCore::GetInstance().CreateModuleObject(
						OpenConfiguratorCLI::GetInstance().networkName,
						nodeId, interfaceId,
						modId, modposition,
						objId, GetObjectType(objIdType),
						pResult.parameters[row].at(1),						/**< name */
						GetPLKDataType(dataType),
						GetObjAccessType(accessType),
						GetPDOMapping(pResult.parameters[row].at(6)), 		/**< PDOmapping */
						pResult.parameters[row].at(3),						/**< defaultValue */
						actualValue, rangeSelector);
				if (!res.IsSuccessful())
				{
					CLIResult clires = CLILogger::GetInstance().HandleCoreApiFailed("Create Module Object", res);

					CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, clires);
				}
				else
				{
					if ((!pResult.parameters[row].at(9).empty()) || (!pResult.parameters[row].at(10).empty()))
					{
						Result res = OpenConfiguratorCore::GetInstance().SetObjectLimits(
								OpenConfiguratorCLI::GetInstance().networkName,
								nodeId, GetNewObjectIndex(nodeId, interfaceId, modId, modposition, objId),
								pResult.parameters[row].at(9),				/**< lowLimit */
								pResult.parameters[row].at(10));			/**< highLimit */
						if (!res.IsSuccessful())
						{
							CLIResult clires = CLILogger::GetInstance().HandleCoreApiFailed("Set Object Limits", res);

							CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, clires);
						}
					}
				}
			}

			ParserResult subpResult;

			CLIResult res = subpResult.CreateResult(element, kSubObjectXpathExpression.c_str(),
													kFormatStrSubObjectXpathExpression,
													pResult.node.at(row));

			if (!res.IsSuccessful())
			{
				CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);;
			}
			else
			{
				for (std::uint32_t subrow = 0; subrow < subpResult.parameters.size(); subrow++)
				{
					std::uint8_t subObjId = (std::uint8_t)std::strtol(subpResult.parameters[subrow].at(0).c_str(), NULL, 16);
					std::uint8_t subObjIdType = (std::uint8_t)std::stoi(subpResult.parameters[subrow].at(2).c_str());
					std::string subObjaccessType = subpResult.parameters[subrow].at(5);		/**< Access type */
					std::string subObjActualValue = subpResult.parameters[subrow].at(7);	/**< Actual value */
					std::string subObjuniqueIdRef = subpResult.parameters[subrow].at(8);	/**< UniqueID ref */
					std::string subObjdataType = subpResult.parameters[subrow].at(4);		/**< Data type */

					if ((subObjaccessType == "ro") || (subObjaccessType == "const"))
					{
						subObjActualValue = "";
					}

					if (subpResult.parameters[subrow].at(8).empty())						/**< Is uniqueIdRef empty */
					{
						Result res = OpenConfiguratorCore::GetInstance().CreateModuleSubObject(
								OpenConfiguratorCLI::GetInstance().networkName,
								nodeId, interfaceId,
								modId, modposition, objId, subObjId, GetObjectType(subObjIdType),
								subpResult.parameters[subrow].at(1),						/**< sub object name */
								GetPLKDataType(subpResult.parameters[subrow].at(4)), 		/**< sub object dataType */
								GetObjAccessType(subObjaccessType),
								GetPDOMapping(subpResult.parameters[subrow].at(6)),			/**< sub object PDOmapping */
								subpResult.parameters[subrow].at(3),						/**< sub object defaultValue */
								subObjActualValue);
						if (!res.IsSuccessful())
						{
							CLIResult clires = CLILogger::GetInstance().HandleCoreApiFailed("Create Module Sub Object", res);

							CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, clires);
						}
						else
						{
							/**< If any one of the sub object lowLimit and highLimit has values */
							if ((!subpResult.parameters[subrow].at(9).empty()) || (!subpResult.parameters[subrow].at(10).empty()))
							{
								Result res = OpenConfiguratorCore::GetInstance().SetSubObjectLimits(
										OpenConfiguratorCLI::GetInstance().networkName,
										nodeId, GetNewObjectIndex(nodeId, interfaceId, modId, modposition, objId),
										(std::uint8_t)GetNewSubObjectIndex(nodeId, interfaceId, modId, modposition, GetNewObjectIndex(nodeId, interfaceId, modId, modposition, objId), subObjId),
										subpResult.parameters[subrow].at(9),			/**< sub object lowLimit */
										subpResult.parameters[subrow].at(10));			/**< sub object highLimit */
								if (!res.IsSuccessful())
								{
									CLIResult clires = CLILogger::GetInstance().HandleCoreApiFailed("Set Sub Object Limits", res);

									CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, clires);
								}
							}
						}
					}
					else if ((!subObjuniqueIdRef.empty()) && (!subObjdataType.empty()))
					{
						/**< Both sub object uniqueIdRef and dataType has values */
						Result res = OpenConfiguratorCore::GetInstance().CreateModuleParameterSubObject(
								OpenConfiguratorCLI::GetInstance().networkName,
								nodeId, interfaceId,
								modId, modposition, objId, subObjId,
								GetObjectType(subObjIdType),
								subpResult.parameters[subrow].at(1),					/**< sub object name */
								GetPLKDataType(subObjdataType),
								GetObjAccessType(subObjaccessType),
								GetPDOMapping(subpResult.parameters[subrow].at(6)),		/**< sub object PDOmapping */
								subObjuniqueIdRef,
								subpResult.parameters[subrow].at(3),					/**< sub object defaultValue */
								subObjActualValue);
						if (!res.IsSuccessful())
						{
							CLIResult clires = CLILogger::GetInstance().HandleCoreApiFailed("Create Module Parameter Sub Object", res);

							CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, clires);
						}
					}
					else if ((!subObjuniqueIdRef.empty()) && (subObjdataType.empty()))
					{
						/**< Both sub object uniqueIdRef and dataType has values */
						Result res = OpenConfiguratorCore::GetInstance().CreateModuleParameterSubObject(
								OpenConfiguratorCLI::GetInstance().networkName,
								nodeId, interfaceId,
								modId, modposition, objId, subObjId, GetObjectType(subObjIdType),
								subpResult.parameters[subrow].at(1),					/**< sub object name */
								PlkDataType::UNDEFINED,
								GetObjAccessType(subObjaccessType),
								GetPDOMapping(subpResult.parameters[subrow].at(6)),		/**< sub object PDOmapping */
								subObjuniqueIdRef,
								subpResult.parameters[subrow].at(3),					/**< sub object defaultValue */
								subObjActualValue);
						if (!res.IsSuccessful())
						{
							CLIResult clires = CLILogger::GetInstance().HandleCoreApiFailed("Create Module Parameter Sub Object", res);

							CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, clires);
						}
					}
					
					CLIResult result = UpdateForcedObjects(forcedObject, forcedSubObject, nodeId,
															pResult.parameters[row].at(0), subpResult.parameters[subrow].at(0), 
															objId, subObjId, actualValue, subObjActualValue, true,
															interfaceId, modId, modposition);
					if (!result.IsSuccessful())
					{
						CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, result);
					}
				}
			}
		}
	}

	return CLIResult();
}

std::uint32_t ProjectParser::GetNewObjectIndex(std::uint8_t nodeId, std::string interfaceId, 
											   std::string modId, std::uint32_t modposition,
											   std::uint32_t objId) 
{
	std::uint32_t index = 0;
	std::int32_t subIndex = 0;

	Result res = OpenConfiguratorCore::GetInstance().GetModuleObjectCurrentIndex(OpenConfiguratorCLI::GetInstance().networkName,
																				nodeId, interfaceId, modId, 
																				modposition, objId, -1, index, subIndex);
	if(!res.IsSuccessful())
	{
		CLIResult clires = CLILogger::GetInstance().HandleCoreApiFailed("Get Module Object Current Index", res);

		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, clires);
	}

	return index;
}

std::int32_t ProjectParser::GetNewSubObjectIndex(std::uint8_t nodeId, std::string interfaceId, 
												 std::string modId, std::uint32_t modposition,
												 std::uint32_t objId, std::uint8_t subObjId) 
{
	std::uint32_t index = 0;
	std::int32_t subIndex = 0;
	std::cout << objId;
	Result res = OpenConfiguratorCore::GetInstance().GetModuleObjectCurrentIndex(OpenConfiguratorCLI::GetInstance().networkName,
																				nodeId, interfaceId, modId, 
																				modposition, 0, subObjId, index, subIndex);
	if(!res.IsSuccessful())
	{
		CLIResult clires = CLILogger::GetInstance().HandleCoreApiFailed("Get Module Object Current Sub Index", res);

		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, clires);
	}

	return subIndex;
}

std::string ProjectParser::GetNewParameterId(std::uint8_t nodeId, std::string interfaceId, 
											 std::string modId, std::uint32_t modposition, 
											 std::string uniqueID)
{
	std::string parameterName;

	Result res = OpenConfiguratorCore::GetInstance().GetModuleParameterCurrentName(OpenConfiguratorCLI::GetInstance().networkName,
																					nodeId, interfaceId, modId, 
																					modposition, uniqueID, parameterName);
	if (!res.IsSuccessful())
	{
		CLIResult clires = CLILogger::GetInstance().HandleCoreApiFailed("Get Module Parameter Current Name", res);

		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, clires);
	}

	return parameterName;
}

CLIResult ProjectParser::CreateForcedObjects(ParserElement& pElement, std::string xPathExpression,
									 std::uint8_t nodeId, std::vector<std::string> forcedObj, 
									 std::vector<std::string> forcedSubObj)
{
	ParserResult pResult;
	CLIResult cliresult;
	std::cout << nodeId;
	cliresult = pResult.CreateResult(pElement, xPathExpression.c_str(),
										kFormatStrxPathExpressionObject);

	if (!cliresult.IsSuccessful())
	{
		return cliresult;
	}
	else
	{
		for (std::int32_t row = 0; row < pResult.parameters.size(); row++)
		{
			std::string index = pResult.parameters[row].at(0);
			forcedNodeObj.push_back(index);								/** Updates the vector based on existing object **/

			std::string subIndex = pResult.parameters[row].at(1);
			forcedNodeSubObj.push_back(subIndex);						/** Updates the vector based on existing sub-object **/
		}
	}

	return CLIResult();
}

CLIResult ProjectParser::UpdateForcedObjects(std::vector<std::string> forcedObject, 
											 std::vector<std::string> forcedSubObject, 
											 std::uint8_t nodeId, std::string objectIndex, 
											 std::string SubobjectIndex, std::uint32_t objectId, 
											 std::uint8_t subObjectId, std::string objectValue, 
											 std::string subobjectValue, bool value,
											 std::string interfaceId, std::string modId, 
											 std::uint32_t modposition)
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
						if (value)
						{
							objectId = GetNewObjectIndex(nodeId, interfaceId, modId, modposition, objectId);
							subObjectId = (std::uint8_t)(GetNewSubObjectIndex(nodeId, interfaceId, modId, modposition, objectId, subObjectId));
						}

						Result res = OpenConfiguratorCore::GetInstance().SetSubObjectActualValue(
											OpenConfiguratorCLI::GetInstance().networkName,
											nodeId, objectId, subObjectId, subobjectValue, true, false);
						if (!res.IsSuccessful())
						{
							CLIResult clires = CLILogger::GetInstance().HandleCoreApiFailed("Set Sub Object Actual Value", res);

							CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, clires);
						}
					}
				}
			}
			else
			{
				if (subObjectIdOfVector.empty())
				{
					for (std::string object : forcedObject)
					{
						if (objectIndex.compare(object) == 0)
						{
							if (value)
							{
								objectId = GetNewObjectIndex(nodeId, interfaceId, modId, modposition, objectId);
							}

							Result res = OpenConfiguratorCore::GetInstance().SetObjectActualValue(
													OpenConfiguratorCLI::GetInstance().networkName,
													nodeId, objectId, objectValue, true, false);
							if (!res.IsSuccessful())
							{
								CLIResult clires = CLILogger::GetInstance().HandleCoreApiFailed("Set Object Actual Value", res);

								CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, clires);
							}
						}
					}
				}
			}
		}
	}

	return CLIResult();
}

IEC_Datatype ProjectParser::GetDataType(std::string dataType)
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

ParameterAccess ProjectParser::GetParameterAccess(std::string access)
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

ObjectType ProjectParser::GetObjectType(std::uint8_t objType)
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

PlkDataType ProjectParser::GetPLKDataType(std::string plkDataType)
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

AccessType ProjectParser::GetObjAccessType(std::string accessType)
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

PDOMapping ProjectParser::GetPDOMapping(std::string pdoMapp)
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

