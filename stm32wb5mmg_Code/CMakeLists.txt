cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

set(CMAKE_C_STANDARD                11)
set(CMAKE_C_STANDARD_REQUIRED       ON)
set(CMAKE_C_EXTENSIONS              ON)
set(CMAKE_CXX_STANDARD              17)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_CXX_EXTENSIONS            ON)
set(PROJ_PATH                       ${CMAKE_CURRENT_SOURCE_DIR})
message("Build type: "              ${CMAKE_BUILD_TYPE})
PROJECT(ESteth LANGUAGES ASM C CXX)

add_compile_definitions("LOG_LEVEL=LOG_LEVEL_DEBUG")
#set if the printout output is none, Segger or ITM(STLINK), will have effect only if the Log_level is != LOG_LEVEL_OFF
SET(PRINT_OP "SEGGER")


#set(CMAKE_C_COMPILER_WORKS 1)
#set(CMAKE_FIND_DEBUG_MODE TRUE)

#STM32 HAL Family must be set
SET(STM32_FAMILY WB)
#PREFIX must be set
SET(HAL_PREFIX stm32wbxx_)
set(MCU_FAMILY STM32WBxx)
set(MCU_LINE STM32WB5M)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
set(STM32Cube_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Hal${STM32_FAMILY}" CACHE INTERNAL "")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")



# INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Application/Include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/debuglog/Inc)
INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}/Application/AudioProcess/Inc
  ${CMAKE_CURRENT_SOURCE_DIR}/Application/Filter/Inc
  ${CMAKE_CURRENT_SOURCE_DIR}/Application/BLE/Inc
  ${CMAKE_CURRENT_SOURCE_DIR}/Application/BLE/Middlewares/ST/STM32_WPAN
  ${CMAKE_CURRENT_SOURCE_DIR}/Application/BLE/Middlewares/ST/STM32_WPAN/ble 
  ${CMAKE_CURRENT_SOURCE_DIR}/Application/BLE/Middlewares/ST/STM32_WPAN/ble/core
  ${CMAKE_CURRENT_SOURCE_DIR}/Application/BLE/Middlewares/ST/STM32_WPAN/ble/core/auto
  ${CMAKE_CURRENT_SOURCE_DIR}/Application/BLE/Middlewares/ST/STM32_WPAN/ble/core/template
  ${CMAKE_CURRENT_SOURCE_DIR}/Application/BLE/Middlewares/ST/STM32_WPAN/ble/svc/Inc
  ${CMAKE_CURRENT_SOURCE_DIR}/Application/BLE/Middlewares/ST/STM32_WPAN/ble/svc/Src
  ${CMAKE_CURRENT_SOURCE_DIR}/Application/BLE/Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread
  ${CMAKE_CURRENT_SOURCE_DIR}/Application/BLE/Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci
  ${CMAKE_CURRENT_SOURCE_DIR}/Application/BLE/Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl
  ${CMAKE_CURRENT_SOURCE_DIR}/Application/BLE/Middlewares/ST/STM32_WPAN/utilities
  ${CMAKE_CURRENT_SOURCE_DIR}/Application/BLE/Middlewares/ST/ARM/DSP/Inc
  ${CMAKE_CURRENT_SOURCE_DIR}/Application/BLE/Middlewares/ST/ARM/DSP/Lib
  ${CMAKE_CURRENT_SOURCE_DIR}/Application/BLE/STM32_WPAN/App
  ${CMAKE_CURRENT_SOURCE_DIR}/Application/BLE/Utilities/lpm/tiny_lpm
  ${CMAKE_CURRENT_SOURCE_DIR}/Application/BLE/Utilities/sequencer
  ${CMAKE_CURRENT_SOURCE_DIR}/Application/Opus/celt/
  ${CMAKE_CURRENT_SOURCE_DIR}/Application/Opus/celt/arm
  ${CMAKE_CURRENT_SOURCE_DIR}/Application/Opus/Inc
  ${CMAKE_CURRENT_SOURCE_DIR}/Application/Opus/silk
  ${CMAKE_CURRENT_SOURCE_DIR}/Application/Opus/silk/arm
  ${CMAKE_CURRENT_SOURCE_DIR}/Application/Opus/silk/fixed
  ${CMAKE_CURRENT_SOURCE_DIR}/Application/Opus/src
    )

