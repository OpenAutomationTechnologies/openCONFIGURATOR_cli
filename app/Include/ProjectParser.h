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
					  * \param nodeId Specifies the node id value of node
					  * \param pathXDC Specifies the XDC path of node
					  * \return CLIResult
					  */
					CLIResult UpdateNodeIdCollection(std::uint8_t nodeId, const std::string& pathXDC);

					/**
					  * \brief Updates the Device POWERLINK profile body of XDD/XDC into core library
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \return CLIResult
					  */
					CLIResult ImportProfileBodyDevicePOWERLINK(ParserElement& element, std::uint8_t nodeId);

					/**
					  * \brief Updates the Communication POWERLINK profile body of XDD/XDC into core library
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \return CLIResult
					  */
					CLIResult ImportProfileBodyCommunicationPOWERLINK(ParserElement& element, std::uint8_t nodeId);

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
					  * \brief Creates Struct and Array data type parse results
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \return CLIResult
					  */
					CLIResult CreateDataType(ParserElement& element, std::uint8_t nodeId);

					/**
					  * \brief Creates Struct data type parse results
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \return CLIResult
					  */
					CLIResult CreateStructDataType(ParserElement& element, std::uint8_t nodeId);

					/**
					  * \brief Creates Array data type parse results
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \return CLIResult
					  */
					CLIResult CreateArrayDataType(ParserElement& element, std::uint8_t nodeId);

					/**
					  * \brief Creates parameter template related parse results
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \return CLIResult
					  */
					CLIResult CreateParameterTemplate(ParserElement& element, std::uint8_t nodeId);

					/**
					  * \brief Creates parameter group related parse results
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \return CLIResult
					  */
					CLIResult CreateParameterGroup(ParserElement& element, std::uint8_t nodeId);

					/**
					  * \brief Creates parameter list related parse results
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \return CLIResult
					  */
					CLIResult CreateParameterList(ParserElement& element, std::uint8_t nodeId);

					/**
					  * \brief Creates child parameter group related parse results
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \param parameterGroupNode Parent parameter group node 
					  * \param paramGroupUId Parent parameter group node ID
					  * \return CLIResult
					  */
					CLIResult CreateChildParameterGroup(ParserElement& element, std::uint8_t nodeId, 
													xercesc::DOMNode* parameterGroupNode, 
													std::string paramGroupUId);

					/**
					  * \brief Creates object related parse results and set parameters
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \return CLIResult
					  */
					CLIResult CreateObject(ParserElement& element, std::uint8_t nodeId);

					/**
					  * \brief Sets allowed values
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \return CLIResult
					  */
					CLIResult SetParamAllowedValue(ParserElement& element, std::uint8_t nodeId, 
												xercesc::DOMNode* node, std::string uniqueId);

					/**
					  * \brief Sets allowed range values
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \return CLIResult
					  */
					CLIResult SetParamAllowedRange(ParserElement& element, std::uint8_t nodeId, 
												xercesc::DOMNode* node, std::string uniqueId);

					/**
					  * \brief Sets actual values
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \return CLIResult
					  */
					CLIResult SetParamActualValue(ParserElement& element, std::uint8_t nodeId, 
												xercesc::DOMNode* node, std::string uniqueId);

					/**
					  * \brief Sets dafault values
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \return CLIResult
					  */
					CLIResult SetParamDefaultValue(ParserElement& element, std::uint8_t nodeId, 
												xercesc::DOMNode* node, std::string uniqueId);

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

