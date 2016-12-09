/**
 * \file ParserElement.cpp
 *
 * \brief Implementation of ParserElement module
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 0.1
 *
 */ // REVIEW_COMMENT: copyright and License

#include "ParserElement.h"

ParserElement::ParserElement()
{
	/**< Initialize the Xerces usage */
	xercesc::XMLPlatformUtils::Initialize();

	domParser = new xercesc::XercesDOMParser();
}

ParserElement::~ParserElement()
{
	delete [] domParser;

	/**< Release the Xerces usage */
	xercesc::XMLPlatformUtils::Terminate();
}

CLIResult ParserElement::CreateElement(std::string file) // REVIEW_COMMENT: accept input as part of constructor
{
	try
	{
		filePath = file;

		/**< Input project XML file to DOM parse() function */
		domParser->parse(filePath.c_str());

		/**< Store the entire project XML file in DOMDocument */
		docHandle = domParser->getDocument(); // REVIEW_COMMENT: check for release ()
		if (docHandle == NULL)
		{
			boost::format formatter(kMsgNullPtrFound[CLILogger::GetInstance().languageIndex]);// REVIEW_COMMENT: 80 characters limit per line
			formatter // REVIEW_COMMENT: keep it in a single line
			% "create DOM parser document";

			return CLIResult(CLIErrorCode::NULL_POINTER_FOUND, formatter.str());
		}

		/**< Store the Top node element of the document in root */
		docElement = docHandle->getDocumentElement(); // REVIEW_COMMENT: check for release ()
		if (docElement == NULL)
		{
			boost::format formatter(kMsgNullPtrFound[CLILogger::GetInstance().languageIndex]);// REVIEW_COMMENT: 80 characters limit per line
			formatter// REVIEW_COMMENT: keep it in a single line
			% "create DOM parser element";

			return CLIResult(CLIErrorCode::NULL_POINTER_FOUND, formatter.str());
		}
	}
	catch (std::exception& ex) // REVIEW_COMMENT: e
	{
		return CLILogger::GetInstance().HandleExceptionCaught("Create Element", ex);// REVIEW_COMMENT: 80 characters limit per line
	}

	return CLIResult();
}
