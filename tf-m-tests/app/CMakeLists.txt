#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.13)
project(tfm_ns LANGUAGES ASM C)

# For multi-core projects, the NS app can be run on a different CPU to the
# Secure code. To facilitate this, we once again reload the compiler to load the
# setting for the NS CPU. Cmake settings are directory scoped so this affects
# anything loaded from or declared in this dir.
if (TFM_MULTI_CORE_TOPOLOGY)
    include(${CMAKE_SOURCE_DIR}/platform/ext/target/${TFM_PLATFORM}/preload_ns.cmake)
    tfm_toolchain_reload_compiler()

    # Enable TFM_MULTI_CORE_NS_OS when building with tf-m-tests NS App.
    set(TFM_MULTI_CORE_NS_OS ON CACHE BOOL "Enable NS RTOS support in NS mailbox")
endif()

# In actual NS integration, NS side build should include the source files
# exported by TF-M build.
# Directly include interface folder to simplify the NS build in this demo, since
# install always occurs at the end of build.
set(INTERFACE_SRC_DIR    ${CMAKE_SOURCE_DIR}/interface/src)
set(INTERFACE_INC_DIR    ${CMAKE_SOURCE_DIR}/interface/include)

#################### TF-M NS interface (header only) ###########################

add_library(tfm_ns_interface INTERFACE)

target_include_directories(tfm_ns_interface
    INTERFACE
        ${INTERFACE_INC_DIR}
        ${CMAKE_BINARY_DIR}/generated/interface/include
        ${INTERFACE_INC_DIR}/os_wrapper
        $<$<BOOL:${TFM_MULTI_CORE_TOPOLOGY}>:${INTERFACE_INC_DIR}/multi_core>
)

# PSA interface files are generated from a template
add_dependencies(tfm_ns_interface
    tfm_generated_files
)

# Include selection of Secure Partitions from TF-M build.
# It can be replaced by NS side configurations later.
target_link_libraries(tfm_ns_interface
    INTERFACE
        tfm_partition_defs
)

target_compile_definitions(tfm_ns_interface
    INTERFACE
        $<$<BOOL:${TFM_PSA_API}>:TFM_PSA_API>
        $<$<STREQUAL:${TEST_PSA_API},IPC>:PSA_API_TEST_IPC>
        $<$<BOOL:${TFM_NS_CLIENT_IDENTIFICATION}>:TFM_NS_CLIENT_IDENTIFICATION>
        $<$<BOOL:${TFM_MULTI_CORE_TOPOLOGY}>:TFM_MULTI_CORE_TOPOLOGY>
        $<$<BOOL:${TFM_MULTI_CORE_NS_OS}>:TFM_MULTI_CORE_NS_OS>
        $<$<BOOL:${FORWARD_PROT_MSG}>:FORWARD_PROT_MSG>
)

###################### TF-M NS interface api (NS lib) ##########################

add_library(tfm_api_ns STATIC)

target_sources(tfm_api_ns
    PRIVATE
        $<$<BOOL:${TFM_NS_CLIENT_IDENTIFICATION}>:${INTERFACE_SRC_DIR}/tfm_nspm_svc_handler.c>
        $<$<BOOL:${TFM_NS_CLIENT_IDENTIFICATION}>:${INTERFACE_SRC_DIR}/tfm_nspm_api.c>
)

