#pragma once

#include "component.h"

namespace K
{
	class Texture;
	class Sampler;

	struct MaterialSubset
	{
		std::vector<std::shared_ptr<Texture>> texture_vector;
		std::vector<std::shared_ptr<Sampler>> sampler_vector;
		MaterialConstantBuffer material_CB;
	};

	using MaterialContainer = std::vector<MaterialSubset>;

	class ENGINE_DLL Material final : public Component
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual CPTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

		void SetToShader(int _container_idx, int _subset_idx);

		void SetTexture(std::string const& _tag, int _slot, int _container_idx, int _subset_idx);
		void SetSampler(std::string const& _tag, int _slot, int _container_idx, int _subset_idx);
		void SetMaterialConstantBuffer(MaterialConstantBuffer const& _material_CB, int _container_idx, int _subset_idx);

	private:
		Material() = default;
		Material(Material const& _other);
		Material(Material&& _other) noexcept;
		Material& operator=(Material const&) = delete;
		Material& operator=(Material&&) noexcept = default;

		virtual void _Finalize() override;

		std::vector<std::unique_ptr<MaterialContainer, std::function<void(MaterialContainer*)>>> material_container_vector_{};
	};
}
