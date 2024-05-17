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
    gui::Master master;

    gui::Label* fpsCounter;

    int clicks = 0;
    gui::Widget* frame_clicks;
    gui::PushButton* button_clicks;
    gui::Label* label_clicks;

    gui::Widget* frame_check;
    gui::CheckButton* button_check;
    gui::Label* label_check;

    gui::Widget* frame_radio;
    gui::RadioGroup<int> radioGroup;
    gui::Label* label_radio;

    gui::Label* testLabel;
    gui::Animation* animation_1 = nullptr;
    gui::Animation* animation_2 = nullptr;
    gui::Animation* animation_3 = nullptr;
    gui::Animation* animation_4 = nullptr;

    GuiTest(const std::shared_ptr<platform::IWindow>& platform, const std::shared_ptr<gl::IContext>& gl,
            res::Resource<font::Font> font)
            : platform(platform), gl(gl), master(gui::Master(platform, gl))
    {

        gui::LabelTemplate fpslabelTemplate{
                .text = gui::TextTemplate{
                        .text = U" ",
                        .font = font,
                        .size = 14,
                },
        };
        fpsCounter = master.create_widget<gui::Label>(fpslabelTemplate);

        // #############################################################################################################
        // Designs
        // #############################################################################################################

        auto v_list_1 = master.create_widget<gui::ListBox>(gui::ListBoxTemplate{
                .base = {
                        .geometry = {
                                .anchor = {
                                        .position = gui::AnchorPosition::TOP_LEFT,
                                        .offset = gml::Vec2f(10, 40),
                                },
                                .size_hint = gml::Vec2<gui::SizeHint>(gui::SizeHint::FIT_CHILDREN),
                        },
                        .border = {.thickness = 1},
                },
                .orientation = gui::ListBoxTemplate::VERTICAL,
        });
        v_list_1->create_widget<gui::Label>(gui::LabelTemplate{
                .base = {.color = gl::Color::LIGHT_BLUE,
                        .padding = {5, 5, 5, 5}},
                .text = {
                        .text = U"Widget design options",
                        .font = font,
                        .size = 20,
                },
        });

        auto* designFrame = v_list_1->create_widget<gui::ListBox>(gui::ListBoxTemplate{
                .base = {
                        .geometry = {
                                .size_hint = gml::Vec2<gui::SizeHint>(gui::SizeHint::FIT_CHILDREN),
                        },
                        .padding = {5, 5, 5, 5},
                },
                .orientation = gui::ListBoxTemplate::HORIZONTAL,
                .spacing = 10.0f,
        });
        designFrame->create_widget<gui::Label>(gui::LabelTemplate{
                .base = {.padding = {2, 2, 2, 2}},
                .text = {
                        .text = U"flat",
                        .font = font,
                },
        });
        designFrame->create_widget<gui::Label>(gui::LabelTemplate{
                .base = {
                        .border = {.thickness = 1},
                        .padding = {2, 2, 2, 2}
                },
                .text = {
                        .text = U"border",
                        .font = font,
                },
        });
        designFrame->create_widget<gui::Label>(gui::LabelTemplate{
                .base = {
                        .shadow = {.offset = 3.0f, .hardness = 0.4f},
                        .color = gl::Color::WHITE,
                        .padding = {2, 2, 2, 2}
                },
                .text = {
                        .text = U"shadow",
                        .font = font,
                },
        });
        designFrame->create_widget<gui::Label>(gui::LabelTemplate{
                .base = {
                        .texture = {
                                .filename = "assets/texture.png"
                        },
                        .padding = {10, 4, 10, 4}
                },
                .text = {
                        .text = U"texture",
                        .font = font,
                        .color = gl::Color::WHITE,
                },
        });


        // #############################################################################################################
        // Layouts
        // #############################################################################################################

        auto* v_list_2 = master.create_widget<gui::ListBox>(gui::ListBoxTemplate{
                .base = {
                        .geometry = {
                                .anchor = {
                                        .position = gui::AnchorPosition::TOP_LEFT,
                                        .offset = gml::Vec2f(10, 40 + v_list_1->actual_size().y() + 50),
                                },
                                .size_hint = {gui::SizeHint::FIT_CHILDREN, gui::SizeHint::FIT_CHILDREN},
                        },
                        .border = {.thickness = 1},
                },
                .orientation = gui::ListBoxTemplate::VERTICAL,
        });
        v_list_2->create_widget<gui::Label>(gui::LabelTemplate{
                .base = {.color = gl::Color::LIGHT_BLUE, .padding = {2, 2, 2, 2}},
                .text = {
                        .text = U"Widget layout options",
                        .font = font,
                        .size = 20,
                },
        });
        v_list_2->create_widget<gui::Label>(gui::LabelTemplate{
                .base = {.padding = {2, 2, 2, 2}},
                .text = {
                        .text = U"Vertical list:",
                        .font = font,
                },
        });

        auto vListFrame = v_list_2->create_widget<gui::ListBox>(gui::ListBoxTemplate{
                .base = {
                        .geometry = {.size_hint = {gui::SizeHint::FIT_CHILDREN, gui::SizeHint::FIT_CHILDREN}},
                        .padding = {6, 2, 6, 2},
                },
                .orientation = gui::ListBoxTemplate::VERTICAL,
                .spacing = 2,
        });
        auto element_template = gui::LabelTemplate{
                .base = {
                        .border = {.thickness = 1},
                        .padding = {2, 2, 2, 2}
                },
                .text = {
                        .text = U"element",
                        .font = font
                }
        };
        vListFrame->create_widget<gui::Label>(element_template);
        vListFrame->create_widget<gui::Label>(element_template);
        vListFrame->create_widget<gui::Label>(element_template);

        v_list_2->create_widget<gui::Label>(gui::LabelTemplate{
                .base = {.padding = {2, 2, 2, 2}},
                .text = {
                        .text = U"Horizontal list:",
                        .font = font,
                }
        });

        auto hListFrame = v_list_2->create_widget<gui::ListBox>(gui::ListBoxTemplate{
                .base = {
                        .geometry = {
                                .size_hint = gml::Vec2<gui::SizeHint>(gui::SizeHint::FIT_CHILDREN),
                        },
                        .padding = {6, 2, 6, 2},
                },
                .orientation = gui::ListBoxTemplate::HORIZONTAL,
                .spacing = 2.0f,
        });
        hListFrame->create_widget<gui::Label>(element_template);
        hListFrame->create_widget<gui::Label>(element_template);
        hListFrame->create_widget<gui::Label>(element_template);

        v_list_2->create_widget<gui::Label>(gui::LabelTemplate{
                .base = {.padding = {2, 2, 2, 2}},
                .text = {
                        .text = U"Grid:",
                        .font = font
                }
        });
        v_list_2->create_widget<gui::Label>(gui::LabelTemplate{
                .base = {.border = {.thickness = 1}, .padding = {2, 2, 2, 2}},
                .text = {
                        .text = U"TODO",
                        .font = font
                }
        });


        // #############################################################################################################
        // Animations
        // #############################################################################################################

        auto h_list = master.create_widget<gui::ListBox>(gui::ListBoxTemplate{
                .base = {
                        .geometry = {
                                .anchor = {
                                        .position = gui::AnchorPosition::TOP_LEFT,
                                        .offset = gml::Vec2<float>(10 + v_list_1->actual_size().x() + 50, 40),
                                },
                        },
                        .border = {
                                .thickness = 1,
                        },
                        .color = gl::Color::TRANSPARENT,
                },
                .orientation = gui::ListBoxTemplate::HORIZONTAL,
        });

        testLabel = h_list->create_widget<gui::Label>(gui::LabelTemplate{
                .base = {
                        .geometry = {
                                .preferred_size = {
                                        .value = {80, 25},
                                },
                                .size_hint = {gui::SizeHint::FIXED, gui::SizeHint::FIXED},
                        },
                        .border = {
                                .thickness = 1,
                                .color = gl::Color::BROWN,
                        },
                        .color = gl::Color::LIGHT_BLUE,
                        .padding = {2, 2, 2, 2},
                },
                .text = {
                        .text = U"nested",
                        .font = font
                }
        });
        h_list->create_widget<gui::Label>(gui::LabelTemplate{
                .base = {
                        .border = {
                                .thickness = 1,
                                .color = gl::Color::GREEN,
                        },
                        .color = gl::Color::ORANGE,
                        .padding = {2, 2, 2, 2},
                },
                .text = {
                        .text = U"animations",
                        .font = font
                }
        });


        animation_1 = h_list->create_animation<gui::MoveAnimation>(h_list->offset(),
                                                                   h_list->offset() + gml::Vec2<float>(200, 0),
                                                                   2);
        animation_1->setOnAnimationStop([this] { onAnimation1stop(); });
        animation_2 = h_list->create_animation<gui::MoveAnimation>(h_list->offset() + gml::Vec2<float>(200, 0),
                                                                   h_list->offset(), 2);
        animation_2->setOnAnimationStop([this] { onAnimation2stop(); });

        animation_3 = testLabel->create_animation<gui::SizeAnimation>(testLabel->actual_size(),
                                                                      testLabel->actual_size() +
                                                                      gml::Vec2<float>(50, 25),
                                                                      2);
        animation_3->setOnAnimationStop([this] { onAnimation3stop(); });
        animation_4 = testLabel->create_animation<gui::SizeAnimation>(testLabel->actual_size() +
                                                                      gml::Vec2<float>(50, 25),
                                                                      testLabel->actual_size(), 2);
        animation_4->setOnAnimationStop([this] { onAnimation4stop(); });


        // #############################################################################################################
        // Buttons
        // #############################################################################################################

        auto frame_4 = master.create_widget<gui::ListBox>(gui::ListBoxTemplate{
                .base = {
                        .geometry = {
                                .anchor = {
                                        .position = gui::AnchorPosition::TOP_LEFT,
                                        .offset = gml::Vec2<float>(10 + v_list_2->actual_size().x() + 50,
                                                                   40 + v_list_1->actual_size().y() + 50),
                                },
                                .size_hint = gml::Vec2<gui::SizeHint>(gui::SizeHint::FIT_CHILDREN),
                        },
                        .border = {.thickness = 1},
                },
                .orientation = gui::ListBoxTemplate::VERTICAL,
        });
        frame_4->create_widget<gui::Label>(gui::LabelTemplate{
                .base = {.color = gl::Color::LIGHT_BLUE, .padding = {2, 2, 2, 2}},
                .text = {
                        .text = U"Buttons:",
                        .font = font,
                        .size = 20,
                }
        });

        auto h_list_buttons = frame_4->create_widget<gui::ListBox>(gui::ListBoxTemplate{
                .base = {
                        .geometry = {
                                .size_hint = gml::Vec2<gui::SizeHint>(gui::SizeHint::FIT_CHILDREN),
                        },
                        .padding = {5, 5, 5, 5},
                },
                .orientation = gui::ListBoxTemplate::VERTICAL,
                .spacing = 20,
        });

        frame_clicks = h_list_buttons->create_widget<gui::ListBox>(gui::ListBoxTemplate{
                .base = {
                        .geometry = {
                                .size_hint = gml::Vec2<gui::SizeHint>(gui::SizeHint::FIT_CHILDREN),
                        },
                },
                .orientation = gui::ListBoxTemplate::HORIZONTAL,
        });
        frame_clicks->create_widget<gui::Label>(gui::LabelTemplate{
                .base = {.padding = {2, 2, 2, 2}},
                .text = {
                        .text = U"Push button: ",
                        .font = font
                }
        });
        button_clicks = frame_clicks->create_widget<gui::PushButton>(gui::PushButtonTemplate{
                .base = {
                        .border = {.thickness = 1},
                        .color = gl::Color::LIGHT_BLUE,
                },
                .click = {
                        .border = {.thickness = 2},
                        .color = gl::Color::BLUE,
                },
                .hover = {
                        .border = {.thickness = 2},
                        .color = gl::Color::LIGHT_BLUE,
                },
                .command = [this] { on_button_1_click(); },
        });
        label_clicks = button_clicks->create_widget<gui::Label>(gui::LabelTemplate{
                .base = {.padding = {2, 2, 2, 2}},
                .text = {
                        .text = U"clicks: 0",
                        .font = font,
                },
        });


        frame_check = h_list_buttons->create_widget<gui::ListBox>(gui::ListBoxTemplate{
                .base = {
                        .geometry = {
                                .size_hint = gml::Vec2<gui::SizeHint>(gui::SizeHint::FIT_CHILDREN),
                        },
                        .color = gl::Color::TRANSPARENT
                },
                .orientation = gui::ListBoxTemplate::HORIZONTAL,
        });
        frame_check->create_widget<gui::Label>(gui::LabelTemplate{
                .base = {.padding = {2, 2, 2, 2}},
                .text = {
                        .text = U"Check button: ",
                        .font = font
                }
        });
        button_check = frame_check->create_widget<gui::CheckButton>(gui::CheckButtonTemplate{
                .base = {
                        .border = {.thickness = 1},
                        .color = gl::Color::LIGHT_GREY,
                },
                .selected = {
                        .border = {.thickness = 1},
                        .color = gl::Color::LIGHT_BLUE,
                },
                .command = [this](bool state) -> void { this->on_button_2_click(state); },
        });
        label_check = button_check->create_widget<gui::Label>(gui::LabelTemplate{
                .base = {
                        .color = gl::Color::TRANSPARENT,
                        .padding = {2, 2, 2, 2},
                },
                .text = {
                        .text = U"state: off",
                        .font = font,
                }
        });

        frame_radio = h_list_buttons->create_widget<gui::ListBox>(gui::ListBoxTemplate{
                .base = {
                        .geometry = {
                                .size_hint = gml::Vec2<gui::SizeHint>(gui::SizeHint::FIT_CHILDREN),
                        },
                        .color = gl::Color::TRANSPARENT
                },
                .orientation = gui::ListBoxTemplate::VERTICAL
        });
        frame_radio->create_widget<gui::Label>(gui::LabelTemplate{
                .base = {
                        .padding = {2, 2, 2, 2},
                },
                .text = {
                        .text = U"Radio Buttons:",
                        .font = font
                }
        });
        auto frame_radio_2 = frame_radio->create_widget<gui::ListBox>(gui::ListBoxTemplate{
                .base = {
                        .geometry = {
                                .size_hint = gml::Vec2<gui::SizeHint>(gui::SizeHint::FIT_CHILDREN),
                        },
                        .color = gl::Color::TRANSPARENT
                },
                .orientation = gui::ListBoxTemplate::HORIZONTAL,
        });
        auto radio_group_box = frame_radio_2->create_widget<gui::ListBox>(gui::ListBoxTemplate{
                .base = {
                        .geometry = {
                                .size_hint = gml::Vec2<gui::SizeHint>(gui::SizeHint::FIT_CHILDREN),
                        },
                        .color = gl::Color::TRANSPARENT
                },
                .orientation = gui::ListBoxTemplate::VERTICAL,
        });
        radioGroup = gui::RadioGroup<int>(0, [this](int value) {
            this->on_radio_click(value);
        });
        auto radio_button_1 = radioGroup.addButton(
                radio_group_box->create_widget<gui::CheckButton>(gui::CheckButtonTemplate{
                        .base = {
                                .border = {.thickness = 1},
                                .color = gl::Color::LIGHT_GREY,
                        },
                        .selected = {
                                .border = {.thickness = 1},
                                .color = gl::Color::LIGHT_BLUE,
                        },
                }), 1, true);
        radio_button_1->create_widget<gui::Label>(gui::LabelTemplate{
                .base = {
                        .color = gl::Color::TRANSPARENT,
                        .padding = {2, 2, 2, 2},
                },
                .text = {
                        .text = U"Value 1",
                        .font = font,
                }
        });
        auto radio_button_2 = radioGroup.addButton(
                radio_group_box->create_widget<gui::CheckButton>(gui::CheckButtonTemplate{
                        .base = {
                                .border = {.thickness = 1},
                                .color = gl::Color::LIGHT_GREY,
                        },
                        .selected = {
                                .border = {.thickness = 1},
                                .color = gl::Color::LIGHT_BLUE,
                        },
                }), 2);
        radio_button_2->create_widget<gui::Label>(gui::LabelTemplate{
                .base = {
                        .color = gl::Color::TRANSPARENT,
                        .padding = {2, 2, 2, 2},
                },
                .text = {
                        .text = U"Value 2",
                        .font = font,
                }
        });
        auto radio_button_3 = radioGroup.addButton(
                radio_group_box->create_widget<gui::CheckButton>(gui::CheckButtonTemplate{
                        .base = {
                                .border = {.thickness = 1},
                                .color = gl::Color::LIGHT_GREY,
                        },
                        .selected = {
                                .border = {.thickness = 1},
                                .color = gl::Color::LIGHT_BLUE,
                        },
                }), 3);
        radio_button_3->create_widget<gui::Label>(gui::LabelTemplate{
                .base = {
                        .color = gl::Color::TRANSPARENT,
                        .padding = {2, 2, 2, 2},
                },
                .text = {
                        .text = U"Value 3",
                        .font = font,
                }
        });
        label_radio = frame_radio_2->create_widget<gui::Label>(gui::LabelTemplate{
                .base = {
                        .padding = {2, 2, 2, 2},
                },
                .text = {
                        .text = U"selected: 1",
                        .font = font
                }
        });

        // #############################################################################################################
        // Entries
        // #############################################################################################################

        auto entryFrame = master.create_widget<gui::ListBox>(gui::ListBoxTemplate{
                .base = {
                        .geometry = {.anchor = {.offset = gml::Vec2<float>(
                                10 + v_list_2->actual_size().x() + 50 + frame_4->actual_size().x() + 50,
                                40 + v_list_1->actual_size().y() + 50),}},
                        .border = {.thickness = 1}
                },
                .orientation = gui::ListBoxTemplate::VERTICAL,
        });
        entryFrame->create_widget<gui::Label>(gui::LabelTemplate{
                .base = {
                        .color = gl::Color::LIGHT_BLUE,
                        .padding = {2, 2, 2, 2},
                },
                .text = {
                        .text = U"Text input",
                        .font = font,
                },
        });
        entryFrame->create_widget<gui::TextEntry>(gui::TextEntryTemplate{
                .base = {
                        .geometry = {
                                .preferred_size = {.value = {100.0f, 20.0f}},
                                .size_hint = {gui::SizeHint::FIXED, gui::SizeHint::FIXED},
                        },
                        .border = {.thickness = 1},
                        .padding = {2, 2, 2, 2},
                },
                .placeholder_text = {
                        .text = U"input text...",
                        .font = font,
                        .color = gl::Color::GREY,
                },
                .input_text = {
                        .text = U"",
                        .font = font,
                },
        });
    }

    void on_button_1_click()
    {
        clicks++;
        label_clicks->set_text(U"clicks: " + utils::toUTF32(clicks));
    }

    void on_button_2_click(bool state) const
    {
        if (state) {
            label_check->set_text(U"state: on");
        } else {
            label_check->set_text(U"state: off");
        }
    }

    void on_radio_click(int value) const
    {
        label_radio->set_text(U"selected: " + utils::toUTF32(value));
    }

    void onAnimation1stop() const
    {
        animation_2->start();
    }

    void onAnimation2stop() const
    {
        animation_1->start();
    }

    void onAnimation3stop() const
    {
        animation_4->start();
    }

    void onAnimation4stop() const
    {
        animation_3->start();
    }
};

void displayFPS(GuiTest& gui, double dt)
{
    double frameRate = 1.0 / dt;
    gui.fpsCounter->set_text(utils::toUTF32((int) frameRate));
}

int main()
{
    std::shared_ptr<platform::desktop::GlfwWindow> window = std::make_shared<platform::desktop::GlfwWindow>(800, 400,
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
    double dt = 1.0 / 60.0;
    double frame_time;
    while (!window->shouldDestroy()) {
        frame_time = window->getTimeStep();
        timer += frame_time;

        if (timer > 1) {
            displayFPS(guiTest, frame_time);
            timer = 0.0;
        }

        while (frame_time > 0) {
            double delta_time = std::min(frame_time, dt);
            guiTest.master.update(delta_time);
            frame_time -= delta_time;
        }

        guiTest.master.render();

        window->swapBuffers();
        window->pollEvents();
    }

    return 0;
}
