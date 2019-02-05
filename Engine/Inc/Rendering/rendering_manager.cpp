#include "Engine.h"
#include "rendering_manager.h"

#include "device_manager.h"
#include "Resource/resource_manager.h"
#include "Resource/mesh.h"
#include "Resource/sampler.h"
#include "shader.h"
#include "render_state.h"
#include "rasterizer_state.h"
#include "depth_stencil_state.h"
#include "blend_state.h"
#include "render_target.h"
#include "mrt.h"
#include "World/world_manager.h"
#include "Object/Actor/actor.h"
#include "Object/Actor/camera_actor.h"
#include "Object/Component/light.h"

std::shared_ptr<K::Shader> K::RenderingManager::shader_dummy_{};
std::shared_ptr<K::ConstantBuffer> K::RenderingManager::CB_dummy_{};
std::shared_ptr<K::RenderState> K::RenderingManager::render_state_dummy_{};
std::shared_ptr<K::RenderTarget> K::RenderingManager::render_target_dummy_{};
std::shared_ptr<K::MRT> K::RenderingManager::MRT_dummy_{};

void K::RenderingManager::Initialize()
{
	try
	{
#pragma region Shader
		std::vector<CSO_DESC> cso_desc_vector{};
		cso_desc_vector.push_back({ SHADER_TYPE::VERTEX, L"BasicVS.cso" });
		cso_desc_vector.push_back({ SHADER_TYPE::PIXEL, L"BasicPS.cso" });

		std::vector<D3D11_INPUT_ELEMENT_DESC> input_element_desc_vector{};
		input_element_desc_vector.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		input_element_desc_vector.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });

		_CreateShader(BASIC_SHADER, cso_desc_vector, input_element_desc_vector, SHADER_PATH);

		cso_desc_vector.clear();
		cso_desc_vector.push_back({ SHADER_TYPE::VERTEX, L"BasicTexVS.cso" });
		cso_desc_vector.push_back({ SHADER_TYPE::PIXEL, L"BasicTexPS.cso" });

		input_element_desc_vector.clear();
		input_element_desc_vector.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		input_element_desc_vector.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });

		_CreateShader(BASIC_TEX_SHADER, cso_desc_vector, input_element_desc_vector, SHADER_PATH);

		cso_desc_vector.clear();
		cso_desc_vector.push_back({ SHADER_TYPE::VERTEX, L"BasicAnimation2DVS.cso" });
		cso_desc_vector.push_back({ SHADER_TYPE::PIXEL, L"BasicTexPS.cso" });

		input_element_desc_vector.clear();
		input_element_desc_vector.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		input_element_desc_vector.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });

		_CreateShader(BASIC_ANIMATION_2D_SHADER, cso_desc_vector, input_element_desc_vector, SHADER_PATH);

		cso_desc_vector.clear();
		cso_desc_vector.push_back({ SHADER_TYPE::VERTEX, L"BasicNormalVS.cso" });
		cso_desc_vector.push_back({ SHADER_TYPE::PIXEL, L"BasicNormalPS.cso" });

		input_element_desc_vector.clear();
		input_element_desc_vector.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		input_element_desc_vector.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		input_element_desc_vector.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });

		_CreateShader(BASIC_NORMAL_SHADER, cso_desc_vector, input_element_desc_vector, SHADER_PATH);

		cso_desc_vector.clear();
		cso_desc_vector.push_back({ SHADER_TYPE::VERTEX, L"ColliderVS.cso" });
		cso_desc_vector.push_back({ SHADER_TYPE::PIXEL, L"ColliderPS.cso" });

		input_element_desc_vector.clear();
		input_element_desc_vector.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });

		_CreateShader(COLLIDER_SHADER, cso_desc_vector, input_element_desc_vector, SHADER_PATH);

		cso_desc_vector.clear();
		cso_desc_vector.push_back({ SHADER_TYPE::VERTEX, L"InstanceTexVS.cso" });
		cso_desc_vector.push_back({ SHADER_TYPE::PIXEL, L"BasicTexPS.cso" });

		input_element_desc_vector.clear();
		input_element_desc_vector.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		input_element_desc_vector.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 });
		input_element_desc_vector.push_back({ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 });
		input_element_desc_vector.push_back({ "WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 });
		input_element_desc_vector.push_back({ "WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 });
		input_element_desc_vector.push_back({ "WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 });
		input_element_desc_vector.push_back({ "WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 });

		_CreateShader(INSTANCE_TEX_SHADER, cso_desc_vector, input_element_desc_vector, SHADER_PATH);

		cso_desc_vector.clear();
		cso_desc_vector.push_back({ SHADER_TYPE::VERTEX, L"GBufferVS.cso" });
		cso_desc_vector.push_back({ SHADER_TYPE::PIXEL, L"GBufferPS.cso" });

		input_element_desc_vector.clear();
		input_element_desc_vector.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		input_element_desc_vector.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		input_element_desc_vector.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });

		_CreateShader(GBUFFER_SHADER, cso_desc_vector, input_element_desc_vector, SHADER_PATH);

		cso_desc_vector.clear();
		cso_desc_vector.push_back({ SHADER_TYPE::VERTEX, L"DeferredLightingVS.cso" });
		cso_desc_vector.push_back({ SHADER_TYPE::PIXEL, L"DeferredLightingPS.cso" });

		input_element_desc_vector.clear();
		input_element_desc_vector.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });

		_CreateShader(DEFERRED_LIGHTING_SHADER, cso_desc_vector, input_element_desc_vector, SHADER_PATH);

		//DeferredLightingCalculateColorPS

		cso_desc_vector.clear();
		cso_desc_vector.push_back({ SHADER_TYPE::VERTEX, L"DeferredLightingCalculateColorVS.cso" });
		cso_desc_vector.push_back({ SHADER_TYPE::PIXEL, L"DeferredLightingCalculateColorPS.cso" });

		input_element_desc_vector.clear();
		input_element_desc_vector.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });

		_CreateShader(DEFERRED_LIGHTING_CALCULATE_COLOR, cso_desc_vector, input_element_desc_vector, SHADER_PATH);
#pragma endregion

#pragma region RenderState
		_CreateRasterizerState(
			RS_LIGHT_VOLUME_PASS_1,
			D3D11_FILL_SOLID,
			D3D11_CULL_FRONT,
			false,
			0,
			0.f,
			0.f,
			true,
			false,
			false,
			false
		);

		_CreateRasterizerState(
			RS_LIGHT_VOLUME_PASS_2,
			D3D11_FILL_SOLID,
			D3D11_CULL_BACK,
			false,
			0,
			0.f,
			0.f,
			true,
			false,
			false,
			false
		);

		_CreateRasterizerState(
			RS_WIREFRAME_CULL_BACK,
			D3D11_FILL_WIREFRAME,
			D3D11_CULL_BACK,
			false,
			0,
			0.f,
			0.f,
			true,
			false,
			false,
			false
		);

		_CreateRasterizerState(
			RS_WIREFRAME_CULL_FRONT,
			D3D11_FILL_WIREFRAME,
			D3D11_CULL_FRONT,
			false,
			0,
			0.f,
			0.f,
			true,
			false,
			false,
			false
		);

		_CreateRasterizerState(
			RS_WIREFRAME_CULL_NONE,
			D3D11_FILL_WIREFRAME,
			D3D11_CULL_NONE,
			false,
			0,
			0.f,
			0.f,
			true,
			false,
			false,
			false
		);

		D3D11_DEPTH_STENCILOP_DESC front_face{};
		front_face.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		front_face.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		front_face.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		front_face.StencilFunc = D3D11_COMPARISON_NEVER;

		D3D11_DEPTH_STENCILOP_DESC back_face{};
		back_face.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		back_face.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		back_face.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		back_face.StencilFunc = D3D11_COMPARISON_NEVER;

		_CreateDepthStencilState(
			DEPTH_DISABLE,
			false, D3D11_DEPTH_WRITE_MASK_ZERO, D3D11_COMPARISON_NEVER,
			false, D3D11_DEFAULT_STENCIL_READ_MASK, D3D11_DEFAULT_STENCIL_WRITE_MASK, front_face, back_face
		);

		front_face = {};
		front_face.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		front_face.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		front_face.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		front_face.StencilFunc = D3D11_COMPARISON_NEVER;

		back_face = {};
		back_face.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		back_face.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		back_face.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		back_face.StencilFunc = D3D11_COMPARISON_ALWAYS;

		_CreateDepthStencilState(
			DSS_LIGHT_VOLUME_PASS_1,
			true, D3D11_DEPTH_WRITE_MASK_ZERO, D3D11_COMPARISON_GREATER,
			false, D3D11_DEFAULT_STENCIL_READ_MASK, D3D11_DEFAULT_STENCIL_WRITE_MASK, front_face, back_face
		);

		front_face = {};
		front_face.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		front_face.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		front_face.StencilPassOp = D3D11_STENCIL_OP_ZERO;
		front_face.StencilFunc = D3D11_COMPARISON_EQUAL;

		back_face = {};
		back_face.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		back_face.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		back_face.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		back_face.StencilFunc = D3D11_COMPARISON_NEVER;

		_CreateDepthStencilState(
			DSS_LIGHT_VOLUME_PASS_2,
			true, D3D11_DEPTH_WRITE_MASK_ZERO, D3D11_COMPARISON_LESS,
			true, D3D11_DEFAULT_STENCIL_READ_MASK, D3D11_DEFAULT_STENCIL_WRITE_MASK, front_face, back_face
		);

		std::vector<D3D11_RENDER_TARGET_BLEND_DESC> render_target_blend_desc_vector{};

		D3D11_RENDER_TARGET_BLEND_DESC rtbd{};
		rtbd.BlendEnable = true;
		rtbd.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		rtbd.BlendOp = D3D11_BLEND_OP_ADD;
		rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
		rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
		rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		render_target_blend_desc_vector.push_back(rtbd);

		_CreateBlendState(ALPHA_BLEND, false, false, render_target_blend_desc_vector);

		render_target_blend_desc_vector.clear();

		rtbd = {};
		rtbd.BlendEnable = true;
		rtbd.SrcBlend = D3D11_BLEND_ONE;
		rtbd.DestBlend = D3D11_BLEND_ONE;
		rtbd.BlendOp = D3D11_BLEND_OP_ADD;
		rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
		rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
		rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		render_target_blend_desc_vector.push_back(rtbd);

		_CreateBlendState(LIGHT_BLEND, false, false, render_target_blend_desc_vector);
#pragma endregion

#pragma region ConstantBuffer
		_CreateConstantBuffer(TRANSFORM, 0, sizeof(TransformConstantBuffer), static_cast<uint8_t>(SHADER_TYPE::VERTEX) | static_cast<uint8_t>(SHADER_TYPE::PIXEL));
		_CreateConstantBuffer(MATERIAL, 1, sizeof(MaterialConstantBuffer), static_cast<uint8_t>(SHADER_TYPE::VERTEX) | static_cast<uint8_t>(SHADER_TYPE::PIXEL));
		_CreateConstantBuffer(ANIMATION_2D, 2, sizeof(ANIMATION_2D_FRAME_DESC), static_cast<uint8_t>(SHADER_TYPE::VERTEX) | static_cast<uint8_t>(SHADER_TYPE::PIXEL));
		_CreateConstantBuffer(COLLIDER, 3, sizeof(Vector4), static_cast<uint8_t>(SHADER_TYPE::VERTEX) | static_cast<uint8_t>(SHADER_TYPE::PIXEL));
		_CreateConstantBuffer(LIGHT, 4, sizeof(LightConstantBuffer), static_cast<uint8_t>(SHADER_TYPE::VERTEX) | static_cast<uint8_t>(SHADER_TYPE::PIXEL));
		_CreateConstantBuffer(COMMON, 5, sizeof(CommonConstantBuffer), static_cast<uint8_t>(SHADER_TYPE::VERTEX) | static_cast<uint8_t>(SHADER_TYPE::PIXEL));
#pragma endregion

#pragma region RenderTarget
		//_CreateRenderTarget(BASIC_RENDER_TARGET, Vector3{ static_cast<float>(RESOLUTION::WIDTH), static_cast<float>(RESOLUTION::HEIGHT), 1.f }, Vector3{ 0.f, 0.f, 5.f }, DXGI_FORMAT_R8G8B8A8_UNORM);

		_CreateRenderTarget(ALBEDO_RENDER_TARGET, Vector3{ 100.f, 100.f, 1.f }, Vector3{ 750.f, -400.f, 5.f }, DXGI_FORMAT_R32G32B32A32_FLOAT);
		_CreateRenderTarget(NORMAL_RENDER_TARGET, Vector3{ 100.f, 100.f, 1.f }, Vector3{ 750.f, -300.f, 5.f }, DXGI_FORMAT_R32G32B32A32_FLOAT);
		_CreateRenderTarget(DEPTH_RENDER_TARGET, Vector3{ 100.f, 100.f, 1.f }, Vector3{ 750.f, -200.f, 5.f }, DXGI_FORMAT_R32G32B32A32_FLOAT);
		_CreateRenderTarget(MATERIAL_RENDER_TARGET, Vector3{ 100.f, 100.f, 1.f }, Vector3{ 750.f, -100.f, 5.f }, DXGI_FORMAT_R32G32B32A32_FLOAT);

		_CreateRenderTarget(LIGHT_AMBIENT_RENDER_TARGET, Vector3{ 100.f, 100.f, 1.f }, Vector3{ 650.f, -400.f, 5.f }, DXGI_FORMAT_R32G32B32A32_FLOAT);
		_CreateRenderTarget(LIGHT_DIFFUSE_RENDER_TARGET, Vector3{ 100.f, 100.f, 1.f }, Vector3{ 650.f, -300.f, 5.f }, DXGI_FORMAT_R32G32B32A32_FLOAT);
		_CreateRenderTarget(LIGHT_SPECULAR_RENDER_TARGET, Vector3{ 100.f, 100.f, 1.f }, Vector3{ 650.f, -200.f, 5.f }, DXGI_FORMAT_R32G32B32A32_FLOAT);

		_CreateRenderTarget(DEFERRED_LIGHTING_RENDER_TARGET, Vector3{ 100.f, 100.f, 1.f }, Vector3{ 550.f, -400.f, 5.f }, DXGI_FORMAT_R32G32B32A32_FLOAT);

		_CreateMRT(GBUFFER_MRT);
		AddRTV(GBUFFER_MRT, ALBEDO_RENDER_TARGET);
		AddRTV(GBUFFER_MRT, NORMAL_RENDER_TARGET);
		AddRTV(GBUFFER_MRT, DEPTH_RENDER_TARGET);
		AddRTV(GBUFFER_MRT, MATERIAL_RENDER_TARGET);

		_CreateMRT(LIGHT_MRT);
		AddRTV(LIGHT_MRT, LIGHT_AMBIENT_RENDER_TARGET);
		AddRTV(LIGHT_MRT, LIGHT_DIFFUSE_RENDER_TARGET);
		AddRTV(LIGHT_MRT, LIGHT_SPECULAR_RENDER_TARGET);
#pragma endregion

		for (auto& render_group : render_group_array_)
			render_group.reserve(100);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "RenderingManager::Initialize" << std::endl;
	}
}

