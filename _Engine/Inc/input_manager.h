#pragma once

namespace K
{
	struct KEYBOARD_STATE
	{
		bool down[MAX_KEY_SIZE];
		bool press[MAX_KEY_SIZE];
		bool up[MAX_KEY_SIZE];
		uint8_t key[MAX_KEY_SIZE];
	};

	struct KEY_STATE
	{
		bool down{};
		bool press;
		bool up;
		uint8_t key;
		std::vector<uint8_t> combination_key;
	};

	class ENGINE_DLL InputManager final : public Singleton<InputManager>
	{
		friend class Singleton<InputManager>;
	public:
		virtual void Initialize() override;

		void Initialize(HINSTANCE _instance, HWND _window);

		void Update(float _time);

		bool KeyDown(std::string const& _tag) const;
		bool KeyPress(std::string const& _tag) const;
		bool KeyUp(std::string const& _tag) const;

		static std::unique_ptr<KEY_STATE, std::function<void(KEY_STATE*)>> key_state_dummy_;

	private:
		InputManager() = default;
		InputManager(InputManager const&) = delete;
		InputManager(InputManager&&) noexcept = delete;
		InputManager& operator=(InputManager const&) = delete;
		InputManager& operator=(InputManager&&) noexcept = delete;

		virtual void _Finalize() override;

		void _UpdateKeyboard(float _time);

		std::unique_ptr<KEY_STATE, std::function<void(KEY_STATE*)>> const& _FindKeyState(std::string const& _tag) const;

		void _CreateKeyState(std::string const& _tag, uint8_t _key, std::vector<uint8_t> const& _combination_key);

		Microsoft::WRL::ComPtr<IDirectInput8> input_{};
		Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard_{};

		KEYBOARD_STATE keyboard_state_{};
		std::unordered_map<std::string, std::unique_ptr<KEY_STATE, std::function<void(KEY_STATE*)>>> key_state_map_{};
	};
}
