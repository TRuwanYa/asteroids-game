cmake_minimum_required(VERSION 3.24)

project(Asteroids LANGUAGES CXX)

set(ASTEROIDS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Asteroids"
    CACHE PATH "Directory containing the game's .cpp and .hpp files")
set(ASTEROIDS_ASSET_DIR "${ASTEROIDS_SOURCE_DIR}/assets"
    CACHE PATH "Directory containing images, sounds, and fonts")

file(GLOB ASTEROIDS_SOURCES CONFIGURE_DEPENDS
    "${ASTEROIDS_SOURCE_DIR}/*.cpp"
)

if(NOT ASTEROIDS_SOURCES)
    message(FATAL_ERROR
        "No .cpp files found in ${ASTEROIDS_SOURCE_DIR}. "
        "Set ASTEROIDS_SOURCE_DIR to your actual game source directory.")
endif()

if(NOT IS_DIRECTORY "${ASTEROIDS_ASSET_DIR}")
    message(FATAL_ERROR
        "Assets directory not found: ${ASTEROIDS_ASSET_DIR}. "
        "Set ASTEROIDS_ASSET_DIR to your actual assets directory.")
endif()

include(FetchContent)

set(BUILD_SHARED_LIBS ON CACHE BOOL "Build SFML shared libraries" FORCE)
set(SFML_BUILD_GRAPHICS ON CACHE BOOL "" FORCE)
set(SFML_BUILD_WINDOW ON CACHE BOOL "" FORCE)
set(SFML_BUILD_AUDIO ON CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)


if(CMAKE_VERSION VERSION_GREATER_EQUAL "4.0"
   AND NOT DEFINED CMAKE_POLICY_VERSION_MINIMUM)
    set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
endif()

FetchContent_Declare(sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(sfml)

add_executable(Asteroids ${ASTEROIDS_SOURCES})

target_compile_features(Asteroids PRIVATE cxx_std_17)
set_target_properties(Asteroids PROPERTIES
    CXX_EXTENSIONS OFF
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:Asteroids>"
)
target_include_directories(Asteroids PRIVATE "${ASTEROIDS_SOURCE_DIR}")
target_link_libraries(Asteroids PRIVATE
    sfml-graphics
    sfml-audio
    sfml-window
    sfml-system
)

set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    PROPERTY VS_STARTUP_PROJECT Asteroids)

add_custom_command(TARGET Asteroids POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${ASTEROIDS_ASSET_DIR}"
        "$<TARGET_FILE_DIR:Asteroids>/assets"
    VERBATIM
)

if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(ASTEROIDS_OPENAL_ARCH x64)
    else()
        set(ASTEROIDS_OPENAL_ARCH x86)
    endif()

    add_custom_command(TARGET Asteroids POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "$<TARGET_FILE:sfml-graphics>"
            "$<TARGET_FILE:sfml-audio>"
            "$<TARGET_FILE:sfml-window>"
            "$<TARGET_FILE:sfml-system>"
            "${sfml_SOURCE_DIR}/extlibs/bin/${ASTEROIDS_OPENAL_ARCH}/openal32.dll"
            "$<TARGET_FILE_DIR:Asteroids>"
        VERBATIM
    )
endif()

