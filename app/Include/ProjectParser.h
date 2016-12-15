/**
 * \class ProjectParser
 *
 * \brief Parses XML file using ParserElement and ParserResult
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

#ifndef _PROJECT_PARSER_H_
#define _PROJECT_PARSER_H_

#include "OpenConfiguratorCli.h"
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
					/** \brief Default constructor of the class 
					  */
					ProjectParser();

					/** \brief Destructor of the class 
					  */
					~ProjectParser();

					/** \brief Creates single instance
					  * \return Static instance of the class
					  */
					static ProjectParser& GetInstance();

					/** \brief Parses the XML file
					  * \param xmlPath			Name of the XML file with path
					  * \return CliResult
					  */
					CliResult ParseXmlFile(const std::string& xmlPath);

				private:
					/** \brief Creates deault configuration values for project
					  * \param element 		Element that contains handle of XML
					  * \return CliResult
					  */
					CliResult CreateProjectConfiguration(const ParserElement& element);

					/** \brief Creates MN node results
					  * \param element 		Element that contains handle of XML
					  * \return CliResult
					  */
					CliResult CreateMnNodeResults(const ParserElement& element);

					/** \brief Creates RMN node results
					  * \param element 		Element that contains handle of XML
					  * \return CliResult
					  */
					CliResult CreateRmnNodeResults(const ParserElement& element);

					/** \brief Creates CN node results
					  * \param element 		Element that contains handle of XML
					  * \return CliResult
					  */
					CliResult CreateCnNodeResults(const ParserElement& element);

					/** \brief Updates the XDC node information into core library
					  * \param nodeId 			Node ID value of node
					  * \param pathXdc 			XDC path of node
					  * \param forcedObject 	Vector of forced objects in node
					  * \param forcedSubObject 	Vector of forced sub-objects in node
					  * \param interfceId 		Inteface ID of modular node
					  * \param modId 			ID of module
					  * \param modPosition 		Position of module
					  * \return CliResult
					  */
					CliResult UpdateNodeIdCollection(const std::uint8_t nodeId, 
									const std::string& pathXdc, 
									const std::vector<std::string>& forcedObject, 
									const std::vector<std::string>& forcedSubObject, 
									const std::string& interfceId, 
									const std::string& modId, 
									const std::uint32_t modPosition);

					/** \brief Updates the modular node interface information of 
					  *        XDC into core library
					  * \param nodeId 			Node ID value of modular head node
					  * \param pathXdc 			The XDC path of modular head node
					  * \param maxModPosition	Max module position output
					  * \return CliResult
					  */
					CliResult CreateInterface(const std::uint8_t nodeId, 
									const std::string& pathXdc,
									std::uint8_t& maxModulePosition);

					/** \brief Updates the Device POWERLINK profile body of 
					  *        XDD/XDC into core library
					  * \param element 		Element that contains handle of XDC
					  * \param nodeId 		ID value of node
					  * \param interfceId 	Inteface ID of modular node
					  * \param modId 		ID of module
					  * \param modPosition 	Position of module
					  * \return CliResult
					  */
					CliResult ImportProfileBodyDevice(const ParserElement& element, 
									const std::uint8_t nodeId, 
									const std::string& interfaceId, 
									const std::string& modId, 
									const std::uint32_t modPosition);

					/** \brief Updates the Communication POWERLINK profile body 
					  *        of XDD/XDC into core library
					  * \param element 			Element that contains handle of XDC
					  * \param nodeId 			ID value of node
					  * \param forcedObject 	Vector of forced objects in node
					  * \param forcedSubObject 	Vector of forced sub-objects in node
					  * \param interfceID 		Inteface ID of modular node
					  * \param modId 			ID of module
					  * \param modPosition 		Position of module
					  * \return CliResult
					  */
					CliResult ImportProfileBodyCommn(const ParserElement& element, 
									const std::uint8_t nodeId, 
									const std::vector<std::string>& forcedObject, 
									const std::vector<std::string>& forcedSubObject,
									const std::string& interfaceId, 
									const std::string& modId, 
									const std::uint32_t modPosition);

					/**
					  * \brief Updates the Dynamic channels of XDD/XDC into core library
					  * \param element 			Element that contains handle of XDC
					  * \param nodeId 			ID value of node
					  * \return CliResult
					  */
					CliResult CreateDynamicChannels(const ParserElement& element,
									const std::uint8_t nodeId);

					/**
					  * \brief Gets the access type of dynamic channel
					  * \param accessType 			Access type of dynamic channels
					  * \return DynamicChannelAccessType
					  */
					DynamicChannelAccessType GetDynamicChannelAccessType(const std::string& accessType);

					/** \brief Creates network management general features
					  * \param element 		Element that contains handle of XDC
					  * \param nodeId 		ID value of node
					  * \return CliResult
					  */
					CliResult CreateNwMgtGeneralFeatures(const ParserElement& element, 
									const std::uint8_t nodeId);

					/** \brief Creates network management MN features
					  * \param element 		Element that contains handle of XDC
					  * \param nodeId 		ID value of node
					  * \return CliResult
					  */
					CliResult CreateNwMgtMnFeatures(const ParserElement& element, 
									const std::uint8_t nodeId);

					/** \brief Creates network management CN features
					  * \param element 		Element that contains handle of XDC
					  * \param nodeId 		ID value of node
					  * \return CliResult
					  */
					CliResult CreateNwMgtCnFeatures(const ParserElement& element, 
									const std::uint8_t nodeId);

					/** \brief Creates node assignment values of node
					  * \param element 			Element that contains handle of XDC
					  * \param xPathExpression 	xpath expression of XML
					  * \param nodeId 			ID value of node
					  * \return CliResult
					  */
					CliResult CreateNodeAssignment(const ParserElement& element, 
									const std::string& xPathExpression, 
									const std::uint8_t nodeId);

					/** \brief Sets node assignment values of node into library
					  * \param nodeassign 		Node assignment attribute
					  * \param nodeId 			ID value of node
					  * \param value 			Node assignment attribute value
					  * \return CliResult
					  */
					CliResult SetNodeAssignment(const NodeAssignment nodeassign, 
									const std::uint8_t nodeId, 
									const std::string& value);

					/** \brief Creates Struct and Array data type parse results
					  * \param element 		Element that contains handle of XDC
					  * \param nodeId 		ID value of node
					  * \param interfaceId 	Inteface ID of module node
					  * \param modId 		ID value of module
					  * \param modPosition 	Position value of module
					  * \return CliResult
					  */
					CliResult CreateDataType(const ParserElement& element, 
									const std::uint8_t nodeId, 
									const std::string& interfaceId, 
									const std::string& modId, 
									const std::uint32_t modPosition);

					/** \brief Creates Struct data type parse results
					  * \param element 		Element that contains handle of XDC
					  * \param nodeId 		ID value of node
					  * \param interfaceId	Inteface ID of module node
					  * \param modId 		ID value of module
					  * \param modPosition 	Position value of module
					  * \return CliResult
					  */
					CliResult CreateStructDataType(const ParserElement& element, 
									const std::uint8_t nodeId, 
									const std::string& interfaceId, 
									const std::string& modId, 
									const std::uint32_t modPosition);

					/** \brief Creates Array data type parse results
					  * \param element 		Element that contains handle of XDC
					  * \param nodeId 		ID value of node
					  * \param interfaceId 	Inteface ID of module node
					  * \param modId 		ID value of module
					  * \param modPosition 	Position value of module
					  * \return CliResult
					  */
					CliResult CreateArrayDataType(const ParserElement& element, 
									const std::uint8_t nodeId, 
									const std::string& interfaceId, 
									const std::string& modId, 
									const std::uint32_t modPosition);

					/** \brief Creates parameter template related parse results
					  * \param element 		Element that contains handle of XDC
					  * \param nodeId 		Specifies the Id value of node
					  * \param interfaceId 	Inteface ID of module node
					  * \param modId 		ID value of module
					  * \param modPosition 	Position value of module
					  * \return CliResult
					  */
					CliResult CreateParameterTemplate(const ParserElement& element, 
									const std::uint8_t nodeId, 
									const std::string& interfaceId, 
									const std::string& modId, 
									const std::uint32_t modPosition);

					/** \brief Creates parameter group related parse results
					  * \param element 		Element that contains handle of XDC
					  * \param nodeId 		ID value of node
					  * \param interfaceId 	Inteface ID of module node
					  * \param modId 		ID value of module
					  * \param modPosition 	Position value of module
					  * \return CliResult
					  */
					CliResult CreateParameterGroup(const ParserElement& element, 
									const std::uint8_t nodeId, 
									const std::string& interfaceId,
									const std::string& modId, 
									const std::uint32_t modPosition);

					/** \brief Creates parameter list related parse results
					  * \param element 		Element that contains handle of XDC
					  * \param nodeId 		ID value of node
					  * \param interfaceId 	Inteface ID of module node
					  * \param modId 		ID value of module
					  * \param modPosition 	Position value of module
					  * \return CliResult
					  */
					CliResult CreateParameterList(const ParserElement& element,
									const std::uint8_t nodeId, 
									const std::string& interfaceId, 
									const std::string& modId, 
									const std::uint32_t modPosition);

					/** \brief Creates child parameter group related parse results
					  * \param element 				Element that contains handle of XDC
					  * \param nodeId 				ID value of node
					  * \param parameterGroupNode	Parent parameter group node 
					  * \param paramGroupUId 		Parent parameter group node ID
					  * \param interfaceId 			Inteface ID of module node
					  * \param modId 				ID value of module
					  * \param modPosition 			Position value of module
					  * \return CliResult
					  */
					CliResult CreateChildParameterGroup(const ParserElement& element, 
									const std::uint8_t nodeId, 
									const xercesc::DOMNode* parameterGroupNode, 
									const std::string& paramGroupUId, 
									const std::string& interfaceId, 
									const std::string& modId, 
									const std::uint32_t modPosition);

					/** \brief Creates object related parse results and set parameters
					  * \param element 			Element that contains handle of XDC
					  * \param nodeId 			ID value of node
					  * \param forcedObject 	Vector of forced objects in node
					  * \param forcedSubObject 	Vector of forced sub-objects in node
					  * \return CliResult
					  */
					CliResult CreateObject(const ParserElement& element, 
									const std::uint8_t nodeId, 
									const std::vector<std::string>& forcedObject, 
									const std::vector<std::string>& forcedSubObject);

					/** \brief Sets allowed values
					  * \param element 		Element that contains handle of XDC
					  * \param nodeId 		ID value of node
					  * \param interfaceId 	Inteface ID of module node
					  * \param modId 		ID value of module
					  * \param modPosition 	Position value of module
					  * \return CliResult
					  */
					CliResult SetParamAllowedValue(const ParserElement& element,
									const std::uint8_t nodeId, 
									const xercesc::DOMNode* node, 
									const std::string& uniqueId, 
									const std::string& interfaceId, 
									const std::string& modId, 
									const std::uint32_t modPosition);

					/** \brief Sets allowed range values
					  * \param element 		Element that contains handle of XDC
					  * \param nodeId 		ID value of node
					  * \param interfaceId 	Inteface ID of module node
					  * \param modId 		ID value of module
					  * \param modPosition 	Position value of module
					  * \return CliResult
					  */
					CliResult SetParamAllowedRange(const ParserElement& element, 
									const std::uint8_t nodeId, 
									const xercesc::DOMNode* node, 
									const std::string& uniqueId, 
									const std::string& interfaceId, 
									const std::string& modId, 
									const std::uint32_t modPosition);

					/** \brief Sets actual values
					  * \param element 		Element that contains handle of XDC
					  * \param nodeId 		ID value of node
					  * \param interfaceId 	Inteface ID of module node
					  * \param modId 		ID value of module
					  * \param modPosition 	Position value of module
					  * \return CliResult
					  */
					CliResult SetParamActualValue(const ParserElement& element, 
									const std::uint8_t nodeId, 
									const xercesc::DOMNode* node, 
									const std::string& uniqueId, 
									const std::string& interfaceId, 
									const std::string& modId, 
									const std::uint32_t modPosition);

					/** \brief Sets dafault values
					  * \param element 		Element that contains handle of XDC
					  * \param nodeId 		ID value of node
					  * \param interfaceId 	Inteface ID of module node
					  * \param modId 		ID value of module
					  * \param modPosition 	Position value of module
					  * \return CliResult
					  */
					CliResult SetParamDefaultValue(const ParserElement& element, 
									const std::uint8_t nodeId, 
									const xercesc::DOMNode* node, 
									const std::string& uniqueId, 
									const std::string& interfaceId, 
									const std::string& modId, 
									const std::uint32_t modPosition);

					/** \brief Gets the addressing value of modular device from the XDC
					  * \param modAddressing 	Module addressing value in XDC
					  * \return ModuleAddressing
					  */
					ModuleAddressing GetModuleAddressing(const std::string& modAddressing);

					/** \brief Gets the sort number of modular device from the XDC
					  * \param sortNumber 		Sort number value in XDC
					  * \return SortNumber
					  */
					SortNumber GetSortNumber(const std::string& sortNumber);

					/** \brief Gets the sort mode of modular device from the XDC
					  * \param sortMode 		Sort mode value in XDC
					  * \return SortMode
					  */
					SortMode GetSortMode(const std::string& sortMode);

					/** \brief Creates module of node into library
					  * \param nodeId 			ID value of node
					  * \param forcedObject 	Forced objects vector of node
					  * \param forcedSubObject 	Forced sub-objects vector of node
					  * \param interfaceId 		Interface ID value of modular node
					  * \param moduleName 		Name of module
					  * \param modulePosition 	Position value of module
					  * \param moduleAddress 	Address value of module
					  * \param modulePathToXDC 	PathToXDC value of module
					  * \param maxModulePosition Max module position
					  * \return CliResult
					  */
					CliResult CreateModule(const std::uint8_t nodeId, 
									const std::vector<std::string>& forcedObject, 
									const std::vector<std::string>& forcedSubObject, 
									const std::string& interfaceId, 
									const std::string& moduleName, 
									const std::string& modulePosition, 
									const std::string& moduleAddress, 
									const std::string& modulePathToXDC,
									const std::uint8_t maxModulePosition);

					/** \brief Creates the object list of modules from XDC into core library
					  * \param element 			Element that contains handle of XDC
					  * \param nodeId 			ID value of node
					  * \param forcedObject 	Forced objects vector of node
					  * \param forcedSubObject 	Forced sub-objects vector of node
					  * \param interfaceId 		Inteface ID of modular node
					  * \param modId 			ID value of module
					  * \param modPosition 		Position value of module
					  * \return CliResult
					  */
					CliResult CreateModuleObject(const ParserElement & element,
									const std::uint8_t nodeId, 
									const std::vector<std::string>& forcedObject, 
									const std::vector<std::string>& forcedSubObject, 
									const std::string& interfaceId, 
									const std::string& modId, 
									const std::uint32_t modPosition);

					/** \brief Gets the index of module object from library
					  * \param nodeId 			ID value of node
					  * \param interfaceId 		Inteface ID  of modular node
					  * \param modId 			ID value of module
					  * \param modPosition 		Position value of module
					  * \param objId 			ID value of module object
					  * \param index			New object index output
					  * \return CliResult
					  */
					CliResult GetNewObjectIndex(const std::uint8_t nodeId, 
									const std::string& interfaceId, 
									const std::string& modId, 
									const std::uint32_t modPosition, 
									const std::uint32_t objId,
									std::uint32_t& index);

					/** \brief Gets sub-index of module sub-object from the library
					  * \param nodeId 			ID value of node
					  * \param interfaceId 		Inteface ID  of modular node
					  * \param modId 			ID value of module
					  * \param modPosition 		Position value of module
					  * \param subObjId 		ID value of module sub object
					  * \param subIndex			New sub object index output
					  * \return CliResult
					  */
					CliResult GetNewSubObjectIndex(const std::uint8_t nodeId,
									const std::string& interfaceId, 
									const std::string& modId, 
									const std::uint32_t modPosition, 
									const std::uint8_t subObjId,
									std::uint32_t& subIndex);
					
					/** \brief Gets unique ID of parameter from the library
					  * \param nodeId 			ID value of node
					  * \param interfaceId 		Inteface ID  of modular node
					  * \param modId 			ID value of module
					  * \param modPosition 		Position value of module
					  * \param uniqueID 		ID value of module parameters
					  * \param paramId			New parameter ID output
					  * \return CliResult
					  */
					 CliResult GetNewParameterId(const std::uint8_t nodeId, 
									const std::string& interfaceId, 
									const std::string& modId, 
									const std::uint32_t modPosition, 
									const std::string& uniqueID,
									std::string& paramId);
					
					/** \brief Updates the forced object list of node
					  * \param element 			Element that contains handle of XDC
					  * \param xPathExpression 	Xpath expression of forced objects in XML
					  * \param forcedObject 	Forced objects vector output of node
					  * \param forcedSubObject 	Forced sub-objects vector output of node
					  * \return CliResult
					  */
					CliResult CreateForcedObjects(const ParserElement& pElement,
									const std::string& xPathExpression, 
									std::vector<std::string>& forcedObject, 
									std::vector<std::string>& forcedSubObject);
					
					/** \brief Updates the forced object list of node into core library
					  * \param forcedObject 		Forced objects vector of node
					  * \param forcedSubObject 		Forced sub-objects vector of node
					  * \param nodeId 				ID value of node
					  * \param objectIndex 			Index value of object
					  * \param SubobjectIndex 		Sub-index value of sub-object
					  * \param objectId 			ID value of object
					  * \param subObjectId 			ID value of sub-object
					  * \param objectActualValue 	Actual value of object
					  * \param subobjectActualValue Actual value of sub-object
					  * \param moduleCheck 			Checks module or node
					  * \param interfaceId 			Inteface ID  of modular node
					  * \param modId 				ID value of module
					  * \param modPosition 			Position value of module
					  * \return CliResult
					  */
					CliResult UpdateForcedObjects(const std::vector<std::string>& forcedObject, 
									const std::vector<std::string>& forcedSubObject, 
									const std::uint8_t nodeId,
									const std::string& objectIndex, 
									const std::string& SubobjectIndex, 
									const std::uint32_t objectId, 
									const std::uint8_t subObjectId, 
									const std::string& objectActualValue, 
									const std::string& subobjectActualValue,
									const bool moduleCheck, 
									const std::string& interfaceId, 
									const std::string& modId, 
									const std::uint32_t modPosition);

					/** \brief Gets the IEC_Datatype based on the string retrieved from XDC
					  * \param dataType Specifies the dataType value of object or parameter
					  * \return IEC_Datatype
					  */
					IEC_Datatype GetDataType(const std::string& dataType);

					/** \brief Gets the ParameterAccess based on the value retrieved from XDC
					  * \param access 		The access value of parameter
					  * \return ParameterAccess
					  */
					ParameterAccess GetParameterAccess(const std::string& access);

					/** \brief Gets the ObjectType of object from XDC
					  * \param objType 		Specifies the type of an object
					  * \return ObjectType
					 */
					ObjectType GetObjectType(const std::uint8_t objType);

					/** \brief Gets the PlkDataType of object from XDC
					  * \param plkDataType 	Specifies the data type of an object
					  * \return PlkDataType
					  */
					PlkDataType GetPlkDataType(const std::string& plkDataType);

					/** \brief Gets the AccessType of object from XDC
					  * \param accessType 	Specifies the access type of object
					  * \return AccessType
					  */
					AccessType GetObjAccessType(const std::string& accessType);

					/** \brief Gets the PDOMapping of object or sub-object from XDC
					  * \param pdoMapp Specifies the mapping value of object or sub-object
					  * \return PDOMapping
					  */
					PDOMapping GetPdoMapping(const std::string& pdoMapp);

					/** \brief Gets the IEC_Datatype of object or sub-object 
					  *        based on element in XDC
					  * \param node 		Instance of DOMNode
					  * \return IEC_Datatype
					  */
					IEC_Datatype GetIecDataType(const xercesc::DOMNode* node);

			}; // end of class ProjectParser
		} // end of namespace Application
	} // end of namespace POWERLINK
} // end of namespace IndustrialNetwork

#endif // _PROJECT_PARSER_H_