std::shared_ptr<K::Shader> const& K::RenderingManager::FindShader(std::string const& _tag) const
{
	auto iter = shader_map_.find(_tag);

	if (iter == shader_map_.end())
		return shader_dummy_;

	return iter->second;
}

std::shared_ptr<K::RenderState> const& K::RenderingManager::FindRenderState(std::string const& _tag) const
{
	auto iter = render_state_map_.find(_tag);

	if (iter == render_state_map_.end())
		return render_state_dummy_;

	return iter->second;
}

std::shared_ptr<K::ConstantBuffer> const& K::RenderingManager::FindConstantBuffer(std::string const& _tag) const
{
	auto iter = CB_map_.find(_tag);

	if (iter == CB_map_.end())
		return CB_dummy_;

	return iter->second;
}

std::shared_ptr<K::RenderTarget> const& K::RenderingManager::FindRenderTarget(std::string const& _tag) const
{
	auto iter = render_target_map_.find(_tag);

	if (iter == render_target_map_.end())
		return render_target_dummy_;

	return iter->second;
}

std::shared_ptr<K::MRT> const& K::RenderingManager::FindMRT(std::string const& _tag) const
{
	auto iter = MRT_map_.find(_tag);

	if (iter == MRT_map_.end())
		return MRT_dummy_;

	return iter->second;
}

