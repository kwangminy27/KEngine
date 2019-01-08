#include "type.hlsl"

VS_OUTPUT_POSITION_TEX BasicTexVS(VS_INPUT_POSITION_TEX _input)
{
	VS_OUTPUT_POSITION_TEX output = (VS_OUTPUT_POSITION_TEX)0;

	output.position = mul(float4(_input.position, 1.f), g_WVP);
	output.uv = _input.uv;

	return output;
}