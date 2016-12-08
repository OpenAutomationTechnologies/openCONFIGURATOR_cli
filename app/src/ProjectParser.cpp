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

					/**< Core Library API calls to import the XDD of node */
					CLIResult clires = UpdateNodeIdCollection(nodeId,
										mnResult.parameters[row].at(1));	/**< pathToXDC */
					if (!clires.IsSuccessful())
					{
						return CLILogger::GetInstance().HandleCliApiFailed("Update MN Node Id Collection", clires);
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

					/**< Core Library API call to create Node */
					Result res = OpenConfiguratorCore::GetInstance().CreateNode(
										OpenConfiguratorCLI::GetInstance().networkName,
										nodeId,
										cnResult.parameters[row].at(2),		/**< name */
										false);
					if (!res.IsSuccessful())
					{
						return CLILogger::GetInstance().HandleCoreApiFailed("Create CN Node", res);
					}

					/**< Core Library API calls to import the XDD of node */
					CLIResult clires = UpdateNodeIdCollection(nodeId,
										cnResult.parameters[row].at(1));	/**< pathToXDC */
					if (!clires.IsSuccessful())
					{
						return CLILogger::GetInstance().HandleCliApiFailed("Update MN Node Id Collection", clires);
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
				data = GetDataType(childNodeName);
			}
		}
	}

	return data;
}

CLIResult ProjectParser::CreateStructDataType(ParserElement& element, std::uint8_t nodeId)
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
					pResult.parameters[row].at(1));
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
							varDeclSize);														/**< sub item size */
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

CLIResult ProjectParser::CreateArrayDataType(ParserElement& element, std::uint8_t nodeId)
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
							data);
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

CLIResult ProjectParser::CreateDataType(ParserElement& element, std::uint8_t nodeId)
{
	CLIResult res;

	/**< Creates struct data type of node into the core library */
	res = CreateStructDataType(element, nodeId);
	if (!res.IsSuccessful())
	{
		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);
	}

	/**< Creates array data type of node into the core library */
	res = CreateArrayDataType(element, nodeId);
	if (!res.IsSuccessful())
	{
		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);
	}

	return CLIResult();
}

CLIResult ProjectParser::SetParamAllowedValue(ParserElement & element,
										 std::uint8_t nodeId,
										 xercesc::DOMNode* node,
										 std::string uniqueId)
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
										 std::string uniqueId)
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
										 std::string uniqueId)
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
										 std::string uniqueId)
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

CLIResult ProjectParser::CreateParameterTemplate(ParserElement& element, std::uint8_t nodeId)
{
	ParserResult pResult;

	CLIResult crres = pResult.CreateResult(element, kParameterTemplateXpathExpression,
								kFormatStrParameterTemplateXpathExpression);
	if (!crres.IsSuccessful())
	{
		return crres;
	}
	else
	{
		for (std::int32_t row = 0; row < pResult.parameters.size(); row++)
		{
			ParameterAccess accessOfParameter = GetParameterAccess(pResult.parameters[row].at(1));
			IEC_Datatype data = GetIECDataType(pResult.node.at(row));

			ParserResult subpResult;

			CLIResult subcrres = subpResult.CreateResult(element, kParameterDataTypeIdRefXpathExpression,
										kFormatStrParameterDataTypeIdRefXpathExpression,
										pResult.node.at(row));
			if (!subcrres.IsSuccessful())
			{
				CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
			}
			else
			{
				if (subpResult.resultNodeValue == NULL)
				{
					LOG_INFO() << "DataTypeIdRef not available.";

					/**< Core Library API call to create Parameter */
					Result res = OpenConfiguratorCore::GetInstance().CreateParameter(
							OpenConfiguratorCLI::GetInstance().networkName,
							nodeId, pResult.parameters[row].at(0),
							accessOfParameter, data, true);
					if (!res.IsSuccessful())
					{
						return CLILogger::GetInstance().HandleCoreApiFailed("Create Parameter", res);
					}
				}
				else
				{
					for (std::int32_t subrow = 0; subrow < subpResult.parameters.size(); subrow++)
					{
						ParameterAccess paramAccessSubNode = GetParameterAccess(subpResult.parameters[subrow].at(1));

						/**< Core Library API call to create Parameter */
						Result res = OpenConfiguratorCore::GetInstance().CreateParameter(
								OpenConfiguratorCLI::GetInstance().networkName,
								nodeId, pResult.parameters[row].at(0),
								subpResult.parameters[subrow].at(0),
								paramAccessSubNode, true);
						if (!res.IsSuccessful())
						{
							return CLILogger::GetInstance().HandleCoreApiFailed("Create parameter", res);
						}
					}
				}
			}

			/**< Updates the allowed values of parameter template.*/
			subcrres = SetParamAllowedValue(element, nodeId, pResult.node.at(row), pResult.parameters[row].at(0));
			if (!subcrres.IsSuccessful())
			{
				CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
			}

			/**< Updates the allowed range of parameter template.*/
			subcrres = SetParamAllowedRange(element, nodeId, pResult.node.at(row), pResult.parameters[row].at(0));
			if (!subcrres.IsSuccessful())
			{
				CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
			}

			/**< Updates the default value of parameter template.*/
			subcrres = SetParamDefaultValue(element, nodeId, pResult.node.at(row), pResult.parameters[row].at(0));
			if (!subcrres.IsSuccessful())
			{
				CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
			}

			/**< Updates the actual value of parameter template.*/
			subcrres = SetParamActualValue(element,  nodeId, pResult.node.at(row), pResult.parameters[row].at(0));
			if (!subcrres.IsSuccessful())
			{
				CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
			}
		}
	}

	return CLIResult();
}

