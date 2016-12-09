/**
 * \file CLIResult
 *
 * \brief Error code handler using IResult of core library
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 0.1
 *
 */ // REVIEW_COMMENT: copyright and License

#include "CLIResult.h"

CLIResult::CLIResult(const CLIErrorCode& errorCode, const std::string& errorMessage = "") :
	IResult<CLIErrorCode>(errorCode, errorMessage)
{
}

CLIResult::CLIResult(const CLIErrorCode& errorCode) :
	IResult<CLIErrorCode>(errorCode)
{
}

CLIResult::CLIResult() :
	IResult<CLIErrorCode>(CLIErrorCode::SUCCESS)
{
}

CLIResult::~CLIResult()
{
}

bool CLIResult::IsSuccessful() // REVIEW_COMMENT: check for bool
{
	return (this->GetErrorType() == CLIErrorCode::SUCCESS);
}
