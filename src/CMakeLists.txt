set(TARGET qwlroots)
set(CMAKE_NAME ${CMAKE_PROJECT_NAME})
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
    Core
    Gui
    REQUIRED
)

set(WLROOTS_MINIMUM_REQUIRED 0.16.0)
find_package(PkgConfig REQUIRED)
pkg_search_module(WLROOTS REQUIRED IMPORTED_TARGET wlroots>=${WLROOTS_MINIMUM_REQUIRED})
pkg_search_module(WAYLAND_SERVER REQUIRED IMPORTED_TARGET wayland-server)
pkg_search_module(PIXMAN REQUIRED IMPORTED_TARGET pixman-1)

pkg_get_variable(WLR_HAVE_DRM_BACKEND wlroots have_drm_backend)
pkg_get_variable(WLR_HAVE_X11_BACKEND wlroots have_x11_backend)
pkg_get_variable(WLR_HAVE_LIBINPUT_BACKEND wlroots have_libinput_backend)
pkg_get_variable(WLR_HAVE_XWAYLAND wlroots have_xwayland)
pkg_get_variable(WLR_HAVE_GLES2_RENDERER wlroots have_gles2_renderer)
pkg_get_variable(WLR_HAVE_VULKAN_RENDERER wlroots have_vulkan_renderer)
pkg_get_variable(WLR_HAVE_GBM_ALLOCATOR wlroots have_gbm_allocator)
pkg_get_variable(WLR_HAVE_SESSION wlroots have_session)
setup_package_version_variables(WLROOTS)
include(CMakePackageConfigHelpers)
configure_file(${PROJECT_SOURCE_DIR}/src/cmake/qwconfig.h.in ${CMAKE_CURRENT_SOURCE_DIR}/qwconfig.h)

message("wlroots version=${WLROOTS_VERSION}, major=${WLROOTS_VERSION_MAJOR}, minor=${WLROOTS_VERSION_MINOR}, patch=${WLROOTS_VERSION_PATCH}")

add_definitions(-DWLR_USE_UNSTABLE -DQW_LIBRARY)

pkg_search_module(WAYLAND_PROTOCOLS REQUIRED wayland-protocols)
ws_generate(server wayland-protocols stable/xdg-shell/xdg-shell.xml xdg-shell-protocol)

set(SOURCES
    qwglobal.cpp
    qwbackend.cpp
    qwdisplay.cpp
    render/qwrenderer.cpp
    render/qwtexture.cpp
    render/qwallocator.cpp
    types/qwbuffer.cpp
    types/qwcompositor.cpp
    types/qwsubcompositor.cpp
    types/qwdatadevice.cpp
    types/qwdamagering.cpp
    types/qwoutputlayout.cpp
    types/qwscene.cpp
    types/qwseat.cpp
    types/qwxdgshell.cpp
    types/qwcursor.cpp
    types/qwxcursormanager.cpp
    types/qwinputdevice.cpp
    types/qwkeyboard.cpp
    types/qwpointer.cpp
    types/qwtablet.cpp
    types/qwtabletpad.cpp
    types/qwoutput.cpp
    types/qwxdgdecorationmanagerv1.cpp
    types/qwidle.cpp
    types/qwkeyboardgroup.cpp
    util/qwsignalconnector.cpp
    interfaces/qwbackendinterface.cpp
    interfaces/qwoutputinterface.cpp
    interfaces/qwkeyboardinterface.cpp
    interfaces/qwbufferinterface.h
    interfaces/qwbufferinterface.cpp
)

set(HEADERS
    qwglobal.h
    qwbackend.h
    qwdisplay.h
    render/qwrenderer.h
    render/qwtexture.h
    render/qwallocator.h
    types/qwbuffer.h
    types/qwcompositor.h
    types/qwsubcompositor.h
    types/qwdatadevice.h
    types/qwdamagering.h
    types/qwoutputlayout.h
    types/qwscene.h
    types/qwseat.h
    types/qwxdgshell.h
    types/qwcursor.h
    types/qwxcursormanager.h
    types/qwinputdevice.h
    types/qwkeyboard.h
    types/qwpointer.h
    types/qwtablet.h
    types/qwtabletpad.h
    types/qwoutput.h
    types/qwxdgdecorationmanagerv1.h
    types/qwidle.h
    types/qwkeyboardgroup.h
    util/qwsignalconnector.h
    interfaces/qwinterface.h
    interfaces/qwbackendinterface.h
    interfaces/qwoutputinterface.h
    interfaces/qwkeyboardinterface.h
    qwconfig.h
)

set(PRIVATE_HEADERS
    types/qwinputdevice_p.h
)

pkg_search_module(WLR_PROTOCOLS wlr-protocols)
if (WLR_PROTOCOLS_FOUND)
    ws_generate(server wlr-protocols unstable/wlr-layer-shell-unstable-v1.xml wlr-layer-shell-unstable-v1-protocol)
    set(PROTOCOL_HEADERS
        types/qwlayershellv1.h
    )
    set(PROTOCOL_SOURCES
        types/qwlayershellv1.cpp
    )
    list(APPEND HEADERS ${PROTOCOL_HEADERS})
    list(APPEND SOURCES ${PROTOCOL_SOURCES})
endif ()

add_library(${TARGET}
    SHARED
    ${SOURCES}
    ${HEADERS}
    ${PRIVATE_HEADERS}
)

set_target_properties(${TARGET}
    PROPERTIES
        VERSION ${CMAKE_PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        PUBLIC_HEADER "${HEADERS}"
        EXPORT_NAME ${CMAKE_NAME}
)

set(QT_LIBRAIES
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui)

target_link_libraries(${TARGET}
    PRIVATE
    ${QT_LIBRAIES}
    PkgConfig::WLROOTS
    PkgConfig::WAYLAND_SERVER
    PkgConfig::PIXMAN
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

target_include_directories(${TARGET}
    PRIVATE
    ${Qt${QT_VERSION_MAJOR}Gui_PRIVATE_INCLUDE_DIRS}
)

target_include_directories(${TARGET}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/render>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/types>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/util>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/interfaces>
)

target_include_directories(${TARGET}
    INTERFACE
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${TARGET}>
)

target_link_directories(${TARGET}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_LIBDIR}>
)

install(TARGETS ${TARGET}
    EXPORT ${CMAKE_NAME}Targets
    LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER
        COMPONENT Development
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${TARGET}
)

include(${PROJECT_SOURCE_DIR}/cmake/Helpers.cmake)
add_cmake_module(${CMAKE_NAME} ${TARGET} ${CMAKE_INSTALL_FULL_INCLUDEDIR}/${TARGET})
add_pkgconfig_module(${TARGET} ${TARGET} ${CMAKE_INSTALL_FULL_INCLUDEDIR}/${TARGET}
    "wlroots >= ${WLROOTS_MINIMUM_REQUIRED}, Qt${QT_VERSION_MAJOR}Gui"
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_NAME}Config.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_NAME}
)

install(EXPORT ${CMAKE_NAME}Targets
    FILE ${CMAKE_NAME}Targets.cmake
    NAMESPACE ${CMAKE_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_NAME}
)
