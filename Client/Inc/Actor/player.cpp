#include "Client.h"
#include "player.h"

void K::Player::Initialize()
{
	try
	{
		auto const& object_manager = ObjectManager::singleton();
		auto const& resource_manager = ResourceManager::singleton();
		auto const& rendering_manager = RenderingManager::singleton();

		AddComponent(object_manager->CreateComponent<Transform>(TAG{ TRANSFORM, 0 }));
		AddComponent(object_manager->CreateComponent<Material>(TAG{ MATERIAL, 0 }));
		AddComponent(object_manager->CreateComponent<Renderer>(TAG{ RENDERER, 0 }));

		auto const& transform = FindComponent(TAG{ TRANSFORM, 0 });
		CPTR_CAST<Transform>(transform)->set_local_scaling(Vector3::One);

		auto const& renderer = FindComponent(TAG{ RENDERER, 0 });
		CPTR_CAST<Renderer>(renderer)->set_mesh(resource_manager->FindMesh("cottage"));
		CPTR_CAST<Renderer>(renderer)->set_shader(rendering_manager->FindShader(BUMP_MAPPING_SHADER));

		set_render_group_type(RENDER_GROUP_TYPE::NORMAL);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Player::Initialize" << std::endl;
	}
}

K::APTR K::Player::Clone() const
{
	return APTR{ new Player, [](Player* _p) {
		_p->_Finalize();
		delete _p;
	} };
}

void K::Player::Serialize(InputMemoryStream& _imstream)
{
}

void K::Player::Serialize(OutputMemoryStream& _omstream)
{
}

K::Player::Player(Player const& _other) : ActorClient(_other)
{
}

K::Player::Player(Player&& _other) noexcept : ActorClient(std::move(_other))
{
}

void K::Player::_Finalize()
{
}

void K::Player::_Update(float _time)
{
}
