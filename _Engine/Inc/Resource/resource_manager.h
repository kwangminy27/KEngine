#pragma once

namespace K
{
	class Mesh;
	class Texture;
	class Sampler;

	struct TempMaterial
	{
		std::string name;
		std::vector<std::string> diffuse_maps;
		std::vector<std::string> specular_maps;
		std::vector<std::string> normal_maps;
		Vector4 ambient;
		Vector4 diffuse;
		Vector4 specular;
		Vector4 emissive;
		float specular_exp;
	};

	class ENGINE_DLL ResourceManager final : public Singleton<ResourceManager>
	{
		friend class Singleton<ResourceManager>;
		friend class Mesh;
	public:
		virtual void Initialize() override;

		std::shared_ptr<Mesh> const& FindMesh(std::string const& _tag) const;
		std::shared_ptr<Texture> const& FindTexture(std::string const& _tag) const;
		std::shared_ptr<Sampler> const& FindSampler(std::string const& _tag) const;
		std::shared_ptr<ANIMATION_2D_CLIP_DESC> const& FindAnimation2DClip(std::string const& _tag) const;
		std::vector<std::unique_ptr<TempMaterial>> const& FindMaterials(std::string const& _tag) const;

		static std::shared_ptr<Mesh> mesh_dummy_;
		static std::shared_ptr<Texture> texture_dummy_;
		static std::shared_ptr<Sampler> sampler_dummy_;
		static std::shared_ptr<ANIMATION_2D_CLIP_DESC> animation_2d_clip_dummy_;
		static std::vector<std::unique_ptr<TempMaterial>> materials_dummy_;

	private:
		ResourceManager() = default;
		ResourceManager(ResourceManager const&) = delete;
		ResourceManager(ResourceManager&&) noexcept = delete;
		ResourceManager& operator=(ResourceManager const&) = delete;
		ResourceManager& operator=(ResourceManager&&) noexcept = delete;

		virtual void _Finalize() override;

		void _LoadAsset(std::string const& _tag, std::wstring const& _file_name);
		void _LoadMesh(std::string const& _tag, std::wstring const& _file_name, std::string const& _path_tag);
		void _LoadMaterial(std::string const& _tag, std::wstring const& _file_name, std::string const& _path_tag);

		void _CreateMesh(
			std::string const& _tag, D3D11_PRIMITIVE_TOPOLOGY _topology,
			void* _vtx_data, int _vtx_stride, int _vtx_count, D3D11_USAGE _vtx_usage);
		void _CreateMesh(
			std::string const& _tag, D3D11_PRIMITIVE_TOPOLOGY _topology,
			void* _vtx_data, int _vtx_stride, int _vtx_count, D3D11_USAGE _vtx_usage,
			void* _idx_data, int _idx_stride, int _idx_count, D3D11_USAGE _idx_usage, DXGI_FORMAT _idx_format);
		void _CreateMesh(
			std::string const& _tag, D3D11_PRIMITIVE_TOPOLOGY _topology,
			void* _vtx_data, int _vtx_stride, int _vtx_count, D3D11_USAGE _vtx_usage,
			void* _idx_data, int _idx_stride, int _idx_count, D3D11_USAGE _idx_usage, DXGI_FORMAT _idx_format,
			void* _inst_data, int _inst_stride, int _inst_count, D3D11_USAGE _inst_usage);
		void _CreateTexture2D(std::string const& _tag, std::wstring const& _file_name, std::string const& _path_tag);
		void _CreateSampler(
			std::string const& _tag,
			D3D11_FILTER _filter,
			D3D11_TEXTURE_ADDRESS_MODE _address_u,
			D3D11_TEXTURE_ADDRESS_MODE _address_v,
			D3D11_TEXTURE_ADDRESS_MODE _address_w);
		void _CreateAnimation2DClip(std::string const& _tag, std::wstring const& _file_name, std::string const& _path_tag);

		void _CreateSphereMesh(std::string const& _tag, float _radius, int _slice_count, int _stack_count);
		void _CreateCylinderMesh(std::string const& _tag, float _height, float _radius, int _slice_count);
		void _CreateCapsuleMesh(std::string const& _tag, float _height, float _radius, int _slice_count, int _stack_count);
		void _CreateSpotlightMesh(std::string const& _tag, float _height, float _radius, int _slice_count, int _stack_count);

		void _CreateSphereVolume(std::string const& _tag, float _radius, int _slice_count, int _stack_count);
		void _CreateSpotlightVolume(std::string const& _tag, float _height, float _radius, int _slice_count, int _stack_count);

		std::unordered_map<std::string, std::shared_ptr<Mesh>> mesh_map_{};
		std::unordered_map<std::string, std::shared_ptr<Texture>> texture_map_{};
		std::unordered_map<std::string, std::shared_ptr<Sampler>> sampler_map_{};
		std::unordered_map<std::string, std::shared_ptr<ANIMATION_2D_CLIP_DESC>> animation_2d_clip_map_{};
		std::unordered_map<std::string, std::vector<std::unique_ptr<TempMaterial>>> materials_map_{};
	};
}
