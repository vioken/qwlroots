set(TARGET qwlroots)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
    Core
    Gui
    REQUIRED
)

find_package(PkgConfig)
pkg_search_module(WLROOTS REQUIRED wlroots>=0.15)
pkg_search_module(PIXMAN REQUIRED pixman-1)
pkg_get_variable(WLR_HAVE_VULKAN wlroots have_vulkan_renderer)

setup_package_version_variables(WLROOTS)
add_definitions(-DWLR_VERSION_STRING="${WLROOTS_VERSION}"
                -DWLR_VERSION_MAJOR=${WLROOTS_VERSION_MAJOR}
                -DWLR_VERSION_MINOR=${WLROOTS_VERSION_MINOR}
                -DWLR_VERSION_PATCH=${WLROOTS_VERSION_PATCH})

add_definitions(-DWLR_USE_UNSTABLE -DQW_LIBRARY)

set(SOURCES
    qwglobal.cpp
    qwbackend.cpp
    render/qwrenderer.cpp
    render/qwtexture.cpp
    render/qwallocator.cpp
    types/qwbuffer.cpp
    types/qwcompositor.cpp
    util/qwsignalconnector.cpp
)

set(HEADERS
    qwglobal.h
    qwbackend.h
    render/qwrenderer.h
    render/qwtexture.h
    render/qwallocator.h
    types/qwbuffer.h
    types/qwcompositor.h
    util/qwsignalconnector.h
)

add_library(${TARGET}
    SHARED
    ${SOURCES}
)

set_target_properties(${TARGET}
    PROPERTIES
        VERSION ${CMAKE_PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        PUBLIC_HEADER "${GLOBAL_HEADERS} ${HEADERS}"
)

set(QT_LIBRAIES
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui)

target_link_libraries(${TARGET}
    PRIVATE
    ${QT_LIBRAIES}
    ${WLROOTS_LIBRARIES}
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

target_include_directories(${TARGET}
    PRIVATE
    ${Qt${QT_VERSION_MAJOR}Gui_PRIVATE_INCLUDE_DIRS}
    ${WLROOTS_INCLUDE_DIRS}
)

target_include_directories(${TARGET}
    PUBLIC
)

install(TARGETS ${TARGET}
    LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER
        COMPONENT Development
        DESTINATION include/${TARGET}
)
