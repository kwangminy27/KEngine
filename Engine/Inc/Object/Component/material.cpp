#include "Engine.h"
#include "material.h"

#include "Resource/resource_manager.h"
#include "Resource/texture.h"
#include "Resource/Sampler.h"
#include "Rendering/rendering_manager.h"

void K::Material::Initialize()
{
	try
	{
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Material::Initialize" << std::endl;
	}
}

K::CPTR K::Material::Clone() const
{
	return CPTR{ new Material{ *this }, [](Component* _p) {
		static_cast<Material*>(_p)->_Finalize();
		delete static_cast<Material*>(_p);
	} };
}

void K::Material::Serialize(InputMemoryStream& _imstream)
{
}

void K::Material::Serialize(OutputMemoryStream& _omstream)
{
}

void K::Material::SetToShader(int _container_idx, int _subset_idx)
{
	auto& material_subset = material_container_vector_.at(_container_idx)->at(_subset_idx);

	for (auto i = 0; i < material_subset.texture_vector.size(); ++i)
	{
		if (auto const& texture = material_subset.texture_vector.at(i))
			texture->SetToShader(i);
	}

	for (auto i = 0; i < material_subset.sampler_vector.size(); ++i)
	{
		if (auto const& sampler = material_subset.sampler_vector.at(i))
			sampler->SetToShader(i);
	}

	RenderingManager::singleton()->UpdateConstantBuffer(MATERIAL, &material_subset.material_CB);
}

void K::Material::SetTexture(std::string const& _tag, int _slot, int _container_idx, int _subset_idx)
{
	if (false == ResourceManager::singleton()->FindTexture(_tag))
		return;

	if (_container_idx >= material_container_vector_.size())
		material_container_vector_.resize(_container_idx + 1);

	if (nullptr == material_container_vector_.at(_container_idx))
	{
		material_container_vector_.at(_container_idx) = std::unique_ptr<MaterialContainer, std::function<void(MaterialContainer*)>>{ new MaterialContainer, [](MaterialContainer* _p) {
			delete _p;
		} };
	}

	if (_subset_idx >= material_container_vector_.at(_container_idx)->size())
		material_container_vector_.at(_container_idx)->resize(_subset_idx + 1);

	auto& material_subset = material_container_vector_.at(_container_idx)->at(_subset_idx);

	if (_slot >= material_subset.texture_vector.capacity())
		material_subset.texture_vector.resize(_slot + 1);

	material_subset.texture_vector.at(_slot) = ResourceManager::singleton()->FindTexture(_tag);
}

void K::Material::SetSampler(std::string const& _tag, int _slot, int _container_idx, int _subset_idx)
{
	if (_container_idx >= material_container_vector_.size())
		material_container_vector_.resize(_container_idx + 1);

	if (nullptr == material_container_vector_.at(_container_idx))
	{
		material_container_vector_.at(_container_idx) = std::unique_ptr<MaterialContainer, std::function<void(MaterialContainer*)>>{ new MaterialContainer, [](MaterialContainer* _p) {
			delete _p;
		} };
	}

	if (_subset_idx >= material_container_vector_.at(_container_idx)->size())
		material_container_vector_.at(_container_idx)->resize(_subset_idx + 1);

	auto& material_subset = material_container_vector_.at(_container_idx)->at(_subset_idx);

	if (_slot >= material_subset.sampler_vector.capacity())
		material_subset.sampler_vector.resize(_slot + 1);

	material_subset.sampler_vector.at(_slot) = ResourceManager::singleton()->FindSampler(_tag);
}

void K::Material::SetMaterialConstantBuffer(MaterialConstantBuffer const& _material_CB, int _container_idx, int _subset_idx)
{
	if (_container_idx >= material_container_vector_.size())
		material_container_vector_.resize(_container_idx + 1);

	if (nullptr == material_container_vector_.at(_container_idx))
	{
		material_container_vector_.at(_container_idx) = std::unique_ptr<MaterialContainer, std::function<void(MaterialContainer*)>>{ new MaterialContainer, [](MaterialContainer* _p) {
			delete _p;
		} };
	}

	if (_subset_idx >= material_container_vector_.at(_container_idx)->size())
		material_container_vector_.at(_container_idx)->resize(_subset_idx + 1);

	material_container_vector_.at(_container_idx)->at(_subset_idx).material_CB = _material_CB;
}

K::Material::Material(Material const& _other) : Component(_other)
{
	material_container_vector_.clear();
	for (auto const& material_container : _other.material_container_vector_)
	{
		material_container_vector_.push_back(std::unique_ptr<MaterialContainer, std::function<void(MaterialContainer*)>>{ new MaterialContainer{ *material_container.get() }, [](MaterialContainer* _p) {
			delete _p;
		} });
	}
}

K::Material::Material(Material&& _other) noexcept : Component(std::move(_other))
{
	material_container_vector_ = std::move(_other.material_container_vector_);
}

void K::Material::_Finalize()
{
}
