#include <core/gl/graphics.h>
#include <core/platform/Window.h>
#include <utils/strings.h>

#include <gui/gui.h>
#include "core/platform/desktop/GlfwWindow.h"

class GuiTest
{
private:
	std::shared_ptr<platform::IWindow> platform;
	std::shared_ptr<gl::IContext> gl;

public:
	gui::Master* master;

	gui::Label* fpsCounter;

	int clicks = 0;
	gui::Frame* frame_clicks;
	gui::PushButton* button_clicks;
	gui::Label* label_clicks;

	gui::Frame* frame_check;
	gui::CheckButton* button_check;
	gui::Label* label_check;

	gui::Frame* frame_radio;
	gui::RadioGroup<int>* radioGroup;
	gui::Label* label_radio;

	gui::Label* testLabel;
	gui::Animation* animation_1 = nullptr;
	gui::Animation* animation_2 = nullptr;
	gui::Animation* animation_3 = nullptr;
	gui::Animation* animation_4 = nullptr;

	GuiTest(const std::shared_ptr<platform::IWindow>& platform, const std::shared_ptr<gl::IContext>& gl, res::Resource<font::Font> font)
		: platform(platform), gl(gl)
	{
		master = new gui::Master(platform, gl);

		gui::LabelTemplate fpslabelTemplate{
            .text = gui::TextTemplate{
                    .text = U" ",
                    .font = font,
                    .size = 14,
            },
        };

		fpsCounter = master->createWidget<gui::Label>(nullptr, fpslabelTemplate);

		/*
		*
		* Design Frame
		*
		*/

		gui::FrameTemplate frameTemplate;
		frameTemplate.geometry.anchor = gui::Anchor::TOP_LEFT;
		frameTemplate.geometry.offset = gml::Vec2<float>(10, 40);
		frameTemplate.border.size = 1;
		frameTemplate.color = gl::Color::TRANSPARENT;
		frameTemplate.layoutType = gui::LayoutType::V_LIST_LAYOUT;
		frameTemplate.layoutMargin = gml::Vec2f(5.0f);

		gui::Frame* frame_1 = master->createWidget<gui::Frame>(nullptr, frameTemplate);


		gui::LabelTemplate labelTemplate{
            .text = {
                    .text = U"Widget design options",
                    .font = font,
                    .size = 20,
            },
        };

		master->createWidget<gui::Label>(frame_1, labelTemplate);


		gui::FrameTemplate designsFrameTemplate;
		designsFrameTemplate.geometry.offset.y() = 5.0f;
		designsFrameTemplate.color = gl::Color::TRANSPARENT;
		designsFrameTemplate.layoutType = gui::LayoutType::H_LIST_LAYOUT;

		gui::Frame* designFrame = master->createWidget<gui::Frame>(frame_1, designsFrameTemplate);


		gui::LabelTemplate flatLabelTemplate{
                .text = {
                        .text = U"flat",
                        .font = font,
                },
        };
		flatLabelTemplate.geometry.offset.y() = 1.0f;

		master->createWidget<gui::Label>(designFrame, flatLabelTemplate);

		gui::LabelTemplate borderLabelTemplate{
                .text = {
                        .text = U"border",
                        .font = font,
                },
        };
		borderLabelTemplate.geometry.offset.x() = 5.0f;
		borderLabelTemplate.border.size = 1;

		master->createWidget<gui::Label>(designFrame, borderLabelTemplate);

		gui::LabelTemplate shadowLabelTemplate{
                .text = {
                        .text = U"shadow",
                        .font = font,
                },
        };
		shadowLabelTemplate.geometry.offset.x() = 5.0f;
		shadowLabelTemplate.border.size = 1;
		shadowLabelTemplate.shadow.offset = 3;
		shadowLabelTemplate.shadow.hardness = 0.4f;

		master->createWidget<gui::Label>(designFrame, shadowLabelTemplate);



		/*
		*
		* Layout Frame
		*
		*/

		gui::FrameTemplate frameTemplate2;
		frameTemplate2.geometry.anchor = gui::Anchor::TOP_LEFT;
		frameTemplate2.geometry.offset = gml::Vec2<float>(10, 40 + frame_1->getSize().y() + 50);
		frameTemplate2.border.size = 1;
		frameTemplate2.color = gl::Color::TRANSPARENT;
		frameTemplate2.layoutType = gui::LayoutType::V_LIST_LAYOUT;
		frameTemplate2.layoutMargin = gml::Vec2f(5.0f);

		gui::Frame* frame_2 = master->createWidget<gui::Frame>(nullptr, frameTemplate2);


		gui::LabelTemplate labelTemplate2{
            .text = {
                    .text = U"Widget layout options",
                    .font = font,
                    .size = 20,
            },
        };

		master->createWidget<gui::Label>(frame_2, labelTemplate2);


		gui::LabelTemplate vListLabelTemplate{
                .text = {
                        .text = U"- vertical list",
                        .font = font,
                },
        };
		vListLabelTemplate.geometry.offset.x() = 5.0f;

		master->createWidget<gui::Label>(frame_2, vListLabelTemplate);


		gui::FrameTemplate frameTemplateVList;
		frameTemplateVList.geometry.offset = gml::Vec2<float>(20, 0);
		frameTemplateVList.color = gl::Color::TRANSPARENT;
		frameTemplateVList.layoutType = gui::LayoutType::V_LIST_LAYOUT;
		frameTemplateVList.layoutMargin = gml::Vec2f(5.0f);

		gui::Frame* vListFrame = master->createWidget<gui::Frame>(frame_2, frameTemplateVList);

		gui::LabelTemplate vListElementTemplate{
            .text = {
                    .text = U"element",
                    .font = font
            }
        };
		vListElementTemplate.border.size = 1;

		master->createWidget<gui::Label>(vListFrame, vListElementTemplate);
		master->createWidget<gui::Label>(vListFrame, vListElementTemplate);
		master->createWidget<gui::Label>(vListFrame, vListElementTemplate);



		gui::LabelTemplate hListLabelTemplate{
            .text = {
                    .text = U"- horizontal list",
                    .font = font,
            }
        };
		hListLabelTemplate.geometry.offset.x() = 5.0f;

		master->createWidget<gui::Label>(frame_2, hListLabelTemplate);


		gui::FrameTemplate frameTemplateHList;
		frameTemplateHList.geometry.offset = gml::Vec2<float>(20, 0);
		frameTemplateHList.color = gl::Color::TRANSPARENT;
		frameTemplateHList.layoutType = gui::LayoutType::H_LIST_LAYOUT;
		frameTemplateHList.layoutMargin = gml::Vec2f(5.0f);

		gui::Frame* hListFrame = master->createWidget<gui::Frame>(frame_2, frameTemplateHList);

		gui::LabelTemplate hListElementTemplate{
            .text = {
                    .text = U"element",
                    .font = font
            }
        };
		hListElementTemplate.border.size = 1;

		master->createWidget<gui::Label>(hListFrame, hListElementTemplate);
		master->createWidget<gui::Label>(hListFrame, hListElementTemplate);
		master->createWidget<gui::Label>(hListFrame, hListElementTemplate);


		gui::LabelTemplate gridLabelTemplate{
            .text = {
                    .text = U"- grid",
                    .font = font
            }
        };
		gridLabelTemplate.geometry.offset.x() = 5.0f;

		master->createWidget<gui::Label>(frame_2, gridLabelTemplate);

		gui::LabelTemplate gridElementTemplate{
            .text = {
                    .text = U"ToDo",
                    .font = font
            }
        };
		gridElementTemplate.geometry.offset = gml::Vec2<float>(20, 0);
		gridElementTemplate.border.size = 1;

		master->createWidget<gui::Label>(frame_2, gridElementTemplate);


		/*
		*
		* Animation Layout
		*
		*/

		gui::FrameTemplate frameTemplate3;
		frameTemplate3.geometry.anchor = gui::Anchor::TOP_LEFT;
		frameTemplate3.geometry.offset = gml::Vec2<float>(10 + frame_1->getSize().x() + 50, 40);
		frameTemplate3.border.size = 1;
		frameTemplate3.color = gl::Color::TRANSPARENT;
		frameTemplate3.layoutType = gui::LayoutType::H_LIST_LAYOUT;
		frameTemplate3.layoutMargin = gml::Vec2f(5.0f);

		gui::Frame* frame_3 = master->createWidget<gui::Frame>(nullptr, frameTemplate3);

		gui::LabelTemplate animationElementTemplate{
            .text = {
                    .text = U"yay",
                    .font = font
            }
        };
		animationElementTemplate.border.size = 1;
		animationElementTemplate.border.color = gl::Color::BROWN;
		animationElementTemplate.color = gl::Color::LIGHT_BLUE;
		testLabel = master->createWidget<gui::Label>(frame_3, animationElementTemplate);

		animationElementTemplate.geometry.offset.x() = 5.0f;
		animationElementTemplate.border.color = gl::Color::GREEN;
		animationElementTemplate.color = gl::Color::ORANGE;
		animationElementTemplate.text.text = U"animations";
		master->createWidget<gui::Label>(frame_3, animationElementTemplate);


		animation_1 = frame_3->createAnimation<gui::MoveAnimation>(master, frame_3->getPosition(), frame_3->getPosition() + gml::Vec2<float>(200, 0), 2);
		animation_1->setOnAnimationStop(std::bind(&GuiTest::onAnimation1stop, this));
		animation_2 = frame_3->createAnimation<gui::MoveAnimation>(master, frame_3->getPosition() + gml::Vec2<float>(200, 0), frame_3->getPosition(), 2);
		animation_2->setOnAnimationStop(std::bind(&GuiTest::onAnimation2stop, this));

		animation_3 = testLabel->createAnimation<gui::SizeAnimation>(master, testLabel->getSize(), testLabel->getSize() + gml::Vec2<float>(50, 25), 2);
		animation_3->setOnAnimationStop(std::bind(&GuiTest::onAnimation3stop, this));
		animation_4 = testLabel->createAnimation<gui::SizeAnimation>(master, testLabel->getSize() + gml::Vec2<float>(50, 25), testLabel->getSize(), 2);
		animation_4->setOnAnimationStop(std::bind(&GuiTest::onAnimation4stop, this));


		/*
		*
		* Button Frame
		*
		*/

		gui::FrameTemplate frameTemplate4;
		frameTemplate4.geometry.anchor = gui::Anchor::TOP_LEFT;
		frameTemplate4.geometry.offset = gml::Vec2<float>(10 + frame_2->getSize().x() + 50, 40 + frame_1->getSize().y() + 50);
		frameTemplate4.border.size = 1;
		frameTemplate4.color = gl::Color::TRANSPARENT;
		frameTemplate4.layoutType = gui::LayoutType::V_LIST_LAYOUT;
		frameTemplate4.layoutMargin = gml::Vec2f(5.0f);

		gui::Frame* frame_4 = master->createWidget<gui::Frame>(nullptr, frameTemplate4);

		gui::LabelTemplate labelTemplateButtons{
            .text = {
                    .text = U"Button showcase",
                    .font = font
            }
        };
		labelTemplateButtons.text.size = 20;

		master->createWidget<gui::Label>(frame_4, labelTemplateButtons);


		gui::FrameTemplate frameTemplateButtons;
		frameTemplateButtons.border.size = 1;
		frameTemplateButtons.color = gl::Color::TRANSPARENT;
		frameTemplateButtons.layoutType = gui::LayoutType::H_LIST_LAYOUT;
		frameTemplateButtons.layoutMargin = gml::Vec2f(5.0f);

		gui::Frame* frame_buttons = master->createWidget<gui::Frame>(frame_4, frameTemplateButtons);


		gui::FrameTemplate frameTemplateClicks;
		frameTemplateClicks.color = gl::Color::TRANSPARENT;
		frameTemplateClicks.layoutType = gui::LayoutType::V_LIST_LAYOUT;

		frame_clicks = master->createWidget<gui::Frame>(frame_buttons, frameTemplateClicks);

		gui::FrameTemplate frameTemplateCheck;
		frameTemplateCheck.geometry.offset = gml::Vec2f(25.0f, 0.0f);
		frameTemplateCheck.color = gl::Color::TRANSPARENT;
		frameTemplateCheck.layoutType = gui::LayoutType::V_LIST_LAYOUT;

		frame_check = master->createWidget<gui::Frame>(frame_buttons, frameTemplateCheck);

		gui::FrameTemplate frameTemplateRadio;
		frameTemplateRadio.geometry.offset = gml::Vec2f(25.0f, 0.0f);
		frameTemplateRadio.color = gl::Color::TRANSPARENT;
		frameTemplateRadio.layoutType = gui::LayoutType::V_LIST_LAYOUT;

		frame_radio = master->createWidget<gui::Frame>(frame_buttons, frameTemplateRadio);


		gui::LabelTemplate labelTemplateClicks{
            .text = {
                    .text = U"clicks: 0",
                    .font = font
            }
        };

		label_clicks = master->createWidget<gui::Label>(frame_clicks, labelTemplateClicks);

		gui::ButtonTemplate buttonTemplateClicks{
            .text = {
                    .text = U"Push Button",
                    .font = font,
            },
            .command = std::bind(&GuiTest::on_button_1_click, this),
        };
		buttonTemplateClicks.geometry.offset = gml::Vec2<float>(0, 5);
		buttonTemplateClicks.color = gl::Color::BLUE;
		buttonTemplateClicks.hoverColor = 0xDDDDDDFFu;
		buttonTemplateClicks.clickColor = gl::Color::BLUE;
		buttonTemplateClicks.border.size = 1;
		buttonTemplateClicks.text.alignY = gui::TextAlignmentY::CENTER;
		buttonTemplateClicks.text.alignX = gui::TextAlignmentX::CENTER;

		button_clicks = master->createWidget<gui::PushButton>(frame_clicks, buttonTemplateClicks);


		gui::LabelTemplate labelTemplateCheck{
            .text = {
                    .text = U"state: unselected",
                    .font = font
            }
        };

		label_check = master->createWidget<gui::Label>(frame_check, labelTemplateCheck);

		buttonTemplateClicks.text.text = U"Check Button";
		buttonTemplateClicks.command = std::bind(&GuiTest::on_button_2_click, this);

		button_check = master->createWidget<gui::CheckButton>(frame_check, buttonTemplateClicks);


		gui::LabelTemplate labelTemplateRadio{
            .text = {
                    .text = U"state: 1",
                    .font = font
            }
        };

		label_radio = master->createWidget<gui::Label>(frame_radio, labelTemplateRadio);

		gui::FrameTemplate radioTemplate;
		radioTemplate.geometry.offset = gml::Vec2<float>(0.0f, 5.0f);
		radioTemplate.layoutType = gui::LayoutType::V_LIST_LAYOUT;
		gui::ButtonTemplate radioButtonTemplate{
            .text = {
                    .text = U"",
                    .font = font
            },
            .command = std::bind(&GuiTest::on_radio_click, this)
        };
		radioButtonTemplate.border.size = 1;
		radioButtonTemplate.hoverColor = 0xDDDDDDFF;
		radioButtonTemplate.clickColor = gl::Color::GREY;

		radioGroup = master->createWidget<gui::RadioGroup<int>>(frame_radio, radioTemplate, radioButtonTemplate, 0);
		radioGroup->addButton(U"Radio Button 1", 1, true);
		radioGroup->addButton(U"Radio Button 2", 2);
		radioGroup->addButton(U"Radio Button 3", 3);

		gui::TextEntryTemplate textEntryTemplate{
            .defaultText = {
                    .text = U"entry",
                    .font = font
            },
            .inputText = {
                    .text = U"",
                    .font = font
            },
            .command = nullptr
        };
		textEntryTemplate.border.size = 1;
		textEntryTemplate.defaultText.color = gl::Color::GREY;

		master->createWidget<gui::TextEntry>(frame_radio, textEntryTemplate, master);

	}
	~GuiTest()
	{
		delete master;
	}

