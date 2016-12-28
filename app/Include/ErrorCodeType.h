/**
 * \file ErrorCodeType
 *
 * \brief Includes the data members of Error Code type
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

#ifndef _ERROR_CODE_TYPE_H_
#define _ERROR_CODE_TYPE_H_

#include "ErrorCodeDescType.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Application
		{
			/** Unknown error code value */
			const std::uint32_t kUnknownCodeValue = 0xFFFF;

			class ErrorCodeType
			{
				public:
					/** \brief Default constructor of the class
					  */
					ErrorCodeType() :
						value(""),
						originalCode(kUnknownCodeValue),
						toolCode(kUnknownCodeValue),
						descriptions()
					{
					}

					/** \brief Destructor of the class
					  */
					~ErrorCodeType()
					{
					}

					/** Enum name of the error code */
					std::string value;

					/** Original enum error code value */
					std::uint32_t originalCode;

					/** Tool code value of the error */
					std::uint32_t toolCode;

					/** Error description messages for different languages */
					std::vector<ErrorCodeDescType> descriptions;

			}; // end of class ErrorCodeType
		} // end of namespace Application
	} // end of namespace POWERLINK
} // end of namespace IndustrialNetwork

#endif // _ERROR_CODE_TYPE_H_
