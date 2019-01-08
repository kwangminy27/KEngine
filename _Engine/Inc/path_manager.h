#pragma once

namespace K
{
	class ENGINE_DLL PathManager final : public Singleton<PathManager>
	{
		friend class Singleton<PathManager>;
	public:
		virtual void Initialize() override;

		std::filesystem::path const& FindPath(std::string const& _tag) const;

		static std::filesystem::path path_dummy_;

	private:
		PathManager() = default;
		PathManager(PathManager const&) = delete;
		PathManager(PathManager&&) noexcept = delete;
		PathManager& operator=(PathManager const&) = delete;
		PathManager& operator=(PathManager&&) noexcept = delete;

		virtual void _Finalize() override;

		void _CreateRootPath();
		void _CreatePath(std::string const& _tag, std::filesystem::path const& _path, std::string const& _base_path_tag = ROOT_PATH);

		std::unordered_map<std::string, std::filesystem::path> path_map_{};
	};
}
