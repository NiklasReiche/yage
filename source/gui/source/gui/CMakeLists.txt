# TODO: find a way to do this at build time
#set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS shaders/textShader.vert)
#file(READ shaders/textShader.vert TEXT_VERT)
#set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS shaders/textShader.frag)
#file(READ shaders/textShader.frag TEXT_FRAG)
#configure_file(shaders.in.cpp shaders.cpp @ONLY)

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
        shaders.h
        texturemanager.h
        texturemanager.cpp

        gui.h
)