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
configure_file(${PROJECT_SOURCE_DIR}/src/cmake/qwconfig.h.in generate/qwconfig.h @ONLY)

message("wlroots version=${WLROOTS_VERSION}, major=${WLROOTS_VERSION_MAJOR}, minor=${WLROOTS_VERSION_MINOR}, patch=${WLROOTS_VERSION_PATCH}")

pkg_search_module(WAYLAND_PROTOCOLS REQUIRED wayland-protocols)
ws_generate(server wayland-protocols stable/xdg-shell/xdg-shell.xml xdg-shell-protocol)
ws_generate(server wayland-protocols unstable/fullscreen-shell/fullscreen-shell-unstable-v1.xml fullscreen-shell-unstable-v1-protocol)
ws_generate(server wayland-protocols unstable/tablet/tablet-unstable-v2.xml tablet-unstable-v2-protocol)
ws_generate(server wayland-protocols unstable/pointer-constraints/pointer-constraints-unstable-v1.xml pointer-constraints-unstable-v1-protocol)

set(SOURCES
    qwglobal.cpp
    qwbackend.cpp
    qwdisplay.cpp
    render/qwrenderer.cpp
    render/qwtexture.cpp
    render/qwallocator.cpp
    render/qwegl.cpp
    types/qwbuffer.cpp
    types/qwclientbuffer.cpp
    types/qwcompositor.cpp
    types/qwexportdmabufmanagerv1.cpp
    types/qwsubcompositor.cpp
    types/qwdatadevice.cpp
    types/qwdatasource.cpp
    types/qwdatadevicev1.cpp
    types/qwdatacontrolmanagerv1.cpp
    types/qwdamagering.cpp
    types/qwdrm.cpp
    types/qwdrmleasev1.cpp
    types/qwdrmleasev1manager.cpp
    types/qwdrmleaserequestv1.cpp
    types/qwoutputlayout.cpp
    types/qwscene.cpp
    types/qwseat.cpp
    types/qwprimaryselection.cpp
    types/qwswitch.cpp
    types/qwxdgshell.cpp
    types/qwcursor.cpp
    types/qwxcursormanager.cpp
    types/qwinputdevice.cpp
    types/qwtouch.cpp
    types/qwkeyboard.cpp
    types/qwpointer.cpp
    types/qwtablet.cpp
    types/qwtablettool.cpp
    types/qwtabletpad.cpp
    types/qwoutput.cpp
    types/qwxdgdecorationmanagerv1.cpp
    types/qwidle.cpp
    types/qwidleinhibitv1.cpp
    types/qwinputinhibitmanager.cpp
    types/qwkeyboardgroup.cpp
    types/qwpresentation.cpp
    types/qwviewporter.cpp
    types/qwxdgforeignregistry.cpp
    types/qwxdgoutputv1.cpp
    types/qwxdgactivationv1.cpp
    types/qwxdgactivationtokenv1.cpp
    types/qwtextinputv3.cpp
    types/qwtextinputmanagerv3.cpp
    types/qwtabletmanagerv2.cpp
    types/qwsinglepixelbufferv1.cpp
    types/qwsessionlockv1.cpp
    types/qwsessionlockmanagerv1.cpp
    types/qwsessionlocksurfacev1.cpp
    types/qwscreencopyv1.cpp
    types/qwrelativepointerv1.cpp
    types/qwrelativepointermanagerv1.cpp
    types/qwprimaryselectionv1.cpp
    types/qwpointergesturesv1.cpp
    types/qwpointerconstraintv1.cpp
    types/qwpointerconstraintsv1.cpp
    types/qwoutputmanagerv1.cpp
    types/qwoutputconfigurationv1.cpp
    types/qwoutputconfigurationheadv1.cpp
    types/qwlinuxdmabufv1.cpp
    types/qwlinuxdmabuffeedbackv1.cpp
    types/qwinputmethodv2.cpp
    types/qwinputmethodkeyboardgrabv2.cpp
    types/qwinputpopupsurfacev2.cpp
    types/qwidlenotifyv1.cpp
    types/qwforeigntoplevelhandlev1.cpp
    types/qwforeigntoplevelmanagerv1.cpp
    types/qwxdgforeignv1.cpp
    types/qwxdgforeignv2.cpp
    types/qwvirtualkeyboardv1.cpp
    types/qwvirtualpointerv1.cpp
    types/qwgammacontorlv1.cpp
    types/qwfullscreenshellv1.cpp
    types/qwtabletv2.cpp
    types/qwtabletv2tablet.cpp
    types/qwtabletv2tabletool.cpp
    types/qwtabletv2tablepad.cpp
    types/qwtabletmanagerv2.cpp
    types/qwkeyboardshortcutsinhibitv1.cpp
    types/qwkeyboardshortcutsinhibitmanagerv1.cpp
    util/qwsignalconnector.cpp
    interfaces/qwbackendinterface.cpp
    interfaces/qwoutputinterface.cpp
    interfaces/qwkeyboardinterface.cpp
    interfaces/qwbufferinterface.cpp
    interfaces/qwrendererinterface.cpp
    interfaces/qwswitchinterface.cpp
    interfaces/qwpointerinterface.cpp
    interfaces/qwtabletpadinterface.cpp
)

