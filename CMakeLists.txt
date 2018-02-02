#********************************************************
# General Configuration
#********************************************************
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT(stringozzi CXX)

#********************************************************
# Perform Stringozzi static check
#********************************************************
#ADD_CUSTOM_TARGET(stringozzi.check ALL cppcheck  --enable=all ../src/parser.cpp ../include/parser.h ../test/parser.test.cpp
#                  COMMENT "Static check of C++ Check"
#)

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
ADD_CUSTOM_TARGET(stringozzi.test.exec ALL ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/stringozzi.test${CMAKE_EXECUTABLE_SUFFIX}
                  DEPENDS stringozzi.test 
                  COMMENT "Run tests and generate coverage boards"
)

#********************************************************
# Generate Documentation
#********************************************************
ADD_CUSTOM_TARGET(stringozzi.doc ALL doxygen docs/Doxyfile
                  DEPENDS stringozzi.test.exec
		  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                  COMMENT "Generate Documentation"
)
