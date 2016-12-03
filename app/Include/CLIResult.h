/**
 * \class CLIResult
 *
 * \brief Error codes and messages
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 0.1
 *
 */

#if !defined CLI_RESULT_H
#define CLI_RESULT_H

#include <string>

#include "OpenConfiguratorCLI.h"
#include "IResult.h"
#include "CLIErrorCode.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Application
		{
			namespace ErrorHandling
			{
				const std::string kMsgLessNoOfParams = "Project XML file name and output path are mandatory";
				const std::string kMsgOutputPathNotFound = "Project output path not found from command line parameters";
				const std::string kMsgXMLFileNotFound = "Project XML file not found from command line parameters";
				const std::string kMsgXMLFileNotExists = "XML file not exists";
				const std::string kMsgXMLFileExtnIncorrect = "XML file extension is incorrect";
				const std::string kMsgOutputPathNotExiists = "Output path not exists";

				/** \brief Functionalities to handle the error messages
				  */
				class CLIResult : public IndustrialNetwork::Fieldbus::IResult<IndustrialNetwork::POWERLINK::Application::ErrorHandling::CLIErrorCode>
				{

					public:
						/// Default constructor of the class
						CLIResult();

						/// Descriptor of the class
						virtual ~CLIResult();

						explicit CLIResult(const CLIErrorCode& errorCode);

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
