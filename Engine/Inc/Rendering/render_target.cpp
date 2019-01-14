#include "Engine.h"
#include "render_target.h"

#include "device_manager.h"
#include "Resource/resource_manager.h"
#include "Resource/mesh.h"
#include "Resource/sampler.h"
#include "rendering_manager.h"
#include "shader.h"
#include "World/world_manager.h"
#include "Object/Actor/camera_actor.h"

void K::RenderTarget::Render(float _time)
{
	auto const& resource_manager = ResourceManager::singleton();
	auto const& rendering_manager = RenderingManager::singleton();

	auto const& camera = WorldManager::singleton()->FindCamera(TAG{ DEFAULT_CAMERA, 0 });

	TransformConstantBuffer transform_CB{};
	transform_CB.world = Matrix::CreateScaling(scaling_) * Matrix::CreateTranslation(translation_);
	transform_CB.view = camera->view();
	transform_CB.projection = camera->projection();
	transform_CB.WV = transform_CB.world * transform_CB.view;
	transform_CB.WVP = transform_CB.world * transform_CB.view * transform_CB.projection;

	transform_CB.world = transform_CB.world.Transpose();
	transform_CB.view = transform_CB.view.Transpose();
	transform_CB.projection = transform_CB.projection.Transpose();
	transform_CB.WV = transform_CB.WV.Transpose();
	transform_CB.WVP = transform_CB.WVP.Transpose();

	rendering_manager->UpdateConstantBuffer(TRANSFORM, &transform_CB);

	rendering_manager->FindShader(shader_tag_)->SetToShader();

	SetToShader(0);

	resource_manager->FindSampler(sampler_tag_)->SetToShader(0);

	resource_manager->FindMesh(TEX_RECT)->Render();
}

void K::RenderTarget::Clear()
{
	auto const& context = DeviceManager::singleton()->context();

	context->ClearRenderTargetView(RTV_.Get(), DirectX::Colors::MediumSlateBlue);
	context->ClearDepthStencilView(DSV_.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void K::RenderTarget::SetTarget()
{
	auto const& context = DeviceManager::singleton()->context();
	
	context->OMGetRenderTargets(1, &old_RTV_, &old_DSV_);

	auto DSV = old_DSV_;

	if (DSV_)
		DSV = DSV_;

	context->OMSetRenderTargets(1, RTV_.GetAddressOf(), DSV.Get());
}

void K::RenderTarget::ResetTarget()
{
	DeviceManager::singleton()->context()->OMSetRenderTargets(1, old_RTV_.GetAddressOf(), old_DSV_.Get());
}

void K::RenderTarget::SetToShader(int _slot)
{
	DeviceManager::singleton()->context()->PSSetShaderResources(_slot, 1, SRV_.GetAddressOf());
}

void K::RenderTarget::set_mesh_tag(std::string const& _tag)
{
	mesh_tag_ = _tag;
}

void K::RenderTarget::set_shader_tag(std::string const& _tag)
{
	shader_tag_ = _tag;
}

void K::RenderTarget::set_sampler_tag(std::string const& _tag)
{
	sampler_tag_ = _tag;
}

K::RenderTarget::RenderTarget(RenderTarget const& _other)
{
	scaling_ = _other.scaling_;
	translation_ = _other.translation_;
	mesh_tag_ = _other.mesh_tag_;
	shader_tag_ = _other.shader_tag_;
	sampler_tag_ = _other.sampler_tag_;
	RTV_ = _other.RTV_;
	DSV_ = _other.DSV_;
	SRV_ = _other.SRV_;
	old_RTV_ = _other.old_RTV_;
	old_DSV_ = _other.old_DSV_;
}

K::RenderTarget::RenderTarget(RenderTarget&& _other) noexcept
{
	scaling_ = std::move(_other.scaling_);
	translation_ = std::move(_other.translation_);
	mesh_tag_ = std::move(_other.mesh_tag_);
	shader_tag_ = std::move(_other.shader_tag_);
	sampler_tag_ = std::move(_other.sampler_tag_);
	RTV_ = std::move(_other.RTV_);
	DSV_ = std::move(_other.DSV_);
	SRV_ = std::move(_other.SRV_);
	old_RTV_ = std::move(_other.old_RTV_);
	old_DSV_ = std::move(_other.old_DSV_);
}

void K::RenderTarget::_CreateRenderTarget(Vector3 const& _scaling, Vector3 const& _translation)
{
	auto const& device = DeviceManager::singleton()->device();

	scaling_ = _scaling;
	translation_ = _translation;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture{};

	D3D11_TEXTURE2D_DESC dtd{};
	dtd.Width = static_cast<int>(RESOLUTION::WIDTH);
	dtd.Height = static_cast<int>(RESOLUTION::HEIGHT);
	dtd.ArraySize = 1;
	dtd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dtd.SampleDesc.Count = 1;
	dtd.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	ThrowIfFailed(device->CreateTexture2D(&dtd, nullptr, &texture));
	ThrowIfFailed(device->CreateRenderTargetView(texture.Get(), nullptr, &RTV_));
	ThrowIfFailed(device->CreateShaderResourceView(texture.Get(), nullptr, &SRV_));

	dtd = {};
	dtd.Width = static_cast<int>(RESOLUTION::WIDTH);
	dtd.Height = static_cast<int>(RESOLUTION::HEIGHT);
	dtd.ArraySize = 1;
	dtd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dtd.SampleDesc.Count = 1;
	dtd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ThrowIfFailed(device->CreateTexture2D(&dtd, nullptr, &texture));
	ThrowIfFailed(device->CreateDepthStencilView(texture.Get(), nullptr, &DSV_));

	set_mesh_tag(TEX_RECT);
	set_shader_tag(BASIC_TEX_SHADER);
	set_sampler_tag(LINEAR_SAMPLER);
}
