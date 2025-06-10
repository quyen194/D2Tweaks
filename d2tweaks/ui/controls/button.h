#pragma once

#include <cstdint>
#include <functional>

#include <pugixml.hpp>

#include "d2tweaks/ui/controls/control.h"
#include "d2tweaks/ui/rect.h"

namespace diablo2 {
namespace structures {
struct cell_file;
}
}  // namespace diablo2

namespace d2_tweaks {
namespace common {
class asset;
}

namespace ui {
namespace controls {
class image;

class button final : public control {
  rect m_rect;
  image* m_image;
  int32_t m_frame_down;
  int32_t m_frame_up;
  int32_t m_click_sound;

  std::wstring m_popup;

  uint32_t m_res_count;
  std::vector<respos> m_respos;

  // object stage
  std::function<void()> m_on_click;
  bool m_is_down;
  int32_t m_current_frame;

 public:
  button(menu* menu,
         const rect& rect,
         const std::function<void()>& onClick,
         common::asset* image,
         int32_t frameDown,
         int32_t frameUp,
         int32_t clickSound = -1);
  explicit button(menu* menu, const pugi::xml_node& node);
  virtual ~button();

  void set(button &obj);
  void set_attr(button &obj);
  void set_state(button &obj);

  void set_x(int32_t value) override;
  int32_t get_x() const override { return m_rect.get_x(); }

  void set_y(int32_t value) override;
  int32_t get_y() const override { return m_rect.get_y(); }

  void set_frame_down(int32_t value) { m_frame_down = value; }
  int32_t get_frame_down() { return m_frame_down; }

  void set_frame_up(int32_t value) { m_frame_up = value; }
  int32_t get_frame_up() { return m_frame_up; }

  void set_popup(const std::wstring& popup) { m_popup = popup; }
  std::wstring popup() const { return m_popup; }

  void set_on_click(const std::function<void()>& on_click) {
    m_on_click = on_click;
  }
  std::function<void()> get_on_click() const { return m_on_click; }

  void set_current_frame(int32_t value) { m_current_frame = value; }
  int32_t get_current_frame() { return m_current_frame; }

  void draw() override;
  void draw(int32_t offsetX, int32_t offsetY) override;

  void left_mouse(int32_t offsetX,
                  int32_t offsetY,
                  bool up,
                  bool& block) override;
  void right_mouse(int32_t offsetX,
                   int32_t offsetY,
                   bool up,
                   bool& block) override;

  void key_event(int32_t offsetX,
                 int32_t offsetY,
                 uint32_t key,
                 bool up,
                 bool& block) override;
};

}  // namespace controls
}  // namespace ui
}  // namespace d2_tweaks
