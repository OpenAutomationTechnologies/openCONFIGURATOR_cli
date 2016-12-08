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

#include "ParserElement.h"

ParserElement::ParserElement()
{
	/**< Initialize the Xerces usage */
	xercesc::XMLPlatformUtils::Initialize();

	domParser = new xercesc::XercesDOMParser;
}

ParserElement::~ParserElement()
{
	delete [] domParser;

	/**< Release the Xerces usage */
	xercesc::XMLPlatformUtils::Terminate();
}

CLIResult ParserElement::CreateElement(std::string file)
{
	try
	{
		filePath = file;

		/**< Input project XML file to DOM parse() function */
		domParser->parse(filePath.c_str());

		/**< Store the entire project XML file in DOMDocument */
		docHandle = domParser->getDocument();
		if (docHandle == NULL)
		{
			boost::format formatter(kMsgNullPtrFound[CLILogger::GetInstance().languageIndex]);
			formatter
			% "create DOM parser document";

			return CLIResult(CLIErrorCode::NULL_POINTER_FOUND, formatter.str());
		}

		/**< Store the Top node element of the document in root */
		docElement = docHandle->getDocumentElement();
		if (docElement == NULL)
		{
			boost::format formatter(kMsgNullPtrFound[CLILogger::GetInstance().languageIndex]);
			formatter
			% "create DOM parser element";

			return CLIResult(CLIErrorCode::NULL_POINTER_FOUND, formatter.str());
		}
	}
	catch (std::exception& ex)
	{
		return CLILogger::GetInstance().HandleExceptionCaught("Create Element", ex);
	}

	return CLIResult();
}
