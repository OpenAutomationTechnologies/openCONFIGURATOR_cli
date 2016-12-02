/**
 * \file ParserElement.cpp
 *
 * \brief Implementation of ParserElement module
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 0.1
 *
 */

#include "OpenConfiguratorCore.h"
#include "OpenConfiguratorCLI.h"
#include "ParserElement.h"

ParserElement::ParserElement()
{
	XMLPlatformUtils::Initialize();  /**< Initialize the Xerces usage */
	this->domParser = new xercesc::XercesDOMParser();
}

ParserElement::~ParserElement()
{
	delete[] this->domParser;
	XMLPlatformUtils::Terminate();   /**< Release the Xerces usage */
}

bool ParserElement::CreateElement(std::string file)
{
	filePath = file;
	/**< Input project XML file to DOM parse() function */
	this->domParser->parse(filePath.c_str()); 

	/**< Store the entire project XML file in DOMDocument */
	this->docHandle = this->domParser->getDocument(); 
	/**< Store the Top node element of the document in root */
	this->docElement = docHandle->getDocumentElement(); 

	return true;
}