void K::RenderingManager::UpdateConstantBuffer(std::string const& _tag, void* _data)
{
	auto const& context = DeviceManager::singleton()->context();
	auto const& CB = FindConstantBuffer(_tag);

	if (!CB)
		throw std::exception{ "RenderingManager::UpdateConstantBuffer" };

	D3D11_MAPPED_SUBRESOURCE ms{};
	ThrowIfFailed(context->Map(CB->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &ms));

	memcpy_s(ms.pData, CB->size, _data, CB->size);

	context->Unmap(CB->buffer.Get(), 0);

	if (CB->shader_flag & static_cast<int>(SHADER_TYPE::VERTEX))
		context->VSSetConstantBuffers(CB->slot, 1, CB->buffer.GetAddressOf());

	if (CB->shader_flag & static_cast<int>(SHADER_TYPE::PIXEL))
		context->PSSetConstantBuffers(CB->slot, 1, CB->buffer.GetAddressOf());
}

void K::RenderingManager::Render(float _time)
{
	_UpdateCommonConstantBuffer(_time);

	switch (mode_)
	{
	case K::GAME_MODE::_2D:
		_Render2D(_time);
		break;
	case K::GAME_MODE::FORWARD:
		_RenderForward(_time);
		break;
	case K::GAME_MODE::DEFERRED:
		_RenderDeferred(_time);
		break;
	}

	for (auto& e : render_target_map_)
		e.second->Render(_time);

	for (auto& render_group : render_group_array_)
		render_group.clear();
}