if (${TFM_PSA_API})
    target_sources(tfm_api_ns PRIVATE
        $<$<OR:$<BOOL:{$FORWARD_PROT_MSG}>,$<BOOL:${TFM_PARTITION_PLATFORM}>>:${INTERFACE_SRC_DIR}/tfm_platform_ipc_api.c>
        $<$<OR:$<BOOL:{$FORWARD_PROT_MSG}>,$<BOOL:${TFM_PARTITION_PROTECTED_STORAGE}>>:${INTERFACE_SRC_DIR}/tfm_ps_ipc_api.c>
        $<$<OR:$<BOOL:{$FORWARD_PROT_MSG}>,$<BOOL:${TFM_PARTITION_INTERNAL_TRUSTED_STORAGE}>>:${INTERFACE_SRC_DIR}/tfm_its_ipc_api.c>
        $<$<OR:$<BOOL:{$FORWARD_PROT_MSG}>,$<BOOL:${TFM_PARTITION_CRYPTO}>>:${INTERFACE_SRC_DIR}/tfm_crypto_ipc_api.c>
        $<$<OR:$<BOOL:{$FORWARD_PROT_MSG}>,$<BOOL:${TFM_PARTITION_INITIAL_ATTESTATION}>>:${INTERFACE_SRC_DIR}/tfm_initial_attestation_ipc_api.c>
    )

    if (TFM_MULTI_CORE_TOPOLOGY)
        target_sources(tfm_api_ns PRIVATE
            ${INTERFACE_SRC_DIR}/multi_core/tfm_ns_mailbox.c
            ${INTERFACE_SRC_DIR}/multi_core/tfm_multi_core_ns_api.c
            ${INTERFACE_SRC_DIR}/multi_core/tfm_multi_core_psa_ns_api.c
            $<$<BOOL:${TFM_MULTI_CORE_NS_OS}>:${INTERFACE_SRC_DIR}/multi_core/tfm_ns_mailbox_rtos_api.c>
        )
    else()
        target_sources(tfm_api_ns PRIVATE
           ${INTERFACE_SRC_DIR}/tfm_ns_interface.c
           ${INTERFACE_SRC_DIR}/tfm_psa_ns_api.c
        )
    endif()
else()
    target_sources(tfm_api_ns PRIVATE
        $<$<BOOL:${TFM_PARTITION_PLATFORM}>:${INTERFACE_SRC_DIR}/tfm_platform_func_api.c>
        $<$<BOOL:${TFM_PARTITION_AUDIT_LOG}>:${INTERFACE_SRC_DIR}/tfm_audit_func_api.c>
        $<$<BOOL:${TFM_PARTITION_PROTECTED_STORAGE}>:${INTERFACE_SRC_DIR}/tfm_ps_func_api.c>
        $<$<BOOL:${TFM_PARTITION_INTERNAL_TRUSTED_STORAGE}>:${INTERFACE_SRC_DIR}/tfm_its_func_api.c>
        $<$<BOOL:${TFM_PARTITION_CRYPTO}>:${INTERFACE_SRC_DIR}/tfm_crypto_func_api.c>
        $<$<BOOL:${TFM_PARTITION_INITIAL_ATTESTATION}>:${INTERFACE_SRC_DIR}/tfm_initial_attestation_func_api.c>
        ${INTERFACE_SRC_DIR}/tfm_psa_ns_api.c
        ${INTERFACE_SRC_DIR}/tfm_ns_interface.c
    )
endif()

target_link_libraries(tfm_api_ns
    PUBLIC
        tfm_ns_interface
    PRIVATE
        platform_ns
        # CMSIS is currently only required to provide the NS client IDs. In
        # future, this should probably be made more OS agnostic
        $<$<BOOL:${TFM_NS_CLIENT_IDENTIFICATION}>:CMSIS_5_tfm_ns>
)

############################# PSA test integration #############################

if(TEST_PSA_API AND NOT PSA_ARCH_TESTS_BINARY_PATH)
    if(NOT SUITE)
        set(SUITE ${TEST_PSA_API})
    endif()

    if (NOT DEFINED PSA_API_TEST_TARGET)
        string(REGEX REPLACE ".*/" "" PSA_API_TEST_TARGET ${TFM_PLATFORM})
    endif()

    if(NOT TARGET)
        if (NOT "${TEST_PSA_API}" STREQUAL "IPC")
            set(TARGET tgt_dev_apis_tfm_${PSA_API_TEST_TARGET})
        else()
            set(TARGET tgt_ff_tfm_${PSA_API_TEST_TARGET})
        endif()
    endif()


    if(NOT PSA_INCLUDE_PATHS)
        set(PSA_INCLUDE_PATHS ${INTERFACE_INC_DIR}/
                              ${CMAKE_BINARY_DIR}/generated/api-tests/platform/manifests/
                              ${CMAKE_BINARY_DIR}/generated/interface/include
        )
    endif()

    if(NOT SP_HEAP_MEM_SUPP)
        set(SP_HEAP_MEM_SUPP 0)
    endif()
    if(NOT PLATFORM_PSA_ISOLATION_LEVEL)
        set(PLATFORM_PSA_ISOLATION_LEVEL ${TFM_ISOLATION_LEVEL})
    endif()

    if (NOT TOOLCHAIN)
        if (${CMAKE_C_COMPILER_ID} STREQUAL GNU)
            set(TOOLCHAIN GNUARM)
        elseif (${CMAKE_C_COMPILER_ID} STREQUAL ARMClang)
            set(TOOLCHAIN ARMCLANG)
        endif()
    endif()

    if (NOT CPU_ARCH)
        if (${CMAKE_SYSTEM_ARCHITECTURE} STREQUAL armv8-m.main)
            set(CPU_ARCH armv8m_ml)
        elseif (${CMAKE_SYSTEM_ARCHITECTURE} STREQUAL armv8-m.base)
            set(CPU_ARCH armv8m_bl)
        elseif (${CMAKE_SYSTEM_ARCHITECTURE} STREQUAL armv7-m)
            set(CPU_ARCH armv7m)
        endif()
    endif()

    add_subdirectory(${PSA_ARCH_TESTS_PATH}/api-tests ${CMAKE_CURRENT_BINARY_DIR}/psa_api_tests)