	void on_button_1_click()
	{
		clicks++;
		label_clicks->setText(U"clicks: " + utils::toUTF32(clicks));
	}

	void on_button_2_click()
	{
		if (button_check->getState()) {
			label_check->setText(U"state: selected");
		}
		else {
			label_check->setText(U"state: unselected");
		}
	}

	void on_radio_click()
	{
		label_radio->setText(U"state: " + utils::toUTF32(radioGroup->getState()));
	}

	void onAnimation1stop()
	{
		animation_2->start();
	}

	void onAnimation2stop()
	{
		animation_1->start();
	}

	void onAnimation3stop()
	{
		animation_4->start();
	}

	void onAnimation4stop()
	{
		animation_3->start();
	}
};

void displayFPS(GuiTest& gui, double dt)
{
	double frameRate = 1.0 / dt;
	gui.fpsCounter->setText(utils::toUTF32((int)frameRate));
}

int main()
{
    std::shared_ptr<platform::desktop::GlfwWindow> window = std::make_shared<platform::desktop::GlfwWindow>(1400, 900,
                                                                                                "GUI Demo");
    std::shared_ptr<gl::IContext> context = gl::createContext(window);

    res::Store<font::Font> fontStore;
    font::FontFileLoader fontLoader(context->getTextureCreator(), window->getFileReader());
    res::Resource<font::Font> font = fontStore.loadResource(fontLoader, std::string("assets/OpenSans-Regular.font"));
	
	GuiTest guiTest(window, context, font);

	guiTest.animation_1->start();
	guiTest.animation_3->start();


	window->disableVSync();
	window->show();
	window->getTimeStep();
	double timer = 0.0;
	while (!window->shouldDestroy())
	{
		double dt = window->getTimeStep();
		timer += dt;

		if (timer > 0.2) {
			displayFPS(guiTest, dt);
			timer = 0.0;
		}

		guiTest.master->update(dt);

		window->swapBuffers();
        window->pollEvents();
	}

	return 0;
}
