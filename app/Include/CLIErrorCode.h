/**
 * \file CLIErrorCode
 *
 * \brief Error codes enums of CLI application
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 0.1
 *
 */

#if !defined CLI_ERROR_CODE_H
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
				/**
				  *\brief Represents the error codes returned by the openCONFIGURATOR CLI
				  */
				enum class CLIErrorCode : std::uint8_t
				{
					SUCCESS = 0,//!< Operation completed successfully.
					LESS_NO_OF_PARAMS, //!< Project XML file name and output path are mandatory
					OUTPUT_PATH_NOT_FOUND, //!< Project output path not found from command line parameters
					XML_FILE_NOT_FOUND, //!< Project XML file not found from command line parameters
					XML_FILE_NOT_EXISTS, //!< XML file not exists
					XML_FILE_EXTN_INCORRECT, //!< XML file extension is incorrect
					OUTPUT_PATH_NOT_EXISTS //!< Output path not exists
				};
			} // end of namespace ErrorHandling
		} // end of namespace Application
	} // end of namespace POWERLINK
} // end of namespace IndustrialNetwork
#endif // CLI_ERROR_CODE_H