endif()

############################# Test integration #################################

add_library(tfm_ns_integration_test STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_ns_integration_test
    PRIVATE
        tfm_integ_test.c
)

target_include_directories(tfm_ns_integration_test
    PUBLIC
        .
)

target_link_libraries(tfm_ns_integration_test
    PUBLIC
        $<$<BOOL:${TEST_NS}>:tfm_ns_tests>
        tfm_test_framework_ns
    PRIVATE
        tfm_ns_interface
        tfm_api_ns
        CMSIS_5_tfm_ns
)

target_compile_definitions(tfm_ns_integration_test
    PUBLIC
        $<$<BOOL:${TEST_NS}>:TEST_FRAMEWORK_NS>
        $<$<BOOL:${TEST_S}>:TEST_FRAMEWORK_S>
)

############################# TFM NS app #######################################

add_executable(tfm_ns)

target_sources(tfm_ns
    PRIVATE
        main_ns.c
        $<$<BOOL:${TEST_PSA_API}>:psa_api_test.c>
)

target_link_libraries(tfm_ns
    PRIVATE
        platform_ns
        CMSIS_5_tfm_ns
        $<$<OR:$<BOOL:${TEST_NS}>,$<BOOL:${TEST_S}>>:tfm_ns_integration_test>
        $<$<BOOL:${TEST_PSA_API}>:val_nspe>
        $<$<BOOL:${TEST_PSA_API}>:pal_nspe>
        $<$<BOOL:${TEST_PSA_API}>:test_combine>
        $<$<NOT:$<BOOL:${TFM_MULTI_CORE_TOPOLOGY}>>:tfm_s_veneers>
        tfm_api_ns
        tfm_ns_log
)

target_compile_definitions(tfm_ns
    PUBLIC
    $<$<BOOL:${TEST_PSA_API}>:PSA_API_TEST_NS>
)

set_target_properties(tfm_ns PROPERTIES
    SUFFIX ".axf"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_link_options(tfm_ns
    PRIVATE
        $<$<C_COMPILER_ID:GNU>:-Wl,-Map=${CMAKE_BINARY_DIR}/bin/tfm_ns.map>
        $<$<C_COMPILER_ID:ARMClang>:--map>
        $<$<C_COMPILER_ID:IAR>:--map\;${CMAKE_BINARY_DIR}/bin/tfm_ns.map>
)

add_convert_to_bin_target(tfm_ns)

############################# CMSIS ############################################

include(FetchContent)

set(FETCHCONTENT_QUIET FALSE)
cmake_policy(SET CMP0079 NEW)

add_library(CMSIS_5_tfm_ns INTERFACE)

target_sources(CMSIS_5_tfm_ns
    INTERFACE
        ${CMSIS_5_PATH}/RTOS2/RTX/Config/RTX_Config.c
        ${CMSIS_5_PATH}/RTOS2/RTX/Source/rtx_lib.c
        ${CMAKE_CURRENT_SOURCE_DIR}/os_wrapper_cmsis_rtos_v2.c
)

target_include_directories(CMSIS_5_tfm_ns
    INTERFACE
        ${CMSIS_5_PATH}/Core/Include
        ${CMSIS_5_PATH}/RTOS2/Include
        ${CMSIS_5_PATH}/RTOS2/RTX/Include
        ${CMSIS_5_PATH}/RTOS2/RTX/Config
)

target_link_libraries(CMSIS_5_tfm_ns
    INTERFACE
        platform_ns
)
