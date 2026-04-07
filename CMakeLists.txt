cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
	set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
	set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")
endif()
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(GEODE_SET_TARGET_AS_SYSTEM ON)

project(IconKitFilterAndSort)

add_library(${PROJECT_NAME} SHARED
	src/constants/other.cpp
	src/constants/authors.cpp
	src/constants/categories.cpp

	src/hooks/GameManager.cpp
	src/hooks/GJGarageLayer.cpp
	src/hooks/PurchaseItemPopup.cpp

	src/popups/DisplayOptionsPopup.cpp
	src/popups/SortPopup.cpp
	src/popups/FilterPopup.cpp
	src/popups/AuthorFilterPopup.cpp
	src/popups/CategoryFilterPopup.cpp
	src/popups/FilterAndSortPopup.cpp

	src/utils/SortIconButtonSprite.cpp
	src/utils/BoundCCMenu.cpp
	src/utils/LinkedCCMenu.cpp
	src/utils/CCMenuItemTogglerSpoof.cpp

	src/iconkit.cpp
	src/logic.cpp
	src/main.cpp
	src/api/api.cpp
)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_sources(${PROJECT_NAME} PRIVATE src/debug.cpp)
endif()
include_directories(${PROJECT_NAME} PUBLIC include)


if (NOT DEFINED ENV{GEODE_SDK})
	message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
	message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

target_compile_definitions(${PROJECT_NAME} PUBLIC MORE_ICONS_EVENTS)

if (MSVC)
	message(FATAL_ERROR "MSVC is not supported. Either switch to clang or remove this line in CMakeLists.txt and pray.")
else()
	target_compile_options(
		${PROJECT_NAME} PUBLIC
		"-Werror"
		"-Wall" "-Wextra" "-Wpedantic" "-Wconversion" "-Wshadow" "-Wnull-dereference" "-Wdouble-promotion"
		"-Wno-dollar-in-identifier-extension" "-Wno-c99-designator"
	)
endif()

# Set up dependencies, resources, link Geode
setup_geode_mod(${PROJECT_NAME})
