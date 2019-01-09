#include "Client.h"
#include "Level/intro_level.h"

int WINAPI wWinMain(HINSTANCE _instance, HINSTANCE _prev_instance, PWSTR _cmd_line, int _cmd_show)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	auto const& core = K::Core::singleton();
	auto const& world_manager = K::WorldManager::singleton();

	core->Initialize(L"KEngine", L"KEngine", _instance, K::GAME_MODE::_2D);

	world_manager->CreateLevel<K::IntroLevel>(K::TAG{ "IntroLevel", 0 });

	core->Run();

	K::Core::singleton().reset();

	return 0;
}
