################################################################################
#
# Project: openCONFIGURATOR commandline tool v2.0
#
# (c) Bernecker + Rainer Industrie-Elektronik Ges.m.b.H.
#     B&R Strasse 1, A-5142 Eggelsberg
#     www.br-automation.com
#
# Description: Main CMAKE file of openCONFIGURATOR commandline tool v2.0
#
# License:
#
#   Redistribution and use in source and binary forms, with or without
#   modification, are permitted provided that the following conditions
#   are met:
#
#   1. Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#
#   2. Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in the
#      documentation and/or other materials provided with the distribution.
#
#   3. Neither the name of the copyright holders nor the names of its
#      contributors may be used to endorse or promote products derived
#      from this software without prior written permission. For written
#      permission, please contact office@br-automation.com.
#
#   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
#   FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
#   COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
#   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
#   BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
#   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
#   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#   POSSIBILITY OF SUCH DAMAGE.
#
#   Severability Clause:
#
#       If a provision of this License is or becomes illegal, invalid or
#       unenforceable in any jurisdiction, that shall not affect:
#       1. the validity or enforceability in that jurisdiction of any other
#          provision of this License; or
#       2. the validity or enforceability in other jurisdictions of that or
#          any other provision of this License.
#
################################################################################
CMAKE_MINIMUM_REQUIRED (VERSION 3.1.0)

INCLUDE(CMakeDependentOption)

PROJECT (openconfigurator_cl_app)
###############################################################################
# Compiler flags for MS Visual Studio
###############################################################################
IF(MSVC)
	SET(CMAKE_CXX_FLAGS "/wd\"4512\" /wd\"4251\" /wd\"4091\" /Zi /nologo /W4 /WX- /Od /Oy- /Ob0 /D_CRT_SECURE_NO_WARNINGS /D_SCL_SECURE_NO_WARNINGS /Gm- /EHsc /GS /fp:precise /Zc:wchar_t /Zc:forScope /Gd /analyze- /errorReport:queue /GR")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /DDEBUG")
	STRING(FIND "${CMAKE_GENERATOR}" "Win64" FOUND)
	IF(${FOUND} EQUAL -1)
		SET(INSTALL_DIR "${CMAKE_SOURCE_DIR}/bin/win32/x86/\${BUILD_TYPE}")
		SET(INSTALL_DIR_VS "${CMAKE_SOURCE_DIR}/bin/win32/x86/$(Configuration)")
	ELSE()
		SET(INSTALL_DIR "${CMAKE_SOURCE_DIR}/bin/win32/x86_64/\${BUILD_TYPE}")
		SET(INSTALL_DIR_VS "${CMAKE_SOURCE_DIR}/bin/win32/x86_64/$(Configuration)")
	ENDIF()
ELSEIF(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Weffc++ -std=c++11 -pthread")
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
	IF(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
		SET(INSTALL_DIR "${CMAKE_SOURCE_DIR}/bin/linux/x86_64")
	ELSE()
		SET(INSTALL_DIR "${CMAKE_SOURCE_DIR}/bin/linux/x86")
	ENDIF()
	IF(MINGW OR CYGWIN)
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -U__STRICT_ANSI__ -D__NO_INLINE__ -Wl,--kill-at")
	ENDIF()
ENDIF(MSVC)
###############################################################################
# Find BOOST libraries
###############################################################################
SET(Boost_DEBUG TRUE)
SET(Boost_USE_STATIC_RUNTIME OFF)
FIND_PACKAGE(Boost COMPONENTS log log_setup chrono date_time filesystem system thread REQUIRED)
# Necessary for Auto-Linking under VS, ignored under Unix
ADD_DEFINITIONS(/DBOOST_ALL_DYN_LINK)
IF(NOT Boost_FOUND)
	MESSAGE(SEND_ERROR "Boost libraries not found. Please add BOOST_ROOT environment variable to system.")
# Boost version above 1.55.0 require boost regex to be linked
ELSEIF(Boost_FOUND AND Boost_MINOR_VERSION GREATER 55 AND Boost_MINOR_VERSION LESS 59)
	FIND_PACKAGE(Boost COMPONENTS log log_setup chrono date_time filesystem system thread regex REQUIRED)
# Boost version above 1.59.0 require boost atomic to be linked
ELSEIF(Boost_FOUND AND Boost_MINOR_VERSION GREATER 59)
	FIND_PACKAGE(Boost COMPONENTS atomic log log_setup chrono date_time filesystem system thread regex REQUIRED)
ENDIF()

# Require Visual Studio 2012 or newer required
IF (MSVC)
	STRING(REPLACE " " ";" VS_NAME ${CMAKE_GENERATOR})
	LIST(GET VS_NAME 2 VS_VERSION)
	IF (NOT ${VS_VERSION} GREATER 10)
		MESSAGE(FATAL_ERROR "Visual Studio 2012 or newer is required, you have " ${CMAKE_GENERATOR})
	ENDIF()

	STRING(FIND "${CMAKE_GENERATOR}" "Win64" FOUND)
	IF(${FOUND} EQUAL -1)
		SET(INSTALL_DIR "${CMAKE_SOURCE_DIR}/bin/win32/x86/\${BUILD_TYPE}")
		SET(INSTALL_DIR_VS "${CMAKE_SOURCE_DIR}/bin/win32/x86/$(Configuration)")
	ELSE()
		SET(INSTALL_DIR "${CMAKE_SOURCE_DIR}/bin/win32/x86_64/\${BUILD_TYPE}")
		SET(INSTALL_DIR_VS "${CMAKE_SOURCE_DIR}/bin/win32/x86_64/$(Configuration)")
	ENDIF()
ENDIF()

###############################################################################
# Add the openCONFIGURATOR library as submodule
###############################################################################
ADD_SUBDIRECTORY(openCONFIGURATOR_2.0_core)
###############################################################################
# Add commandline target
###############################################################################
ADD_SUBDIRECTORY(app)

