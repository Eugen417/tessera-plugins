#include "weather_informer.h"
#include <string>

namespace esphome::weather_informer {

using tessera::Font;
namespace ui = tessera::ui;

class DashboardTile : public tessera::Tile {
 public:
  explicit DashboardTile(const tessera::Plugin *plugin) : plugin_(plugin) {}

  void create(const tessera::TileContext &c) override {
    width_ = c.width;
    height_ = c.height;

    // --- ГРАФИКА: ДУГА ТЕМПЕРАТУРЫ (как на фото) ---
    temp_arc_ = lv_arc_create(c.parent);
    lv_obj_set_size(temp_arc_, ui::pct(45), ui::pct(70));
    ui::set_align(temp_arc_, LV_ALIGN_LEFT_MID, ui::mm(5), 0);
    lv_arc_set_bg_angles(temp_arc_, 135, 45); // 270 градусов
    lv_arc_set_angles(temp_arc_, 135, 270);
    
    // Убираем кружок-ползунок
    lv_obj_remove_style(temp_arc_, NULL, LV_PART_KNOB);
    
    // Стилизуем фон дуги
    lv_obj_set_style_arc_width(temp_arc_, ui::mm(4), LV_PART_MAIN);
    lv_obj_set_style_arc_color(temp_arc_, ui::tone_color(tessera::Tone::MUTED), LV_PART_MAIN);
    
    // Стилизуем заполненную часть (желтым/акцентным)
    lv_obj_set_style_arc_width(temp_arc_, ui::mm(4), LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(temp_arc_, ui::tone_color(tessera::Tone::ACCENT), LV_PART_INDICATOR);

    // Текст температуры внутри дуги
    temp_val_ = ui::label(temp_arc_, "--", Font::DIGITS, ui::pct(100), ui::pct(50));
    ui::set_align(temp_val_, LV_ALIGN_CENTER);

    // --- ГРАФИКА: КОМПАС ВЕТРА (основа) ---
    wind_bg_ = lv_obj_create(c.parent);
    lv_obj_set_size(wind_bg_, ui::pct(45), ui::pct(70));
    ui::set_align(wind_bg_, LV_ALIGN_RIGHT_MID, -ui::mm(5), 0);
    lv_obj_set_style_radius(wind_bg_, LV_RADIUS_CIRCLE, 0); // Делаем круглым
    lv_obj_set_style_border_width(wind_bg_, ui::mm(2), 0);
    lv_obj_set_style_border_color(wind_bg_, ui::tone_color(tessera::Tone::MUTED), 0);
    lv_obj_set_style_bg_opa(wind_bg_, LV_OPA_TRANSP, 0);

    // Текст внутри компаса
    wind_val_ = ui::label(wind_bg_, "Wind", Font::MEDIUM, ui::pct(100), ui::pct(40));
    ui::set_align(wind_val_, LV_ALIGN_CENTER);
  }

  void on_state(const tessera::TileContext &c) override {
    if (c.entity) {
      // 1. Получаем главное значение (Температуру)
      std::string state = c.entity->state;
      ui::set_text(temp_val_, state.c_str());
      
      // Имитация заполнения дуги в зависимости от температуры (от -20 до +40)
      try {
        float temp_f = std::stof(state);
        int arc_val = (temp_f + 20) * (100.0 / 60.0);
        if (arc_val < 0) arc_val = 0;
        if (arc_val > 100) arc_val = 100;
        lv_arc_set_value(temp_arc_, arc_val);
      } catch(...) {}

      // 2. Получаем атрибуты (ветер и т.д.)
      auto wind = c.entity->attribute("wind_speed");
      if (wind) {
        std::string w_str = *wind + " m/s";
        ui::set_text(wind_val_, w_str.c_str());
      }
    }
  }

 private:
  const tessera::Plugin *plugin_;
  int width_, height_;
  
  lv_obj_t *temp_arc_;
  lv_obj_t *temp_val_;
  lv_obj_t *wind_bg_;
  lv_obj_t *wind_val_;
};

void WeatherInformer::setup() {
  add_tile("dashboard", [this]() { return new DashboardTile(this); });
}

}  // namespace esphome::weather_informer

