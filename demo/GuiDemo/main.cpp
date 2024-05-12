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
        * Design Frame -------------------------------------------------------------------------------------------------
        *
        */

        auto v_list_1 = master->createWidget<gui::VListBox>(nullptr, gui::WidgetTemplate{
                .geometry = {
                        .anchor = {
                                .offset = gml::Vec2f(10, 40),
                                .position = gui::AnchorPosition::TOP_LEFT
                        },
                        .size_hint = gml::Vec2<gui::SizeHint>(gui::SizeHint::FIT_CHILDREN),
                },
                .border = {.thickness = 1},
                .color = gl::Color::TRANSPARENT
        });
        master->createWidget<gui::Label>(v_list_1, gui::LabelTemplate{
                .base = {.color = gl::Color::LIGHT_BLUE},
                .text = {
                        .text = U"Widget design options",
                        .font = font,
                        .size = 20,
                },
        });

        auto* designFrame = master->createWidget<gui::HListBox>(v_list_1, gui::WidgetTemplate{
                .geometry = {
                        .size_hint = gml::Vec2<gui::SizeHint>(gui::SizeHint::FIT_CHILDREN),
                },
                .color = gl::Color::TRANSPARENT
        });
        master->createWidget<gui::Label>(designFrame, gui::LabelTemplate{
                .text = {
                        .text = U"flat",
                        .font = font,
                },
        });
        master->createWidget<gui::Label>(designFrame, gui::LabelTemplate{
                .base = {
                        .border = {.thickness = 1}
                },
                .text = {
                        .text = U"border",
                        .font = font,
                },
        });
        master->createWidget<gui::Label>(designFrame, gui::LabelTemplate{
                .base = {
                        .shadow = {.offset = 3, .hardness = 0.4f}
                },
                .text = {
                        .text = U"shadow",
                        .font = font,
                },
        });


        /*
        *
        * Layout Frame -------------------------------------------------------------------------------------------------
        *
        */

        auto* v_list_2 = master->createWidget<gui::VListBox>(nullptr, gui::WidgetTemplate{
                .geometry = {
                        .anchor = {
                                .offset = gml::Vec2f(10, 40 + v_list_1->actual_size().y() + 50),
                                .position = gui::AnchorPosition::TOP_LEFT,
                        },
                        .size_hint = {gui::SizeHint::FIT_CHILDREN, gui::SizeHint::FIT_CHILDREN},
                },
                .border = {.thickness = 1},
                .color = gl::Color::TRANSPARENT,
        });
        master->createWidget<gui::Label>(v_list_2, gui::LabelTemplate{
                .base = {.color = gl::Color::LIGHT_BLUE},
                .text = {
                        .text = U"Widget layout options",
                        .font = font,
                        .size = 20,
                },
        });
        master->createWidget<gui::Label>(v_list_2, gui::LabelTemplate{
                .text = {
                        .text = U"Vertical list:",
                        .font = font,
                },
        });

        auto vListFrame = master->createWidget<gui::VListBox>(v_list_2, gui::WidgetTemplate{
                .geometry = {.size_hint = {gui::SizeHint::FIT_CHILDREN, gui::SizeHint::FIT_CHILDREN}},
                .color = gl::Color::TRANSPARENT,
        });
        auto element_template = gui::LabelTemplate{
                .base = {
                        .border = {.thickness = 1}
                },
                .text = {
                        .text = U"element",
                        .font = font
                }
        };
        master->createWidget<gui::Label>(vListFrame, element_template);
        master->createWidget<gui::Label>(vListFrame, element_template);
        master->createWidget<gui::Label>(vListFrame, element_template);

        master->createWidget<gui::Label>(v_list_2, gui::LabelTemplate{
                .text = {
                        .text = U"Horizontal list:",
                        .font = font,
                }
        });

        auto hListFrame = master->createWidget<gui::HListBox>(v_list_2, gui::WidgetTemplate{
                .geometry = {
                        .size_hint = gml::Vec2<gui::SizeHint>(gui::SizeHint::FIT_CHILDREN),
                },
                .color = gl::Color::TRANSPARENT,
        });
        master->createWidget<gui::Label>(hListFrame, element_template);
        master->createWidget<gui::Label>(hListFrame, element_template);
        master->createWidget<gui::Label>(hListFrame, element_template);

        master->createWidget<gui::Label>(v_list_2, gui::LabelTemplate{
                .text = {
                        .text = U"Grid:",
                        .font = font
                }
        });
        master->createWidget<gui::Label>(v_list_2, gui::LabelTemplate{
                .base = {.border = {.thickness = 1}},
                .text = {
                        .text = U"TODO",
                        .font = font
                }
        });


        /*
        *
        * Animation Layout
        *
        */
        auto h_list = master->createWidget<gui::HListBox>(nullptr, gui::WidgetTemplate{
                .geometry = {
                        .anchor = {
                                .offset = gml::Vec2<float>(10 + v_list_1->actual_size().x() + 50, 40),
                                .position = gui::AnchorPosition::TOP_LEFT,
                        }
                },
                .border = {
                        .thickness = 1,
                },
                .color = gl::Color::TRANSPARENT,
        });

        testLabel = master->createWidget<gui::Label>(h_list, gui::LabelTemplate{
                .base = {
                        .border = {
                                .thickness = 1,
                                .color = gl::Color::BROWN,
                        },
                        .color = gl::Color::LIGHT_BLUE,
                },
                .text = {
                        .text = U"yay",
                        .font = font
                }
        });
        master->createWidget<gui::Label>(h_list, gui::LabelTemplate{
                .base = {
                        .border = {
                                .thickness = 1,
                                .color = gl::Color::GREEN,
                        },
                        .color = gl::Color::ORANGE,
                },
                .text = {
                        .text = U"animations",
                        .font = font
                }
        });


        animation_1 = h_list->create_animation<gui::MoveAnimation>(master, h_list->offset(),
                                                                   h_list->offset() + gml::Vec2<float>(200, 0),
                                                                   2);
        animation_1->setOnAnimationStop(std::bind(&GuiTest::onAnimation1stop, this));
        animation_2 = h_list->create_animation<gui::MoveAnimation>(master,
                                                                   h_list->offset() + gml::Vec2<float>(200, 0),
                                                                   h_list->offset(), 2);
        animation_2->setOnAnimationStop(std::bind(&GuiTest::onAnimation2stop, this));

        animation_3 = testLabel->create_animation<gui::SizeAnimation>(master, testLabel->actual_size(),
                                                                      testLabel->actual_size() +
                                                                      gml::Vec2<float>(50, 25),
                                                                      2);
        animation_3->setOnAnimationStop(std::bind(&GuiTest::onAnimation3stop, this));
        animation_4 = testLabel->create_animation<gui::SizeAnimation>(master,
                                                                      testLabel->actual_size() +
                                                                      gml::Vec2<float>(50, 25),
                                                                      testLabel->actual_size(), 2);
        animation_4->setOnAnimationStop(std::bind(&GuiTest::onAnimation4stop, this));


        /*
        *
        * Button Frame
        *
        */
        auto frame_4 = master->createWidget<gui::VListBox>(nullptr, gui::WidgetTemplate{
                .geometry = {
                        .anchor = {
                                .offset = gml::Vec2<float>(10 + v_list_2->actual_size().x() + 50,
                                                           40 + v_list_1->actual_size().y() + 50),
                                .position = gui::AnchorPosition::TOP_LEFT,
                        },
                        .size_hint = gml::Vec2<gui::SizeHint>(gui::SizeHint::FIT_CHILDREN),
                },
                .border = {.thickness = 1},
                .color = gl::Color::TRANSPARENT,
        });
        master->createWidget<gui::Label>(frame_4, gui::LabelTemplate{
                .text = {
                        .text = U"Button showcase",
                        .font = font,
                        .size = 20
                }
        });

        auto h_list_buttons = master->createWidget<gui::VListBox>(frame_4, gui::WidgetTemplate{
                .geometry = {
                        .size_hint = gml::Vec2<gui::SizeHint>(gui::SizeHint::FIT_CHILDREN),
                },
                .border = {.thickness = 1},
                .color = gl::Color::TRANSPARENT,
        });

        frame_clicks = master->createWidget<gui::HListBox>(h_list_buttons, gui::WidgetTemplate{
                .geometry = {
                        .size_hint = gml::Vec2<gui::SizeHint>(gui::SizeHint::FIT_CHILDREN),
                },
                .color = gl::Color::TRANSPARENT
        });
        master->createWidget<gui::Label>(frame_clicks, gui::LabelTemplate{
                .text = {
                        .text = U"Push button: ",
                        .font = font
                }
        });
        button_clicks = master->createWidget<gui::PushButton>(frame_clicks, gui::ButtonTemplate{
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
                .command = std::bind(&GuiTest::on_button_1_click, this),
        });
        label_clicks = master->createWidget<gui::Label>(button_clicks, gui::LabelTemplate{
                .base = {
                        .color = gl::Color::TRANSPARENT,
                },
                .text = {
                        .text = U"clicks: 0",
                        .font = font,
                },
        });


        frame_check = master->createWidget<gui::HListBox>(h_list_buttons, gui::WidgetTemplate{
                .geometry = {
                        .size_hint = gml::Vec2<gui::SizeHint>(gui::SizeHint::FIT_CHILDREN),
                },
                .color = gl::Color::TRANSPARENT
        });
        master->createWidget<gui::Label>(frame_check, gui::LabelTemplate{
                .text = {
                        .text = U"Check button: ",
                        .font = font
                }
        });
        button_check = master->createWidget<gui::CheckButton>(frame_check, gui::ButtonTemplate{
                .base = {
                        .border = {.thickness = 1},
                        .color = gl::Color::LIGHT_BLUE,
                },
                .click = {
                        .border = {.thickness = 1},
                        .color = gl::Color::LIGHT_BLUE,
                },
                .hover = {
                        .border = {.thickness = 2},
                        .color = gl::Color::LIGHT_BLUE,
                },
                .command = std::bind(&GuiTest::on_button_2_click, this),
        });
        label_check = master->createWidget<gui::Label>(button_check, gui::LabelTemplate{
                .base = {
                        .color = gl::Color::TRANSPARENT,
                },
                .text = {
                        .text = U"state: off",
                        .font = font,
                }
        });

        frame_radio = master->createWidget<gui::VListBox>(h_list_buttons, gui::WidgetTemplate{
                .geometry = {
                        .size_hint = gml::Vec2<gui::SizeHint>(gui::SizeHint::FIT_CHILDREN),
                },
                .color = gl::Color::TRANSPARENT
        });
        label_radio = master->createWidget<gui::Label>(frame_radio, gui::LabelTemplate{
                .text = {
                        .text = U"state: 1",
                        .font = font
                }
        });
        radioGroup = gui::RadioGroup<int>(0, [this](int value) {
            this->on_radio_click(value);
        });
        auto radio_button_1 = radioGroup.addButton(master->createWidget<gui::RadioButton<int>>(frame_radio, gui::ButtonTemplate{
                .base = {
                        .border = {.thickness = 1}
                },
                .click = {
                        .border = {.thickness = 1},
                        .color = gl::Color::GREY,
                },
                .hover = {
                        .border = {.thickness = 1},
                        .color = 0xDDDDDDFF,
                },
        }, 1), true);
        master->createWidget<gui::Label>(radio_button_1, gui::LabelTemplate{
            .base = {
                    .color = gl::Color::TRANSPARENT,
            },
            .text = {
                    .text = U"Value 1",
                    .font = font,
            }
        });
        auto radio_button_2 = radioGroup.addButton(master->createWidget<gui::RadioButton<int>>(frame_radio, gui::ButtonTemplate{
                .base = {
                        .border = {.thickness = 1}
                },
                .click = {
                        .border = {.thickness = 1},
                        .color = gl::Color::GREY,
                },
                .hover = {
                        .border = {.thickness = 1},
                        .color = 0xDDDDDDFF,
                },
        }, 2));
        master->createWidget<gui::Label>(radio_button_2, gui::LabelTemplate{
                .base = {
                        .color = gl::Color::TRANSPARENT,
                },
                .text = {
                        .text = U"Value 2",
                        .font = font,
                }
        });
        auto radio_button_3 = radioGroup.addButton(
                master->createWidget<gui::RadioButton<int>>(frame_radio, gui::ButtonTemplate{
                        .base = {
                                .border = {.thickness = 1}
                        },
                        .click = {
                                .border = {.thickness = 1},
                                .color = gl::Color::GREY,
                        },
                        .hover = {
                                .border = {.thickness = 1},
                                .color = 0xDDDDDDFF,
                        },
                }, 3));
        master->createWidget<gui::Label>(radio_button_3, gui::LabelTemplate{
                .base = {
                        .color = gl::Color::TRANSPARENT,
                },
                .text = {
                        .text = U"Value 3",
                        .font = font,
                }
        });

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
        textEntryTemplate.border.thickness = 1;
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
        if (button_check->state()) {
            label_check->setText(U"state: on");
        } else {
            label_check->setText(U"state: off");
        }
    }

    void on_radio_click(int value)
    {
        label_radio->setText(U"state: " + utils::toUTF32(value));
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
    gui.fpsCounter->setText(utils::toUTF32((int) frameRate));
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
    while (!window->shouldDestroy()) {
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
