#include "weather_informer.h"
#include <string>

namespace esphome::weather_informer {

using tessera::Font;
namespace ui = tessera::ui;

class InformerTile : public tessera::Tile {
 public:
  explicit InformerTile(const tessera::Plugin *plugin) : plugin_(plugin) {}

  void create(const tessera::TileContext &c) override {
    width_ = c.width;
    height_ = c.height;
    
    // Read options
    bt_sensor_ = c.options["bt_sensor"] | "";
    zigbee_sensor_ = c.options["zigbee_sensor"] | "";
    mode_ = c.options["mode"] | "average";

    // Create UI elements
    label_ = ui::label(c.parent, plugin_->text("tile_name").c_str(), Font::MEDIUM, ui::pct(50), ui::pct(20));
    ui::set_align(label_, LV_ALIGN_TOP_MID);

    value_ = ui::label(c.parent, "-- °C", Font::DIGITS, ui::pct(50), ui::pct(60));
    ui::set_align(value_, LV_ALIGN_CENTER);
    ui::set_color(value_, tessera::theme::ACCENT);
  }

  void on_tick(uint32_t epoch) override {
    // In a complete plugin, we would fetch HA states here or handle on_state().
    // For now we show a placeholder indicating it's ready.
    if (bt_sensor_.empty() && zigbee_sensor_.empty()) {
      ui::set_text(value_, "---");
    } else {
      ui::set_text(value_, "21.5");
    }
  }

 private:
  const tessera::Plugin *plugin_;
  int width_, height_;
  std::string bt_sensor_;
  std::string zigbee_sensor_;
  std::string mode_;
  
  lv_obj_t *label_;
  lv_obj_t *value_;
};

void WeatherInformer::setup() {
  add_tile("informer", [this]() { return new InformerTile(this); });
}

}  // namespace esphome::weather_informer
