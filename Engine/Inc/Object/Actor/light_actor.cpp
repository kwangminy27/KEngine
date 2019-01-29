#include "Engine.h"
#include "light_actor.h"

#include "device_manager.h"
#include "Resource/resource_manager.h"
#include "Resource/sampler.h"
#include "Resource/mesh.h"
#include "Rendering/rendering_manager.h"
#include "Rendering/shader.h"
#include "Rendering/render_state.h"
#include "World/world_manager.h"
#include "Object/object_manager.h"
#include "Object/Actor/camera_actor.h"
#include "Object/Component/light.h"

void K::LightActor::Initialize()
{
	try
	{
		auto const& object_manager = ObjectManager::singleton();

		auto light = object_manager->CreateComponent<Light>(TAG{ LIGHT, 0 });
		AddComponent(light);

		set_render_group_type(RENDER_GROUP_TYPE::LIGHT);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "LightActor::Initialize" << std::endl;
	}
}

K::APTR K::LightActor::Clone() const
{
	return APTR{ new LightActor{ *this }, [](LightActor* _p) {
		_p->_Finalize();
		delete _p;
	} };
}

void K::LightActor::Serialize(InputMemoryStream& _imstream)
{
}

void K::LightActor::Serialize(OutputMemoryStream& _omstream)
{
}

void K::LightActor::_Render(float _time)
{
	auto const& light = CPTR_CAST<Light>(FindComponent(TAG{ LIGHT, 0 }));

	light->UpdateConstantBuffer();

	auto const& resource_manager = ResourceManager::singleton();
	auto const& rendering_manager = RenderingManager::singleton();

	rendering_manager->FindShader(DEFERRED_LIGHTING_SHADER)->SetToShader();

	auto const& light_blend = rendering_manager->FindRenderState(LIGHT_BLEND);
	auto const& depth_disable = rendering_manager->FindRenderState(DEPTH_DISABLE);

	light_blend->SetState();
	depth_disable->SetState();
	{
		resource_manager->FindSampler(POINT_SAMPLER)->SetToShader(1);
		resource_manager->FindMesh(FULL_SCREEN_RECT)->Render();
	}
	depth_disable->ResetState();
	light_blend->ResetState();
}

K::LightActor::LightActor(LightActor const& _other) : Actor(_other)
{
}

K::LightActor::LightActor(LightActor&& _other) noexcept : Actor(std::move(_other))
{
}

void K::LightActor::_Finalize()
{
}
