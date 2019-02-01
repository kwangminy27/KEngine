#include "Engine.h"
#include "light_actor.h"

#include "device_manager.h"
#include "Resource/resource_manager.h"
#include "Resource/sampler.h"
#include "Resource/mesh.h"
#include "Rendering/rendering_manager.h"
#include "Rendering/shader.h"
#include "Rendering/render_state.h"
#include "Rendering/depth_stencil_state.h"
#include "World/world_manager.h"
#include "Object/object_manager.h"
#include "Object/Actor/camera_actor.h"
#include "Object/Component/transform.h"
#include "Object/Component/light.h"

void K::LightActor::Initialize()
{
	try
	{
		auto const& object_manager = ObjectManager::singleton();

		auto transform = object_manager->CreateComponent<Transform>(TAG{ TRANSFORM, 0 });
		CPTR_CAST<Transform>(transform)->set_model_scaling(Vector3::One);
		CPTR_CAST<Transform>(transform)->set_model_rotation(Quaternion::CreateFromYawPitchRoll(0.f, 90.f, 0.f));
		CPTR_CAST<Transform>(transform)->set_local_translation(-Vector3::UnitZ);
		AddComponent(transform);

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

#include "input_manager.h"

void K::LightActor::_Render(float _time)
{
	auto const& input_manager = InputManager::singleton();

	auto const& light = CPTR_CAST<Light>(FindComponent(TAG{ LIGHT, 0 }));
	auto const& transform = CPTR_CAST<Transform>(FindComponent(TAG{ TRANSFORM, 0 }));
	
	light->UpdateConstantBuffer();

	transform->set_local_translation(light->position());

	auto direction = light->direction();
	direction.Normalize();

	if (1.f != Vector3::UnitZ.Dot(direction))
	{
		auto axis = Vector3::UnitZ.Cross(direction);
		axis.Normalize();

		auto angle = DirectX::XMConvertToDegrees(acos(Vector3::UnitZ.Dot(direction)));

		transform->set_local_rotation(Quaternion::CreateFromAxisAngle(axis, angle));
	}

	transform->UpdateConstantBuffer();

	auto const& resource_manager = ResourceManager::singleton();
	auto const& rendering_manager = RenderingManager::singleton();

	rendering_manager->FindShader(DEFERRED_LIGHTING_SHADER)->SetToShader();

	auto const& light_blend = rendering_manager->FindRenderState(LIGHT_BLEND);

	light_blend->SetState();
	{
		resource_manager->FindSampler(POINT_SAMPLER)->SetToShader(1);

		if (LIGHT_TYPE::DIRECTIONAL == static_cast<LIGHT_TYPE>(light->type()))
		{
			auto const& depth_disable = rendering_manager->FindRenderState(DEPTH_DISABLE);

			depth_disable->SetState();
			{
				resource_manager->FindMesh(FULL_SCREEN_RECT)->Render();
			}
			depth_disable->ResetState();
		}
		else
		{
			// Light Volume Pass 1
			auto const& test = rendering_manager->FindRenderState("Test");
			auto const& rs_light_volume_pass_1 = rendering_manager->FindRenderState(RS_LIGHT_VOLUME_PASS_1);
			auto const& depth_light_volume_pass_1 = rendering_manager->FindRenderState(DSS_LIGHT_VOLUME_PASS_1);

			test->SetState();
			rs_light_volume_pass_1->SetState();
			std::static_pointer_cast<DepthStencilState>(depth_light_volume_pass_1)->set_stencil_ref(1);
			depth_light_volume_pass_1->SetState();
			{
				if (LIGHT_TYPE::POINT == static_cast<LIGHT_TYPE>(light->type()))
					resource_manager->FindMesh(SPHERE_VOLUME)->Render();
				else if (LIGHT_TYPE::SPOT == static_cast<LIGHT_TYPE>(light->type()))
					resource_manager->FindMesh(SPOTLIGHT_VOLUME)->Render();
			}
			depth_light_volume_pass_1->ResetState();
			rs_light_volume_pass_1->ResetState();
			test->ResetState();

			// Light Volume Pass 2
			auto const& rs_light_volume_pass_2 = rendering_manager->FindRenderState(RS_LIGHT_VOLUME_PASS_2);
			auto const& depth_light_volume_pass_2 = rendering_manager->FindRenderState(DSS_LIGHT_VOLUME_PASS_2);

			rs_light_volume_pass_2->SetState();
			std::static_pointer_cast<DepthStencilState>(depth_light_volume_pass_2)->set_stencil_ref(1);
			depth_light_volume_pass_2->SetState();
			{
				if (LIGHT_TYPE::POINT == static_cast<LIGHT_TYPE>(light->type()))
					resource_manager->FindMesh(SPHERE_VOLUME)->Render();
				else if (LIGHT_TYPE::SPOT == static_cast<LIGHT_TYPE>(light->type()))
					resource_manager->FindMesh(SPOTLIGHT_VOLUME)->Render();

				auto const& test = rendering_manager->FindRenderState(RS_WIREFRAME_CULL_BACK);
				auto const& depth_disable = rendering_manager->FindRenderState(DEPTH_DISABLE);

				test->SetState();
				depth_disable->SetState();
				{
					if (LIGHT_TYPE::POINT == static_cast<LIGHT_TYPE>(light->type()))
						resource_manager->FindMesh(SPHERE_VOLUME)->Render();
					else if (LIGHT_TYPE::SPOT == static_cast<LIGHT_TYPE>(light->type()))
						resource_manager->FindMesh(SPOTLIGHT_VOLUME)->Render();
				}
				depth_disable->ResetState();
				test->ResetState();
			}
			depth_light_volume_pass_2->ResetState();
			rs_light_volume_pass_2->ResetState();
		}
	}
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
