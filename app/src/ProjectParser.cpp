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

bool ProjectParser::ParserXMLFile(std::string xmlPath)
{
	try
	{
		/**< Core Library API call to create Network */
		Result res = OpenConfiguratorCore::GetInstance().CreateNetwork(
						OpenConfiguratorCLI::GetInstance().GetNetworkName());

		ParserElement xmlParserElement;
		ParserResult mnResult, cnResult;

		if (xmlParserElement.CreateElement(xmlPath) == true)
		{
			if (mnResult.CreateResult(xmlParserElement, mnXpathExpression,
										"nodeID,pathToXDC,name") == true)
			{
				for (int row = 0; row < mnResult.parameters.size(); row++)
				{
					uint8_t nodeId = (uint8_t) atoi(mnResult.parameters[row].at(0).c_str());

					/**< Core Library API call to create Node */
					Result res = OpenConfiguratorCore::GetInstance().CreateNode(
								OpenConfiguratorCLI::GetInstance().GetNetworkName(),
								nodeId, 
								mnResult.parameters[row].at(2));			/* name */

					if (!res.IsSuccessful())
					{
						LOG_ERROR() << res.GetErrorMessage();
						return false;
					}

					/**< Core Library API calls to import the XDD of node */
					UpdateNodeIdCollection(nodeId, 
						mnResult.parameters[row].at(1));	/* pathToXDC */
				}
			}
			else
			{
				// CLI error
				return false;
			}

			if (cnResult.CreateResult(xmlParserElement, cnXpathExpression.c_str(),
										"nodeID,pathToXDC,name") == true)
			{
				for (int row = 0; row < cnResult.parameters.size(); row++)
				{
					uint8_t nodeId = (uint8_t) atoi(cnResult.parameters[row].at(0).c_str());

					/**< Core Library API call to create Node */
					Result res = OpenConfiguratorCore::GetInstance().CreateNode(
								OpenConfiguratorCLI::GetInstance().GetNetworkName(),
								nodeId, 
								cnResult.parameters[row].at(2),		/* name */
								false);
					if (!res.IsSuccessful())
					{
						LOG_ERROR() << res.GetErrorMessage();
						return false;
					}

					/**< Core Library API calls to import the XDD of node */
					UpdateNodeIdCollection(nodeId, 
						cnResult.parameters[row].at(1));	/* pathToXDC */
				}
			}
			else
			{
				// CLI error
				return false;
			}
		}
		else
		{
			// CLI error
			return false;
		}

	}
	catch (const std::exception& e)
	{
		std::cerr << e.what();
	}

	return true;
}

IEC_Datatype ProjectParser::GetIECDataType(DOMNode* node)
{
	IEC_Datatype data = IEC_Datatype::UNDEFINED;

	if (node->hasChildNodes())
	{
		DOMNodeList* childNode = node-> getChildNodes();
		const XMLSize_t nodeCount = childNode->getLength();

		for (XMLSize_t index = 0; index < nodeCount; index++)
		{
			DOMNode* currentNode = childNode->item(index);

			if ((currentNode->getNodeType()) && (currentNode->getNodeType() == DOMNode::ELEMENT_NODE))
			{
				char* subNodeName = XMLString::transcode(currentNode->getNodeName());
				std::string childNodeName = subNodeName;
				data = GetDataType(childNodeName);
			}
		}
	}

	return data;
}

