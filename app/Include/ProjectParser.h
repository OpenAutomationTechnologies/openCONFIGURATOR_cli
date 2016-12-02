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
					  * \return true if parsing is success; false otherwise
					  */
					bool ParserXMLFile(std::string xmlPath);

				private:
					/**
					  * \brief Updates the information of Node XDC into library API
					  * \param nodeId Specifies the node id value of node
					  * \param pathXDC Specifies the XDC path of node
					  * \return Nothing
					  */
					bool UpdateNodeIdCollection(std::uint8_t nodeId, const std::string& pathXDC);

					/**
					  * \brief Updates the Device POWERLINK profile body of XDD/XDC into core library
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \return Nothing
					  */
					bool ImportProfileBodyDevicePOWERLINK(ParserElement& element, std::uint8_t nodeId);

					/**
					  * \brief Updates the Communication POWERLINK profile body of XDD/XDC into core library
					  * \param element element that contains handle of XDC document
					  * \param nodeId Specifies the Id value of node
					  * \return Nothing
					  */
					bool ImportProfileBodyCommunicationPOWERLINK(ParserElement& element, std::uint8_t nodeId);

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


					bool AddDataType(ParserElement& element, std::uint8_t nodeId);
					bool CreateStructDataType(ParserElement& element, std::uint8_t nodeId);
					bool CreateArrayDataType(ParserElement& element, std::uint8_t nodeId);
					bool CreateParameterTemplate(ParserElement& element, std::uint8_t nodeId);
					bool CreateParameterGroup(ParserElement& element, std::uint8_t nodeId);
					bool CreateParameterList(ParserElement& element, std::uint8_t nodeId);
					bool CreateChildParameterGroup(ParserElement& element, std::uint8_t nodeId, xercesc::DOMNode* parameterGroupResult, std::string uniqueId);
					bool SetParamAllowedValue(ParserElement& element,DOMNode* node,std::uint8_t nodeId,std::string uniqueId);
					bool SetParamAllowedRange(ParserElement& element,DOMNode* node,std::uint8_t nodeId,std::string uniqueId);
					bool SetParamActualValue(ParserElement& element,DOMNode* node,std::uint8_t nodeId,std::string uniqueId);
					bool SetParamDefaultValue(ParserElement& element,DOMNode* node,std::uint8_t nodeId,std::string uniqueId);
					bool CreateObject(ParserElement& element, std::uint8_t nodeId);
			};
		} // end of namespace Application
	} // end of namespace POWERLINK
} // end of namespace IndustrialNetwork
#endif // PROJECT_PARSER_H

