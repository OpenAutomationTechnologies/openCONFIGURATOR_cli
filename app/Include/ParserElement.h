/**
 * \class ParserElement
 *
 * \brief Set of handles of a file to be parsed
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 0.1
 *
 */

#ifndef PARSER_ELEMENT_H
#define PARSER_ELEMENT_H

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

					/// Descriptor of the class
					~ParserElement();

					/** \brief Creates and parser element
					  * \param file XML or XCD file to be parsed
					  * \return CLIResult
					  */
					CLIResult CreateElement(std::string file);

					xercesc::DOMDocument* docHandle;			///< Document handle

					xercesc::DOMElement* docElement;			///< Document element handle

				private:
					std::string filePath;						///< file that has to be parsed

					xercesc::XercesDOMParser* domParser;		///< Create DOM parser

			}; // end of class ParserElement
		} // end of namespace Application
	} // end of namespace POWERLINK
} // end of namespace IndustrialNetwork

#endif // PARSER_ELEMENT_H
