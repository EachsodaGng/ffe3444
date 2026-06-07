#define WIN32_LEAN_AND_MEAN




#include "scimitar/scimitar.hpp"
#include "scimitar/hooks/hooks.hpp"

#include "directx/Hook/hook.hpp"



auto on_attach() -> unsigned long __stdcall
{
    Beep(750, 300);

	hooks::Init();

    return 0;
}



auto __stdcall DllMain(HMODULE dll_instance, std::int32_t reason, [[maybe_unused]] void* r8) -> bool
{
	if (reason == 1) {


		utils::memory::image_base = utils::importer::get_safe_module(_x(L"RainbowSix.exe")); //<-- Get Module Base
		utils::memory::text_size = utils::importer::get_section_size(utils::memory::image_base, _x(".text")); //<-- Get .text Section Size
		utils::memory::rdata_size = utils::importer::get_section_size(utils::memory::image_base, _x(".rdata")); //<-- Get .rdata Section Size
		utils::memory::rdata_virtualaddress = utils::importer::get_virtual_address(utils::memory::image_base, _x(".rdata")); //<-- Get .rdata Section Virtual Address

		utils::trampoline_jmp = (void*)utils::memory::scan_pattern(utils::memory::image_base, utils::memory::text_size, _x("FF 27")); // <-- Get Trampoline Address (Used For Spoof Call)

		d3d12::mainModule = dll_instance;

		//SetupConsole(); //<-- Setups Debug Console (CMD Window)

		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)on_attach, 0, 0, 0); 
	}
	else if (reason == 2) {




	}
	return true;
}