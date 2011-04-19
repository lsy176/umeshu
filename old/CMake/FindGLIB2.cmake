if( UNIX )
    find_path( GTK_glibconfig_INCLUDE_PATH
        NAMES glibconfig.h
        PATHS
        /usr/lib/glib-2.0/include
        /usr/openwin/share/include
        /usr/local/include/glib12
        /usr/lib/glib/include
        /usr/local/lib/glib/include
        /opt/gnome/include
        /opt/gnome/lib/glib/include
        /opt/local/lib/glib-2.0/include
        ${GTK_WIN_LIBRARY_DIR}/glib-2.0/include
        )


    find_path( GTK_glib_INCLUDE_PATH
        NAMES glib.h
        PATH_SUFFIXES gtk-2.0 glib-2.0 gtk-1.2 glib-1.2 glib20 glib12
        PATHS
        /usr/include
        /usr/openwin/share/include
        /usr/lib/glib/include
        /opt/gnome/include
        /opt/local/include/glib-2.0
        )

    find_library( GTK_glib_LIBRARY
        NAMES  glib-2.0 glib20 glib-1.2 glib12 glib
        PATHS  /usr/openwin/lib
        /opt/gnome/lib
        /usr/lib
        /opt/local/lib
        )

    include( FindPackageHandleStandardArgs )
    FIND_PACKAGE_HANDLE_STANDARD_ARGS( GLIB DEFAULT_MSG GTK_glib_LIBRARY GTK_glibconfig_INCLUDE_PATH GTK_glib_INCLUDE_PATH )

    if( GLIB_FOUND )
        set( GLIB_INCLUDE_DIR  ${GTK_glibconfig_INCLUDE_PATH} ${GTK_glib_INCLUDE_PATH} )
        set( GLIB_LIBRARY ${GTK_glib_LIBRARY} )
    else()
        set( GLIB_INCLUDE_DIR )
        set( GLIB_LIBRARY )
    endif()

endif()