void K::RenderingManager::AddActor(APTR const& _actor)
{
	auto render_group_type = _actor->render_group_type();

	switch (render_group_type)
	{
	case RENDER_GROUP_TYPE::MAX:
		return;
	}

	render_group_array_.at(static_cast<int>(render_group_type)).push_back(_actor.get());
}

void K::RenderingManager::AddRTV(std::string const& _MRT_tag, std::string const& _render_target_tag)
{
	auto _MRT = FindMRT(_MRT_tag);

	if (nullptr == _MRT)
		throw std::exception{ "RenderingManager::AddRTV" };

	_MRT->AddRTV(_render_target_tag);
}

void K::RenderingManager::AddDSV(std::string const& _MRT_tag, std::string const& _render_target_tag)
{
	auto _MRT = FindMRT(_MRT_tag);

	if (nullptr == _MRT)
		throw std::exception{ "RenderingManager::AddRTV" };

	_MRT->AddDSV(_render_target_tag);
}

K::GAME_MODE K::RenderingManager::mode() const
{
	return mode_;
}

void K::RenderingManager::set_mode(GAME_MODE _mode)
{
	mode_ = _mode;
}

void K::RenderingManager::_Finalize()
{
}

void K::RenderingManager::_UpdateCommonConstantBuffer(float _time)
{
	D3D11_VIEWPORT viewport{};
	uint32_t viewport_count = 1;
	DeviceManager::singleton()->context()->RSGetViewports(&viewport_count, &viewport);

	CommonConstantBuffer common_CB{};
	common_CB.viewport = Vector2{ viewport.Width, viewport.Height };
	common_CB.time = _time;

	UpdateConstantBuffer(COMMON, &common_CB);
}

