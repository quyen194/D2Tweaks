
#include <Windows.h>

#include <algorithm>
#include <pugixml.hpp>
#include <stdio.h>

#include <spdlog/spdlog.h>

#include "diablo2/utils/mpq_ifstream.h"

#include "diablo2/d2client.h"

#include "d2tweaks/ui/controls/button.h"
#include "d2tweaks/ui/controls/checkbox.h"
#include "d2tweaks/ui/controls/control.h"
#include "d2tweaks/ui/controls/group.h"
#include "d2tweaks/ui/controls/image.h"
#include "d2tweaks/ui/controls/label.h"

#include "d2tweaks/ui/menu.h"

using namespace diablo2;

namespace d2_tweaks {
namespace ui {

using namespace controls;

menu::menu() : m_x(0), m_y(0), m_width(0), m_height(0), m_res_count(0) {}

bool menu::load_xml(const char* path) {
  // utils::mpq_ifstream file(path);
  // auto file = fopen((char*)path, "r+");

  // if (!file)
  //   return false;

  // pugi::xml_document doc;
  // const auto result = doc.load_file(path);

  utils::mpq_ifstream file(path);

  if (!file.good())
    return false;

  pugi::xml_document doc;
  const auto result = doc.load(file);

  if (!result)
    return false;

  const auto root = doc.select_nodes("/interface/menu");

  if (root.size() == 0)
    return false;

  const auto menuNode = root.first();

  m_x = menuNode.node().attribute("default_pos_x").as_int(0);
  m_y = menuNode.node().attribute("default_pos_y").as_int(0);
  m_res_count = menuNode.node().attribute("resolution_count").as_int(1);

  char buf[32] = {0};

  ui::respos temp;
  for (uint32_t i = 1; i <= m_res_count; i++) {
    sprintf_s(buf, "res%d_x", i);
    temp.res_x = menuNode.node().attribute(buf).as_int(0);
    sprintf_s(buf, "res%d_y", i);
    temp.res_y = menuNode.node().attribute(buf).as_int(0);
    sprintf_s(buf, "pos%d_x", i);
    temp.pos_x = menuNode.node().attribute(buf).as_int(0);
    sprintf_s(buf, "pos%d_y", i);
    temp.pos_y = menuNode.node().attribute(buf).as_int(0);

    if (temp.pos_x == d2_client::screen_width() &&
        temp.pos_y == d2_client::screen_height()) {
      m_x = temp.pos_x;
      m_y = temp.pos_y;
    }

    m_respos.push_back(temp);
  }

  m_name = menuNode.node().attribute("name").as_string();
  m_width = menuNode.node().attribute("width").as_int(-1);
  m_height = menuNode.node().attribute("height").as_int(-1);

  for (auto child : menuNode.node().children()) {
    const auto name = child.name();

    if (strcmp(name, "button") == 0) {
      auto obj = new button(this, child);
      if (!add_control(obj)) {
        if (obj) {
          delete obj;
        }
      }
      continue;
    }

    if (strcmp(name, "checkbox") == 0) {
      auto obj = new checkbox(this, child);
      if (!add_control(obj)) {
        if (obj) {
          delete obj;
        }
      }
      continue;
    }

    if (strcmp(name, "group") == 0) {
      auto obj = new group(this, child);
      if (!add_control(obj)) {
        if (obj) {
          delete obj;
        }
      }
      continue;
    }

    if (strcmp(name, "image") == 0) {
      auto obj = new image(this, child);
      if (!add_control(obj)) {
        if (obj) {
          delete obj;
        }
      }
      continue;
    }

    if (strcmp(name, "label") == 0) {
      auto obj = new label(this, child);
      if (!add_control(obj)) {
        if (obj) {
          delete obj;
        }
      }
      continue;
    }
  }

  if (m_width == -1) {
    for (auto control : m_controls)
      if (control->get_width() > m_width)
        m_width = control->get_width();
  }

  if (m_height == -1) {
    for (auto control : m_controls)
      if (control->get_height() > m_height)
        m_height = control->get_height();
  }

  return true;
}

bool menu::add_control(control* obj) {
  if (obj == nullptr)
    return false;

  if (obj->get_name().empty()) {
    spdlog::error("Cannot add control: empty name!");
    return false;
  }

  const auto it = m_named_controls.find(obj->get_name());

  if (it != m_named_controls.end()) {
    // update existing control
    control* current = m_named_controls[obj->get_name()];

    if (obj->get_type() != current->get_type()) {
      return false;
    }

    switch (obj->get_type()) {
      case type::kButton: {
        auto exp_obj = dynamic_cast<button*>(current);
        auto new_obj = dynamic_cast<button*>(obj);
        exp_obj->set_attr(*new_obj);
      } break;
      case type::kCheckbox: {
        auto exp_obj = dynamic_cast<checkbox*>(current);
        auto new_obj = dynamic_cast<checkbox*>(obj);
        exp_obj->set_attr(*new_obj);
      } break;
      case type::kGroup: {
        auto exp_obj = dynamic_cast<group*>(current);
        auto new_obj = dynamic_cast<group*>(obj);
        exp_obj->set_attr(*new_obj);
      } break;
      case type::kImage: {
        auto exp_obj = dynamic_cast<image*>(current);
        auto new_obj = dynamic_cast<image*>(obj);
        exp_obj->set_attr(*new_obj);
      } break;
      case type::kLabel: {
        auto exp_obj = dynamic_cast<label*>(current);
        auto new_obj = dynamic_cast<label*>(obj);
        exp_obj->set_attr(*new_obj);
      } break;
    }

    return false;
  }

  m_named_controls[obj->get_name()] = obj;
  m_controls.push_back(obj);

  return true;
}

void menu::remove_control(control* control) {
  if (control == nullptr)
    return;

  m_controls.erase(
            std::remove(m_controls.begin(), m_controls.end(), control),
            m_controls.end());
}

void menu::draw() {
  uint32_t screen_width = d2_client::screen_width();
  uint32_t screen_height = d2_client::screen_height();

  for (auto control : m_controls) {
    if (!control->get_visible() || control->get_parent() != nullptr)
      continue;

    for (auto it = m_respos.begin(); it != m_respos.end(); it++) {
      if (it->res_x == screen_width &&
          it->res_y == screen_height) {
        m_x = it->pos_x;
        m_y = it->pos_y;
        break;
      }
    }

    control->draw(m_x, m_y);
  }
}

bool menu::left_mouse(bool up) {
  auto block = false;

  for (auto control : m_controls) {
    if (!control->get_enabled() || control->get_parent() != nullptr)
      continue;

    auto tblock = false;
    control->left_mouse(m_x, m_y, up, tblock);
    block |= tblock;
  }

  return block;
}

bool menu::right_mouse(bool up) {
  auto block = false;

  for (auto control : m_controls) {
    if (!control->get_enabled() || control->get_parent() != nullptr)
      continue;

    auto tblock = false;
    control->right_mouse(m_x, m_y, up, tblock);
    block |= tblock;
  }

  return block;
}

bool menu::middle_mouse(bool up) {
  auto block = false;

  for (auto control : m_controls) {
    if (!control->get_enabled() || control->get_parent() != nullptr)
      continue;

    auto tblock = false;
    control->middle_mouse(m_x, m_y, up, tblock);
    block |= tblock;
  }

  return block;
}

bool menu::mouse_wheel(bool up) {
  auto block = false;

  for (auto control : m_controls) {
    if (!control->get_enabled() || control->get_parent() != nullptr)
      continue;

    auto tblock = false;
    control->mouse_wheel(m_x, m_y, up, tblock);
    block |= tblock;
  }

  return block;
}

bool menu::key_event(uint32_t key, bool up) {
  auto block = false;

  for (auto control : m_controls) {
    if (!control->get_enabled() || control->get_parent() != nullptr)
      continue;

    auto tblock = false;
    control->key_event(m_x, m_y, key, up, tblock);
    block |= tblock;
  }

  return block;
}

}  // namespace ui
}  // namespace d2_tweaks
