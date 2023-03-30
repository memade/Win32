function(install_sub_project entry_path entry_name)
		file(GLOB SRCS "${entry_path}/*.c" "${entry_path}/*.cc" "${entry_path}/*.cpp")	
		file(GLOB HEADS "${entry_path}/*.h" "${entry_path}/*.hpp")	
		set(SOURCES
		${SRCS}
		${HEADS}
		)
		add_executable(${entry_name} ${SOURCES})
		set_target_properties(${entry_name} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
		if(MSVC AND ENABLE_MSVC_CRT_MT)
		set_property(TARGET ${entry_name} PROPERTY
		MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
		endif()
		target_include_directories(${entry_name} 
		PRIVATE 
		${SOURCES_DIR}
		${OPENSSL_INCLUDE_DIR}
		)
		target_link_directories(${entry_name} 
		PRIVATE 
		${SOLUTION_ROOT_DIR}/librarys/openssl/3.0.5/$(Platform)/
		)
endfunction(install_sub_project entry_path entry_name)


# Discover the projects that use CMake in the subdirectories.
# Note that explicit cmake invocation is required every time a new project is
# added or removed.
file(GLOB entries *)
foreach(entry ${entries})
 #if(IS_DIRECTORY ${entry} AND EXISTS ${entry}/CMakeLists.txt)
	if(IS_DIRECTORY ${entry})
		get_filename_component(origin_entry_name "${entry}" NAME)

		install_sub_project(${entry} ${origin_entry_name})
			
		set_target_properties(${origin_entry_name} PROPERTIES FOLDER "tests")
	 message(STATUS "current filename ${origin_entry_name}") 
		message(STATUS "current path ${entry}")
 endif()
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