#include "OpenConfiguratorCore.h"
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>

using namespace IndustrialNetwork::POWERLINK::Core::API;
XERCES_CPP_NAMESPACE_USE

int main()
{
	std::cout << "OK" << std::endl;
	OpenConfiguratorCore::GetInstance();
	// Initialize the XML4C2 system.
	try
	{
		XMLPlatformUtils::Initialize();
		XMLPlatformUtils::Terminate();
	}

	catch (const XMLException& toCatch)
	{
		char* pMsg = XMLString::transcode(toCatch.getMessage());
		XERCES_STD_QUALIFIER cerr << "Error during Xerces-c Initialization.\n"
		                          << "  Exception message:"
		                          << pMsg;
		XMLString::release(&pMsg);
		return 1;
	}
	return 0;
}
