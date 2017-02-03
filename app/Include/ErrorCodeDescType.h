/**
 * \file ErrorCodeDescType
 *
 * \brief Includes the data members of Error Code description type
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

#ifndef ERROR_CODE_DESC_TYPE_H_
#define ERROR_CODE_DESC_TYPE_H_

#include <cstdint>
#include "Language.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Application
		{
			class ErrorCodeDescType
			{
				public:
					/** \brief Default constructor of the class
					  */
					ErrorCodeDescType() :
						lang(IndustrialNetwork::POWERLINK::Core::CoreConfiguration::Language::EN),
						value("")
					{
					}

					/** \brief Destructor of the class
					  */
					~ErrorCodeDescType()
					{
					}

					/** Language of the error message */
					IndustrialNetwork::POWERLINK::Core::CoreConfiguration::Language lang;

					/** Error message */
					std::string value;

			}; // end of class ErrCodeDescType
		} // end of namespace Application
	} // end of namespace POWERLINK
} // end of namespace IndustrialNetwork

#endif // _ERROR_CODE_DESC_TYPE_H_
