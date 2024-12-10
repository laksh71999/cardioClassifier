# SET(WPAN_source_dir  ${CMAKE_CURRENT_SOURCE_DIR}/MiddleWare/STM32_WPAN)

# SET(WPAN_SRC_FILES
# shci_tl.c
# hci_tl.c
# tl_mbox.c
# hci_tl_if.c
# shci_tl_if.c
# shci.c
# stm_queue.c
# stm_list.c
# dbg_trace.c
# otp.c
# osal.c
# ble_hal_aci.c
# ble_hci_le.c
# ble_events.c
# ble_gap_aci.c
# ble_l2cap_aci.c
# ble_gatt_aci.c
# svc_ctl.c
# )

# SET(SRC_SEARCH_PATH 
# ${WPAN_source_dir}/interface/patterns/ble_thread/tl
# ${WPAN_source_dir}/interface/patterns/ble_thread/
# ${WPAN_source_dir}/interface/patterns/ble_thread/tl
# ${WPAN_source_dir}/interface/patterns/ble_thread/tl
# ${WPAN_source_dir}/interface/patterns/ble_thread/tl
# ${WPAN_source_dir}/interface/patterns/ble_thread/tl
# ${WPAN_source_dir}/interface/patterns/ble_thread/shci
# ${WPAN_source_dir}/utilities
# ${WPAN_source_dir}/utilities
# ${WPAN_source_dir}/utilities
# ${WPAN_source_dir}/utilities
# ${WPAN_source_dir}/ble/core/template
# ${WPAN_source_dir}/ble/core/auto
# ${WPAN_source_dir}/ble/core/auto
# ${WPAN_source_dir}/ble/core/auto
# ${WPAN_source_dir}/ble/core/auto
# ${WPAN_source_dir}/ble/core/auto
# ${WPAN_source_dir}/ble/core/auto
# ${WPAN_source_dir}/ble/svc/Src
# )
# SET(WPAN_HEADERS
# stm32_wpan_common.h
# hw.h
# tl.h
# shci_tl.h
# mbox_def.h
# hci_tl.h
# shci.h
# stm_queue.h
# utilities_common.h
# otp.h
# dbg_trace.h
# stm_list.h
# ble_common.h
# ble_defs.h
# ble_const.h
# compiler.h
# osal.h
# ble_core.h
# ble_bufsize.h
# ble_types.h
# ble_l2cap_aci.h
# ble_gatt_aci.h
# ble_hal_aci.h
# ble_gap_aci.h
# ble_hci_le.h
# ble_events.h
# ble_legacy.h
# ble_std.h
# uuid.h
# ias.h
# hids.h
# mesh.h
# crs_stm.h
# bls.h
# hts.h
# tps.h
# zdd_stm.h
# eds_stm.h
# motenv_stm.h
# template_stm.h
# bas.h
# lls.h
# dis.h
# p2p_stm.h
# hrs.h
# svc_ctl.h
# otas_stm.h
# common_blesvc.h
# ble.h
# )


# FOREACH(SRC ${WPAN_SRC_FILES})
#     STRING(MAKE_C_IDENTIFIER "${SRC}" SRC_CLEAN)
# 	SET(WPAN_${SRC_CLEAN}_FILE WPAN_SRC_FILE-NOTFOUND)
# 	FIND_FILE(WPAN_${SRC_CLEAN}_FILE ${SRC}
# 		PATHS ${SRC_SEARCH_PATH}
# 		CMAKE_FIND_ROOT_PATH_BOTH  
# 	)
# 	LIST(APPEND _WPAN_SOURCES ${WPAN_${SRC_CLEAN}_FILE})
# ENDFOREACH()




# file(GLOB_RECURSE
#   FOUND_FILES
#   LIST_DIRECTORIES false
#   ${WPAN_source_dir}/*/*.h)


# foreach(H_FILE ${FOUND_FILES} )
	
# 	get_filename_component(_T_DIR ${H_FILE} DIRECTORY )
	
# 	LIST(APPEND WPAN_INCLUDE_DIRS ${_T_DIR})
# 	endforeach()

# 	LIST(REMOVE_DUPLICATES WPAN_INCLUDE_DIRS)
# SET(WPAN_SOURCES
# 	${_WPAN_SOURCES}  ${_ASM_SOURCES}
# )

# LIST(APPEND INC_PATH ${WPAN_INCLUDE_DIRS})
# INCLUDE(FindPackageHandleStandardArgs)

# FIND_PACKAGE_HANDLE_STANDARD_ARGS(WPAN DEFAULT_MSG WPAN_INCLUDE_DIRS WPAN_SOURCES)





