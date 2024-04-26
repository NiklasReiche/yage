# TODO: find a way to do this at build time
#set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS shaders/textShader.vert)
#file(READ shaders/textShader.vert TEXT_VERT)
#set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS shaders/textShader.frag)
#file(READ shaders/textShader.frag TEXT_FRAG)
#configure_file(shaders.in.cpp shaders.cpp @ONLY)

target_sources(yage_gui
        PRIVATE
        core.h
)