/**
 * \class CliResult
 *
 * \brief Error code messages and handling functionalities for CLI
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

#ifndef CLI_RESULT_H_
#define CLI_RESULT_H_

#include "IResult.h"
#include "CliErrorCode.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Application
		{
			namespace ErrorHandling
			{
				const std::string kMsgAppDescription[] = { "openCONFIGURATOR CLI application", "openCONFIGURATOR CLI application" };
				const std::string kMsgInvalidParameters[] = { "Invalid parameters.", "Ungueltige Parameter." };
				const std::string kMsgExceptionCaught[] = { "Exception thrown at function \"%s\".", "Exception wurde in der Funktion \"%s\" ausgeloest." };
				const std::string kMsgOutputPathNotFound[] = { "Project output path not found with the command line parameters.", "Projektausgabepfad wurde in den Aufrufparametern nicht gefunden." };
				const std::string kMsgXmlFileNotFound[] = { "Project XML file name is not found from command line parameters.", "Projekt XML Datei wurde in den Aufrufparametern nicht gefunden." };
				const std::string kMsgFileNotExists[] = { "File \"%s\" does not exists.", "Die Datei \"%s\" existiert nicht." };
				const std::string kMsgFileExtnIncorrect[] = { "Fileextension \"%s\" is incorrect.", "Dateierweiterung \"%s\" ist ungueltig." };
				const std::string kMsgOutputPathNotExiists[] = { "Output path \"%s\" not exists.", "Projektausgabepfad \"%s\" existiert nicht." };
				const std::string kMsgErrorLoadingGrammer[] = { "Failed to load grammer to DOM Parser.", "Initialisierung des DOM Parsers ist fehlgeschlagen." };
				const std::string kMsgFileSchemaNotValid[] = { "XML schema validation of file \"%s\" failed.", "Die XML Schemavaliddierung der Datei \"%s\" ist fehlgeschlagen." };
				const std::string kMsgNullPtrFound[] = { "NULL pointer execption raised at \"%s\".", "NULL Zeigerfehler wurde in der Funktion \"%s\" ausgeloest." };
				const std::string kMsgNoResultForXPath[] = { "No result for the XPath \"%s\".", "Kein Resultat fuer den folgenden XPath \"%s\"." };
				const std::string kMsgUnableToOpenLogFile[] = { "Unable to open the log file \"%s\".", "Die Logdatei \"%s\" konnte nicht geoeffnet werden." };
				const std::string kMsgErrorTableNotLoaded[] = { "Failed to load error code table.", "Die Fehlercodetabelle konnte nicht geladen werden." };
				const std::string kMsgErrorInfoNotFound[] = { "Failed to find error information.", "Die Fehlerinformation konnte nicht gefunden werden." };
				const std::string kMsgConfGenerationSuccess[] = { "POWERLINK configuration files generated successfully under \"%s\".", "POWERLINK Konfigurationsdateien wurden unter \"%s\" erfolgreich erstellt." };
				const std::string kMsgXmlValidationError[] = { "XML validation error at at line %llu column %llu: %s.", "XML Validierungsfehler in Zeile %llu Spalte %llu: %s." };

				/** \brief Functionalities to handle the error messages
				  */
				class CliResult : public IndustrialNetwork::Fieldbus::IResult<IndustrialNetwork::POWERLINK::Application::ErrorHandling::CliErrorCode>
				{

					public:
						/** \brief Default constructor of the class
						  */
						CliResult();

						/** \brief Destructor of the class
						  */
						virtual ~CliResult();

						/** \brief Constructor of the class with error code
						  */
						explicit CliResult(const CliErrorCode& errorCode);

						/** \brief Constructor of the class with error code and message
						  */
						CliResult(const CliErrorCode& errorCode, const std::string& errorMessage);

						/** \brief Status of the function
						  * \return true if function is successful; false otherwise
						  */
						bool IsSuccessful();
				};

			} // end of namespace ErrorHandling
		} // end of namespace Application
	} // end of namespace POWERLINK
} // end of namespace IndustrialNetwork

#endif // _CLI_RESULT_H_
