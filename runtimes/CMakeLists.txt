function(install_sub_project entry_path entry_name)
		file(GLOB SRCS "${entry_path}/*.c" "${entry_path}/*.cc" "${entry_path}/*.cpp")	
		file(GLOB HEADS "${entry_path}/*.h" "${entry_path}/*.hpp")	
		set(SOURCES
		${SRCS}
		${HEADS}
		)

		add_library(${entry_name} STATIC ${SOURCES})

		if(MSVC AND ENABLE_MSVC_CRT_MT)
		set_property(TARGET ${entry_name} PROPERTY
		MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
		endif(MSVC AND ENABLE_MSVC_CRT_MT)
		
		MSVC_USE_PCH(${entry_name} ${entry_path}/stdafx.h ${entry_path}/stdafx.cpp)
	#[[	
		target_include_directories(${entry_name} 
		PRIVATE 
		${SOLUTION_INCLUDE_DIR}
		${SOLUTION_ROOT_DIR}/third_party/
		${SOLUTION_ROOT_DIR}/third_party/wxWidgets/wxWidgets-3.2.2.1/include/
		${SOLUTION_ROOT_DIR}/third_party/wxWidgets/wxWidgets-3.2.2.1/include/msvc/
		)
		]]
		set(WXLIB_DIR ${SOLUTION_ROOT_DIR}/third_party/wxWidgets/wxWidgets-3.2.2.1/lib/vc_lib/)
		if(${CMAKE_VS_PLATFORM_NAME} STREQUAL "x64")
		set(WXLIB_DIR ${SOLUTION_ROOT_DIR}/third_party/wxWidgets/wxWidgets-3.2.2.1/lib/vc_x64_lib/)
		endif(${CMAKE_VS_PLATFORM_NAME} STREQUAL "x64")
		
endfunction(install_sub_project entry_path entry_name)


# Discover the projects that use CMake in the subdirectories.
# Note that explicit cmake invocation is required every time a new project is
# added or removed.
file(GLOB entries *)
foreach(entry ${entries})
 #if(IS_DIRECTORY ${entry} AND EXISTS ${entry}/CMakeLists.txt)
	if(IS_DIRECTORY ${entry})
		get_filename_component(origin_entry_name "${entry}" NAME)
		
		string(COMPARE EQUAL ${origin_entry_name} "cef" result_cef)
		
		string(COMPARE EQUAL ${origin_entry_name} "openssl" result_openssl)
		if(NOT result_openssl AND NOT result_cef)
		install_sub_project(${entry} ${origin_entry_name})
		
		set(CEF_INCLUDE_DIR ${SOLUTION_ROOT_DIR}/third_party/cef/109.0.5414.120/x86/)
		if(${CMAKE_VS_PLATFORM_NAME} STREQUAL "x64")
		set(CEF_INCLUDE_DIR ${SOLUTION_ROOT_DIR}/third_party/cef/109.0.5414.120/x64/)
		endif(${CMAKE_VS_PLATFORM_NAME} STREQUAL "x64")
			
		if(${origin_entry_name} STREQUAL "openssl")
		elseif(${origin_entry_name} STREQUAL "cef")
		elseif(${origin_entry_name} STREQUAL "win")
			target_include_directories(${origin_entry_name} 
			PRIVATE 
			${SOLUTION_INCLUDE_DIR}
			${SOLUTION_ROOT_DIR}/third_party/
			)
		elseif(${origin_entry_name} STREQUAL "cef++")			
			target_include_directories(${origin_entry_name} 
			PRIVATE 
			${SOLUTION_INCLUDE_DIR}
			${SOLUTION_ROOT_DIR}/third_party/
			${CEF_INCLUDE_DIR}/
			)		
		elseif(${origin_entry_name} STREQUAL "zip++")
			target_include_directories(${origin_entry_name} 
			PRIVATE 
			${SOLUTION_INCLUDE_DIR}
			${SOLUTION_ROOT_DIR}/third_party/
			)		
		elseif(${origin_entry_name} STREQUAL "wxskin")		
			target_include_directories(${origin_entry_name} 
			PRIVATE 
			${SOLUTION_INCLUDE_DIR}
			${SOLUTION_ROOT_DIR}/third_party/
			${CEF_INCLUDE_DIR}/
			${SOLUTION_ROOT_DIR}/third_party/imgui/
			${SOLUTION_ROOT_DIR}/third_party/wxWidgets/wxWidgets-3.2.2.1/include/
			${SOLUTION_ROOT_DIR}/third_party/wxWidgets/wxWidgets-3.2.2.1/include/msvc/
			)
		endif()
		set_target_properties(${origin_entry_name} PROPERTIES FOLDER "runtimes")
		endif(NOT result_openssl AND NOT result_cef)

		
 endif(IS_DIRECTORY ${entry})
endforeach(entry)


##[[
#  get_filename_component(origin_entry_name "${entry}" NAME)
#  string(TOUPPER ${origin_entry_name} upper_entry_name)
#  if(
#  (${upper_entry_name} STREQUAL "WINUI") 
#   OR 
#  (${upper_entry_name} STREQUAL "LIBUVPP") 
 # OR 
  #(${upper_entry_name} STREQUAL "LIBCURLPP")
  #OR
#  (${upper_entry_name} STREQUAL "PROXY"))
#  add_subdirectory(${entry})
#  set_target_properties(${origin_entry_name} PROPERTIES FOLDER "projects")
#  elseif(${upper_entry_name} STREQUAL "MT5PROJECTS")
  ##[[
  #add_subdirectory(${entry})
  #set_target_properties(${origin_entry_name} PROPERTIES FOLDER "projects")
  #]]
  #MT项目需要单独配置CMAKE操作
  #message(STATUS "append project - ${origin_entry_name}")
#  elseif() 
#  endif()
  #message(STATUS ${entry_name})
#]]