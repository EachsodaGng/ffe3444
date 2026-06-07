#pragma once
#include "../../hooks/hooks.hpp"
#include "../../scimitar.hpp"



auto hk::entity(__int64 rcx) -> __int64
{
    
    if (!original_entity_hook)
        return 0;
    
    using func_t = __int64(__fastcall*)(__int64);

   
    if (!utils::memory::valid_pointer(rcx))
		return utils::spoof_call(reinterpret_cast<func_t>(original_entity_hook), rcx); // <-- Validates Pointer, if it's not valid we skip the caching part and just call the original function
   

    {
        auto entity = reinterpret_cast<scimitar::Entity*>(rcx);
        std::lock_guard<std::mutex> lock(entity_mutex);
        cached_entities.insert(entity); 

		//caches entities, 
        //this is done because the game calls this function
        //everytime the game calls this function we cache the rcx (a1) and add it in our cache
        
    }
    


    return utils::spoof_call(reinterpret_cast<func_t>(original_entity_hook), rcx);
}





