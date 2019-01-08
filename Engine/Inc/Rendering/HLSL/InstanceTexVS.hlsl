#include "type.hlsl"

VS_OUTPUT_POSITION_TEX InstanceTexVS(VS_INPUT_POSITION_TEX_INSTANCE _input)
{
	VS_OUTPUT_POSITION_TEX output = (VS_OUTPUT_POSITION_TEX)0;

	output.position = mul(mul(mul(float4(_input.position, 1.f), _input.world), g_view), g_projection);

	output.uv.x = _input.position.x == -0.5f ? _input.LT.x : _input.RB.x;
	output.uv.y = _input.position.y == -0.5f ? _input.RB.y : _input.LT.y;

	return output;
}
