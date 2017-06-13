#########################
# source/core/Cmake
#########################

set( YAGE_IMAGE_SOURCE

	"${YAGE_MODULES_PATH}/image/bmp.h"
	"${YAGE_MODULES_PATH}/image/bmp.cpp"
	"${YAGE_MODULES_PATH}/image/enum.h"
	"${YAGE_MODULES_PATH}/image/image.h"
	"${YAGE_MODULES_PATH}/image/image.cpp"
	"${YAGE_MODULES_PATH}/image/img.h"
	"${YAGE_MODULES_PATH}/image/img.cpp"
	
)

set( YAGE_FONT_SOURCE

	"${YAGE_MODULES_PATH}/font/fileformat.h"
	"${YAGE_MODULES_PATH}/font/font.h"
	"${YAGE_MODULES_PATH}/font/fontHeader.h"
	"${YAGE_MODULES_PATH}/font/loader.h"
	"${YAGE_MODULES_PATH}/font/loader.cpp"
	"${YAGE_MODULES_PATH}/font/text.h"
	"${YAGE_MODULES_PATH}/font/text.cpp"
	
)
set( YAGE_FONT_CONVERTER_SOURCE

	"${YAGE_MODULES_PATH}/font/converter/converter.h"
	"${YAGE_MODULES_PATH}/font/converter/converter.cpp"
	"${YAGE_MODULES_PATH}/font/converter/ftMath.h"
	"${YAGE_MODULES_PATH}/font/converter/ftMath.cpp"
	
)

set( YAGE_GUI_SOURCE

	"${YAGE_MODULES_PATH}/gui/gui.h"
	"${YAGE_MODULES_PATH}/gui/core.h"
	"${YAGE_MODULES_PATH}/gui/master.h"
	"${YAGE_MODULES_PATH}/gui/master.cpp"
	"${YAGE_MODULES_PATH}/gui/widget.h"
	"${YAGE_MODULES_PATH}/gui/widget.cpp"
	"${YAGE_MODULES_PATH}/gui/input.h"
	"${YAGE_MODULES_PATH}/gui/input.cpp"
	"${YAGE_MODULES_PATH}/gui/button.h"
	"${YAGE_MODULES_PATH}/gui/button.cpp"
	"${YAGE_MODULES_PATH}/gui/entry.h"
	"${YAGE_MODULES_PATH}/gui/entry.cpp"
	"${YAGE_MODULES_PATH}/gui/fontmanager.h"
	"${YAGE_MODULES_PATH}/gui/fontmanager.cpp"
	"${YAGE_MODULES_PATH}/gui/frame.h"
	"${YAGE_MODULES_PATH}/gui/frame.cpp"
	"${YAGE_MODULES_PATH}/gui/label.h"
	"${YAGE_MODULES_PATH}/gui/label.cpp"
	"${YAGE_MODULES_PATH}/gui/interface.h"
	"${YAGE_MODULES_PATH}/gui/renderer.h"
	"${YAGE_MODULES_PATH}/gui/renderer.cpp"
	
)