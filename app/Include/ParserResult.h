/**
 * \class ParserResult
 *
 * \brief Creates results after parsing files
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 0.1
 *
 */

#ifndef PARSER_RESULT_H
#define PARSER_RESULT_H

#include "OpenConfiguratorCLI.h"
#include "ParserElement.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Application
		{
			class ParserResult
			{
				public:
					/// Default constructor of the class
					ParserResult();

					/// Descriptor of the class
					~ParserResult();

					/** \brief Parses and produces attributes from root
					  * \param element file element to be parsed
					  * \param transcodeString Transcode input string for parsing
					  * \param formatString Attribute names as string separated by comma
					  * \return true on successful creation; false otherwise
					  */
					bool CreateResult(ParserElement& pElement,
										std::string transcodeString,
										std::string formatString);

					/** \brief Parses and produces attributes by considering
					  *        an sub node element as parent
					  * \param element file element to be parsed
					  * \param transcodeString Transcode input string for parsing
					  * \param formatString Attribute names as string separated by comma
					  * \param parentNode Sub node to be considered as parent
					  * \return true on successful creation; false otherwise
					  */
					bool CreateResult(ParserElement& pElement,
										std::string transcodeString,
										std::string formatString,
										DOMNode * parentNode);

					std::vector<std::vector<std::string> > parameters;		///< Parsed parameters

					DOMNode * resultNodeValue;								///< Parent node

					std::vector<DOMNode *> node;							///< Parsed node

				private:
					/**
					  * \brief Gets the value of attribute available in the XDC
					  * \param node Instance of DOMNode
					  * \param attributeName The name of the attribute
					  * \return attribute value
					  */
					std::string GetAttributeValue(DOMNode* domNode, std::string attributeName);

			}; // end of class ParserResult
		} // end of namespace Application
	} // end of namespace POWERLINK
} // end of namespace IndustrialNetwork

#endif // PARSER_RESULT_H
