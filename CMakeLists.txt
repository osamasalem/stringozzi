#********************************************************
# General Configuration
#********************************************************
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT(stringozzi CXX)

#********************************************************
# Compiler Configuration
#********************************************************

INCLUDE_DIRECTORIES( include/ )
#SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY 		bin)
#SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG 	${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
#SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
#SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)
#SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG lib)
#SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE lib)

SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY 	${CMAKE_BINARY_DIR}/../lib)
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY  	${CMAKE_BINARY_DIR}/../lib)
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY 	${CMAKE_BINARY_DIR}/../bin)
SET( CMAKE_DOCS_OUTPUT_DIRECTORY 		${CMAKE_BINARY_DIR}/../doc)
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG 	${CMAKE_BINARY_DIR}/../lib)
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG  	${CMAKE_BINARY_DIR}/../lib)
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG 	${CMAKE_BINARY_DIR}/../bin)

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
TARGET_LINK_LIBRARIES(stringozzi.test stringozzi-shared gtest)
ENABLE_TESTING()
ADD_TEST(stringozzi.test stringozzi.test)


#********************************************************
# Perform Stringozzi tests and generate Coverage
#********************************************************
ADD_CUSTOM_TARGET(stringozzi.coverage ALL opencppcoverage.exe --source stringozzi\\src   -- ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/stringozzi.test.exe
                  DEPENDS stringozzi.test 
                  COMMENT "Run tests and generate coverage boards"
)

#********************************************************
# Generate Documentation
#********************************************************
ADD_CUSTOM_TARGET(stringozzi.doc ALL doxygen.exe docs/Doxyfile
                  DEPENDS stringozzi.test 
				  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                  COMMENT "Generate Documentation"
)