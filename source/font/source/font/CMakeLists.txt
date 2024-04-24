target_sources(yage_font
        PRIVATE
        fileformat.h
        font.h
        text.h
        text.cpp
        #shaders.h
        loader.h
        loader.cpp
        fontHeader.h

        converter/converter.h
        converter/converter.cpp
        converter/ftMath.h
        converter/ftMath.cpp
        converter/img.h
        converter/img.cpp
)