bool ProjectParser::CreateStructDataType(ParserElement& element, std::uint8_t nodeId)
{
	ParserResult pResult;

	if (pResult.CreateResult(element, structXpathExpression.c_str(),
								"uniqueID,name") == true)
	{
		for (int row = 0; row < pResult.parameters.size(); row++)
		{
			/**< Core Library API call to create StructDatatype */
			Result res = OpenConfiguratorCore::GetInstance().CreateStructDatatype(
					OpenConfiguratorCLI::GetInstance().GetNetworkName(),
					nodeId, pResult.parameters[row].at(0),
					pResult.parameters[row].at(1));
			if (!res.IsSuccessful())
			{
				LOG_ERROR() <<  res.GetErrorMessage();
			}

			ParserResult varDecResult;

			if (varDecResult.CreateResult(element, structVarXpathExpression.c_str(),
										"uniqueID,name,size", pResult.node.at(row)) == true)
			{
				for (int subrow = 0; subrow < varDecResult.parameters.size(); subrow++)
				{
					IEC_Datatype data = GetIECDataType(varDecResult.node.at(subrow));

					uint32_t varDeclSize;
					if (varDecResult.parameters[subrow].at(2).empty())
					{
						/* Set default value to size if value is empty */
						varDeclSize = 1U;
					}
					else
					{
						varDeclSize = (uint32_t) atoi(varDecResult.parameters[subrow].at(2).c_str());
					}

					/**< Core Library API call to create VarDeclaration */
					Result res = OpenConfiguratorCore::GetInstance().CreateVarDeclaration(
							OpenConfiguratorCLI::GetInstance().GetNetworkName(),
							nodeId,
							pResult.parameters[row].at(0),										/* parent uniqueID */
							varDecResult.parameters[subrow].at(0),								/* sub item uniqueID */
							varDecResult.parameters[subrow].at(1), 								/* sub item name */
							data,
							varDeclSize);														/* sub item size */
					if (!res.IsSuccessful())
					{
						LOG_ERROR() <<  res.GetErrorMessage();
					}
				}
			}
			else
			{
				// CLI error
				return false;
			}
		}
	}
	else
	{
		// CLI error
		return false;
	}
}

bool ProjectParser::CreateArrayDataType(ParserElement & element, std::uint8_t nodeId)
{
	ParserResult pResult;

	if (pResult.CreateResult(element, arrayXpathExpression.c_str(),
								"uniqueID,name") == true)
	{
		for (int row = 0; row < pResult.parameters.size(); row++)
		{
			ParserResult subpResult;

			IEC_Datatype data = GetIECDataType(pResult.node.at(row));

			if (subpResult.CreateResult(element, arraySubRangeXpathExpression.c_str(),
										"lowerLimit,upperLimit", pResult.node.at(row)) == true)
			{
				for (int subrow = 0; subrow < subpResult.parameters.size(); subrow++)
				{
					uint32_t lowerLimit;
					uint32_t upperLimit;

					if (subpResult.parameters[subrow].at(0).empty())
					{
						/* Defalut value to lowerLimit in case it is empty */
						lowerLimit = 1U;
					}
					else
					{
						lowerLimit = (uint32_t)subpResult.parameters[subrow].at(0).c_str();
					}

					if (subpResult.parameters[subrow].at(1).empty())
					{
						/* Defalut value to upperLimit in case it is empty */
						upperLimit = 1U;
					}
					else
					{
						upperLimit = (uint32_t)subpResult.parameters[subrow].at(1).c_str();
					}

					/**< Core Library API call to create ArrayDatatype */
					Result res = OpenConfiguratorCore::GetInstance().CreateArrayDatatype(
							OpenConfiguratorCLI::GetInstance().GetNetworkName(),
							nodeId,
							pResult.parameters[row].at(0),  								/* uniqueID */
							pResult.parameters[row].at(1),  								/* name */
							lowerLimit,
							upperLimit,
							data);
					if (!res.IsSuccessful())
					{
						LOG_ERROR() << res.GetErrorMessage();
					}
				}
			}
			else
			{
				// CLI error
				return false;
			}
		}
	}
	else
	{
		// CLI error
		return false;
	}
}

bool ProjectParser::AddDataType(ParserElement & element, std::uint8_t nodeId)
{
	/**< Creates struct data type of node into the core library */
	ProjectParser::GetInstance().CreateStructDataType(element, nodeId);

	/**< Creates array data type of node into the core library */
	ProjectParser::GetInstance().CreateArrayDataType(element, nodeId);

	return true;
}

bool ProjectParser::SetParamAllowedValue(ParserElement & element,
										 DOMNode* node,
										 std::uint8_t nodeId,
										 std::string uniqueId)
{
	ParserResult pResult;

	if (pResult.CreateResult(element,
								parameterAllowedValueXpathExpression.c_str(),
								"value", node) == true)
	{
		std::vector<std::string> allowedValuelist;

		for (int row = 0; row < pResult.parameters.size(); row++)
		{
			allowedValuelist.push_back(pResult.parameters[row].at(0));
		}

		/**< Core Library API call to set allowed parameter values */
		Result res = OpenConfiguratorCore::GetInstance().SetParameterAllowedValues(
				OpenConfiguratorCLI::GetInstance().GetNetworkName(),
				nodeId,
				uniqueId,
				allowedValuelist);
		if (!res.IsSuccessful())
		{
			LOG_ERROR() << res.GetErrorMessage();
		}
	}
	else
	{
		// CLI error
		return false;
	}
}

