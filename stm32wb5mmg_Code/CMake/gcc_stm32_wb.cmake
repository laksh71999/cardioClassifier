
GET_FILENAME_COMPONENT(STM32_CMAKE_DIR ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)
SET(CMAKE_MODULE_PATH ${STM32_CMAKE_DIR} ${CMAKE_MODULE_PATH})
SET(TARGET_TRIPLET "arm-none-eabi")
add_definitions(-DSTM32WB5Mxx)
add_definitions(-DUSE_HAL_DRIVER)

add_definitions(-DCORE_CM4)
add_definitions(-DARM_MATH_CM4)

add_definitions(-DHAVE_CONFIG_H)
add_definitions(-DOPUS_ARM_ASM)
add_definitions(-DOPUS_ARM_INLINE_ASM)

set(MCU_LINKER_SCRIPT "STM32WB5MMGHX_FLASH.ld")
SET(LINKER_FILE "${STM32_CMAKE_DIR}/../Linker/${MCU_LINKER_SCRIPT}")

#IF( TOOLCHAIN_PREFIX )
#     FILE(TO_CMAKE_PATH "${TOOLCHAIN_PREFIX}" TOOLCHAIN_PREFIX)
#ELSE()
#     MESSAGE(FATAL_ERROR "No TOOLCHAIN_PREFIX specified.")
#ENDIF()

IF(NOT TARGET_TRIPLET)
    SET(TARGET_TRIPLET "arm-none-eabi")
    MESSAGE(STATUS "No TARGET_TRIPLET specified, using default: " ${TARGET_TRIPLET})
ENDIF()


SET(TOOLCHAIN_BIN_DIR "${TOOLCHAIN_PREFIX}/bin")
SET(TOOLCHAIN_INC_DIR "${TOOLCHAIN_PREFIX}/${TARGET_TRIPLET}/include")
SET(TOOLCHAIN_LIB_DIR "${TOOLCHAIN_PREFIX}/${TARGET_TRIPLET}/lib")

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR arm)
SET(CMAKE_CROSSCOMPILING 1)

IF (WIN32)
    SET(TOOL_EXECUTABLE_SUFFIX ".exe")
ELSE()
    SET(TOOL_EXECUTABLE_SUFFIX "")
ENDIF()


IF(${CMAKE_VERSION} VERSION_LESS 3.6.0)
    INCLUDE(CMakeForceCompiler)
    CMAKE_FORCE_C_COMPILER("${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-gcc${TOOL_EXECUTABLE_SUFFIX}" GNU)
    CMAKE_FORCE_CXX_COMPILER("${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-g++${TOOL_EXECUTABLE_SUFFIX}" GNU)
ELSE()
    SET(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
    SET(CMAKE_C_COMPILER "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-gcc${TOOL_EXECUTABLE_SUFFIX}")
    SET(CMAKE_CXX_COMPILER "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-g++${TOOL_EXECUTABLE_SUFFIX}")
ENDIF()
SET(CMAKE_ASM_COMPILER "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-gcc${TOOL_EXECUTABLE_SUFFIX}")
SET(CMAKE_C_LINK_EXECUTABLE "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-g++${TOOL_EXECUTABLE_SUFFIX}" GNU)

SET(CMAKE_OBJCOPY "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-objcopy${TOOL_EXECUTABLE_SUFFIX}" CACHE INTERNAL "objcopy tool")
SET(CMAKE_OBJDUMP "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-objdump${TOOL_EXECUTABLE_SUFFIX}" CACHE INTERNAL "objdump tool")
SET(CMAKE_SIZE "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-size${TOOL_EXECUTABLE_SUFFIX}" CACHE INTERNAL "size tool")
SET(CMAKE_DEBUGER "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-gdb${TOOL_EXECUTABLE_SUFFIX}" CACHE INTERNAL "debuger")
SET(CMAKE_CPPFILT "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-c++filt${TOOL_EXECUTABLE_SUFFIX}" CACHE INTERNAL "C++filt")


SET(CMAKE_C_FLAGS_DEBUG "-Og -g3" CACHE INTERNAL "c compiler flags debug")
SET(CMAKE_CXX_FLAGS_DEBUG "-Og -g3" CACHE INTERNAL "cxx compiler flags debug")
SET(CMAKE_ASM_FLAGS_DEBUG "-g" CACHE INTERNAL "asm compiler flags debug")
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "" CACHE INTERNAL "linker flags debug")

