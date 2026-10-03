cmake_minimum_required(VERSION 3.20)

project(eeprom_programmer LANGUAGES C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

file(GLOB UTIL_SOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/*.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cxx"
)

if(NOT UTIL_SOURCES)
	message(FATAL_ERROR "No C or C++ source files found in ${CMAKE_CURRENT_SOURCE_DIR}")
endif()

add_executable(eeprom_programmer ${UTIL_SOURCES})

if(WIN32)
	target_link_libraries(eeprom_programmer PRIVATE kernel32)
endif()

target_include_directories(eeprom_programmer PUBLIC ".")

set_target_properties(eeprom_programmer PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_SOURCE_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)

message("Build Complete")
