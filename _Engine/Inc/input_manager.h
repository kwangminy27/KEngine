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

	struct KEY_AXIS
	{
		struct AXIS
		{
			float scale;
			uint8_t key;
		};

		AXIS info;
		std::function<void(float, float)> callback;
	};

	struct KEY_ACTION
	{
		enum class CALLBACK_TYPE
		{
			DOWN,
			PRESS,
			UP,
			MAX
		};

		struct ACTION
		{
			bool down;
			bool press;
			bool up;
			uint8_t key;
			std::vector<uint8_t> combination_key;
		};

		ACTION info;
		std::array<std::function<void(float)>, static_cast<int>(CALLBACK_TYPE::MAX)> callback_array;
	};

	class ENGINE_DLL InputManager final : public Singleton<InputManager>
	{
		friend class Singleton<InputManager>;
	public:
		virtual void Initialize() override;

		void Initialize(HINSTANCE _instance, HWND _window);

		void Update(float _time);

		static std::unique_ptr<KEY_AXIS, std::function<void(KEY_AXIS*)>> key_axis_dummy_;
		static std::unique_ptr<KEY_ACTION, std::function<void(KEY_ACTION*)>> key_action_dummy_;

	private:
		InputManager() = default;
		InputManager(InputManager const&) = delete;
		InputManager(InputManager&&) noexcept = delete;
		InputManager& operator=(InputManager const&) = delete;
		InputManager& operator=(InputManager&&) noexcept = delete;

		virtual void _Finalize() override;

		void _UpdateKeyboard(float _time);

		std::unique_ptr<KEY_AXIS, std::function<void(KEY_AXIS*)>> const& _FindKeyAxis(std::string const& _tag) const;
		std::unique_ptr<KEY_ACTION, std::function<void(KEY_ACTION*)>> const& _FindKeyAction(std::string const& _tag) const;

		void _CreateKeyAxis(std::string const& _tag, uint8_t _key, float _scale);
		void _CreateKeyAction(std::string const& _tag, uint8_t _key);

		Microsoft::WRL::ComPtr<IDirectInput8> input_{};
		Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard_{};
		KEYBOARD_STATE keyboard_state_{};
		std::unordered_map<std::string, std::unique_ptr<KEY_AXIS, std::function<void(KEY_AXIS*)>>> key_axis_map_{};
		std::unordered_map<std::string, std::unique_ptr<KEY_ACTION, std::function<void(KEY_ACTION*)>>> key_action_map_{};
	};
}
