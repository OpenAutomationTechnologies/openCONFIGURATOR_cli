/**
 * \class ParserResult
 *
 * \brief Creates results after parsing files
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 0.1
 *
 */ // REVIEW_COMMENT: Copyright and license

#ifndef PARSER_RESULT_H // REVIEW_COMMENT: underscore
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

					/// Descriptor of the class // REVIEW_COMMENT: spelling
					~ParserResult();
// REVIEW_COMMENT: align all the param descriptions
					/** \brief Parses and produces attributes from root
					  * \param pElement file element to be parsed // REVIEW_COMMENT: File 
					  * \param transcodeString Transcode input string for parsing
					  * \param formatString Attribute names as string separated by comma
					  * \return CLIResult
					  */
					CLIResult CreateResult(ParserElement& pElement,// REVIEW_COMMENT: const
										std::string transcodeString, // REVIEW_COMMENT: one more tab
										std::string formatString);
// REVIEW_COMMENT: check for smart pointers
					/** \brief Parses and produces attributes by considering
					  *        an sub node element as parent
					  * \param pElement file element to be parsed// REVIEW_COMMENT: File
					  * \param transcodeString Transcode input string for parsing
					  * \param formatString Attribute names as string separated by comma
					  * \param parentNode Sub node to be considered as parent
					  * \return CLIResult
					  */
					CLIResult CreateResult(ParserElement& pElement,// REVIEW_COMMENT: const
										std::string transcodeString,
										std::string formatString,
										xercesc::DOMNode* parentNode);

					std::vector<std::vector<std::string> > parameters;		///< Parsed parameters

					xercesc::DOMNode* resultNodeValue;						///< Parent node

					std::vector<xercesc::DOMNode*> node;					///< Parsed node

				private:
					/**
					  * \brief Gets the value of attribute available
					  * \param domNode Instance of DOMNode
					  * \param attributeName The name of the attribute
					  * \return attribute value
					  */
					std::string GetAttributeValue(xercesc::DOMNode* domNode,
										std::string attributeName);// REVIEW_COMMENT: 

			}; // end of class ParserResult
		} // end of namespace Application
	} // end of namespace POWERLINK
} // end of namespace IndustrialNetwork

#endif // PARSER_RESULT_H// REVIEW_COMMENT: underscore
