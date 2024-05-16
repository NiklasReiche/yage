# TODO: find a way to do this at build time
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS shaders/textShader.vert)
file(READ shaders/textShader.vert TEXT_VERT)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS shaders/textShader.frag)
file(READ shaders/textShader.frag TEXT_FRAG)
configure_file(shaders.in.cpp shaders.cpp @ONLY)
configure_file(shaders.h shaders.h) #also copy the header to resolve relative include in cpp file

target_sources(yage_font
        PRIVATE
        fileformat.h
        font.h
        text.h
        text.cpp
        loader.h
        loader.cpp
        fontHeader.h
        converter.h
        converter.cpp

        shaders.h
        ${CMAKE_CURRENT_BINARY_DIR}/shaders.cpp
)
