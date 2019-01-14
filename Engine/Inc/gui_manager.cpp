#include "Engine.h"
#include "gui_manager.h"

#include "Object/Actor/actor.h"
#include "Object/Actor/camera_actor.h"
#include "Object/Component/transform.h"
#include "Object/Component/light.h"

void K::GUIManager::Initialize()
{
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGui::StyleColorsClassic();
}

void K::GUIManager::Initialize(HWND _window, ID3D11Device* _device, ID3D11DeviceContext* _context)
{
	ImGui_ImplWin32_Init(_window);
	ImGui_ImplDX11_Init(_device, _context);
}

void K::GUIManager::Render()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	static Actor* actor{};
	static Light* light{};

	ImGui::Begin("KEngine");
	{
		static Vector3 eye = Vector3{ 0.f, 0.f, -5.f };
		static Vector3 focus{};

		ImGui::BulletText("Camera Modification");

		ImGui::SameLine();
		if (ImGui::Button("Camera Reset"))
		{
			eye = Vector3{ 0.f, 0.f, -5.f };
			focus = {};
		}

		ImGui::SliderFloat3("Eye", &eye.x, -5.f, 5.f);
		ImGui::SliderFloat3("Focus", &focus.x, -5.f, 5.f);

		auto const& camera = APTR_CAST<CameraActor>(WorldManager::singleton()->FindCamera(TAG{ DEFAULT_CAMERA, 0 }));

		if (eye != focus)
			camera->CreateView(eye, focus, Vector3::UnitY);

		ImGui::NewLine();

		actor = WorldManager::singleton()->FindActor(TAG{ "Player", 0 }).get();

		auto const& transform = CPTR_CAST<Transform>(actor->FindComponent(TAG{ TRANSFORM, 0 }));
		static Vector3 rotation{};
		Quaternion local_rotation = transform->local_rotation();
		Vector3 local_translation = transform->local_translation();

		ImGui::BulletText("Pyramid Modification");

		ImGui::SameLine();
		if (ImGui::Button("Pyramid Reset"))
		{
			rotation = Vector3::Zero;
			local_rotation = Quaternion::Identity;
			local_translation = Vector3::Zero;
		}

		ImGui::SliderFloat3("Rotation", &rotation.x, 0.f, 360.f);
		ImGui::SliderFloat3("Translation", &local_translation.x, -1.f, 1.f);

		transform->set_local_rotation(local_rotation * Quaternion::CreateFromYawPitchRoll(DirectX::XMConvertToRadians(rotation.y), DirectX::XMConvertToRadians(rotation.x), DirectX::XMConvertToRadians(rotation.z)));
		transform->set_local_translation(local_translation);

		ImGui::NewLine();

		Vector4 ambient{};
		Vector4 diffuse{};
		Vector4 specular{};
		Vector3 position{};
		Vector3 direction{};
		Vector3 attenuation{};
		float falloff{};
		int type{};
		float range{};
		float in_angle{};
		float out_angle{};

		//char buffer[64]{};
		//if (ImGui::InputText("Tag", buffer, 64, ImGuiInputTextFlags_EnterReturnsTrue))
		//	actor = WorldManager::singleton()->FindActor(TAG{ buffer, 0 }).get();

		actor = WorldManager::singleton()->FindActor(TAG{ "DefaultLight", 0 }).get();

		if (nullptr == actor)
			return;

		light = CPTR_CAST<Light>(actor->FindComponent(TAG{ LIGHT, 0 }));

		if (nullptr == light)
			return;

		ambient = light->ambient();
		diffuse = light->diffuse();
		specular = light->specular();
		position = light->position();
		direction = light->direction();
		attenuation = light->attenuation();
		falloff = light->falloff();
		type = light->type();
		range = light->range();
		in_angle = light->in_angle();
		out_angle = light->out_angle();

		ImGui::BulletText("Light Modification");

		ImGui::SameLine();
		if (ImGui::Button("Light Reset"))
		{
			ambient = DirectX::Colors::White.v;
			diffuse = DirectX::Colors::White.v;
			specular = DirectX::Colors::White.v;
			position = -Vector3::UnitZ;
			direction = Vector3::UnitZ;
			attenuation = Vector3::One;
			falloff = 10.f;
			type = static_cast<int>(LIGHT_TYPE::SPOT);
			range = 100.f;
			in_angle = 0.f;
			out_angle = 0.f;
		}

		ImGui::SliderFloat4("Ambient", &ambient.x, 0.f, 1.f);
		ImGui::SliderFloat4("Diffuse", &diffuse.x, 0.f, 1.f);
		ImGui::SliderFloat3("Specular", &specular.x, 0.f, 1.f);
		ImGui::SliderFloat("Specular Power", &specular.w, 0.1f, 100.f);
		ImGui::SliderFloat3("Position", &position.x, -1.f, 1.f);
		ImGui::SliderFloat3("Direction", &direction.x, -1.f, 1.f);
		ImGui::SliderFloat3("Attenuation", &attenuation.x, 0.1f, 1.f);
		ImGui::SliderFloat("Falloff", &falloff, 0.1f, 10.f);
		ImGui::SliderInt("Type", &type, 0, 2);
		ImGui::SliderFloat("range", &range, 0.f, 100.f);
		ImGui::SliderFloat("In-angle", &in_angle, 0.f, 90.f);
		ImGui::SliderFloat("Out-angle", &out_angle, 0.f, 90.f);

		light->set_ambient(ambient);
		light->set_diffuse(diffuse);
		light->set_specular(specular);
		light->set_position(position);
		light->set_direction(direction);
		light->set_attenuation(attenuation);
		light->set_falloff(falloff);
		light->set_type(type);
		light->set_range(range);
		light->set_in_angle(in_angle);
		light->set_out_angle(out_angle);
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void K::GUIManager::_Finalize()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();

	ImGui::DestroyContext();
}