void K::RenderingManager::_CreateShader(
	std::string const& _tag,
	std::vector<CSO_DESC> const& _cso_desc_vector,
	std::vector<D3D11_INPUT_ELEMENT_DESC> const& _input_element_desc_vector,
	std::string const& _path_tag)
{
	if (FindShader(_tag))
		throw std::exception{ "RenderingManager::_CreateShader" };

	auto shader = std::shared_ptr<Shader>{ new Shader, [](Shader* _p) {
		delete _p;
	} };

	shader->_CreateShader(
		_cso_desc_vector,
		_input_element_desc_vector,
		_path_tag
	);

	shader_map_.insert(make_pair(_tag, std::move(shader)));
}

void K::RenderingManager::_CreateRasterizerState(
	std::string const& _tag,
	D3D11_FILL_MODE _fill_mode,
	D3D11_CULL_MODE _cull_mode,
	bool _front_counter_clock_wise,
	int _depth_bias,
	float _depth_bias_clamp,
	float _slope_scale_depth_bias,
	bool _depth_clip_enable,
	bool _scissor_enable,
	bool _multi_sample_enable,
	bool _anti_aliased_line_enable)
{
	auto render_state = std::shared_ptr<RenderState>{ new RasterizerState, [](RasterizerState* _p) {
		delete _p;
	} };

	std::static_pointer_cast<RasterizerState>(render_state)->_CreateState(
		_fill_mode,
		_cull_mode,
		_front_counter_clock_wise,
		_depth_bias,
		_depth_bias_clamp,
		_slope_scale_depth_bias,
		_depth_clip_enable,
		_scissor_enable,
		_multi_sample_enable,
		_anti_aliased_line_enable
	);

	render_state_map_.insert(std::make_pair(_tag, std::move(render_state)));
}