CLIResult ProjectParser::CreateChildParameterGroup(ParserElement& element,
												std::uint8_t nodeId,
												xercesc::DOMNode* parameterGroupNode,
												std::string paramGroupUId)
{
	ParserResult pResult;

	CLIResult crres = pResult.CreateResult(element, kChildParameterGroupXpathExpression,
								kFormatStrChildParameterGroupXpathExpression,
								parameterGroupNode);
	if (!crres.IsSuccessful())
	{
		return crres;
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
						"", "", 0U);
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
						"", "", 0U);
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
										"", "", 0U);
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
										"", "", 0U);
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

CLIResult ProjectParser::CreateParameterGroup(ParserElement& element, std::uint8_t nodeId)
{
	ParserResult pResult;

	CLIResult crres = pResult.CreateResult(element, kParameterGroupXpathExpression,
											kFormatStrParameterGroupXpathExpression);
	if (!crres.IsSuccessful())
	{
		return crres;
	}
	else
	{
		for (std::int32_t row = 0; row < pResult.parameters.size(); row++)
		{
			Result res = OpenConfiguratorCore::GetInstance().CreateParameterGroup(
					OpenConfiguratorCLI::GetInstance().networkName,
					nodeId,
					pResult.parameters[row].at(0), 				/**< uniqueID */
					"", "", 0U);
			if (!res.IsSuccessful())
			{
				return CLILogger::GetInstance().HandleCoreApiFailed("Create Parameter Group", res);
			}

			CLIResult subcrres;
			subcrres = CreateChildParameterGroup(element, nodeId, pResult.node.at(row), pResult.parameters[row].at(0));
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
					Result res = OpenConfiguratorCore::GetInstance().CreateParameterReference(
							OpenConfiguratorCLI::GetInstance().networkName,
							nodeId,
							pResult.parameters[row].at(0),											/**< uniqueID */
							subpResult.parameters[subrow].at(0),									/**< uniqueIDRef */
							subpResult.parameters[subrow].at(2),  									/**< actualValue */
							(std::uint8_t)std::stoi(subpResult.parameters[subrow].at(3).c_str()), 	/**< bitOffset */
							"", "", 0U);
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

CLIResult ProjectParser::CreateParameterList(ParserElement& element, std::uint8_t nodeId)
{
	ParserResult pResult;

	CLIResult crres = pResult.CreateResult(element, kParameterXpathExpression,
											kFormatStrParameterXpathExpression);
	if (!crres.IsSuccessful())
	{
		return crres;
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
						pResult.parameters[row].at(2));		/**< templateIDRef */
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
					CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
				}
				else
				{
					if (subpResult.resultNodeValue == NULL)
					{
						LOG_ERROR() << "DataTypeIdRef not available." << std::endl;
						Result res = OpenConfiguratorCore::GetInstance().CreateParameter(
								OpenConfiguratorCLI::GetInstance().networkName,
								nodeId,
								pResult.parameters[row].at(0),		/**< UniqueID */
								access,
								data,
								false, "", "", 1U);
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
									false);
							if (!res.IsSuccessful())
							{
								return CLILogger::GetInstance().HandleCoreApiFailed("Create Parameter", res);
							}
						}
					}
				}
			}

			CLIResult subcrres;

			/**< Updates the allowed values of parameter.*/
			subcrres = SetParamAllowedValue(element, nodeId, pResult.node.at(row), pResult.parameters[row].at(0));
			if (!subcrres.IsSuccessful())
			{
				CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
			}

			/**< Updates the allowed range of parameter.*/
			subcrres = SetParamAllowedRange(element, nodeId, pResult.node.at(row), pResult.parameters[row].at(0));
			if (!subcrres.IsSuccessful())
			{
				CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
			}

			/**< Updates the default value of parameter.*/
			subcrres = SetParamDefaultValue(element, nodeId, pResult.node.at(row), pResult.parameters[row].at(0));
			if (!subcrres.IsSuccessful())
			{
				CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
			}

			/**< Updates the actual value of parameter.*/
			subcrres = SetParamActualValue(element, nodeId, pResult.node.at(row), pResult.parameters[row].at(0));
			if (!subcrres.IsSuccessful())
			{
				CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
			}

			Result res = OpenConfiguratorCore::GetInstance().CreateParameter(
							OpenConfiguratorCLI::GetInstance().networkName,
							nodeId,
							pResult.parameters[row].at(0), 		/**< uniqueID */
							"",
							access,
							false, "", "", 0U);
			if (!res.IsSuccessful())
			{
				return CLILogger::GetInstance().HandleCoreApiFailed("Create Parameter", res);
			}
		}
	}

	return CLIResult();
}

