/************************************************************************
\file ProjectParser.h
\author Sree Hari Vignesh, Kalycito Infotech Private Limited.
\brief Specifies the method used for executing library API
************************************************************************/

/*------------------------------------------------------------------------------
Copyright (c) 2016, Kalycito Infotech Private Limited
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
#ifndef PROJECT_PARSER_H
#define PROJECT_PARSER_H

#include <fstream>
#include <string>
#include <list>

#include "OpenConfiguratorCore.h"
#include "Parser.h"

namespace IndustrialNetwork
{
	namespace POWERLINK
	{
		namespace Application
		{
			namespace openCONFIGURATORCLI
			{
				class ProjectParser
				{
					public:
						/*
						\Returns the instance of LibraryUtils.
						*/
						static ProjectParser& GetInstance();

						/*
						\Updates the information of Node XDC into library API.
						\param nodeidList Specifies the node id value of node.
						\param cnXDC Specifies the XDC path of node.
						*/
						void UpdateNodeIdCollection(std::uint8_t nodeIdList, const std::string& cnXDC);

						/*
						\Updates the Device POWERLINK profile body of XDD/XDC into core library.
						\param doc Specifies the instance of XDC document.
						\param nodeID Specifies the Id value of node.
						*/
						void ImportProfileBodyDevicePOWERLINK(xercesc::DOMDocument* doc, std::uint8_t nodeId);
						void UpdateManagingNode();
						void UpdateControlledNode();
						void UpdateModules();
						void UpdateParameters();
						void UpdateObjects();
				};
			}
		}
	}
}
#endif // PROJECT_PARSER_H
