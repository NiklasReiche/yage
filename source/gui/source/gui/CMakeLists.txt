# TODO: find a way to do this at build time
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS shaders/widgetShader.vert)
file(READ shaders/widgetShader.vert WIDGET_VERT)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS shaders/widgetShader.frag)
file(READ shaders/widgetShader.frag WIDGET_FRAG)
configure_file(shaders.in.cpp shaders.cpp @ONLY)
configure_file(shaders.h shaders.h) #also copy the header to resolve relative include in cpp file

target_sources(yage_gui
        PRIVATE
        widgets/widget.h
        widgets/widget.cpp
        widgets/root.h
        widgets/root.cpp
        widgets/button.h
        widgets/button.cpp
        widgets/entry.h
        widgets/entry.cpp
        widgets/frame.h
        widgets/frame.cpp
        widgets/label.h
        widgets/label.cpp
        widgets/VListBox.h
        widgets/VListBox.cpp
        widgets/HListBox.h
        widgets/HListBox.cpp

        animation.h
        animation.cpp
        inputmanager.h
        inputmanager.cpp
        master.h
        master.cpp
        renderer.h
        renderer.cpp
        texturemanager.h
        texturemanager.cpp

        shaders.h
        ${CMAKE_CURRENT_BINARY_DIR}/shaders.cpp

        gui.h
)