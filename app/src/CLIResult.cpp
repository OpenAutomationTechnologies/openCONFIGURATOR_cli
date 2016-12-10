/**
 * \file CliResult
 *
 * \brief Error code handler using IResult of core library
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 0.1
 *
 */ // REVIEW_COMMENT: copyright and License

#include "CliResult.h"

CliResult::CliResult(const CliErrorCode& errorCode, const std::string& errorMessage = "") :
	IResult<CliErrorCode>(errorCode, errorMessage)
{
}

CliResult::CliResult(const CliErrorCode& errorCode) :
	IResult<CliErrorCode>(errorCode)
{
}

CliResult::CliResult() :
	IResult<CliErrorCode>(CliErrorCode::SUCCESS)
{
}

CliResult::~CliResult()
{
}

bool CliResult::IsSuccessful() // REVIEW_COMMENT: check for bool
{
	return (this->GetErrorType() == CliErrorCode::SUCCESS);
}
