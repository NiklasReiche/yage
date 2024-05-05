# TODO: find a way to do this at build time
#set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS shaders/textShader.vert)
#file(READ shaders/textShader.vert TEXT_VERT)
#set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS shaders/textShader.frag)
#file(READ shaders/textShader.frag TEXT_FRAG)
#configure_file(shaders.in.cpp shaders.cpp @ONLY)

target_sources(yage_gui
        PRIVATE
        widgets/button.h
        widgets/button.cpp
        widgets/entry.h
        widgets/entry.cpp
        widgets/frame.h
        widgets/frame.cpp
        widgets/label.h
        widgets/label.cpp
        animation.h
        animation.cpp
        core.h
        gui.h
        inputmanager.h
        inputmanager.cpp
        layouts.h
        layouts.cpp
        master.h
        master.cpp
        renderer.h
        renderer.cpp
        root.h
        root.cpp
        shaders.h
        texturemanager.h
        texturemanager.cpp
        widget.h
        widget.cpp
        widgettemplate.h
)