SET(CMAKE_C_FLAGS_RELEASE "-O2" CACHE INTERNAL "c compiler flags release")
SET(CMAKE_CXX_FLAGS_RELEASE "-O2" CACHE INTERNAL "cxx compiler flags release")
SET(CMAKE_ASM_FLAGS_RELEASE "" CACHE INTERNAL "asm compiler flags release")
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "" CACHE INTERNAL "linker flags release")

SET(CMAKE_FIND_ROOT_PATH "${TOOLCHAIN_PREFIX}/${TARGET_TRIPLET}" ${EXTRA_FIND_PATH})
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


SET(MPU_FLAGS "-mthumb  -mcpu=cortex-m4  -mfloat-abi=hard -mfpu=fpv4-sp-d16")
SET(EXTRA_FLAGS "-Wstrict-overflow=3 -Wstringop-overflow=4 -ffreestanding -Wall -Wextra -Wswitch-default -Wshadow -Wdouble-promotion -Wformat=2  -Wimplicit-fallthrough -Wformat-truncation=2 -Wundef -fno-common -fstack-usage -ffunction-sections -fdata-sections")

SET(CMAKE_C_FLAGS " ${MPU_FLAGS} ${EXTRA_FLAGS} " CACHE INTERNAL "c compiler flags")
SET(CMAKE_CXX_FLAGS "${MPU_FLAGS} ${EXTRA_FLAGS} -Wreorder -fno-rtti -fno-exceptions" CACHE INTERNAL "cxx compiler flags")
SET(CMAKE_ASM_FLAGS " -x assembler-with-cpp" CACHE INTERNAL "asm compiler flags")

if(NOT cdefs  MATCHES "LOG_LEVEL=LOG_LEVEL_OFF")
SET(CMAKE_EXE_LINKER_FLAGS "-Xlinker -Map=${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections -Wl,--defsym=malloc_getpagesize_P=0x100 -Wl,--start-group -lc -lm -Wl,--end-group -fno-rtti -static -fno-exceptions -mthumb -mcpu=cortex-m4  --specs=nosys.specs --specs=nano.specs -T${LINKER_FILE}" CACHE INTERNAL "executable linker flags" )
SET(CMAKE_MODULE_LINKER_FLAGS "-Xlinker -Map=${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections -Wl,--defsym=malloc_getpagesize_P=0x100 -Wl,--start-group -lc -lm  -Wl,--end-group -fno-rtti -static -fno-exceptions -mthumb -mcpu=cortex-m4  --specs=nosys.specs --specs=nano.specs -T${LINKER_FILE}" CACHE INTERNAL "module linker flags")
SET(CMAKE_SHARED_LINKER_FLAGS "-Xlinker -Map=${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections -Wl,--defsym=malloc_getpagesize_P=0x100 -Wl,--start-group -lc -lm  -Wl,--end-group -fno-rtti -static -fno-exceptions -mthumb -mcpu=cortex-m4  --specs=nosys.specs --specs=nano.specs -T${LINKER_FILE}" CACHE INTERNAL "shared linker flags")
ELSE()
SET(CMAKE_EXE_LINKER_FLAGS "-Xlinker -Map=${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections -Wl,--defsym=malloc_getpagesize_P=0x100 -Wl,--start-group -lc -lm -Wl,--end-group -static -fno-exceptions -mthumb  -mcpu=cortex-m4 -T${LINKER_FILE} -Wl,--wrap=malloc --no-exceptions --specs=nosys.specs --specs=nano.specs" CACHE INTERNAL "executable linker flags")
SET(CMAKE_MODULE_LINKER_FLAGS "-Xlinker -Map=${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections -Wl,--defsym=malloc_getpagesize_P=0x100 -Wl,--start-group -lc -lm -Wl,--end-group-static -fno-exceptions -mthumb   -mcpu=cortex-m4 -T${LINKER_FILE} -Wl,--wrap=malloc --no-exceptions --specs=nosys.specs --specs=nano.specs" CACHE INTERNAL "module linker flags")
SET(CMAKE_SHARED_LINKER_FLAGS "-Xlinker -Map=${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections -Wl,--defsym=malloc_getpagesize_P=0x100 -Wl,--start-group -lc -lm -Wl,--end-group -static -fno-exceptions -mthumb  -mcpu=cortex-m4 -T${LINKER_FILE} -Wl,--wrap=malloc  --no-exceptions --specs=nosys.specs --specs=nano.specs" CACHE INTERNAL "shared linker flags")
ENDIF()





