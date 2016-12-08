/**
 * \file CLIErrorCode
 *
 * \brief Error code enums of CLI application
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 0.1
 *
 */

#ifndef CLI_ERROR_CODE_H
#define CLI_ERROR_CODE_H

#include <cstdint>

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Application
		{
			namespace ErrorHandling
			{
				/** \brief Represents the error codes returned by the openCONFIGURATOR CLI
				  */
				enum class CLIErrorCode : std::uint8_t
				{
					SUCCESS = 0, /**< Operation completed successfully */
					CORE_API_FAILED, /**< Core library API failed */
					CLI_API_FAILED, /**< CLI library API failed */
					EXCEPTION_CAUGHT, /**< Exception caught */
					LESS_NO_OF_PARAMS, /**< Project XML file name and output path are mandatory */
					OUTPUT_PATH_NOT_FOUND, /**< Project output path not found from command line parameters */
					XML_FILE_NOT_FOUND, /**< Project XML file name is not found from command line parameters */
					XML_FILE_NOT_EXISTS, /**< XML file does not exists */
					XML_FILE_EXTN_INCORRECT, /**< XML file extension is incorrect */
					SCHEMA_FILE_NOT_EXISTS, /**< Schema file does not exists */
					OUTPUT_PATH_NOT_EXISTS, /**< Output path not exists */
					ERROR_LOADING_GRAMMER, /**< Error loading grammer to DOM Parser */
					FILE_SCHEMA_NOT_VALID, /**< Schema of file is not valid */
					NULL_POINTER_FOUND, /**< NULL pointer found */
					NO_RESULT_FOR_XPATH, /**< There is no result for the Managing Node XPath */
				};
			} // end of namespace ErrorHandling
		} // end of namespace Application
	} // end of namespace POWERLINK
} // end of namespace IndustrialNetwork

#endif // CLI_ERROR_CODE_H
