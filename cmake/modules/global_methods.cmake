macro(MEMADE_SET_AS_STARTUP_PROJECT PROJECTNAME)
target_compile_features(${PROJECTNAME} 
PRIVATE 
${PRJ_COMPILE_FEATURES}
)
endmacro(MEMADE_SET_AS_STARTUP_PROJECT)

macro(MEMADE_SET_CMAKE_CONFIGURATION_TYPES ARGS)
if(ARGS STREQUAL "")
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;" CACHE STRING "info" FORCE)
else()
set(CMAKE_CONFIGURATION_TYPES ${ARGS} CACHE STRING "info" FORCE)
endif(ARGS STREQUAL "")
endmacro(MEMADE_SET_CMAKE_CONFIGURATION_TYPES)

macro(MEMADE_SET_CMAKETARGETS_FOLDER)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets")
endmacro(MEMADE_SET_CMAKETARGETS_FOLDER)

#argv#	#是一个下标，0指向第一个参数，累加
#argv	所有的定义时要求传入的参数
#argn	定义时要求传入的参数以外的参数
#argc	传入的实际参数的个数，也就是调用函数是传入的参数个数
#argv0 peOutPath
#argv1 objOutPath
#argv2 libOutPath
macro(MEMADE_OUTPATHCONFIG arg)
#message(STATUS "MEMADE_OUTPATHCONFIG Args total is '${ARGC}'")
set(peOutPath "")
set(objOutPath "")
set(libOutPath "")
set(bSpecifiedPath OFF)

if(${ARGC} GREATER 0)
set(peOutPath ${ARGV0})
endif(${ARGC} GREATER 0)
if(${ARGC} GREATER 1)
set(objOutPath ${ARGV1})
endif(${ARGC} GREATER 1)
if(${ARGC} GREATER 2)
set(libOutPath ${ARGV2})
endif(${ARGC} GREATER 2)
# Assign outpath.
if(${ARGC} GREATER 3)
set(bSpecifiedPath ${ARGV3})
endif(${ARGC} GREATER 3)

if(NOT("${peOutPath}" STREQUAL ""))
set(PUBLISH_RUNTIME ${CMAKE_CURRENT_SOURCE_DIR}/${peOutPath})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PUBLISH_RUNTIME})
if(bSpecifiedPath)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PUBLISH_RUNTIME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PUBLISH_RUNTIME})
else()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PUBLISH_RUNTIME}/${CMAKE_VS_PLATFORM_NAME}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PUBLISH_RUNTIME}/${CMAKE_VS_PLATFORM_NAME}/Release)
endif(bSpecifiedPath)
endif(NOT("${peOutPath}" STREQUAL ""))

if(NOT("${objOutPath}" STREQUAL ""))
set(LIBRARY_RUNTIME ${CMAKE_CURRENT_SOURCE_DIR}/${objOutPath})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_RUNTIME})
if(bSpecifiedPath)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${LIBRARY_RUNTIME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LIBRARY_RUNTIME})
else()
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${LIBRARY_RUNTIME}/${CMAKE_VS_PLATFORM_NAME}/Debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LIBRARY_RUNTIME}/${CMAKE_VS_PLATFORM_NAME}/Release)
endif(bSpecifiedPath)
endif(NOT("${objOutPath}" STREQUAL ""))

if(NOT("${libOutPath}" STREQUAL ""))
set(ARCHIVE_RUNTIME ${CMAKE_CURRENT_SOURCE_DIR}/${libOutPath})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ARCHIVE_RUNTIME})
if(bSpecifiedPath)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${ARCHIVE_RUNTIME})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${ARCHIVE_RUNTIME})
else()
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${ARCHIVE_RUNTIME}/${CMAKE_VS_PLATFORM_NAME}/Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${ARCHIVE_RUNTIME}/${CMAKE_VS_PLATFORM_NAME}/Release)
endif(bSpecifiedPath)
endif(NOT("${libOutPath}" STREQUAL ""))

#[[
message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY == ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG == ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}")
message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE == ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}")

message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY == ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG == ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG}")
message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE == ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE}")

message(STATUS "CMAKE_ARCHIVE_OUTPUT_DIRECTORY == ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
message(STATUS "CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG == ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}")
message(STATUS "CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE == ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE}")
]]
endmacro(MEMADE_OUTPATHCONFIG)


