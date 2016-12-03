/**
 * \file CLIConstants
 *
 * \brief Includes all the transcode constant strings used
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 0.1
 *
 */

#ifndef CLI_CONSTANTS_H
#define CLI_CONSTANTS_H

#include <string>

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Application
		{
			namespace CLIConstants
			{
				const std::string mnXpathExpression = "/openCONFIGURATORProject/NetworkConfiguration/NodeCollection/MN";
				const std::string cnXpathExpression = "/openCONFIGURATORProject/NetworkConfiguration/NodeCollection/CN";
				const std::string structXpathExpression = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/ApplicationProcess/dataTypeList/struct";
				const std::string arrayXpathExpression = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/ApplicationProcess/dataTypeList/array";
				const std::string parameterTemplateXpathExpression = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/ApplicationProcess/templateList/parameterTemplate";
				const std::string parameterGroupXpathExpression = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/ApplicationProcess/parameterGroupList/parameterGroup";
				const std::string parameterXpathExpression = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/ApplicationProcess/parameterList/parameter";
				const std::string parameterDataTypeIdRefXpathExpression = "./dataTypeIdRef";
				const std::string parameterAllowedValueXpathExpression = "./allowedValues/value";
				const std::string parameterAllowedValueRangeXpathExpression = "./allowedValues/range";
				const std::string parameterMinValueXpathExpression = "./minValue";
				const std::string parameterMaxValueXpathExpression = "./maxValue";
				const std::string parameterDefaultValueXpathExpression = "./defaultValue";
				const std::string parameterActualValueXpathExpression = "./actualValue";
				const std::string childParameterGroupXpathExpression = "./parameterGroup";
				const std::string childParameterRefXpathExpression = "./parameterRef";
				const std::string objectXpathExpression = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/ApplicationLayers/ObjectList/Object";
				const std::string subObjectXpathExpression = "./SubObject";
				const std::string structVarXpathExpression = "./varDeclaration";
				const std::string arraySubRangeXpathExpression = "./subrange";
				const std::string forcedObjectXpathExpression = "./ForcedObjects/Object";
				const std::string generalFeatureXpathExpression = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/NetworkManagement/GeneralFeatures";
				const std::string mnFeatureXpathExpression = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/NetworkManagement/MNFeatures";
				const std::string cnFeatureXpathExpression = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/NetworkManagement/CNFeatures";
			} // end of namespace CLIConstants
		} // end of namespace Application
	} // end of namespace POWERLINK
} // end of namespace IndustrialNetwork

#endif // CLI_CONSTANTS_H
