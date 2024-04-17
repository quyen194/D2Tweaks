#pragma once

#include <d2tweaks/server/modules/server_module.h>

namespace d2_tweaks {
	namespace server {
		class server;

		namespace modules {
			class auto_item_pickup final : public server_module {
			public:
				void init() override;
				void tick(diablo2::structures::game* game, diablo2::structures::unit* unit) override;
				bool handle_packet(diablo2::structures::game* game, diablo2::structures::unit* player, common::packet_header* packet) override;
				bool au_pickup_item(diablo2::structures::game* game, diablo2::structures::unit* unit, uint32_t guid);
			};
		}
	}
}
