cmake_minimum_required(VERSION 3.14 FATAL_ERROR)


get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} LANGUAGES ASM C CXX )

file(GLOB_RECURSE MODULE_SOURCES Src/*.c  Src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../MiddleWare/STM32_WPAN/*.c ${CMAKE_CURRENT_SOURCE_DIR}/../MiddleWare/STM32_WPAN/*.cpp)
file(GLOB_RECURSE USER_INCLUDES  Inc/*.h  Inc/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/../MiddleWare/STM32_WPAN/*.h ${CMAKE_CURRENT_SOURCE_DIR}/../MiddleWare/STM32_WPAN/*.hpp)

FOREACH (SRC_EACH_FILE ${MODULE_SOURCES})
if(SRC_EACH_FILE MATCHES "system_([a-zA-Z]+([0-9]+[a-zA-Z]+)+)\.c")
    LIST(APPEND SYS_SRC ${SRC_EACH_FILE})
    
elseif(SRC_EACH_FILE MATCHES "([a-zA-Z]+([0-9]+[a-zA-Z]+)+_it\.c)")
    LIST(APPEND SYS_SRC ${SRC_EACH_FILE})
else()
LIST (APPEND MODULE_SRC ${SRC_EACH_FILE})
endif()
ENDFOREACH()
LIST(REMOVE_DUPLICATES MODULE_SRC)
SET(_STARTUP_SOURCE startup_${MCU_LINE}mghx.s)
STRING(TOLOWER ${_STARTUP_SOURCE} _STARTUP_SOURCE)
SET(_STARTUP_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/../Startup/${_STARTUP_SOURCE})
LIST(APPEND SYS_SRC ${_STARTUP_SOURCE})



FOREACH(EACH_FILE ${USER_INCLUDES})
GET_FILENAME_COMPONENT(_INCPATH ${EACH_FILE} DIRECTORY )
LIST(APPEND INCPATH ${_INCPATH})
endforeach()
LIST(REMOVE_DUPLICATES INCPATH)
LIST(APPEND MODULE_INC_DIR ${INCPATH})
INCLUDE_DIRECTORIES(${INCPATH})
SET(INCPATH ${INCPATH} PARENT_SCOPE)

# INCLUDE_DIRECTORIES(
#   ${CMAKE_CURRENT_SOURCE_DIR}../Application/AudioProcess/Inc
#   ${CMAKE_CURRENT_SOURCE_DIR}../Application/Filter/Inc
#   ${CMAKE_CURRENT_SOURCE_DIR}../Application/BLE/Inc
#   ${CMAKE_CURRENT_SOURCE_DIR}../Application/BLE/Middlewares/ST/STM32_WPAN
#   ${CMAKE_CURRENT_SOURCE_DIR}../Application/BLE/Middlewares/ST/STM32_WPAN/ble 
#   ${CMAKE_CURRENT_SOURCE_DIR}../Application/BLE/Middlewares/ST/STM32_WPAN/ble/core
#   ${CMAKE_CURRENT_SOURCE_DIR}../Application/BLE/Middlewares/ST/STM32_WPAN/ble/core/auto
#   ${CMAKE_CURRENT_SOURCE_DIR}../Application/BLE/Middlewares/ST/STM32_WPAN/ble/core/template
#   ${CMAKE_CURRENT_SOURCE_DIR}../Application/BLE/Middlewares/ST/STM32_WPAN/ble/svc/Inc
#   ${CMAKE_CURRENT_SOURCE_DIR}../Application/BLE/Middlewares/ST/STM32_WPAN/ble/svc/Src
#   ${CMAKE_CURRENT_SOURCE_DIR}../Application/BLE/Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread
#   ${CMAKE_CURRENT_SOURCE_DIR}../Application/BLE/Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci
#   ${CMAKE_CURRENT_SOURCE_DIR}../Application/BLE/Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl
#   ${CMAKE_CURRENT_SOURCE_DIR}../Application/BLE/Middlewares/ST/STM32_WPAN/utilities
#   ${CMAKE_CURRENT_SOURCE_DIR}../Application/BLE/STM32_WPAN/App
#   ${CMAKE_CURRENT_SOURCE_DIR}../Application/BLE/Utilities/lpm/tiny_lpm
#   ${CMAKE_CURRENT_SOURCE_DIR}../Application/BLE/Utilities/sequencer
# )

SET(LIB_NAME "BSP_LIB")
ADD_LIBRARY(${LIB_NAME} OBJECT ${MODULE_SRC})
ADD_LIBRARY(startup OBJECT ${SYS_SRC})

SET(BSP_LIBS ${LIB_NAME} PARENT_SCOPE)