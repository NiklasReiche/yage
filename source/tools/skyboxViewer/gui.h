#include <gui/gui.h>
#include <image/imageReader.h>

class Gui
{
private:
	sys::PlatformHandle* platform;
	gl::GraphicsContext* gl;
	gui::Master* master;
	img::ImageReader reader;

	gui::PushButton* button_top;
	gui::PushButton* button_left;
	gui::PushButton* button_front;
	gui::PushButton* button_right;
	gui::PushButton* button_back;
	gui::PushButton* button_bottom;

	gui::Label* tooltipLabel;

	std::string path_right;
	std::string path_left;
	std::string path_top;
	std::string path_bottom;
	std::string path_back;
	std::string path_front;

#ifdef _WIN32
	std::string installPath = "D:/dev/projects/yage/source/tools/skyboxViewer";
#endif
#ifdef __unix
	std::string installPath = "/home/niklas/dev/yage/source/tools/skyboxViewer";
#endif
	std::string lastPath = installPath + "/assets/skybox";
	int thumbnailSize = 0;

	std::function<void(std::array<std::string, 6>)> loadCallback;

	void setTexture(gui::PushButton* button, std::string filename);
	void on_button_load_click();
	void on_button_top_click();
	void on_button_left_click();
	void on_button_front_click();
	void on_button_right_click();
	void on_button_back_click();
	void on_button_bottom_click();

public:
	Gui(sys::PlatformHandle* platform, gl::GraphicsContext* gl, input::InputController* input);

	void update(double dt)
	{
		master->update(dt);
	}

	void showTooltip() { tooltipLabel->show(); }

	void setLoadCallback(std::function<void(std::array<std::string, 6>)> callback) { loadCallback = callback; }
};