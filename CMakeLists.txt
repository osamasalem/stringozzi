#********************************************************
# General Configuration
#********************************************************
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT(stringozzi CXX)

#SET(CMAKE_BUILD_TYPE  RELEASE)


#********************************************************
# Compiler Configuration
#********************************************************

SET( CMAKE_CXX_FLAGS_RELEASE			"/MT /Z7 /O2")
SET( CMAKE_BINARY_DIR 					${CMAKE_CURRENT_SOURCE_DIR}/output/)
#SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY 	${CMAKE_BINARY_DIR}/lib)
#SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY  	${CMAKE_BINARY_DIR}/lib)
#SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY 	${CMAKE_BINARY_DIR}/bin)
#SET( CMAKE_DOCS_OUTPUT_DIRECTORY 		${CMAKE_BINARY_DIR}/doc)
#SET( LIBRARY_OUTPUT_DIRECTORY_DEBUG 	${CMAKE_BINARY_DIR}/lib)
#SET( ARCHIVE_OUTPUT_DIRECTORY_DEBUG  	${CMAKE_BINARY_DIR}/lib)
#SET( RUNTIME_OUTPUT_DIRECTORY_DEBUG 	${CMAKE_BINARY_DIR}/bin)
#SET( DOCS_OUTPUT_DIRECTORY 				${CMAKE_BINARY_DIR}/doc)

INCLUDE_DIRECTORIES( include/ )

#********************************************************
# Stringozzi
#********************************************************
ADD_LIBRARY( stringozzi						src/parser.cpp )
ADD_LIBRARY( stringozzi-shared SHARED		src/parser.cpp )

#********************************************************
# Google Test
#********************************************************
ADD_SUBDIRECTORY(googletest/)
INCLUDE_DIRECTORIES(googletest/googletest/include)
INCLUDE_DIRECTORIES(googletest/googletest/)

#********************************************************
# Stringozzi Tests
#********************************************************
ADD_EXECUTABLE(stringozzi.test test/parser.test.cpp)
TARGET_LINK_LIBRARIES(stringozzi.test stringozzi gtest)
ENABLE_TESTING()
ADD_TEST(stringozzi.test stringozzi.test)

#********************************************************
# Perform Stringozzi tests and generate Coverage
#********************************************************
ADD_CUSTOM_TARGET(stringozzi.coverage ALL 3rdparty/opencppcoverage/opencppcoverage.exe --source stringozzi\\src   -- build/Release/stringozzi.test.exe
                  DEPENDS stringozzi.test 
				  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                  COMMENT "Run tests and generate coverage boards"
)

#********************************************************
# Generate Documentation
#********************************************************
ADD_CUSTOM_TARGET(stringozzi.doc ALL 3rdparty/doxygen/doxygen.exe docs/Doxyfile
                  DEPENDS stringozzi.test 
				  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                  COMMENT "Generate Documentation"
)