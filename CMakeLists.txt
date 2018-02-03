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

ADD_SUBDIRECTORY(googletest/)
ADD_EXECUTABLE(stringozzi.test test/parser.test.cpp)
TARGET_LINK_LIBRARIES(stringozzi.test stringozzi-shared gtest)
ENABLE_TESTING()
ADD_TEST(stringozzi.test stringozzi.test)

IF(STRINGOZZI_TRAVIS_BUILD)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftest-coverage")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
ENDIF()