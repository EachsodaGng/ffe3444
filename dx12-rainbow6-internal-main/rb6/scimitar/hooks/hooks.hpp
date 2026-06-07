#pragma once
#include <vector>
#include <atomic>

#include <unordered_map>
#include <chrono>
#include <mutex>

#include "../../utils/utils.h"
#include "../../scimitar/scimitar.hpp"


inline std::vector < std::pair<void*, void*>> orig_funcs;
inline std::unordered_map<void*, void*> trampolines;
inline void* original_entity_hook = nullptr;


namespace hk {



	__int64 entity(__int64 a1);


}


inline void hook_fn(void* destination, void* detour)
{
	if (!destination)
		return;

	orig_funcs.push_back({ destination, *reinterpret_cast<void**>(destination) });

	void* trampoline = utils::memory::swap_inline(destination, detour);

	if (trampoline) {



		trampolines[destination] = trampoline;


		if (detour == reinterpret_cast<void*>(hk::entity))
			original_entity_hook = trampoline;

	}
}


inline std::unordered_set<scimitar::Entity*> cached_entities;
inline std::mutex entity_mutex;

namespace hk
{

	


	inline void init()
	{


		void* entity_call = reinterpret_cast<void*>(ImageBase + 0x4C55DE0); // <-- update offset here


		if (entity_call) {

			hook_fn(entity_call, hk::entity);  // <-- hooks entity function


		}
	}

}