/**
 * \file CliConstants
 *
 * \brief Includes all the transcode constant strings and format strings used
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

#ifndef _CLI_CONSTANTS_H_
#define _CLI_CONSTANTS_H_

#include <string>

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Application
		{
			namespace CliConstants
			{
				const std::string kMnXpathExpression = "/openCONFIGURATORProject/NetworkConfiguration/NodeCollection/MN";
				const std::string kCnXpathExpression = "/openCONFIGURATORProject/NetworkConfiguration/NodeCollection/CN";
				const std::string kRmnXpathExpression = "/openCONFIGURATORProject/NetworkConfiguration/NodeCollection/RMN";
				const std::string kAutogenXpathExpression = "/openCONFIGURATORProject/ProjectConfiguration/AutoGenerationSettings";
				const std::string kProjectConfXpathExpression = "/openCONFIGURATORProject/ProjectConfiguration";
				const std::string kAutoSettingXpathExpression = "./Setting";
				const std::string kStructXpathExpression = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/ApplicationProcess/dataTypeList/struct";
				const std::string kArrayXpathExpression = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/ApplicationProcess/dataTypeList/array";
				const std::string kParameterTemplateXpathExpression = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/ApplicationProcess/templateList/parameterTemplate";
				const std::string kParameterGroupXpathExpression = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/ApplicationProcess/parameterGroupList/parameterGroup";
				const std::string kParameterXpathExpression = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/ApplicationProcess/parameterList/parameter";
				const std::string kParameterDataTypeIdRefXpathExpression = "./dataTypeIDRef";
				const std::string kParameterAllowedValueXpathExpression = "./allowedValues/value";
				const std::string kParameterAllowedValueRangeXpathExpression = "./allowedValues/range";
				const std::string kParameterMinValueXpathExpression = "./minValue";
				const std::string kParameterMaxValueXpathExpression = "./maxValue";
				const std::string kParameterDefaultValueXpathExpression = "./defaultValue";
				const std::string kParameterActualValueXpathExpression = "./actualValue";
				const std::string kChildParameterGroupXpathExpression = "./parameterGroup";
				const std::string kChildParameterRefXpathExpression = "./parameterRef";
				const std::string kObjectXpathExpression = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/ApplicationLayers/ObjectList/Object";
				const std::string kDynamicChnlXpathExpression = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/ApplicationLayers/dynamicChannels/dynamicChannel";
				const std::string kSubObjectXpathExpression = "./SubObject";
				const std::string kStructVarXpathExpression = "./varDeclaration";
				const std::string kArraySubRangeXpathExpression = "./subrange";
				const std::string kForcedObjectXpathExpression = "./ForcedObjects/Object";
				const std::string kGeneralFeatureXpathExpression = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/NetworkManagement/GeneralFeatures";
				const std::string kMnFeatureXpathExpression = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/NetworkManagement/MNFeatures";
				const std::string kCnFeatureXpathExpression = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/NetworkManagement/CNFeatures";
				const std::string kIntrfcXpathExpression = "./InterfaceList/Interface";
				const std::string kInterfaceXdcXpathExpression = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/DeviceManager/moduleManagement/interfaceList/interface";
				const std::string kAppLayerInterfaceXpathExpression = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/ApplicationLayers/moduleManagement/interfaceList/interface";
				const std::string kInterfaceRangeXpathExpression = "./rangeList/range";
				const std::string kModuleNodeXpathExpression = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody/DeviceManager/moduleManagement/moduleInterface";
				const std::string kModuleXpathExpression = "./Module";
				const std::string kForcedObjectNodeXpathExpression = "./ForcedObjects/Object";
				const std::string kErrorCodeComponent = "/openCONFIGURATORErrorCodes/ErrorCodes";
				const std::string kErrorCode = "./ErrorCode";
				const std::string kErrorDescription = "./Description";

				const std::uint8_t MN_DEFAULT_NODE_ID = 0xF0;		/** 240 */
				const std::uint8_t MAX_CN_NODE_ID = 0xEF;			/** 239 */

				const std::string kFormatStrMnXpathExpression = "nodeID,pathToXDC,name";
				const std::string kFormatStrCnXpathExpression = "nodeID,pathToXDC,name,enabled";
				const std::string kFormatStrRmnXpathExpression = "nodeID,pathToXDC,name";
				const std::string kFormatStrAutoGenXpathExpression = "id";
				const std::string kFormatProjectConfXpathExpression = "activeAutoGenerationSetting";
				const std::string kFormatStrAutoSetXpathExpression = "name,value,enabled";
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
				const std::string kFormatStrParameterTemplateXpathExpression = "uniqueID,access,templateIDRef";
				const std::string kFormatStrChildParameterGroupXpathExpression = "uniqueID,conditionalUniqueIDRef,conditionalValue,bitOffset,configParameter,groupLevelVisible";
				const std::string kFormatStrParameterGroupXpathExpression = "uniqueID,conditionalUniqueIDRef,conditionalValue,bitOffset,configParameter,groupLevelVisible";
				const std::string kFormatStrChildParameterRefXpathExpression = "uniqueIDRef,access,actualValue,bitOffset";
				const std::string kFormatStrParameterXpathExpression = "uniqueID,access,templateIDRef";
				const std::string kFormatStrParameterDataTypeIdRefXpathExpression = "uniqueID,access";
				const std::string kFormatStrObjectXpathExpression = "index,name,objectType,defaultValue,dataType,accessType,PDOmapping,actualValue,uniqueIDRef,lowLimit,highLimit";
				const std::string kFormatStrGeneralFeatureXpathExpression = "CFMConfigManager,DLLErrBadPhysMode,DLLErrMacBuffer,DLLFeatureCN,DLLFeatureMN,NMTBootTimeNotActive,NMTCycleTimeGranularity,NMTCycleTimeMax,NMTCycleTimeMin,NMTMinRedCycleTime,NMTEmergencyQueueSize,NMTErrorEntries,NMTExtNmtCmds,NMTFlushArpEntry,NMTIsochronous,NMTNetHostNameSet,NMTMaxCNNodeID,NMTMaxCNNumber,NMTMaxHeartbeats,NMTNodeIDByHW,NMTNodeIDBySW,NMTProductCode,NMTPublishActiveNodes,NMTPublishConfigNodes,NMTPublishEmergencyNew,NMTPublishNodeState,NMTPublishOperational,NMTPublishPreOp1,NMTPublishPreOp2,NMTPublishReadyToOp,NMTPublishStopped,NMTPublishTime,NMTRevisionNo,NWLForward,NWLICMPSupport,NWLIPSupport,PDODynamicMapping,PDOGranularity,PDOMaxDescrMem,PDORPDOChannelObjects,PDORPDOChannels,PDORPDOCycleDataLim,PDORPDOOverallObjects,PDOSelfReceipt,PDOTPDOChannelObjects,PDOTPDOCycleDataLim,PDOTPDOOverallObjects,PHYExtEPLPorts,PHYHubDelay,PHYHubIntegrated,PHYHubJitter,RT1RT1SecuritySupport,RT1RT1Support,RT2RT2Support,SDOClient,SDOCmdFileRead,SDOCmdFileWrite,SDOCmdLinkName,SDOCmdReadAllByIndex,SDOCmdReadByName,SDOCmdReadMultParam,SDOCmdWriteAllByIndex,SDOCmdWriteByName,SDOCmdWriteMultParam,SDOMaxConnections,SDOMaxParallelConnections,SDOSeqLayerTxHistorySize,SDOServer,SDOSupportASnd,SDOSupportPDO,SDOSupportUdpIp,DLLMultiplePReqPRes";
				const std::string kFormatStrMnFeatureXpathExpression = "DLLErrMNMultipleMN,DLLMNFeatureMultiplex,DLLMNPResChaining,DLLMNFeaturePResTx,NMTMNASnd2SoC,NMTMNBasicEthernet,NMTMNMultiplCycMax,NMTMNPRes2PReq,NMTMNPRes2PRes,NMTMNPResRx2SoA,NMTMNPResTx2SoA,NMTMNSoA2ASndTx,NMTMNSoC2PReq,NMTNetTime,NMTNetTimeIsRealTime,NMTRelativeTime,NMTServiceUdpIp,NMTSimpleBoot,PDOTPDOChannels,NMTMNDNA,NMTMNRedundancy,DLLMNRingRedundancy,NMTMNMaxAsynchronousSlots";
				const std::string kFormatStrCnFeatureXpathExpression = "DLLCNFeatureMultiplex,DLLCNPResChaining,NMTCNPreOp2ToReady2Op,NMTCNSoC2PReq,NMTCNSetNodeNumberTime,NMTCNDNA,NMTCNMaxAInv";
				const std::string kFormatStrxPathExpression = "transmitsPRes,isMandatory,autostartNode,resetInOperational,verifyAppSwVersion,autoAppSwUpdateAllowed,isChained,isAsyncOnly,isType1Router,isType2Router,nodeID";
				const std::string kFormatStrModuleNodeXpathExpression = "childID,type,moduleAddressing,minPosition,maxPosition,minAddress,maxAddress,maxCount";
				const std::string kFormatStrModuleXpathExpression = "name,position,address,pathToXDC,enabled";
				const std::string kFormatStrxPathExpressionObject = "index,subindex";
				const std::string kFormatStrObjectXpathExpressionModule = "index,name,objectType,defaultValue,dataType,accessType,PDOmapping,actualValue,uniqueIDRef,lowLimit,highLimit,rangeSelector";
				const std::string kFormatStrDynamicChnlXpathExpressionModule = "dataType,accessType,startIndex,endIndex,maxNumber,addressOffset,bitAlignment";
				const std::string kFormatStrSubObjectXpathExpression = "subIndex,name,objectType,defaultValue,dataType,accessType,PDOmapping,actualValue,uniqueIDRef,lowLimit,highLimit";
				const std::string kFormatStrInterfaceXdcXpathExpression = "uniqueID,type,moduleAddressing,maxModules,unusedSlots,multipleModules";
				const std::string kFormatStrInterfaceRangeXpathExpression = "name,baseIndex,maxIndex,maxSubIndex,sortMode,sortNumber,PDOmapping,sortStep";
				const std::string kFormatStrAppLayerInterfaceXpathExpression = "uniqueIDRef";
				const std::string kFormatStrIntrfcXpathExpression = "id";
				const std::string kFormatStrErrorCodeComponent = "component,componentVersion";
				const std::string kFormatStrErrorCode = "value,originalCode,toolCode";
				const std::string kFormatStrErrorDescription = "xml:lang,value";

				const std::string kDefaultAttributeValue = "";

				const std::string kApplicationName = "openCONFIGURATOR";

			} // end of namespace CliConstants
		} // end of namespace Application
	} // end of namespace POWERLINK
} // end of namespace IndustrialNetwork

#endif // _CLI_CONSTANTS_H_
