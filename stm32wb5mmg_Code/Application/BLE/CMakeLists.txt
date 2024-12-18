cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} LANGUAGES ASM C CXX )

file(GLOB_RECURSE MODULE_SRC Src/*.c  Src/*.cpp
    Middlewares/*.c
    STM32_WPAN/*.c
    Utilities/*.c
    ) 
    
file(GLOB_RECURSE USER_INCLUDES  Inc/*.h  Inc/*.hpp )

LIST(REMOVE_DUPLICATES MODULE_SRC)

FOREACH(EACH_FILE ${USER_INCLUDES})
GET_FILENAME_COMPONENT(_INCPATH ${EACH_FILE} DIRECTORY )
LIST(APPEND INCPATH ${_INCPATH})
ENDFOREACH()

LIST(REMOVE_DUPLICATES INCPATH)
#set(INCPATH ${INCPATH} PARENT_SCOPE)

INCLUDE_DIRECTORIES(${INCPATH})
SET(LIB_NAME "${ProjectId}_LIB")
ADD_LIBRARY(${LIB_NAME} OBJECT ${MODULE_SRC})

LIST(APPEND APP_LIB_LIST ${LIB_NAME})
SET(APP_LIB_LIST ${APP_LIB_LIST} PARENT_SCOPE)

IF(BUILD_TESTING)
add_subdirectory(UnitTest)
ENDIF(BUILD_TESTING)

