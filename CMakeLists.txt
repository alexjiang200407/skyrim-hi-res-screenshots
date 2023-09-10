cmake_minimum_required(VERSION 3.21)

project(HiResScreenshots VERSION 0.0.1 LANGUAGES CXX)

set(OUTPUT_FOLDER "C:/Modding/MO2/mods/Hi-Res\ Screenshots")


find_package(CommonLibSSE CONFIG REQUIRED)
find_package(simpleini QUIET)
find_package(imgui CONFIG REQUIRED)


add_compile_definitions(SI_NO_CONVERSION)

# add_commonlibsse_plugin(${PROJECT_NAME} SOURCES ${sources}) # <--- specifies plugin.cpp


# target_sources(${PROJECT_NAME} PRIVATE ${sources})

include(sourcelist.cmake)

#source_group(
#	TREE
#		${CMAKE_CURRENT_SOURCE_DIR}
#	FILES
#		${sources}
#)

#source_group(
#	TREE
#		${CMAKE_CURRENT_BINARY_DIR}
#	FILES
#		${CMAKE_CURRENT_BINARY_DIR}/include/Version.h
#)

#add_library(
#	${PROJECT_NAME}
#	SHARED
#	${sources}
#)

ADD_COMMONLIBSSE_PLUGIN(${PROJECT_NAME} SOURCES ${sources})

target_link_libraries(
	${PROJECT_NAME} 
	PUBLIC 
		CommonLibSSE::CommonLibSSE
		imgui::imgui
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23) # <--- use C++23 standard
target_precompile_headers(${PROJECT_NAME} PRIVATE PCH.h) # <--- PCH.h is required!


if(DEFINED OUTPUT_FOLDER)
    set(DLL_FOLDER "${OUTPUT_FOLDER}/SKSE/Plugins")

    message(STATUS "SKSE plugin output folder: ${DLL_FOLDER}")

    add_custom_command(
        TARGET "${PROJECT_NAME}"
        POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${DLL_FOLDER}"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" "${DLL_FOLDER}/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
        VERBATIM
    )

    # If you perform a "Debug" build, also copy .pdb file (for debug symbols)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_custom_command(
            TARGET "${PROJECT_NAME}"
            POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_PDB_FILE:${PROJECT_NAME}>" "${DLL_FOLDER}/$<TARGET_PDB_FILE_NAME:${PROJECT_NAME}>"
            VERBATIM
        )
    endif()
endif()
