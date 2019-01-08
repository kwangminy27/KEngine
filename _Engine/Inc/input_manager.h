#pragma once

namespace K
{
	struct KEY_DESC
	{
		std::vector<uint8_t> key_vector;
		bool down;
		bool pressed;
		bool up;
	};

	class ENGINE_DLL InputManager final : public Singleton<InputManager>
	{
		friend class Singleton<InputManager>;
	public:
		virtual void Initialize() override;

		void Update();

		bool KeyDown(std::string const& _tag) const;
		bool KeyPressed(std::string const& _tag) const;
		bool KeyUp(std::string const& _tag) const;

		static std::pair<std::string, KEY_DESC> key_desc_dummy_;

	private:
		InputManager() = default;
		InputManager(InputManager const&) = delete;
		InputManager(InputManager&&) noexcept = delete;
		InputManager& operator=(InputManager const&) = delete;
		InputManager& operator=(InputManager&&) noexcept = delete;

		virtual void _Finalize() override;

		KEY_DESC const& _FindKeyDesc(std::string const& _tag) const;

		template <typename T, typename... Types> void _CreateKeyDesc(T const& _key, Types... _Args);
		template <typename... Types> void _CreateKeyDesc(std::string const& _tag, Types... _Args);
		void _CreateKeyDesc();

		std::unordered_map<std::string, KEY_DESC> key_desc_map_{};
	};
}

#include "input_manager.inl"
