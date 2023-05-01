cmake_minimum_required ( VERSION 3.8 )

# enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (chat_gpt_api_cpp)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/bin/Install/ CACHE PATH "Force path to set CMAKE_INSTALL_PREFIX" FORCE)

# set default build output path
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# suppress generation of ZERO_CHECK
set(CMAKE_SUPPRESS_REGENERATION true)

# main.cpp ,...
add_subdirectory(src)

# set ${PROJECT_NAME} as startup project in VS
if(MSVC)
    if(${CMAKE_VERSION} VERSION_LESS "3.6.0")
        message( WARNING "CMake version lower than 3.6. Please update CMake and rerun or manually set start up project in Visual Studio.")
    else()
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
    endif()
endif()

# fix some warings
cmake_policy(SET CMP0074 NEW)
cmake_policy(SET CMP0079 NEW)

# only Windows x64 build are supported for now
set(DEP_ARCH x64)

# add jsoncpp library
find_library (JsonCpp_LIBRARY NAMES jsoncpp_static PATHS ${PROJECT_SOURCE_DIR}/lib/jsoncpp/${DEP_ARCH}/lib)
target_link_libraries(${PROJECT_NAME} ${JsonCpp_LIBRARY})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/lib/jsoncpp/${DEP_ARCH}/include)

# add libcurl library
find_library (CURL_LIBRARY NAMES libcurl_imp PATHS ${PROJECT_SOURCE_DIR}/lib/libcurl/${DEP_ARCH}/lib)
target_link_libraries(${PROJECT_NAME} ${CURL_LIBRARY})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/lib/libcurl/${DEP_ARCH}/include)

# copy libcurl.dll
if(WIN32)
  install(
    FILES
      ${PROJECT_SOURCE_DIR}/lib/libcurl/${DEP_ARCH}/bin/libcurl.dll
    DESTINATION ${CMAKE_INSTALL_PREFIX}
    CONFIGURATIONS Release
  )
endif()