void K::RenderingManager::_CreateDepthStencilState(
	std::string const& _tag,
	BOOL _depth_enable,
	D3D11_DEPTH_WRITE_MASK _depth_write_mask,
	D3D11_COMPARISON_FUNC _depth_func,
	BOOL _stencil_enable,
	UINT8 _stencil_read_mask,
	UINT8 _stencil_write_mask,
	D3D11_DEPTH_STENCILOP_DESC const& _front_face,
	D3D11_DEPTH_STENCILOP_DESC const& _back_face)
{
	auto render_state = std::shared_ptr<RenderState>{ new DepthStencilState, [](DepthStencilState* _p) {
		delete _p;
	} };

	std::static_pointer_cast<DepthStencilState>(render_state)->_CreateState(
		_depth_enable,
		_depth_write_mask,
		_depth_func,
		_stencil_enable,
		_stencil_read_mask,
		_stencil_write_mask,
		_front_face,
		_back_face
	);

	render_state_map_.insert(std::make_pair(_tag, std::move(render_state)));
}

void K::RenderingManager::_CreateBlendState(
	std::string const& _tag,
	bool _alpha_to_coverage_enable,
	bool _independent_blend_enable,
	std::vector<D3D11_RENDER_TARGET_BLEND_DESC> const& _render_target_blend_desc_vector)
{
	auto render_state = std::shared_ptr<RenderState>{ new BlendState, [](BlendState* _p) {
		delete _p;
	} };

	std::static_pointer_cast<BlendState>(render_state)->_CreateState(
		_alpha_to_coverage_enable,
		_independent_blend_enable,
		_render_target_blend_desc_vector
	);

	render_state_map_.insert(std::make_pair(_tag, std::move(render_state)));
}

void K::RenderingManager::_CreateConstantBuffer(std::string const& _tag, uint32_t _slot, uint32_t _size, uint8_t _shader_flag)
{
	auto const& device = DeviceManager::singleton()->device();

	if (FindConstantBuffer(_tag))
		throw std::exception{ "RenderingManager::_CreateConstantBuffer" };

	auto CB = std::shared_ptr<ConstantBuffer>{ new ConstantBuffer, [](ConstantBuffer* _p) {
		delete _p;
	} };

	CB->slot = _slot;
	CB->size = _size;
	CB->shader_flag = _shader_flag;

	D3D11_BUFFER_DESC dbd{};
	dbd.ByteWidth = _size;
	dbd.Usage = D3D11_USAGE_DYNAMIC;
	dbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	dbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	ThrowIfFailed(device->CreateBuffer(&dbd, nullptr, &CB->buffer));

	CB_map_.insert(std::make_pair(_tag, std::move(CB)));
}

void K::RenderingManager::_CreateRenderTarget(std::string const& _tag, Vector3 const& _scaling, Vector3 const& _translation, DXGI_FORMAT _format)
{
	if (FindRenderTarget(_tag))
		throw std::exception{ "RenderingManager::_CreateRenderTarget" };

	auto render_target = std::shared_ptr<RenderTarget>{ new RenderTarget, [](RenderTarget* _p) {
		delete _p;
	} };

	render_target->_CreateRenderTarget(_scaling, _translation, _format);

	render_target_map_.insert(std::make_pair(_tag, std::move(render_target)));
}

