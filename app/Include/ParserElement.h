/**
 * \class ParserElement
 *
 * \brief Set of handles of a file to be parsed
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 0.1
 *
 */// REVIEW_COMMENT: copyright and License

#ifndef PARSER_ELEMENT_H	// REVIEW_COMMENT: Use undescore before and after
#define PARSER_ELEMENT_H
// REVIEW_COMMENT: can be a better name instead ParserElement
#include "OpenConfiguratorCLI.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Application
		{
			class ParserElement
			{
				public:
					/// Default constructor of the class
					ParserElement();

					/// Descriptor of the class// REVIEW_COMMENT: spelling mistake
					~ParserElement();

					/** \brief Creates and parser element // REVIEW_COMMENT: parses
					  * \param file XML or XCD file to be parsed// REVIEW_COMMENT: XDC
					  * \return CLIResult
					  */
					CLIResult CreateElement(std::string file);

					xercesc::DOMDocument* docHandle;			///< Document handle // REVIEW_COMMENT: avoid pointers

					xercesc::DOMElement* docElement;			///< Document element handle

				private:
					std::string filePath;						///< file that has to be parsed

					xercesc::XercesDOMParser* domParser;		///< Create DOM parser

			}; // end of class ParserElement
		} // end of namespace Application
	} // end of namespace POWERLINK
} // end of namespace IndustrialNetwork

#endif // PARSER_ELEMENT_H // REVIEW_COMMENT: underscore
