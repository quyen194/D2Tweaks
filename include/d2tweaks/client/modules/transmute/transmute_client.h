#pragma once

#include <cstdint>
#include <d2tweaks/client/modules/client_module.h>

namespace d2_tweaks {
	namespace client {
		namespace modules {
			class transmute final : public client_module {
			public:
				void init() override;
				void init_early() override;
				void handle_packet(common::packet_header* packet) override;
				void tick() override;
			};
		}
	}
}