bool ProjectParser::SetParamAllowedRange(ParserElement & element,
											DOMNode* node,
											std::uint8_t nodeId,
											std::string uniqueId)
{
	ParserResult pResult;

	if (pResult.CreateResult(element,
								parameterAllowedValueRangeXpathExpression.c_str(),
								"value", node) == true)
	{
		for (int row = 0; row < pResult.parameters.size(); row++)
		{
			ParserResult minpResult;
			ParserResult maxpResult;
			std::string minValue = "";
			std::string maxValue = "";

			if (minpResult.CreateResult(element,
										parameterMinValueXpathExpression.c_str(),
										"value", pResult.node.at(row)) == true)
			{
				minValue = minpResult.parameters[0].at(0);
			}

			if (maxpResult.CreateResult(element,
										parameterMaxValueXpathExpression.c_str(),
										"value", pResult.node.at(row)) == true)
			{
				maxValue = maxpResult.parameters[0].at(0);
			}

			/**< Core Library API call to set allowed parameter values */
			Result res = OpenConfiguratorCore::GetInstance().SetParameterAllowedRange(
					OpenConfiguratorCLI::GetInstance().GetNetworkName(),
					nodeId, uniqueId, minValue, maxValue);
			if (!res.IsSuccessful())
			{
				LOG_ERROR() << res.GetErrorMessage();
			}
		}
	}
	else
	{
		// CLI error
		return false;
	}
}

bool ProjectParser::SetParamActualValue(ParserElement & element,
										DOMNode* node,
										std::uint8_t nodeId,
										std::string uniqueId)
{
	ParserResult pResult;

	if (pResult.CreateResult(element,
								parameterActualValueXpathExpression.c_str(),
								"value", node) == true)
	{
		/**< Core Library API call to set actual parameter values */
		Result res = OpenConfiguratorCore::GetInstance().SetParameterActualValue(
				OpenConfiguratorCLI::GetInstance().GetNetworkName(),
				nodeId, uniqueId, pResult.parameters[0].at(0));
		if (!res.IsSuccessful())
		{
			LOG_ERROR() << res.GetErrorMessage();
		}
	}
	else
	{
		// CLI error
		return false;
	}
}

bool ProjectParser::SetParamDefaultValue(ParserElement & element,
											DOMNode* node,
											std::uint8_t nodeId,
											std::string uniqueId)
{
	ParserResult pResult;

	if (pResult.CreateResult(element,
								parameterDefaultValueXpathExpression.c_str(),
								"value", node) == true)
	{
		/**< Core Library API call to set actual parameter values */
		Result res = OpenConfiguratorCore::GetInstance().SetParameterDefaultValue(
				OpenConfiguratorCLI::GetInstance().GetNetworkName(),
				nodeId, uniqueId, pResult.parameters[0].at(0));
		if (!res.IsSuccessful())
		{
			LOG_ERROR() << res.GetErrorMessage();
		}
	}
	else
	{
		// CLI error
		return false;
	}
}

bool ProjectParser::CreateParameterTemplate(ParserElement & element, std::uint8_t nodeId)
{
	ParserResult pResult;

	if (pResult.CreateResult(element, parameterTemplateXpathExpression.c_str(),
								"uniqueID,access") == true)
	{
		for (int row = 0; row < pResult.parameters.size(); row++)
		{
			ParameterAccess accessOfParameter = GetParameterAccess(pResult.parameters[row].at(1));
			IEC_Datatype data = GetIECDataType(pResult.node.at(row));

			ParserResult subpResult;

			if (subpResult.CreateResult(element, parameterDataTypeIdRefXpathExpression.c_str(),
										"uniqueID,access", pResult.node.at(row)) == true)
			{
				if (subpResult.resultNodeValue == NULL)
				{
					LOG_INFO() << "DataTypeIdRef not available.";

					/**< Core Library API call to create Parameter */
					Result res = OpenConfiguratorCore::GetInstance().CreateParameter(
							OpenConfiguratorCLI::GetInstance().GetNetworkName(),
							nodeId, pResult.parameters[row].at(0),
							accessOfParameter, data, true);
					if (!res.IsSuccessful())
					{
						LOG_ERROR() << res.GetErrorMessage();
					}
				}
				else
				{
					for (int subrow = 0; subrow < subpResult.parameters.size(); subrow++)
					{
						ParameterAccess paramAccessSubNode = GetParameterAccess(subpResult.parameters[subrow].at(1));

						/**< Core Library API call to create Parameter */
						Result res = OpenConfiguratorCore::GetInstance().CreateParameter(
								OpenConfiguratorCLI::GetInstance().GetNetworkName(),
								nodeId, pResult.parameters[row].at(0),
								subpResult.parameters[subrow].at(0),
								paramAccessSubNode, true);
						if (!res.IsSuccessful())
						{
							LOG_ERROR() << res.GetErrorMessage();
						}
					}
				}
			}
			else
			{
				// CLI error
				return false;
			}

			/**< Updates the allowed values of parameter template.*/
			SetParamAllowedValue(element, pResult.node.at(row), nodeId, pResult.parameters[row].at(0));

			/**< Updates the allowed range of parameter template.*/
			SetParamAllowedRange(element, pResult.node.at(row), nodeId, pResult.parameters[row].at(0));

			/**< Updates the default value of parameter template.*/
			SetParamDefaultValue(element, pResult.node.at(row), nodeId, pResult.parameters[row].at(0));

			/**< Updates the actual value of parameter template.*/
			SetParamActualValue(element,  pResult.node.at(row), nodeId, pResult.parameters[row].at(0));
		}
	}
	else
	{
		// CLI error
		return false;
	}
}

