#pragma once

#include <cstdint>
#include <string>

namespace d2_tweaks {
namespace ui {
class menu;

namespace controls {

enum class type : int {
  kButton,
  kCheckbox,
  kGroup,
  kImage,
  kLabel,
};

struct respos {
  uint32_t res_x;
  uint32_t res_y;
  uint32_t pos_x;
  uint32_t pos_y;
};

class control {
  type m_type;
  control* m_parent;
  menu* m_menu;

  std::string m_name;
  bool m_enabled;
  bool m_visible;
  int32_t m_x;
  int32_t m_y;
  int32_t m_width;
  int32_t m_height;

 public:
  control(type type, menu* menu, int32_t x, int32_t y, int32_t w, int32_t h)
      : m_type(type),
        m_parent(nullptr),
        m_menu(menu),
        m_enabled(false),
        m_visible(false),
        m_x(x),
        m_y(y),
        m_width(w),
        m_height(h) {}
  control(control& obj)
      : m_type(obj.m_type),
        m_parent(obj.m_parent),
        m_menu(obj.m_menu),
        m_enabled(obj.m_enabled),
        m_visible(obj.m_visible),
        m_x(obj.m_x),
        m_y(obj.m_y),
        m_width(obj.m_width),
        m_height(obj.m_height) {}

  virtual ~control() = default;

  void set(control &obj) {
    m_type = obj.m_type;
    m_parent = obj.m_parent;
    m_menu = obj.m_menu;
    m_enabled = obj.m_enabled;
    m_visible = obj.m_visible;
    m_x = obj.m_x;
    m_y = obj.m_y;
    m_width = obj.m_width;
    m_height = obj.m_height;
  }

  type get_type() const { return m_type; }

  control* get_parent() const { return m_parent; }

  void set_parent(control* const parent) { m_parent = parent; }

  menu* get_menu() const { return m_menu; }

  void set_menu(menu* const menu) { m_menu = menu; }

  std::string get_name() const { return m_name; }

  void set_name(const std::string& name) { m_name = name; }

  virtual bool get_enabled() const { return m_enabled; }

  virtual void set_enabled(bool value) { m_enabled = value; }

  virtual bool get_visible() const { return m_visible; }

  virtual void set_visible(bool value) { m_visible = value; }

  virtual int32_t get_x() const { return m_x; }

  virtual void set_x(int32_t value) { m_x = value; }

  virtual int32_t get_y() const { return m_y; }

  virtual void set_y(int32_t value) { m_y = value; }

  virtual int32_t get_width() const { return m_width; }

  virtual void set_width(const int32_t width) { m_width = width; }

  virtual int32_t get_height() const { return m_height; }

  virtual void set_height(const int32_t height) { m_height = height; }

  virtual void draw() = 0;
  virtual void draw(int32_t offsetX, int32_t offsetY) = 0;

  virtual void left_mouse(int32_t offsetX,
                          int32_t offsetY,
                          bool up,
                          bool& block) = 0;
  virtual void right_mouse(int32_t offsetX,
                           int32_t offsetY,
                           bool up,
                           bool& block) = 0;
  virtual void middle_mouse(int32_t offsetX,
                            int32_t offsetY,
                            bool up,
                            bool& block) {}
  virtual void mouse_wheel(int32_t offsetX,
                           int32_t offsetY,
                           bool up,
                           bool& block) {}

  virtual void key_event(
      int32_t offsetX, int32_t offsetY, uint32_t key, bool up, bool& block) = 0;
};

}  // namespace controls
}  // namespace ui
}  // namespace d2_tweaks
