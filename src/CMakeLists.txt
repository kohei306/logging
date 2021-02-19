set(CMAKE_CXX_STANDARD 14)
set(PROJECT_NAME logging)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# find_package(ara_logging REQUIRED)

add_library(${PROJECT_NAME} STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/logger.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/logger_map.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/log_manager.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/logging_configurator.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/appender/appender_base.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/appender/console_appender.cpp
 # ${CMAKE_CURRENT_SOURCE_DIR}/appender/aralog_appender.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/appender/appender_config.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/appender/file_appender.cpp
 )


# Set install directories
set(public_header_include_dest "include/${PROJECT_NAME}") 

# libraries
set(lib_dest     "lib/${PROJECT_NAME}")

# for find_package
set(cmake_dest   "lib/${PROJECT_NAME}/cmake")

# Set includes paths
set(header_path "${CMAKE_CURRENT_SOURCE_DIR}")

# Set includes paths
file(GLOB_RECURSE  public_header ${CMAKE_CURRENT_SOURCE_DIR}/../include/logging/*h)
set(header ${public_header})    

# Link include directories
target_include_directories(${PROJECT_NAME}
 PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
 PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include/logging>
 PUBLIC $<INSTALL_INTERFACE:${include_dest}>
)

target_link_libraries(${PROJECT_NAME}
)

## Mark executables and/or libraries for installation
# libraries and executables
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-targets
        LIBRARY DESTINATION "${lib_dest}"
        ARCHIVE DESTINATION "${lib_dest}"
        RUNTIME DESTINATION "bin")

# headers
install(FILES ${header} DESTINATION "${public_header_include_dest}")
