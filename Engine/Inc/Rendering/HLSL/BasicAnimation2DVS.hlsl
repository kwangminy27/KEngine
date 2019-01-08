#include "type.hlsl"

VS_OUTPUT_POSITION_TEX BasicAnimation2DVS(VS_INPUT_POSITION_TEX _input)
{
	VS_OUTPUT_POSITION_TEX output = (VS_OUTPUT_POSITION_TEX)0;

	output.position = mul(float4(_input.position, 1.f), g_WVP);

	output.uv.x = _input.uv.x == 0.f ? g_LT.x : g_RB.x;
	output.uv.y = _input.uv.y == 0.f ? g_LT.y : g_RB.y;

	return output;
}