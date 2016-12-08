/**
 * \file CLIConstants
 *
 * \brief Includes all the transcode constant strings and format strings used
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
				const std::string kMnXpathExpression = "/openCONFIGURATORProject/NetworkConfiguration/NodeCollection/MN";
				const std::string kCnXpathExpression = "/openCONFIGURATORProject/NetworkConfiguration/NodeCollection/CN";
				const std::string kStructXpathExpression = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/ApplicationProcess/dataTypeList/struct";
				const std::string kArrayXpathExpression = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/ApplicationProcess/dataTypeList/array";
				const std::string kParameterTemplateXpathExpression = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/ApplicationProcess/templateList/parameterTemplate";
				const std::string kParameterGroupXpathExpression = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/ApplicationProcess/parameterGroupList/parameterGroup";
				const std::string kParameterXpathExpression = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/ApplicationProcess/parameterList/parameter";
				const std::string kParameterDataTypeIdRefXpathExpression = "./dataTypeIdRef";
				const std::string kParameterAllowedValueXpathExpression = "./allowedValues/value";
				const std::string kParameterAllowedValueRangeXpathExpression = "./allowedValues/range";
				const std::string kParameterMinValueXpathExpression = "./minValue";
				const std::string kParameterMaxValueXpathExpression = "./maxValue";
				const std::string kParameterDefaultValueXpathExpression = "./defaultValue";
				const std::string kParameterActualValueXpathExpression = "./actualValue";
				const std::string kChildParameterGroupXpathExpression = "./parameterGroup";
				const std::string kChildParameterRefXpathExpression = "./parameterRef";
				const std::string kObjectXpathExpression = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/ApplicationLayers/ObjectList/Object";
				const std::string kSubObjectXpathExpression = "./SubObject";
				const std::string kStructVarXpathExpression = "./varDeclaration";
				const std::string kArraySubRangeXpathExpression = "./subrange";
				const std::string kForcedObjectXpathExpression = "./ForcedObjects/Object";
				const std::string kGeneralFeatureXpathExpression = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/NetworkManagement/GeneralFeatures";
				const std::string kMnFeatureXpathExpression = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/NetworkManagement/MNFeatures";
				const std::string kCnFeatureXpathExpression = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/NetworkManagement/CNFeatures";

				const std::string kFormatStrMnXpathExpression = "nodeID,pathToXDC,name";
				const std::string kFormatStrCnXpathExpression = "nodeID,pathToXDC,name";
				const std::string kFormatStrStructXpathExpression = "uniqueID,name";
				const std::string kFormatStrStructVarXpathExpression = "uniqueID,name,size";
				const std::string kFormatStrArrayXpathExpression = "uniqueID,name";
				const std::string kFormatStrArraySubRangeXpathExpression = "lowerLimit,upperLimit";
				const std::string kFormatStrParameterAllowedValueXpathExpression = "value";
				const std::string kFormatStrParameterAllowedValueRangeXpathExpression = "value";
				const std::string kFormatStrParameterMinValueXpathExpression = "value";
				const std::string kFormatStrParameterMaxValueXpathExpression = "value";
				const std::string kFormatStrParameterActualValueXpathExpression = "value";
				const std::string kFormatStrParameterDefaultValueXpathExpression = "value";
				const std::string kFormatStrParameterTemplateXpathExpression = "uniqueID,access";
				const std::string kFormatStrChildParameterGroupXpathExpression = "uniqueID,conditionalUniqueIDRef,conditionalValue,bitOffset,configParameter,groupLevelVisible";
				const std::string kFormatStrParameterGroupXpathExpression = "uniqueID,conditionalUniqueIDRef,conditionalValue,bitOffset,configParameter,groupLevelVisible";
				const std::string kFormatStrChildParameterRefXpathExpression = "uniqueIDRef,access,actualValue,bitOffset";
				const std::string kFormatStrParameterXpathExpression = "uniqueID,access,templateIDRef";
				const std::string kFormatStrParameterDataTypeIdRefXpathExpression = "uniqueID,access";
				const std::string kFormatStrObjectXpathExpression = "index,name,objectType,defaultValue,dataType,accessType,PDOmapping,actualValue,uniqueIdRef,lowLimit,highLimit";
				const std::string kFormatStrSubObjectXpathExpression = "subIndex,name,objectType,defaultValue,dataType,accessType,PDOmapping,actualValue,uniqueIdRef,lowLimit,highLimit";
				const std::string kFormatStrGeneralFeatureXpathExpression = "CFMConfigManager,DLLErrBadPhysMode,DLLErrMacBuffer,DLLFeatureCN,DLLFeatureMN,NMTBootTimeNotActive,NMTCycleTimeGranularity,NMTCycleTimeMax,NMTCycleTimeMin,NMTMinRedCycleTime,NMTEmergencyQueueSize,NMTErrorEntries,NMTExtNmtCmds,NMTFlushArpEntry,NMTIsochronous,NMTNetHostNameSet,NMTMaxCNNodeID,NMTMaxCNNumber,NMTMaxHeartbeats,NMTNodeIDByHW,NMTNodeIDBySW,NMTProductCode,NMTPublishActiveNodes,NMTPublishConfigNodes,NMTPublishEmergencyNew,NMTPublishNodeState,NMTPublishOperational,NMTPublishPreOp1,NMTPublishPreOp2,NMTPublishReadyToOp,NMTPublishStopped,NMTPublishTime,NMTRevisionNo,NWLForward,NWLICMPSupport,NWLIPSupport,PDODynamicMapping,PDOGranularity,PDOMaxDescrMem,PDORPDOChannelObjects,PDORPDOChannels,PDORPDOCycleDataLim,PDORPDOOverallObjects,PDOSelfReceipt,PDOTPDOChannelObjects,PDOTPDOCycleDataLim,PDOTPDOOverallObjects,PHYExtEPLPorts,PHYHubDelay,PHYHubIntegrated,PHYHubJitter,RT1RT1SecuritySupport,RT1RT1Support,RT2RT2Support,SDOClient,SDOCmdFileRead,SDOCmdFileWrite,SDOCmdLinkName,SDOCmdReadAllByIndex,SDOCmdReadByName,SDOCmdReadMultParam,SDOCmdWriteAllByIndex,SDOCmdWriteByName,SDOCmdWriteMultParam,SDOMaxConnections,SDOMaxParallelConnections,SDOSeqLayerTxHistorySize,SDOServer,SDOSupportASnd,SDOSupportPDO,SDOSupportUdpIp,DLLMultiplePReqPRes";
				const std::string kFormatStrMnFeatureXpathExpression = "DLLErrMNMultipleMN,DLLMNFeatureMultiplex,DLLMNPResChaining,DLLMNFeaturePResTx,NMTMNASnd2SoC,NMTMNBasicEthernet,NMTMNMultiplCycMax,NMTMNPRes2PReq,NMTMNPRes2PRes,NMTMNPResRx2SoA,NMTMNPResTx2SoA,NMTMNSoA2ASndTx,NMTMNSoC2PReq,NMTNetTime,NMTNetTimeIsRealTime,NMTRelativeTime,NMTServiceUdpIp,NMTSimpleBoot,PDOTPDOChannels,NMTMNDNA,NMTMNRedundancy,DLLMNRingRedundancy,NMTMNMaxAsynchronousSlots";
				const std::string kFormatStrCnFeatureXpathExpression = "DLLCNFeatureMultiplex,DLLCNPResChaining,NMTCNPreOp2ToReady2Op,NMTCNSoC2PReq,NMTCNSetNodeNumberTime,NMTCNDNA,NMTCNMaxAInv";
			} // end of namespace CLIConstants
		} // end of namespace Application
	} // end of namespace POWERLINK
} // end of namespace IndustrialNetwork

#endif // CLI_CONSTANTS_H
