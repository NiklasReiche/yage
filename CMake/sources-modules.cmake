#########################
# source/core/Cmake
#########################

set( YAGE_IMAGE_SOURCE

	"${YAGE_MODULES_PATH}/image/image.h"
	"${YAGE_MODULES_PATH}/image/image.cpp"
	"${YAGE_MODULES_PATH}/image/enum.h"
	"${YAGE_MODULES_PATH}/image/imageReader.h"
	"${YAGE_MODULES_PATH}/image/imageReader.cpp"
	"${YAGE_MODULES_PATH}/image/imageWriter.h"
	"${YAGE_MODULES_PATH}/image/imageWriter.cpp"

	"${YAGE_MODULES_PATH}/image/formats/bmp.h"
	"${YAGE_MODULES_PATH}/image/formats/bmp.cpp"
	"${YAGE_MODULES_PATH}/image/formats/png.h"
	"${YAGE_MODULES_PATH}/image/formats/png.cpp"
	
)

set( YAGE_FONT_SOURCE

	"${YAGE_MODULES_PATH}/font/fileformat.h"
	"${YAGE_MODULES_PATH}/font/font.h"
	"${YAGE_MODULES_PATH}/font/fontHeader.h"
	"${YAGE_MODULES_PATH}/font/loader.h"
	"${YAGE_MODULES_PATH}/font/loader.cpp"
	"${YAGE_MODULES_PATH}/font/text.h"
	"${YAGE_MODULES_PATH}/font/text.cpp"
	"${YAGE_MODULES_PATH}/font/shaders.h"
	
)
set( YAGE_FONT_CONVERTER_SOURCE

	"${YAGE_MODULES_PATH}/font/converter/converter.h"
	"${YAGE_MODULES_PATH}/font/converter/converter.cpp"
	"${YAGE_MODULES_PATH}/font/converter/ftMath.h"
	"${YAGE_MODULES_PATH}/font/converter/ftMath.cpp"
	"${YAGE_MODULES_PATH}/font/converter/img.h"
	"${YAGE_MODULES_PATH}/font/converter/img.cpp"
	
)

set( YAGE_GUI_SOURCE

	"${YAGE_MODULES_PATH}/gui/gui.h"
	"${YAGE_MODULES_PATH}/gui/core.h"

	"${YAGE_MODULES_PATH}/gui/shaders.h"

	"${YAGE_MODULES_PATH}/gui/master.h"
	"${YAGE_MODULES_PATH}/gui/master.cpp"
	"${YAGE_MODULES_PATH}/gui/renderer.h"
	"${YAGE_MODULES_PATH}/gui/renderer.cpp"
	"${YAGE_MODULES_PATH}/gui/inputmanager.h"
	"${YAGE_MODULES_PATH}/gui/inputmanager.cpp"
	"${YAGE_MODULES_PATH}/gui/fontmanager.h"
	"${YAGE_MODULES_PATH}/gui/fontmanager.cpp"
	"${YAGE_MODULES_PATH}/gui/texturemanager.h"
	"${YAGE_MODULES_PATH}/gui/texturemanager.cpp"

	"${YAGE_MODULES_PATH}/gui/widgettemplate.h"
	"${YAGE_MODULES_PATH}/gui/root.h"
	"${YAGE_MODULES_PATH}/gui/root.cpp"

	"${YAGE_MODULES_PATH}/gui/widget.h"
	"${YAGE_MODULES_PATH}/gui/widget.cpp"
	"${YAGE_MODULES_PATH}/gui/widgets/button.h"
	"${YAGE_MODULES_PATH}/gui/widgets/button.cpp"
	"${YAGE_MODULES_PATH}/gui/widgets/entry.h"
	"${YAGE_MODULES_PATH}/gui/widgets/entry.cpp"
	"${YAGE_MODULES_PATH}/gui/widgets/frame.h"
	"${YAGE_MODULES_PATH}/gui/widgets/frame.cpp"
	"${YAGE_MODULES_PATH}/gui/widgets/label.h"
	"${YAGE_MODULES_PATH}/gui/widgets/label.cpp"

	"${YAGE_MODULES_PATH}/gui/interface.h"
	"${YAGE_MODULES_PATH}/gui/animation.h"
	"${YAGE_MODULES_PATH}/gui/animation.cpp"
	"${YAGE_MODULES_PATH}/gui/layouts.h"
	"${YAGE_MODULES_PATH}/gui/layouts.cpp"
	
)

set( YAGE_GRAPHICS_3D_SOURCE

	"${YAGE_MODULES_PATH}/graphics3d/camera.h"
	"${YAGE_MODULES_PATH}/graphics3d/camera.cpp"

	"${YAGE_MODULES_PATH}/graphics3d/skybox.h"
	"${YAGE_MODULES_PATH}/graphics3d/skybox.cpp"

	"${YAGE_MODULES_PATH}/graphics3d/light/light.h"
	"${YAGE_MODULES_PATH}/graphics3d/light/light.cpp"
	"${YAGE_MODULES_PATH}/graphics3d/light/pointLight.h"
	"${YAGE_MODULES_PATH}/graphics3d/light/pointLight.cpp"
	"${YAGE_MODULES_PATH}/graphics3d/light/dirLight.h"
	"${YAGE_MODULES_PATH}/graphics3d/light/dirLight.cpp"

	"${YAGE_MODULES_PATH}/graphics3d/material.h"
	"${YAGE_MODULES_PATH}/graphics3d/material.cpp"
	"${YAGE_MODULES_PATH}/graphics3d/mesh.h"
	"${YAGE_MODULES_PATH}/graphics3d/mesh.cpp"

	"${YAGE_MODULES_PATH}/graphics3d/sceneGraph/sceneNode.h"
	"${YAGE_MODULES_PATH}/graphics3d/sceneGraph/sceneNode.cpp"
	"${YAGE_MODULES_PATH}/graphics3d/sceneGraph/sceneGroup.h"
	"${YAGE_MODULES_PATH}/graphics3d/sceneGraph/sceneGroup.cpp"
	"${YAGE_MODULES_PATH}/graphics3d/sceneGraph/sceneObject.h"
	"${YAGE_MODULES_PATH}/graphics3d/sceneGraph/sceneObject.cpp"

	"${YAGE_MODULES_PATH}/graphics3d/sceneRenderer.h"
	"${YAGE_MODULES_PATH}/graphics3d/sceneRenderer.cpp"

	"${YAGE_MODULES_PATH}/graphics3d/resourceCache.h"
	"${YAGE_MODULES_PATH}/graphics3d/resourceManager.h"

)

set( YAGE_GRAPHICS_2D_SOURCE 

	"${YAGE_MODULES_PATH}/graphics2d/sprite.h"
	"${YAGE_MODULES_PATH}/graphics2d/sprite.cpp"
	"${YAGE_MODULES_PATH}/graphics2d/spriteAnimation.h"
	"${YAGE_MODULES_PATH}/graphics2d/spriteAnimation.cpp"
	"${YAGE_MODULES_PATH}/graphics2d/renderer.h"
	"${YAGE_MODULES_PATH}/graphics2d/renderer.cpp"

)

set( YAGE_ALGO_SOURCE 

	"${YAGE_MODULES_PATH}/algo/tree/treeNode.h"
	"${YAGE_MODULES_PATH}/algo/tree/treeNode.tpp"

	"${YAGE_MODULES_PATH}/algo/sceneGraph.h"
	"${YAGE_MODULES_PATH}/algo/sceneGraph.cpp"

)