bool ProjectParser::CreateChildParameterGroup(ParserElement & element,
												std::uint8_t nodeId,
												DOMNode* parameterGroupNode,
												std::string paramGroupUId)
{
	ParserResult pResult;

	if (pResult.CreateResult(element, childParameterGroupXpathExpression.c_str(),
		"uniqueID,conditionalUniqueIDRef,conditionalValue,bitOffset,configParameter,groupLevelVisible",
		parameterGroupNode) == true)
	{
		for (int row = 0; row < pResult.parameters.size(); row++)
		{
			if (pResult.parameters[row].at(1).empty())	/* Is conditionalUniqueIDRef empty */
			{
				Result res = OpenConfiguratorCore::GetInstance().CreateParameterGroup(
						OpenConfiguratorCLI::GetInstance().GetNetworkName(),
						nodeId,
						pResult.parameters[row].at(0), 				/* uniqueID */
						paramGroupUId,
						(uint8_t)pResult.parameters[row].at(3).c_str(),		/* bitOffset */
						"", "", 0U);
				if (!res.IsSuccessful())
				{
					LOG_ERROR() << res.GetErrorMessage();
				}
			}
			else
			{
				Result res = OpenConfiguratorCore::GetInstance().CreateParameterGroup(
						OpenConfiguratorCLI::GetInstance().GetNetworkName(),
						nodeId,
						pResult.parameters[row].at(0), 						/* uniqueID */
						paramGroupUId,
						pResult.parameters[row].at(1), 						/* conditionalUniqueIDRef */
						pResult.parameters[row].at(2), 						/* conditionalValue */
						(uint8_t)pResult.parameters[row].at(3).c_str(),		/* bitOffset */
						"", "", 0U);
				if (!res.IsSuccessful())
				{
					LOG_ERROR() << res.GetErrorMessage();
				}
			}

			ParserResult subpResult;

			if (subpResult.CreateResult(element, childParameterRefXpathExpression.c_str(),
										"uniqueIDRef,access,actualValue,bitOffset",
										pResult.node.at(row)) == true)
			{
				for (int subrow = 0; subrow < subpResult.parameters.size(); subrow++)
				{
					ParameterAccess paramAccess = GetParameterAccess(subpResult.parameters[subrow].at(1));

					if (subpResult.parameters[subrow].at(2).empty())	/* Is actualValue empty */
					{
						Result res = OpenConfiguratorCore::GetInstance().CreateParameterReference(
										OpenConfiguratorCLI::GetInstance().GetNetworkName(),
										nodeId, paramGroupUId,
										subpResult.parameters[subrow].at(0),					/* uniqueIDRef */
										"",
										(uint8_t)subpResult.parameters[subrow].at(3).c_str(), 	/* bitOffset */
										"", "", 0U);
						if (!res.IsSuccessful())
						{
							LOG_ERROR() << res.GetErrorMessage();
						}
					}
					else
					{
						Result res = OpenConfiguratorCore::GetInstance().CreateParameterReference(
								OpenConfiguratorCLI::GetInstance().GetNetworkName(),
								nodeId,
								pResult.parameters[row].at(0),							/* uniqueID */
								subpResult.parameters[subrow].at(0),					/* uniqueIDRef */
								subpResult.parameters[subrow].at(2),  					/* actualValue */
								(uint8_t)subpResult.parameters[subrow].at(3).c_str(), 	/* bitOffset */
								"", "", 0U);
						if (!res.IsSuccessful())
						{
							LOG_ERROR() << res.GetErrorMessage();
						}
					}
				}
			}
			else
			{
				// CLI error
				return false;
			}
		}
	}
	else
	{
		// CLI error
		return false;
	}
}