void K::RenderingManager::_CreateMRT(std::string const& _tag)
{
	if (FindMRT(_tag))
		throw std::exception{ "RenderingManager::_CreateMRT" };

	auto _MRT = std::shared_ptr<MRT>{ new MRT, [](MRT* _p) {
		delete _p;
	} };

	MRT_map_.insert(std::make_pair(_tag, std::move(_MRT)));
}

void K::RenderingManager::_Render2D(float _time)
{
	auto const& render_target = FindRenderTarget(BASIC_RENDER_TARGET);

	render_target->Clear();
	render_target->SetTarget();
	{
		for (int i = 0; i <= static_cast<int>(RENDER_GROUP_TYPE::HUD); ++i)
		{
			for (auto const& actor : render_group_array_.at(i))
				actor->__Render(_time);
		}
	}
	render_target->ResetTarget();
}

void K::RenderingManager::_RenderForward(float _time)
{
	auto const& render_target = FindRenderTarget(BASIC_RENDER_TARGET);

	render_target->Clear();
	render_target->SetTarget();
	{
		if (false == render_group_array_.at(static_cast<int>(RENDER_GROUP_TYPE::LIGHT)).empty())
		{
			auto const& light = render_group_array_.at(static_cast<int>(RENDER_GROUP_TYPE::LIGHT)).at(0)->FindComponent(TAG{ LIGHT, 0 });
			CPTR_CAST<Light>(light)->UpdateConstantBuffer();
		}

		for (int i = 0; i <= static_cast<int>(RENDER_GROUP_TYPE::HUD); ++i)
		{
			for (auto const& actor : render_group_array_.at(i))
				actor->__Render(_time);
		}
	}
	render_target->ResetTarget();
}

void K::RenderingManager::_RenderDeferred(float _time)
{
	_RenderGBuffer(_time);
	_RenderLight(_time);

	auto const& deferred_lighting_render_target = FindRenderTarget(DEFERRED_LIGHTING_RENDER_TARGET);

	deferred_lighting_render_target->Clear();
	deferred_lighting_render_target->SetTarget();
	{
		auto const& gbuffer_mrt = FindMRT(GBUFFER_MRT);
		auto const& light_mrt = FindMRT(LIGHT_MRT);

		gbuffer_mrt->Attach(8);
		light_mrt->Attach(12);
		{
			auto const& resource_manager = ResourceManager::singleton();

			resource_manager->FindSampler(POINT_SAMPLER)->SetToShader(1);

			FindShader(DEFERRED_LIGHTING_CALCULATE_COLOR)->SetToShader();
			resource_manager->FindMesh(FULL_SCREEN_RECT)->Render();
		}
		light_mrt->Detach(12);
		gbuffer_mrt->Detach(8);
	}
	deferred_lighting_render_target->ResetTarget();

	deferred_lighting_render_target->RenderFullScreen(_time);

	for (int i = static_cast<int>(RENDER_GROUP_TYPE::UI); i <= static_cast<int>(RENDER_GROUP_TYPE::HUD); ++i)
	{
		for (auto const& actor : render_group_array_.at(i))
			actor->__Render(_time);
	}
}

void K::RenderingManager::_RenderGBuffer(float _time)
{
	auto const& gbuffer_mrt = FindMRT(GBUFFER_MRT);

	gbuffer_mrt->Clear();
	gbuffer_mrt->SetTarget();
	{
		for (int i = 0; i <= static_cast<int>(RENDER_GROUP_TYPE::NORMAL); ++i)
		{
			for (auto const& actor : render_group_array_.at(i))
				actor->__Render(_time);
		}
	}
	gbuffer_mrt->ResetTarget();
}

void K::RenderingManager::_RenderLight(float _time)
{
	auto const& light_mrt = FindMRT(LIGHT_MRT);

	light_mrt->Clear();
	light_mrt->SetTarget();
	{
		auto const& gbuffer_mrt = FindMRT(GBUFFER_MRT);

		gbuffer_mrt->Attach(8);
		{
			for (auto const& actor : render_group_array_.at(static_cast<int>(RENDER_GROUP_TYPE::LIGHT)))
				actor->_Render(_time);
		}
		gbuffer_mrt->Detach(8);
	}
	light_mrt->ResetTarget();
}
