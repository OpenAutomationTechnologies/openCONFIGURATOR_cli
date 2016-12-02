/**
 * \file CLIResult
 *
 * \brief Error code handler using IResult of core library
 *
 * \author Kalycito Infotech Private Limited
 *
 * \version 0.1
 *
 */

#include "CLIResult.h"

using namespace IndustrialNetwork::POWERLINK::Application::ErrorHandling;

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

bool CLIResult::IsSuccessful()
{
	return (this->GetErrorType() == CLIErrorCode::SUCCESS);
}