bool ProjectParser::CreateParameterGroup(ParserElement & element, std::uint8_t nodeId)
{
	ParserResult pResult;

	if (pResult.CreateResult(element, parameterGroupXpathExpression.c_str(),
		"uniqueID,conditionalUniqueIDRef,conditionalValue,bitOffset,configParameter,groupLevelVisible") == true)
	{
		for (int row = 0; row < pResult.parameters.size(); row++)
		{
			Result res = OpenConfiguratorCore::GetInstance().CreateParameterGroup(
					OpenConfiguratorCLI::GetInstance().GetNetworkName(),
					nodeId,
					pResult.parameters[row].at(0), 				/* uniqueID */
					"", "", 0U);
			if (!res.IsSuccessful())
			{
				LOG_ERROR() << res.GetErrorMessage();
			}

			CreateChildParameterGroup(element, nodeId, pResult.node.at(row), pResult.parameters[row].at(0));

			ParserResult subpResult;

			if (subpResult.CreateResult(element, childParameterRefXpathExpression.c_str(),
										"uniqueIDRef,access,actualValue,bitOffset",
										pResult.node.at(row)) == true)
			{
				for (int subrow = 0; subrow < subpResult.parameters.size(); subrow++)
				{
					ParameterAccess paramAccess = GetParameterAccess(subpResult.parameters[subrow].at(1));

					Result res = OpenConfiguratorCore::GetInstance().CreateParameterReference(
							OpenConfiguratorCLI::GetInstance().GetNetworkName(),
							nodeId,
							pResult.parameters[row].at(0),							/* uniqueID */
							subpResult.parameters[subrow].at(0),					/* uniqueIDRef */
							subpResult.parameters[subrow].at(2),  					/* actualValue */
							(uint8_t)subpResult.parameters[subrow].at(3).c_str(), 	/* bitOffset */
							"", "", 0U);
					if (!res.IsSuccessful())
					{
						LOG_ERROR() << res.GetErrorMessage();
					}
				}
			}
			else
			{
				// CLI error
				return false;
			}
		}
	}
	else
	{
		// CLI error
		return false;
	}
}

bool ProjectParser::CreateParameterList(ParserElement & element, std::uint8_t nodeId)
{
	ParserResult pResult;

	if (pResult.CreateResult(element, parameterXpathExpression.c_str(),
										"uniqueID,access,templateIDRef") == true)
	{
		for (int row = 0; row < pResult.parameters.size(); row++)
		{
			ParameterAccess access = GetParameterAccess(pResult.parameters[row].at(1));
			IEC_Datatype data = GetIECDataType(pResult.node.at(row));

			if (!pResult.parameters[row].at(2).empty())		/* Is templateIDRef not empty */
			{
				Result res = OpenConfiguratorCore::GetInstance().CreateParameter(
						OpenConfiguratorCLI::GetInstance().GetNetworkName(),
						nodeId,
						pResult.parameters[row].at(0), 		/* uniqueID */
						access,
						pResult.parameters[row].at(2));		/* templateIDRef */
				if (!res.IsSuccessful())
				{
					LOG_ERROR() << res.GetErrorMessage();
				}
			}
			else
			{
				ParserResult subpResult;

				if (subpResult.CreateResult(element, parameterDataTypeIdRefXpathExpression.c_str(),
											"uniqueID,access", pResult.node.at(row)) == true)
				{
					if (subpResult.resultNodeValue == NULL)
					{
						LOG_ERROR() << "DataTypeIdRef not available." << std::endl;
						Result res = OpenConfiguratorCore::GetInstance().CreateParameter(
								OpenConfiguratorCLI::GetInstance().GetNetworkName(),
								nodeId,
								pResult.parameters[row].at(0),		/* UniqueID */
								access,
								data,
								false, "", "", 1U);
						if (!res.IsSuccessful())
						{
							LOG_ERROR() << res.GetErrorMessage();
						}
					}
					else
					{
						for (int subrow = 0; subrow < subpResult.parameters.size(); subrow++)
						{
							ParameterAccess paramAccess = GetParameterAccess(subpResult.parameters[subrow].at(1));

							Result res = OpenConfiguratorCore::GetInstance().CreateParameter(
									OpenConfiguratorCLI::GetInstance().GetNetworkName(),
									nodeId,
									pResult.parameters[row].at(0),					/* uniqueID */
									subpResult.parameters[subrow].at(0),			/* uniqueID of subresult */
									paramAccess,
									false);
							if (!res.IsSuccessful())
							{
								LOG_ERROR() << res.GetErrorMessage();
							}
						}
					}
				}
				else
				{
					// CLI error
					return false;
				}
			}

			/**< Updates the allowed values of parameter.*/
			SetParamAllowedValue(element, pResult.node.at(row), nodeId, pResult.parameters[row].at(0));

			/**< Updates the allowed range of parameter.*/
			SetParamAllowedRange(element, pResult.node.at(row), nodeId, pResult.parameters[row].at(0));

			/**< Updates the default value of parameter.*/
			SetParamDefaultValue(element, pResult.node.at(row), nodeId, pResult.parameters[row].at(0));

			/**< Updates the actual value of parameter.*/
			SetParamActualValue(element,  pResult.node.at(row), nodeId, pResult.parameters[row].at(0));

			Result res = OpenConfiguratorCore::GetInstance().CreateParameter(
							OpenConfiguratorCLI::GetInstance().GetNetworkName(),
							nodeId,
							pResult.parameters[row].at(0), 		/* uniqueID */
							"",
							access,
							false, "", "", 0U);
			if (!res.IsSuccessful())
			{
				LOG_ERROR() << res.GetErrorMessage();
			}
		}
	}
	else
	{
		// CLI error
		return false;
	}
}

