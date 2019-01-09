#pragma once

namespace K
{
	enum class GAME_MODE
	{
		_2D,
		FORWARD,
		DEFERRED
	};

	enum class RESOLUTION
	{
		WIDTH = 1600,
		HEIGHT = 900
	};

	enum class SHADER_TYPE
	{
		VERTEX = 1 << 0,
		PIXEL = 1 << 1
	};

	enum class ANIMATION_2D_TYPE
	{
		FRAME,
		ATLAS
	};

	enum class ANIMATION_OPTION
	{
		LOOP,
		DESTROY
	};

	enum class TILE_TYPE
	{
		ISOMETRIC,
		ORTHOGRAPHIC
	};

	enum class TILE_OPTION
	{
		NORMAL,
		BLOCKED,
		MAX
	};

	enum class LIGHT_TYPE
	{
		DIRECTIONAL,
		POINT,
		SPOT,
		MAX
	};

	enum class RENDER_GROUP_TYPE
	{
		LANDSCAPE,
		NORMAL,
		ALPHA_1,
		ALPHA_2,
		ALPHA_3,
		UI,
		HUD,
		LIGHT,
		MAX
	};
}
