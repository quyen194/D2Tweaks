#include <d2tweaks/ui/controls/label.h>
#include <common/string_utils.h>
#include <diablo2/d2client.h>

using namespace diablo2;

namespace d2_tweaks {
namespace ui {
namespace controls {

label::label(menu* menu,
             const std::wstring& text,
             int32_t x,
             int32_t y,
             ui_color_t color,
             ui_font_t font)
    : control(type::kLabel, menu, x, y, 0, 0) {
  control::set_enabled(true);
  control::set_visible(true);

  m_text = text;
  m_text_owned = false;
  m_color = color;
  m_font = font;

  control::set_height(0);
  control::set_width(0);
}

label::label(menu* menu, const pugi::xml_node& node)
    : control(type::kLabel, menu, 0, 0, 0, 0) {
  const auto cname = node.attribute("name").as_string();

  char buf[32] = { 0 };

  auto cx = node.attribute("default_pos_x").as_int(0);
  auto cy = node.attribute("default_pos_y").as_int(0);
  m_res_count = node.attribute("resolution_count").as_int(1);

  respos temp;
  for (uint32_t i = 1; i <= m_res_count; i++) {
    sprintf_s(buf, "res%d_x", i);
    temp.res_x = node.attribute(buf).as_int(0);
    sprintf_s(buf, "res%d_y", i);
    temp.res_y = node.attribute(buf).as_int(0);
    sprintf_s(buf, "pos%d_x", i);
    temp.pos_x = node.attribute(buf).as_int(0);
    sprintf_s(buf, "pos%d_y", i);
    temp.pos_y = node.attribute(buf).as_int(0);

    if (temp.pos_x == d2_client::screen_width() &&
        temp.pos_y == d2_client::screen_height()) {
      cx = temp.pos_x;
      cy = temp.pos_y;
    }

    m_respos.push_back(temp);
  }

  const auto ctext = node.attribute("text").as_string();
  const auto ccolor = node.attribute("color").as_int();
  const auto cfont = node.attribute("font").as_int(1);

  control::set_enabled(true);
  control::set_visible(true);

  m_text_owned = true;
  m_text = string_utils::string_to_wstring(ctext);
  m_color = static_cast<ui_color_t>(ccolor);
  m_font = static_cast<ui_font_t>(cfont);

  control::set_x(cx);
  control::set_y(cy);

  control::set_height(0);
  control::set_width(0);

  set_name(cname);
}

void label::set_attr(label& obj) {
  control::set(obj);
  m_text = obj.m_text;
  m_text_owned = obj.m_text_owned;
  m_color = obj.m_color;
  m_font = obj.m_font;
  m_res_count = obj.m_res_count;
  m_respos = obj.m_respos;
}

void label::draw() {
  draw(0, 0);
}

void label::draw(int32_t offsetX, int32_t offsetY) {
  if (!get_visible()) {
    return;
  }

  for (auto it = m_respos.begin(); it != m_respos.end(); it++) {
    if (it->res_x == d2_client::screen_width() &&
        it->res_y == d2_client::screen_height()) {
      set_x(it->pos_x);
      set_y(it->pos_y);
      break;
    }
  }

  const auto font = d2_win::get_current_font();
  d2_win::set_current_font(m_font);

  if (get_width() == 0) {
    set_width(
        d2_win::get_text_pixel_width(const_cast<wchar_t*>(m_text.c_str())));
  }

  if (get_height() == 0) {
    set_height(d2_win::get_current_font_height());
  }

  d2_win::draw_text(const_cast<wchar_t*>(m_text.c_str()),
                    get_x() + offsetX,
                    get_y() + offsetY,
                    m_color,
                    0);
  d2_win::set_current_font(font);
}

void label::left_mouse(int32_t offsetX, int32_t offsetY, bool up, bool& block) {
}

void label::right_mouse(int32_t offsetX,
                        int32_t offsetY,
                        bool up,
                        bool& block) {}

void label::key_event(
    int32_t offsetX, int32_t offsetY, uint32_t key, bool up, bool& block) {}

}  // namespace controls
}  // namespace ui
}  // namespace d2_tweaks