bool ProjectParser::ImportProfileBodyDevicePOWERLINK(ParserElement & element, std::uint8_t nodeId)
{
	/**< Creates datatype in the core library */
	ProjectParser::GetInstance().AddDataType(element, nodeId);

	/**< Creates parameter template of node into the core library */
	ProjectParser::GetInstance().CreateParameterTemplate(element, nodeId);

	/**< Creates parameter Group of node into the core library */
	ProjectParser::GetInstance().CreateParameterGroup(element, nodeId);

	/**< Creates parameter list of node into the core library */
	ProjectParser::GetInstance().CreateParameterList(element, nodeId);

	return true;
}

bool ProjectParser::CreateObject(ParserElement & element, std::uint8_t nodeId)
{
	ParserResult pResult;

	if (pResult.CreateResult(element, objectXpathExpression.c_str(),
		"index,name,objectType,defaultValue,dataType,accessType,PDOmapping,actualValue,uniqueIdRef,lowLimit,highLimit") == true)
	{
		for (int row = 0; row < pResult.parameters.size(); row++)
		{
			uint32_t objId = (uint32_t)strtol(pResult.parameters[row].at(0).c_str(), NULL, 16);
			uint8_t objIdType = (uint8_t)atoi(pResult.parameters[row].at(2).c_str());
			std::string accessType = pResult.parameters[row].at(5);
			std::string dataType = pResult.parameters[row].at(4);
			std::string uniqueIdRef = pResult.parameters[row].at(8);
			std::string actualValue = pResult.parameters[row].at(7);

			if ((accessType == "ro") || (accessType == "const"))
			{
				actualValue = "";
			}

			if ((dataType != "") && (uniqueIdRef == ""))
			{
				Result res = OpenConfiguratorCore::GetInstance().CreateObject(
						OpenConfiguratorCLI::GetInstance().GetNetworkName(),
						nodeId, objId,
						GetObjectType(objIdType),
						pResult.parameters[row].at(1),					/* name */
						GetPLKDataType(dataType),
						GetObjAccessType(accessType),
						GetPDOMapping(pResult.parameters[row].at(6)), 	/* PDOmapping */
						pResult.parameters[row].at(3),                  /* defaultValue */
						actualValue);
				if (!res.IsSuccessful())
				{
					LOG_ERROR() << res.GetErrorMessage();
				}
				else
				{
					if ((!pResult.parameters[row].at(9).empty()) || (!pResult.parameters[row].at(10).empty()))
					{
						Result res = OpenConfiguratorCore::GetInstance().SetObjectLimits(
								OpenConfiguratorCLI::GetInstance().GetNetworkName(),
								nodeId, objId,
								pResult.parameters[row].at(9),				/* lowLimit */
								pResult.parameters[row].at(10));			/* highLimit */
						if (!res.IsSuccessful())
						{
							LOG_ERROR() << res.GetErrorMessage();
						}
					}
				}
			}
			else if ((dataType != "") && (uniqueIdRef != ""))
			{
				Result res = OpenConfiguratorCore::GetInstance().CreateParameterObject(
						OpenConfiguratorCLI::GetInstance().GetNetworkName(),
						nodeId, objId, GetObjectType(objIdType),
						pResult.parameters[row].at(1),					/* name */ 
						GetPLKDataType(dataType),
						GetObjAccessType(accessType),
						GetPDOMapping(pResult.parameters[row].at(6)), 	/* PDOmapping */
						uniqueIdRef,
						pResult.parameters[row].at(3),                  /* defaultValue */
						actualValue);
				if (!res.IsSuccessful())
				{
					LOG_ERROR() << res.GetErrorMessage();
				}
			}
			else if ((dataType == "") && (uniqueIdRef != ""))
			{
				Result res = OpenConfiguratorCore::GetInstance().CreateParameterObject(
						OpenConfiguratorCLI::GetInstance().GetNetworkName(),
						nodeId, objId,
						GetObjectType(objIdType),
						pResult.parameters[row].at(1),					/* name */
						PlkDataType::UNDEFINED,
						GetObjAccessType(accessType),
						GetPDOMapping(pResult.parameters[row].at(6)), 	/* PDOmapping */
						uniqueIdRef,
						pResult.parameters[row].at(3),                  /* defaultValue */
						actualValue);
				if (!res.IsSuccessful())
				{
					LOG_ERROR() << res.GetErrorMessage();
				}

			}
			else if ((dataType == "") && (uniqueIdRef == ""))
			{
				Result res = OpenConfiguratorCore::GetInstance().CreateObject(
						OpenConfiguratorCLI::GetInstance().GetNetworkName(),
						nodeId, objId, GetObjectType(objIdType),
						pResult.parameters[row].at(1),					/* name */
						GetPLKDataType(dataType),
						GetObjAccessType(accessType),
						GetPDOMapping(pResult.parameters[row].at(6)), 	/* PDOmapping */
						pResult.parameters[row].at(3),                  /* defaultValue */
						actualValue);
				if (!res.IsSuccessful())
				{
					LOG_ERROR() << res.GetErrorMessage();
				}
				else
				{
					if ((!pResult.parameters[row].at(9).empty()) || (!pResult.parameters[row].at(10).empty()))
					{
						Result res = OpenConfiguratorCore::GetInstance().SetObjectLimits(
								OpenConfiguratorCLI::GetInstance().GetNetworkName(),
								nodeId, objId,
								pResult.parameters[row].at(9),			/* lowLimit */
								pResult.parameters[row].at(10));		/* highLimit */
						if (!res.IsSuccessful())
						{
							LOG_ERROR() << res.GetErrorMessage();
						}
					}
				}
			}

			ParserResult subpResult;

			if (subpResult.CreateResult(element, subObjectXpathExpression.c_str(),
				"subIndex,name,objectType,defaultValue,dataType,accessType,PDOmapping,actualValue,uniqueIdRef,lowLimit,highLimit",
				pResult.node.at(row)) == true)
			{
				for (int subrow = 0; subrow < subpResult.parameters.size(); subrow++)
				{
					uint8_t subObjId = (uint8_t)strtol(subpResult.parameters[subrow].at(0).c_str(), NULL, 16);
					uint8_t subObjIdType = (uint8_t)atoi(subpResult.parameters[subrow].at(2).c_str());
					std::string subObjaccessType = subpResult.parameters[subrow].at(5);
					std::string subObjActualValue = subpResult.parameters[subrow].at(7);
					std::string subObjuniqueIdRef = subpResult.parameters[subrow].at(8);
					std::string subObjdataType = subpResult.parameters[subrow].at(4);

					if ((subObjaccessType == "ro") || (subObjaccessType == "const"))
					{
						subObjActualValue = "";
					}

					if (subpResult.parameters[subrow].at(8) == "")						/* Is uniqueIdRef empty */
					{
						Result res = OpenConfiguratorCore::GetInstance().CreateSubObject(
								OpenConfiguratorCLI::GetInstance().GetNetworkName(),
								nodeId, objId, subObjId, GetObjectType(subObjIdType),
								subpResult.parameters[subrow].at(1),					/* sub object name */
								GetPLKDataType(subpResult.parameters[subrow].at(4)), 	/* sub object dataType */
								GetObjAccessType(subObjaccessType),
								GetPDOMapping(subpResult.parameters[subrow].at(6)),		/* sub object PDOmapping */
								subpResult.parameters[subrow].at(3),					/* sub object defaultValue */
								subObjActualValue);
						if (!res.IsSuccessful())
						{
							LOG_ERROR() << res.GetErrorMessage();
						}
						else
						{
							/* If any one of the sub object lowLimit and highLimit has values */
							if ((!subpResult.parameters[subrow].at(9).empty()) || (!subpResult.parameters[subrow].at(10).empty()))
							{
								Result res = OpenConfiguratorCore::GetInstance().SetSubObjectLimits(
										OpenConfiguratorCLI::GetInstance().GetNetworkName(),
										nodeId, objId, subObjId,
										subpResult.parameters[subrow].at(9),			/* sub object lowLimit */
										subpResult.parameters[subrow].at(10));			/* sub object highLimit */
								if (!res.IsSuccessful())
								{
									LOG_ERROR() << res.GetErrorMessage();
								}
							}
						}
					}
					else if ((subObjuniqueIdRef != "") && (subObjdataType != ""))
					{
						/* Both sub object uniqueIdRef and dataType has values */
						Result res = OpenConfiguratorCore::GetInstance().CreateParameterSubObject(
								OpenConfiguratorCLI::GetInstance().GetNetworkName(),
								nodeId, objId, subObjId,
								GetObjectType(subObjIdType),
								subpResult.parameters[subrow].at(1),					/* sub object name */
								GetPLKDataType(subObjdataType),
								GetObjAccessType(subObjaccessType),
								GetPDOMapping(subpResult.parameters[subrow].at(6)),		/* sub object PDOmapping */
								subObjuniqueIdRef,
								subpResult.parameters[subrow].at(3),					/* sub object defaultValue */
								subObjActualValue);
						if (!res.IsSuccessful())
						{
							LOG_ERROR() << res.GetErrorMessage();
						}
					}
					else if ((subObjuniqueIdRef != "") && (subObjdataType == ""))
					{
						/* Both sub object uniqueIdRef and dataType has values */
						Result res = OpenConfiguratorCore::GetInstance().CreateParameterSubObject(
								OpenConfiguratorCLI::GetInstance().GetNetworkName(),
								nodeId, objId, subObjId, GetObjectType(subObjIdType),
								subpResult.parameters[subrow].at(1),					/* sub object name */
								PlkDataType::UNDEFINED,
								GetObjAccessType(subObjaccessType),
								GetPDOMapping(subpResult.parameters[subrow].at(6)),		/* sub object PDOmapping */
								subObjuniqueIdRef,
								subpResult.parameters[subrow].at(3),					/* sub object defaultValue */
								subObjActualValue);
						if (!res.IsSuccessful())
						{
							LOG_ERROR() << res.GetErrorMessage();
						}
					}
				}
			}
			else
			{
				// CLI error
				return false;
			}
		}
	}
	else
	{
		// CLI error
		return false;
	}
}