set(HEADERS
    qwglobal.h
    qwbackend.h
    qwdisplay.h
    render/qwrenderer.h
    render/qwtexture.h
    render/qwallocator.h
    render/qwegl.h
    types/qwbuffer.h
    types/qwcompositor.h
    types/qwexportdmabufv1.h
    types/qwsubcompositor.h
    types/qwdatadevice.h
    types/qwdatacontrolv1.h
    types/qwdamagering.h
    types/qwdrm.h
    types/qwdrmleasev1.h
    types/qwoutputlayout.h
    types/qwscene.h
    types/qwseat.h
    types/qwprimaryselection.h
    types/qwswitch.h
    types/qwxdgshell.h
    types/qwcursor.h
    types/qwxcursormanager.h
    types/qwinputdevice.h
    types/qwtouch.h
    types/qwkeyboard.h
    types/qwpointer.h
    types/qwtablet.h
    types/qwtabletpad.h
    types/qwoutput.h
    types/qwxdgdecorationmanagerv1.h
    types/qwidle.h
    types/qwidleinhibitv1.h
    types/qwinputinhibitmanager.h
    types/qwkeyboardgroup.h
    types/qwpresentation.h
    types/qwxdgforeignregistry.h
    types/qwxdgforeignregistry_p.h
    types/qwxdgforeignv1.h
    types/qwxdgforeignv2.h
    types/qwviewporter.h
    types/qwxdgoutputv1.h
    types/qwxdgactivationv1.h
    types/qwpointerconstraintsv1.h
    types/qwtextinputv3.h
    types/qwtabletv2.h
    types/qwsinglepixelbufferv1.h
    types/qwsessionlockv1.h
    types/qwscreencopyv1.h
    types/qwrelativepointerv1.h
    types/qwprimaryselectionv1.h
    types/qwpointergesturesv1.h
    types/qwoutputmanagementv1.h
    types/qwlinuxdmabufv1.h
    types/qwinputmethodv2.h
    types/qwidlenotifyv1.h
    types/qwforeigntoplevelhandlev1.h
    types/qwvirtualkeyboardv1.h
    types/qwvirtualpointerv1.h
    types/qwgammacontorlv1.h
    types/qwfullscreenshellv1.h
    types/qwkeyboardshortcutsinhibitv1.h
    util/qwsignalconnector.h
    interfaces/qwinterface.h
    interfaces/qwbackendinterface.h
    interfaces/qwbufferinterface.h
    interfaces/qwrendererinterface.h
    interfaces/qwoutputinterface.h
    interfaces/qwkeyboardinterface.h
    ${CMAKE_CURRENT_BINARY_DIR}/generate/qwconfig.h
    interfaces/qwswitchinterface.h
    interfaces/qwpointerinterface.h
    interfaces/qwtabletpadinterface.h
)

set(PRIVATE_HEADERS
    types/qwinputdevice_p.h
)

if ((${WLROOTS_VERSION_MAJOR} EQUAL 0) AND (${WLROOTS_VERSION_MINOR} GREATER 16))
    ws_generate(
	server
	wayland-protocols
	staging/content-type/content-type-v1.xml
	content-type-v1-protocol
    )
    list(APPEND SOURCES
        render/qwswapchain.cpp
        types/qwshm.cpp
        types/qwfractionalscalev1.cpp
        types/qwcontenttypev1.cpp
    )
    list(APPEND HEADERS
        render/qwswapchain.h
        types/qwshm.h
        types/qwfractionalscalev1.cpp
        types/qwcontenttypev1.h
    )
    if(WLR_HAVE_XWAYLAND)
        list(APPEND SOURCES
            types/qwxwaylandshellv1.cpp
        )
        list(APPEND HEADERS
            types/qwxwaylandshellv1.h
        )
    endif()
endif()

if (WLR_HAVE_XWAYLAND)
list(APPEND SOURCES
    types/qwxwayland.cpp
    types/qwxwaylandsurface.cpp
    types/qwxwaylandserver.cpp
)
list(APPEND HEADERS
    types/qwxwayland.h
    types/qwxwaylandsurface.h
    types/qwxwaylandserver.h
)
endif()

pkg_search_module(WLR_PROTOCOLS wlr-protocols)
if (WLR_PROTOCOLS_FOUND)
    ws_generate(server wlr-protocols unstable/wlr-layer-shell-unstable-v1.xml wlr-layer-shell-unstable-v1-protocol)
    ws_generate(server wlr-protocols unstable/wlr-output-power-management-unstable-v1.xml wlr-output-power-management-unstable-v1-protocol)
    set(PROTOCOL_HEADERS
        types/qwlayershellv1.h
        types/qwoutputpowermanagementv1.h
    )
    set(PROTOCOL_SOURCES
        types/qwlayershellv1.cpp
        types/qwoutputpowermanagementv1.cpp
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

target_compile_definitions(${TARGET}
PRIVATE
    QW_LIBRARY
    WLR_USE_UNSTABLE
    QT_NO_SIGNALS_SLOTS_KEYWORDS
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
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generate>
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
