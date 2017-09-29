#########################
# source/core/Cmake
#########################

set( YAGE_IMAGE_SOURCE

	"${YAGE_MODULES_PATH}/image/bmp.h"
	"${YAGE_MODULES_PATH}/image/bmp.cpp"
	"${YAGE_MODULES_PATH}/image/png.h"
	"${YAGE_MODULES_PATH}/image/png.cpp"
	"${YAGE_MODULES_PATH}/image/enum.h"
	"${YAGE_MODULES_PATH}/image/image.h"
	"${YAGE_MODULES_PATH}/image/image.cpp"
	"${YAGE_MODULES_PATH}/image/img.h"
	"${YAGE_MODULES_PATH}/image/img.cpp"
	"${YAGE_MODULES_PATH}/image/imageLoader.h"
	"${YAGE_MODULES_PATH}/image/imageLoader.cpp"
	
)

set( YAGE_FONT_SOURCE

	"${YAGE_MODULES_PATH}/font/fileformat.h"
	"${YAGE_MODULES_PATH}/font/font.h"
	"${YAGE_MODULES_PATH}/font/fontHeader.h"
	"${YAGE_MODULES_PATH}/font/loader.h"
	"${YAGE_MODULES_PATH}/font/loader.cpp"
	"${YAGE_MODULES_PATH}/font/text.h"
	"${YAGE_MODULES_PATH}/font/text.cpp"
	"${YAGE_MODULES_PATH}/font/Shaders.h"
	
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

	"${YAGE_MODULES_PATH}/gui/Shaders.h"

	"${YAGE_MODULES_PATH}/gui/Master.h"
	"${YAGE_MODULES_PATH}/gui/Master.cpp"
	"${YAGE_MODULES_PATH}/gui/Renderer.h"
	"${YAGE_MODULES_PATH}/gui/Renderer.cpp"
	"${YAGE_MODULES_PATH}/gui/InputManager.h"
	"${YAGE_MODULES_PATH}/gui/InputManager.cpp"
	"${YAGE_MODULES_PATH}/gui/FontManager.h"
	"${YAGE_MODULES_PATH}/gui/FontManager.cpp"
	"${YAGE_MODULES_PATH}/gui/TextureManager.h"
	"${YAGE_MODULES_PATH}/gui/TextureManager.cpp"

	"${YAGE_MODULES_PATH}/gui/widgetTemplate.h"
	"${YAGE_MODULES_PATH}/gui/rootWidget.h"
	"${YAGE_MODULES_PATH}/gui/rootWidget.cpp"

	"${YAGE_MODULES_PATH}/gui/Widget.h"
	"${YAGE_MODULES_PATH}/gui/Widget.cpp"
	"${YAGE_MODULES_PATH}/gui/widgets/button.h"
	"${YAGE_MODULES_PATH}/gui/widgets/button.cpp"
	"${YAGE_MODULES_PATH}/gui/widgets/entry.h"
	"${YAGE_MODULES_PATH}/gui/widgets/entry.cpp"
	"${YAGE_MODULES_PATH}/gui/widgets/frame.h"
	"${YAGE_MODULES_PATH}/gui/widgets/frame.cpp"
	"${YAGE_MODULES_PATH}/gui/widgets/label.h"
	"${YAGE_MODULES_PATH}/gui/widgets/label.cpp"

	"${YAGE_MODULES_PATH}/gui/interface.h"
	"${YAGE_MODULES_PATH}/gui/Animation.h"
	"${YAGE_MODULES_PATH}/gui/Animation.cpp"
	"${YAGE_MODULES_PATH}/gui/Layouts.h"
	"${YAGE_MODULES_PATH}/gui/Layouts.cpp"
	
)

set( YAGE_GRAPHICS_3D_SOURCE

	"${YAGE_MODULES_PATH}/graphics3d/camera.h"
	"${YAGE_MODULES_PATH}/graphics3d/camera.cpp"

	"${YAGE_MODULES_PATH}/graphics3d/skybox.h"
	"${YAGE_MODULES_PATH}/graphics3d/skybox.cpp"

	"${YAGE_MODULES_PATH}/graphics3d/scene.h"
	"${YAGE_MODULES_PATH}/graphics3d/sceneRenderer.h"
	"${YAGE_MODULES_PATH}/graphics3d/sceneRenderer.cpp"

)