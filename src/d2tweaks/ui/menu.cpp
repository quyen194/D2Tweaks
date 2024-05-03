#include <d2tweaks/ui/menu.h>

#include <algorithm>
#include <pugixml.hpp>
#include <stdio.h>

#include <diablo2/utils/mpq_ifstream.h>
#include <diablo2/d2client.h>
#include <spdlog/spdlog.h>

#include <d2tweaks/ui/controls/control.h>
#include <d2tweaks/ui/controls/button.h>
#include <d2tweaks/ui/controls/image.h>
#include <d2tweaks/ui/controls/label.h>
#include <d2tweaks/ui/controls/checkbox.h>
#include <d2tweaks/ui/controls/group.h>

d2_tweaks::ui::menu::menu() : m_x(0), m_y(0), m_width(0), m_height(0) {}

bool d2_tweaks::ui::menu::load_xml(const char* path) {
	//diablo2::utils::mpq_ifstream file(path);
	//auto file = fopen((char*)path, "r+");

	//if (!file)
	//	return false;

	//pugi::xml_document doc;
	//const auto result = doc.load_file(path);

	diablo2::utils::mpq_ifstream file(path);

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

	char buf[32] = { 0 };

	respos temp;
	for (uint32_t i = 1; i <= m_res_count; i++) {
		sprintf_s(buf, "res%d_x", i);
		temp.res_x = menuNode.node().attribute(buf).as_int(0);
		sprintf_s(buf, "res%d_y", i);
		temp.res_y = menuNode.node().attribute(buf).as_int(0);
		sprintf_s(buf, "pos%d_x", i);
		temp.pos_x = menuNode.node().attribute(buf).as_int(0);
		sprintf_s(buf, "pos%d_y", i);
		temp.pos_y = menuNode.node().attribute(buf).as_int(0);

		if (temp.pos_x == diablo2::d2_client::screen_width() && temp.pos_y == diablo2::d2_client::screen_height()) {
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
			add_control(new controls::button(this, child));
			continue;
		}

		if (strcmp(name, "checkbox") == 0) {
			add_control(new controls::checkbox(this, child));
			continue;
		}

		if (strcmp(name, "group") == 0) {
			add_control(new controls::group(this, child));
			continue;
		}

		if (strcmp(name, "image") == 0) {
			add_control(new controls::image(this, child));
			continue;
		}

		if (strcmp(name, "label") == 0) {
			add_control(new controls::label(this, child));
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

void d2_tweaks::ui::menu::add_control(controls::control* control) {
	if (control == nullptr)
		return;

	if (control->get_name().empty()) {
		spdlog::error("Cannot add control: empty name!");
		return;
	}

	const auto it = m_named_controls.find(control->get_name());

	if (it != m_named_controls.end())
		return;

	m_named_controls[control->get_name()] = control;
	m_controls.push_back(control);
}

void d2_tweaks::ui::menu::remove_control(controls::control* control) {
	if (control == nullptr)
		return;

	m_controls.erase(std::remove(m_controls.begin(), m_controls.end(), control), m_controls.end());
}

void d2_tweaks::ui::menu::draw() {
	for (auto control : m_controls) {
		if (!control->get_visible() || control->get_parent() != nullptr)
			continue;

		for (auto it = m_respos.begin(); it != m_respos.end(); it++) {
			if (it->res_x == diablo2::d2_client::screen_width() && it->res_y == diablo2::d2_client::screen_height()) {
				m_x = it->pos_x;
				m_y = it->pos_y;
				break;
			}
		}

		control->draw(m_x, m_y);
	}
}

bool d2_tweaks::ui::menu::left_mouse(bool up) {
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

bool d2_tweaks::ui::menu::right_mouse(bool up) {
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

bool d2_tweaks::ui::menu::middle_mouse(bool up) {
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

bool d2_tweaks::ui::menu::mouse_wheel(bool up) {
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

bool d2_tweaks::ui::menu::key_event(uint32_t key, bool up) {
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