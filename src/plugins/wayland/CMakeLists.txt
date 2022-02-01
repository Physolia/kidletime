set(idletime_plugin_SRCS
    poller.cpp
    poller.h
)

ecm_add_qtwayland_client_protocol(idletime_plugin_SRCS
    PROTOCOL ${PLASMA_WAYLAND_PROTOCOLS_DIR}/idle.xml
    BASENAME idle
)

add_library(KF5IdleTimeWaylandPlugin MODULE ${idletime_plugin_SRCS})
target_link_libraries(KF5IdleTimeWaylandPlugin
    KF5::IdleTime
    Qt${QT_MAJOR_VERSION}::WaylandClient
    Qt${QT_MAJOR_VERSION}::GuiPrivate
    Wayland::Client
)

install(
    TARGETS
        KF5IdleTimeWaylandPlugin
    DESTINATION
        ${KDE_INSTALL_PLUGINDIR}/kf5/org.kde.kidletime.platforms/
)