CLIResult ProjectParser::ImportProfileBodyDevicePOWERLINK(ParserElement& element, std::uint8_t nodeId)
{
	CLIResult subcrres;

	/**< Creates datatype in the core library */
	subcrres = CreateDataType(element, nodeId);
	if (!subcrres.IsSuccessful())
	{
		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
	}

	/**< Creates parameter template of node into the core library */
	subcrres = CreateParameterTemplate(element, nodeId);
	if (!subcrres.IsSuccessful())
	{
		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
	}

	/**< Creates parameter Group of node into the core library */
	subcrres = CreateParameterGroup(element, nodeId);
	if (!subcrres.IsSuccessful())
	{
		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
	}

	/**< Creates parameter list of node into the core library */
	subcrres = CreateParameterList(element, nodeId);
	if (!subcrres.IsSuccessful())
	{
		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
	}

	return CLIResult();
}

CLIResult ProjectParser::CreateObject(ParserElement& element, std::uint8_t nodeId)
{
	ParserResult pResult;

	CLIResult crres = pResult.CreateResult(element, kObjectXpathExpression,
											kFormatStrObjectXpathExpression);
	if (!crres.IsSuccessful())
	{
		return crres;
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
				}
			}
		}
	}

	return CLIResult();
}


CLIResult ProjectParser::ImportProfileBodyCommunicationPOWERLINK(ParserElement& element, std::uint8_t nodeId)
{
	CLIResult subcrres;

	/**< Creates objest in the core library */
	subcrres = CreateObject(element, nodeId);
	if (!subcrres.IsSuccessful())
	{
		CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, subcrres);
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

CLIResult ProjectParser::UpdateNodeIdCollection(std::uint8_t nodeId, const std::string& cnXDC)
{
	std::string indexString1 = "/" + cnXDC;
	std::string initialPath = OpenConfiguratorCLI::GetInstance().xmlFilePath;
	std::string nextInitialPath = initialPath.substr(0, initialPath.find_last_of("\\/"));
	std::string fullPath = nextInitialPath.append(indexString1);

	CLIResult res;

//	res = ParameterValidator::GetInstance().IsXDCSchemaValid(nextInitialPath);
//	if (!res.IsSuccessful())
//	{
		/**< XDC file schema is not valid */
//		return res;
//	}

	ParserElement element;
	CLIResult ceres;

	ceres = element.CreateElement(nextInitialPath);
	if (!ceres.IsSuccessful())
	{
		return ceres;
	}
	else
	{
		res = ImportProfileBodyDevicePOWERLINK(element, nodeId);
		if (!res.IsSuccessful())
		{
			CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);
		}

		res = ImportProfileBodyCommunicationPOWERLINK(element, nodeId);
		if (!res.IsSuccessful())
		{
			CLILogger::GetInstance().LogMessage(CLIMessageType::CLI_WARN, res);
		}

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

