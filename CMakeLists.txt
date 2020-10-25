#********************************************************
# General Configuration
#********************************************************
CMAKE_MINIMUM_REQUIRED (VERSION 3.0)


PROJECT(stringozzi
        DESCRIPTION "C++ Parsing Expression grammer library for parsing/searching/validating like Regular Expression"
        LANGUAGES CXX)

#********************************************************
# Compiler Configuration
#********************************************************
INCLUDE_DIRECTORIES( include/ )


SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY 	${CMAKE_CURRENT_LIST_DIR}/bin_${CMAKE_HOST_SYSTEM_NAME})
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY  	${CMAKE_CURRENT_LIST_DIR}/bin_${CMAKE_HOST_SYSTEM_NAME})
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY 	${CMAKE_CURRENT_LIST_DIR}/bin_${CMAKE_HOST_SYSTEM_NAME})
SET( CMAKE_DOCS_OUTPUT_DIRECTORY 		${CMAKE_CURRENT_LIST_DIR}/doc)
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG 	${CMAKE_CURRENT_LIST_DIR}/bin_${CMAKE_HOST_SYSTEM_NAME}/debug)
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG  	${CMAKE_CURRENT_LIST_DIR}/bin_${CMAKE_HOST_SYSTEM_NAME}/debug)
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG 	${CMAKE_CURRENT_LIST_DIR}/bin_${CMAKE_HOST_SYSTEM_NAME}/debug)


#********************************************************
# Stringozzi
#********************************************************
ADD_LIBRARY( stringozzi-static			src/Stringozzi.cpp )

ADD_LIBRARY( stringozzi-shared SHARED		src/Stringozzi.cpp )

ADD_SUBDIRECTORY(googletest/ ./bin  EXCLUDE_FROM_ALL)

ADD_EXECUTABLE(stringozzi.test test/Stringozzi.test.cpp src/Stringozzi.cpp)
TARGET_LINK_LIBRARIES(stringozzi.test gtest)

ENABLE_TESTING()
ADD_TEST(stringozzi.test stringozzi.test)


IF(STRINGOZZI_TRAVIS_BUILD)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftest-coverage")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
ENDIF()