if(${PRINT_OP} MATCHES "ITM")
add_compile_definitions(ITM)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/debuglog/ITMLogger/Inc)
elseif(${PRINT_OP} MATCHES "SEGGER")
add_compile_definitions(SEGGER)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/debuglog/Segger/Inc)
endif()

set(INCPATH "" )

FIND_PACKAGE(CMSIS REQUIRED)
FIND_PACKAGE(DSP REQUIRED)
FIND_PACKAGE(STM32WBHAL COMPONENTS  gpio  tim sai rtc dma uart i2c ipcc hsem REQUIRED)  #required modules list need to specifiy


get_directory_property(cdefs COMPILE_DEFINITIONS)
if(NOT cdefs  MATCHES "LOG_LEVEL=LOG_LEVEL_OFF")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/debuglog)
Endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Bsp)



FIND_PACKAGE(APPLICATION COMPONENTS BLE Filter AudioProcess Opus REQUIRED) #required module folder name

INCLUDE_DIRECTORIES(${INCPATH})
INCLUDE_DIRECTORIES(${CMSIS_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${DSP_INCLUDE_DIRS})
message("Include Path: " ${INCPATH})
link_directories(${CMAKE_SOURCE_DIR}/HalWB/Drivers/DSP/Lib)
ADD_EXECUTABLE(${CMAKE_PROJECT_NAME}.elf  ${APP_PATH}/main.cpp  )
# target_include_directories(${CMAKE_PROJECT_NAME}.elf PRIVATE ${CMAKE_SOURCE_DIR}/HalWB/Drivers/DSP/Inc)
target_link_libraries(${CMAKE_PROJECT_NAME}.elf  PRIVATE ${CMSIS_LIBS} ${APP_LIB_LIST} ${BSP_LIBS} ${HAL_LIBS} ${DEBUGLOG_LIBS} ${DSP_LIBS} startup arm_cortexM4lf_math) 

set_target_properties(${CMAKE_PROJECT_NAME}.elf PROPERTIES LINKER_LANGUAGE CXX)

FUNCTION(STM32_ADD_HEX_BIN_TARGETS TARGET)
    IF(EXECUTABLE_OUTPUT_PATH)
      SET(FILENAME "${EXECUTABLE_OUTPUT_PATH}/${TARGET}.elf")
    ELSE()
      SET(FILENAME "${TARGET}.elf")
    ENDIF()
    ADD_CUSTOM_TARGET(${TARGET}.hex ALL DEPENDS ${TARGET}.elf COMMAND ${CMAKE_OBJCOPY} -Oihex  ${FILENAME} ${CMAKE_CURRENT_SOURCE_DIR}/Executable/${TARGET}.hex)

ENDFUNCTION()

FUNCTION(STM32_ADD_DUMP_TARGET TARGET)
    IF(EXECUTABLE_OUTPUT_PATH)
      SET(FILENAME "${EXECUTABLE_OUTPUT_PATH}/${TARGET}.elf")
    ELSE()
      SET(FILENAME "${TARGET}.elf")
    ENDIF()
    ADD_CUSTOM_TARGET(${TARGET}.dump DEPENDS ${TARGET}.elf COMMAND ${CMAKE_OBJDUMP} -x -D -S -s ${FILENAME} | ${CMAKE_CPPFILT} > ${FILENAME}.dump)

ENDFUNCTION()

FUNCTION(STM32_PRINT_SIZE_OF_TARGETS TARGET)
    IF(EXECUTABLE_OUTPUT_PATH)
      SET(FILENAME "${EXECUTABLE_OUTPUT_PATH}/${TARGET}.elf")
    ELSE()
      SET(FILENAME "${TARGET}.elf")
    ENDIF()
    add_custom_command(TARGET ${TARGET}.elf POST_BUILD COMMAND ${CMAKE_SIZE} ${FILENAME})
ENDFUNCTION()

STM32_ADD_HEX_BIN_TARGETS(${CMAKE_PROJECT_NAME})
STM32_PRINT_SIZE_OF_TARGETS(${CMAKE_PROJECT_NAME})
