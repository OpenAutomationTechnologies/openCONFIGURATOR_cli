################################################################################
#
# Project: openCONFIGURATOR commandline tool v2.0
#
# (c) Bernecker + Rainer Industrie-Elektronik Ges.m.b.H.
#     B&R Strasse 1, A-5142 Eggelsberg
#     www.br-automation.com
#
# Description: CMake file for the openCONFIGURATOR commandline tool v2.0
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

PROJECT (openconfigurator_cl_app_exe)

IF(WIN32)
	SET(XERCES_ROOT_DIR ${PROJECT_SOURCE_DIR}/xerces-c-3.1.4 CACHE PATH "Xerces C Root Directory")
	SET(XercesC_INC_DIR ${XERCES_ROOT_DIR}/src)
	SET(XercesC_LIB_DIR ${XERCES_ROOT_DIR}/lib)
	SET(CMAKE_PREFIX_PATH ${XercesC_INC_DIR} ${XercesC_LIB_DIR})
ENDIF()

###############################################################################
# Find XercesC libraries
###############################################################################
INCLUDE(FindXercesC)
FIND_PACKAGE(XercesC REQUIRED)

###############################################################################
# Set general sources and header of core library
###############################################################################
FILE (GLOB_RECURSE LIB_SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
FILE (GLOB_RECURSE LIB_HEADERS "${PROJECT_SOURCE_DIR}/Include/*.h")

###############################################################################
# Set library include directories
###############################################################################
IF(WIN32)
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Include/)
	INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/openCONFIGURATOR_2.0_core/library/Include/)
	INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${XercesC_INCLUDE_DIRS})
ELSE(WIN32)
	# This will cause the Boost-Headers to be included via GNU GCC's "-isystem"
	# option and hence will treat the Boost-Headers as system headers
	# (and suppress warnings for them).
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Include/)
	INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/openCONFIGURATOR_2.0_core/library/Include/)
	INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS} ${XercesC_INCLUDE_DIRS})
ENDIF(WIN32)

###############################################################################
# Create the application
###############################################################################
ADD_EXECUTABLE(${OPEN_CONFIGURATOR_CLI_NAME} ${LIB_SOURCES} ${LIB_HEADERS})

###############################################################################
# Link the application to the dependencies
###############################################################################
TARGET_LINK_LIBRARIES(${OPEN_CONFIGURATOR_CLI_NAME} openconfigurator_core_lib ${XercesC_LIBRARY})

###############################################################################
# Install the application and dependencies to bin directory
###############################################################################
IF(WIN32)
	# Install Boost libraries
	FILE (GLOB XERCES_DEBUG_LIBS "${XercesC_LIB_DIR}/xerces-c_3_1D.dll")
	FILE (GLOB XERCES_RELEASE_LIBS "${XercesC_LIB_DIR}/xerces-c_3_1.dll")
	INSTALL(FILES ${XERCES_DEBUG_LIBS} DESTINATION ${INSTALL_DIR} CONFIGURATIONS Debug)
	INSTALL(FILES ${XERCES_RELEASE_LIBS} DESTINATION ${INSTALL_DIR} CONFIGURATIONS Release)
ENDIF()

INSTALL(
	TARGETS ${OPEN_CONFIGURATOR_CLI_NAME}
	RUNTIME DESTINATION ${INSTALL_DIR}
	LIBRARY DESTINATION ${INSTALL_DIR}
)
