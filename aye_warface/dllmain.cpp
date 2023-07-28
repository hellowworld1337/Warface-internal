#include "includes.hpp"





bool __stdcall DllMain(void* module_instance [[ unused ]], uint64_t call_reason, void* reserved [[ unused ]] ) {
	if (call_reason != 1)
		return false;

	AllocConsole();
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
	gEnv->pGame->GetIGameFramework()->RegisterListener(new CGameFunction(), NULL, FRAMEWORKLISTENERPRIORITY_DEFAULT);

	return true;
}