/**
 * \class ProjectParser
 *
 * \brief Parses XML file using ParserElement and ParserResult
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 0.1
 *
 */

#ifndef PROJECT_PARSER_H
#define PROJECT_PARSER_H

#include "OpenConfiguratorCLI.h"
#include "ParserElement.h"
#include "ParserResult.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Application
		{
			class ProjectParser
			{
				public:
					/// Default constructor of the class
					ProjectParser();

					/// Descriptor of the class
					~ProjectParser();

					/** \brief Creates single instance
					  * \return Static instance of the class
					  */
					static ProjectParser& GetInstance();

					/** \brief Parses the XML file
					  * \param xmlPath Name of the XML file with path
					  * \return CLIResult
					  */
					CLIResult ParserXMLFile(std::string xmlPath);

				private:
					/**
					  * \brief Updates the information of Node XDC into library API
					  * \param nodeId Specifies the node ID value of node
					  * \param pathXDC Specifies the XDC path of node
					  * \param forcedObject Specifies the vector of forced objects in node
					  * \param forcedSubObject Specifies the vector of forced sub-objects in node
					  * \param interfceID Specifies the inteface ID  of modular node
					  * \param modId Specifies the moduleID of module
					  * \param modposition Specifies the modulePosition of module
					  * \return CLIResult
					  */
					CLIResult UpdateNodeIdCollection(std::uint8_t nodeId, const std::string& pathXDC, 
													std::vector<std::string> forcedObject, 
													std::vector<std::string> forcedSubObject, 
													std::string interfceID, std::string modId, 
													std::uint32_t modposition);

					/**
					  * \brief Updates the interface information of modular Node XDC into library API
					  * \param nodeId Specifies the node ID value of modular head node
					  * \param pathXDC Specifies the XDC path of modular head node
					  * \return CLIResult
					  */
					CLIResult CreateInterface(std::uint8_t nodeId, const std::string& pathXDC);

					/**
					  * \brief Updates the Device POWERLINK profile body of XDD/XDC into core library
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \param interfceID Specifies the inteface ID  of modular node
					  * \param modId Specifies the moduleID of module
					  * \param modposition Specifies the modulePosition of module
					  * \return CLIResult
					  */
					CLIResult ImportProfileBodyDevicePOWERLINK(ParserElement& element, std::uint8_t nodeId, 
													std::string interfaceId, std::string modId, 
													std::uint32_t modposition);

					/**
					  * \brief Updates the Communication POWERLINK profile body of XDD/XDC into core library
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \param forcedObject Specifies the vector of forced objects in node
					  * \param forcedSubObject Specifies the vector of forced sub-objects in node
					  * \param interfceID Specifies the inteface ID  of modular node
					  * \param modId Specifies the moduleID of module
					  * \param modposition Specifies the modulePosition of module
					  * \return CLIResult
					  */
					CLIResult ImportProfileBodyCommunicationPOWERLINK(ParserElement& element, std::uint8_t nodeId, 
													std::vector<std::string> forcedObject, 
													std::vector<std::string> forcedSubObject,
													std::string interfaceId, std::string modId, 
													std::uint32_t modposition);

					/**
					  * \brief Creates network management general features
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \return CLIResult
					  */
					CLIResult CreateNetworkManagementGeneralFeatures(ParserElement& element, std::uint8_t nodeId);

					/**
					  * \brief Creates network management MN features
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \return CLIResult
					  */
					CLIResult CreateNetworkManagementMnFeatures(ParserElement& element, std::uint8_t nodeId);

					/**
					  * \brief Creates network management CN features
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \return CLIResult
					  */
					CLIResult CreateNetworkManagementCnFeatures(ParserElement& element, std::uint8_t nodeId);

					/**
					  * \brief Creates node assignment values of node
					  * \param element element that contains handle of XDC document
					  * \param xPathExpression xpath expression of XML
					  * \param nodeId ID value of node
					  * \return CLIResult
					  */
					CLIResult CreateNodeAssignment(ParserElement& element, std::string xPathExpression, std::uint8_t nodeId);

					/**
					  * \brief Sets node assignment values of node into library
					  * \param nodeassign Node assignment attribute
					  * \param nodeId ID value of node
					  * \param value Node assignment attribute value
					  * \return CLIResult
					  */
					CLIResult SetNodeAssignment(NodeAssignment nodeassign, std::uint8_t nodeId, std::string value);

					/**
					  * \brief Creates Struct and Array data type parse results
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \param interfaceId Inteface ID  of module node
					  * \param modId ID value of module
					  * \param modposition Position value of module
					  * \return CLIResult
					  */
					CLIResult CreateDataType(ParserElement& element, std::uint8_t nodeId, 
												std::string interfaceId, std::string modId, 
												std::uint32_t modposition);

					/**
					  * \brief Creates Struct data type parse results
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \param interfaceId Inteface ID  of module node
					  * \param modId ID value of module
					  * \param modposition Position value of module
					  * \return CLIResult
					  */
					CLIResult CreateStructDataType(ParserElement& element, std::uint8_t nodeId, 
												std::string interfaceId, std::string modId, 
												std::uint32_t modposition);

					/**
					  * \brief Creates Array data type parse results
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \param interfaceId Inteface ID  of module node
					  * \param modId ID value of module
					  * \param modposition Position value of module
					  * \return CLIResult
					  */
					CLIResult CreateArrayDataType(ParserElement& element, std::uint8_t nodeId, 
												std::string interfaceId, std::string modId, 
												std::uint32_t modposition);

					/**
					  * \brief Creates parameter template related parse results
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \param interfaceId Inteface ID  of module node
					  * \param modId ID value of module
					  * \param modposition Position value of module
					  * \return CLIResult
					  */
					CLIResult CreateParameterTemplate(ParserElement& element, std::uint8_t nodeId, 
												std::string interfaceId, std::string modId, 
												std::uint32_t modposition);

					/**
					  * \brief Creates parameter group related parse results
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \param interfaceId Inteface ID  of module node
					  * \param modId ID value of module
					  * \param modposition Position value of module
					  * \return CLIResult
					  */
					CLIResult CreateParameterGroup(ParserElement& element, std::uint8_t nodeId, 
												std::string interfaceId, std::string modId, 
												std::uint32_t modposition);

					/**
					  * \brief Creates parameter list related parse results
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \param interfaceId Inteface ID  of module node
					  * \param modId ID value of module
					  * \param modposition Position value of module
					  * \return CLIResult
					  */
					CLIResult CreateParameterList(ParserElement& element, std::uint8_t nodeId, 
												std::string interfaceId, std::string modId, 
												std::uint32_t modposition);

					/**
					  * \brief Creates child parameter group related parse results
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \param parameterGroupNode Parent parameter group node 
					  * \param paramGroupUId Parent parameter group node ID
					  * \param interfaceId Inteface ID  of module node
					  * \param modId ID value of module
					  * \param modposition Position value of module
					  * \return CLIResult
					  */
					CLIResult CreateChildParameterGroup(ParserElement& element, std::uint8_t nodeId, 
												xercesc::DOMNode* parameterGroupNode, 
												std::string paramGroupUId, 
												std::string interfaceId, std::string modId, 
												std::uint32_t modposition);

					/**
					  * \brief Creates object related parse results and set parameters
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \param forcedObject Specifies the vector of forced objects in node
					  * \param forcedSubObject Specifies the vector of forced sub-objects in node
					  * \return CLIResult
					  */
					CLIResult CreateObject(ParserElement& element, std::uint8_t nodeId, 
												std::vector<std::string> forcedObject, 
												std::vector<std::string> forcedSubObject);

					/**
					  * \brief Sets allowed values
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \param interfaceId Inteface ID  of module node
					  * \param modId ID value of module
					  * \param modposition Position value of module
					  * \return CLIResult
					  */
					CLIResult SetParamAllowedValue(ParserElement& element, std::uint8_t nodeId, 
												xercesc::DOMNode* node, std::string uniqueId, 
												std::string interfaceId, std::string modId, 
												std::uint32_t modposition);

					/**
					  * \brief Sets allowed range values
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \param interfaceId Inteface ID  of module node
					  * \param modId ID value of module
					  * \param modposition Position value of module
					  * \return CLIResult
					  */
					CLIResult SetParamAllowedRange(ParserElement& element, std::uint8_t nodeId, 
												xercesc::DOMNode* node, std::string uniqueId, 
												std::string interfaceId, std::string modId, 
												std::uint32_t modposition);

					/**
					  * \brief Sets actual values
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \param interfaceId Inteface ID  of module node
					  * \param modId ID value of module
					  * \param modposition Position value of module
					  * \return CLIResult
					  */
					CLIResult SetParamActualValue(ParserElement& element, std::uint8_t nodeId, 
												xercesc::DOMNode* node, std::string uniqueId, 
												std::string interfaceId, std::string modId, 
												std::uint32_t modposition);

					/**
					  * \brief Sets dafault values
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the ID value of node
					  * \param interfaceId Inteface ID  of module node
					  * \param modId ID value of module
					  * \param modposition Position value of module
					  * \return CLIResult
					  */
					CLIResult SetParamDefaultValue(ParserElement& element, std::uint8_t nodeId, 
												xercesc::DOMNode* node, std::string uniqueId, 
												std::string interfaceId, std::string modId, 
												std::uint32_t modposition);

					/**
					  * \brief Gets the addressing value of modular device from the XDC
					  * \param modAddressing Module addressing value in XDC
					  * \return ModuleAddressing
					  */
					ModuleAddressing GetModuleAddressing(std::string modAddressing);

					/**
					  * \brief Gets the Sort number of modular device from the XDC
					  * \param sortNumber Sort number value in XDC
					  * \return SortNumber
					  */
					SortNumber GetSortNumber(std::string sortNumber);

					/**
					  * \brief Gets the Sort mode of modular device from the XDC
					  * \param sortMode Sort mode value in XDC
					  * \return SortMode
					  */
					SortMode GetSortMode(std::string sortMode);

					/**
					  * \brief Creates module of node into library
					  * \param nodeId ID value of node
					  * \param interfaceId Interface ID value of modular node
					  * \param moduleName Name of module
					  * \param modulePosition Position value of module
					  * \param moduleAddress Address value of module
					  * \param modulePathToXDC PathToXDC value of module
					  * \param moduleEnabled Enabled value of module
					  * \return CLIResult
					  */
					CLIResult CreateModule(std::uint8_t nodeId, std::string interfaceId, 
										std::string moduleName, std::string modulePosition, 
										std::string moduleAddress, std::string modulePathToXDC, 
										std::string moduleEnabled);

					/**
					  * \brief Creates the object list of modules from XDC into core library
					  * \param element Element that contains handle of XDC document
					  * \param nodeId ID value of node
					  * \param forcedObject Forced objects vector of node
					  * \param forcedSubObject Forced sub-objects vector of node
					  * \param interfaceId Inteface ID of modular node
					  * \param modId ID value of module
					  * \param modposition Position value of module
					  * \return CLIResult
					  */
					CLIResult CreateModuleObject(ParserElement & element, std::uint8_t nodeId, 
										std::vector<std::string> forcedObject, 
										std::vector<std::string> forcedSubObject, 
										std::string interfaceId, std::string modId, 
										std::uint32_t modposition);

					/**
					  * \brief Gets the index of module object from library
					  * \param nodeId Id value of node
					  * \param interfaceId Inteface ID  of modular node
					  * \param modId ID value of module
					  * \param modposition Position value of module
					  * \param objId ID value of module object
					  * \return Index of new object
					  */
					std::uint32_t GetNewObjectIndex(std::uint8_t nodeId, std::string interfaceId, 
										std::string modId, std::uint32_t modposition, 
										std::uint32_t objId);

					/**
					  * \brief Gets sub-index of module sub-object from the library
					  * \param nodeId ID value of node
					  * \param interfaceId Inteface ID  of modular node
					  * \param modId ID value of module
					  * \param modposition Position value of module
					  * \param objId ID value of module object
					  * \param subObjId ID value of module sub object
					  * \return Index of new sub object
					  */
					std::int32_t GetNewSubObjectIndex(std::uint8_t nodeId, std::string interfaceId, 
										std::string modId, std::uint32_t modposition, 
										std::uint32_t objId, std::uint8_t subObjId);
					
					/**
					  * \brief Gets unique ID of parameter from the library
					  * \param nodeId ID value of node
					  * \param interfaceId Inteface ID  of modular node
					  * \param modId ID value of module
					  * \param modposition Position value of module
					  * \param uniqueID ID value of module parameters
					  * \return Parameter unique ID
					  */
					std::string GetNewParameterId(std::uint8_t nodeId, std::string interfaceId, 
										std::string modId, std::uint32_t modposition, 
										std::string uniqueID);
					
					/**
					  * \brief Updates the forced object list of node
					  * \param element Element that contains handle of XDC document
					  * \param xPathExpression Xpath expression of forced objects in XML
					  * \param nodeId ID value of node
					  * \param forcedObject Forced objects vector of node
					  * \param forcedSubObject Forced sub-objects vector of node
					  * \return CLIResult
					  */
					CLIResult CreateForcedObjects(ParserElement& pElement, std::string xPathExpression, 
										std::uint8_t nodeId, std::vector<std::string> forcedObject, 
										std::vector<std::string> forcedSubObject);
					
					/**
					  * \brief Updates the forced object list of node into core library
					  * \param forcedObject Forced objects vector of node
					  * \param forcedSubObject Forced sub-objects vector of node
					  * \param nodeId ID value of node
					  * \param objectIndex Index value of object
					  * \param SubobjectIndex Sub-index value of sub-object
					  * \param objectId ID value of object
					  * \param subObjectId ID value of sub-object
					  * \param objectActualValue Actual value of object
					  * \param subobjectActualValue Actual value of sub-object
					  * \param moduleCheck Checks module or node
					  * \param interfaceId Inteface ID  of modular node
					  * \param modId ID value of module
					  * \param modposition Position value of module
					  * \return CLIResult
					  */
					CLIResult UpdateForcedObjects(std::vector<std::string> forcedObject, 
										std::vector<std::string> forcedSubObject, std::uint8_t nodeId,
										std::string objectIndex, std::string SubobjectIndex, 
										std::uint32_t objectId, std::uint8_t subObjectId, 
										std::string objectActualValue, std::string subobjectActualValue,
										bool moduleCheck, std::string interfaceId, 
										std::string modId, std::uint32_t modposition);

					/**< Group of forced objects in Node */
					std::vector<std::string> forcedNodeObj;  

					/**< Group of forced sub objects in Node */
					std::vector<std::string> forcedNodeSubObj;

					/**< Group of forced objects in Module */
					std::vector<std::string> forcedModularObj;

					/**< Group of forced objects in Module */
					std::vector<std::string> forcedModularSubObj;

					/**
					  * \brief Gets the IEC_Datatype based on the string retrieved from XDC
					  * \param dataType Specifies the dataType value of object or parameter
					  * \returns IEC_Datatype
					  */
					IEC_Datatype GetDataType(std::string dataType);

					/**
					  * \brief Gets the ParameterAccess based on the value retrieved from XDC
					  * \param access Specifies the access value of parameter
					  * \return ParameterAccess
					  */
					ParameterAccess GetParameterAccess(std::string access);

					/**
					  * \brief Gets the ObjectType of object from XDC
					  * \param objType Specifies the type of an object
					  * \return ObjectType
					 */
					ObjectType GetObjectType(std::uint8_t objType);

					/**
					  * \brief Gets the PlkDataType of object from XDC
					  * \param plkDataType Specifies the data type of an object
					  * \return PlkDataType
					  */
					PlkDataType GetPLKDataType(std::string plkDataType);

					/**
					 * \brief Gets the AccessType of object from XDC
					 * \param accessType Specifies the access type of object
					 * \return AccessType
					 */
					AccessType GetObjAccessType(std::string accessType);

					/**
					  * \brief Gets the PDOMapping of object or sub-object from XDC
					  * \param pdoMapp Specifies the mapping value of object or sub-object
					  * \return PDOMapping
					  */
					PDOMapping GetPDOMapping(std::string pdoMapp);

					/**
					  * \brief Gets the IEC_Datatype of object or sub-object based on element in XDC
					  * \param node Instance of DOMNode
					  * \return IEC_Datatype
					  */
					IEC_Datatype GetIECDataType(xercesc::DOMNode* node);

			}; // end of class ProjectParser
		} // end of namespace Application
	} // end of namespace POWERLINK
} // end of namespace IndustrialNetwork

#endif // PROJECT_PARSER_H

