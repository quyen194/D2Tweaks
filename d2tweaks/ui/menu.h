#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace d2_tweaks {
namespace ui {

namespace controls {
class control;
}

using namespace controls;

struct respos {
  uint32_t res_x;
  uint32_t res_y;
  uint32_t pos_x;
  uint32_t pos_y;
};

class menu {
  bool m_enabled = false;
  bool m_visible = false;

  std::string m_name;

  int32_t m_x;
  int32_t m_y;
  int32_t m_width;
  int32_t m_height;
  uint32_t m_res_count;

  std::vector<control*> m_controls;
  std::unordered_map<std::string, control*> m_named_controls;
  std::vector<ui::respos> m_respos;

 public:
  menu();
  virtual ~menu() = default;

  const std::vector<control*>& get_controls() const {
    return m_controls;
  }

  const std::string& get_name() const { return m_name; }

  virtual bool get_enabled() const { return m_enabled; }

  virtual void set_enabled(bool value) { m_enabled = value; }

  virtual bool get_visible() const { return m_visible; }

  virtual void set_visible(bool value) { m_visible = value; }

  bool load_xml(const char* path);

  template <typename TControl = control>
  TControl* get_control(const std::string& name) {
    const auto it = m_named_controls.find(name);

    if (it == m_named_controls.end())
      return nullptr;

    return static_cast<TControl*>(it->second);
  }

  virtual bool add_control(control* control);
  // virtual control* get_control(const std::string& name);
  virtual void remove_control(control* control);

  virtual void draw();

  virtual bool left_mouse(bool up);
  virtual bool right_mouse(bool up);
  virtual bool middle_mouse(bool up);
  virtual bool mouse_wheel(bool up);

  virtual bool key_event(uint32_t key, bool up);
};

}  // namespace ui
}  // namespace d2_tweaks
