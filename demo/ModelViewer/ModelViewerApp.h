#pragma once

#include <runtime/Application.h>

#include "MovementListener.h"

class ModelViewerApp final : public yage::Application
{
public:
    void initialize() override;

    void scale_scene(double factor);

private:
    MovementListener inputListener;

    void on_button_click();

    void load_scene(const std::string& filename);

    void setup_light();

    void setup_gui();
};
