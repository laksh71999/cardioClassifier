SET(SEGGER_source_dir  ${CMAKE_CURRENT_SOURCE_DIR}/Application/Segger)

SET(SEGGER_SRC_FILES

SEGGER_RTT_printf.c
SEGGER_RTT.c
SEGGER_SYSVIEW_Config_FreeRTOS.c
SEGGER_SYSVIEW_FreeRTOS.c
SEGGER_SYSVIEW.c
SEGGER_RTT_Syscalls_GCC.c
)

SET(SEGGER_ASM_FILES SEGGER_RTT_ASM_ARMv7M.S)

SET(SEGGER_HEADERS
Global.h
SEGGER_RTT_Conf.h
SEGGER_RTT.h
SEGGER_SYSVIEW_Conf.h
SEGGER_SYSVIEW_ConfDefaults.h
SEGGER_SYSVIEW_FreeRTOS.h
SEGGER_SYSVIEW_Int.h
SEGGER_SYSVIEW.h
SEGGER.h
)
LIST(APPEND _SEGGER_SOURCES ${SEGGER_STARTUP_SOURCE_FILE})


FOREACH(SRC ${SEGGER_SRC_FILES})
    STRING(MAKE_C_IDENTIFIER "${SRC}" SRC_CLEAN)
	SET(SEGGER_${SRC_CLEAN}_FILE SEGGER_SRC_FILE-NOTFOUND)
	FIND_FILE(SEGGER_${SRC_CLEAN}_FILE ${SRC}
		HINTS ${SEGGER_source_dir}/Src
		CMAKE_FIND_ROOT_PATH_BOTH
	)
	LIST(APPEND _SEGGER_SOURCES ${SEGGER_${SRC_CLEAN}_FILE})
ENDFOREACH()

FOREACH(SRC ${SEGGER_ASM_FILES})
    STRING(MAKE_C_IDENTIFIER "${SRC}" SRC_CLEAN)
	SET(SEGGER_${SRC_CLEAN}_FILE SEGGER_SRC_FILE-NOTFOUND)
	FIND_FILE(SEGGER_${SRC_CLEAN}_FILE ${SRC}
		HINTS ${SEGGER_source_dir}
		CMAKE_FIND_ROOT_PATH_BOTH
	)
	LIST(APPEND _ASM_SOURCES ${SEGGER_${SRC_CLEAN}_FILE})
ENDFOREACH()



SET(SEGGER_INCLUDE_DIRS ${SEGGER_source_dir}/Inc)

SET(SEGGER_SOURCES
	${_SEGGER_SOURCES}  ${_ASM_SOURCES}
)


INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(SEGGER DEFAULT_MSG SEGGER_INCLUDE_DIRS SEGGER_SOURCES)