bool ProjectParser::ImportProfileBodyCommunicationPOWERLINK(ParserElement & element, std::uint8_t nodeId)
{
	ProjectParser::GetInstance().CreateObject(element, nodeId);

	return true;
}

/**
/Updates the Library with the information extracted from the node XDC.
/param nodeid Specifies the ID of the Node.
/param cnXDC Specifies the XDC path of Node.
*/
bool ProjectParser::UpdateNodeIdCollection(std::uint8_t nodeId, const std::string& cnXDC)
{
	std::string indexString1 = "/" + cnXDC;
	std::string initialPath = OpenConfiguratorCLI::GetInstance().xmlFilePath;
	std::string nextInitialPath = initialPath.substr(0, initialPath.find_last_of("\\/"));
	std::string fullPath = nextInitialPath.append(indexString1);

	ParserElement element;

	element.CreateElement(nextInitialPath.c_str());

	ImportProfileBodyDevicePOWERLINK(element, nodeId);
	ImportProfileBodyCommunicationPOWERLINK(element, nodeId);

	return true;
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
	else if (pdoMapp == "tpdo")
	{
		return PDOMapping::TPDO;
	}
	else if (pdoMapp == "rpdo")
	{
		return PDOMapping::RPDO;
	}
	return PDOMapping::UNDEFINED;
}

