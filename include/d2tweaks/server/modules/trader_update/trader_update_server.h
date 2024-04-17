#pragma once

#include <cstdint>
#include <d2tweaks/server/modules/server_module.h>

namespace diablo2 {
	namespace structures {
		struct inventory;
		struct game;
		struct unit;
	}
}

namespace d2_tweaks {
	namespace server {
		class server;

		namespace modules {
			class trader_update final : public server_module {
			public:
				void init() override;
				void tick(diablo2::structures::game* game, diablo2::structures::unit* unit) override;
				bool handle_packet(diablo2::structures::game* game, diablo2::structures::unit* player, common::packet_header* packet) override;

			//private:
			//	bool find_free_space(diablo2::structures::inventory* inv, diablo2::structures::unit* item, int32_t inventoryIndex, char page, uint32_t& x, uint32_t& y);
			//	bool send_to_cube(diablo2::structures::game* game, diablo2::structures::unit* player, diablo2::structures::unit* item);
			//	bool move_item_to(diablo2::structures::game* game, diablo2::structures::unit* player, common::packet_header* packet);
			};
		}
	}
}
