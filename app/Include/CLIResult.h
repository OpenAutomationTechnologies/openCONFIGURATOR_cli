/**
 * \class CLIResult
 *
 * \brief Error code messages and handling functionalities for CLI
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 0.1
 *
 */

#ifndef CLI_RESULT_H
#define CLI_RESULT_H

#include "IResult.h"
#include "CLIErrorCode.h"

using namespace IndustrialNetwork::Fieldbus;
using namespace IndustrialNetwork::POWERLINK::Application::ErrorHandling;

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Application
		{
			namespace ErrorHandling
			{
				const std::string kMsgCoreApiFailed[] = {"Core library API: \"%s\" failed",""};
				const std::string kMsgCliApiFailed[] = {"CLI library API: \"%s\" failed",""};
				const std::string kMsgExceptionCaught[] = {"Exception caught at \"%s\" ",""};
				const std::string kMsgLessNoOfParams[] = {"Project XML file name and output path are mandatory",""};
				const std::string kMsgOutputPathNotFound[] = {"Project output path not found from command line parameters",""};
				const std::string kMsgXMLFileNotFound[] = {"Project XML file name is not found from command line parameters",""};
				const std::string kMsgXMLFileNotExists[] = {"XML file \"%s\" does not exists",""};
				const std::string kMsgXMLFileExtnIncorrect[] = {"XML file extension is incorrect",""};
				const std::string kMsgSchemaFileNotExists[] = {"Schema file \"%s\" does not exists",""};
				const std::string kMsgOutputPathNotExiists[] = {"Output path \"%s\" not exists",""};
				const std::string kMsgErrorLoadingGrammer[] = {"Error loading grammer to DOM Parser",""};
				const std::string kMsgFileSchemeNotValid[] = {"Schema of file \"%s\" is not valid",""};
				const std::string kMsgNullPtrFound[] = {"NULL pointer found on \"%s\" ",""};
				const std::string kMsgNoResultForXPath[] = {"There is no result for the Managing Node XPath \"%s\" ",""};

				/** \brief Functionalities to handle the error messages
				  */
				class CLIResult : public IndustrialNetwork::Fieldbus::IResult<IndustrialNetwork::POWERLINK::Application::ErrorHandling::CLIErrorCode>
				{

					public:
						/// Default constructor of the class
						CLIResult();

						/// Descriptor of the class
						virtual ~CLIResult();

						/// Constructor of the class with error code
						explicit CLIResult(const CLIErrorCode& errorCode);

						/// Constructor of the class with error code and message
						CLIResult(const CLIErrorCode& errorCode, const std::string& errorMessage);

						/** \brief Status of the function
						  * \return true if function returns SUCCESS; false otherwise
						  */
						bool IsSuccessful();
				};
			} // end of namespace ErrorHandling
		} // end of namespace Application
	} // end of namespace POWERLINK
} // end of namespace IndustrialNetwork

#endif // CLI_RESULT_H
