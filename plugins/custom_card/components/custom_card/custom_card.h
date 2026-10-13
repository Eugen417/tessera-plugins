#pragma once
#include "esphome/core/component.h"
#include "esphome/components/smart_display/plugin_api.h"

namespace esphome::custom_card {

// The plugin: one tile type ("card") that draws the parts a template sensor lists in its attribute `elements`.
class CustomCard : public Component, public tessera::Plugin {
 public:
  void setup() override;
  float get_setup_priority() const override { return setup_priority::DATA; }
};

}  // namespace esphome::custom_card
