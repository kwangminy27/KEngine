#include "type.hlsl"

VS_OUTPUT_POSITION_COLOR BasicVS(VS_INPUT_POSITION_COLOR _input)
{
	VS_OUTPUT_POSITION_COLOR output = (VS_OUTPUT_POSITION_COLOR)0;

	output.position = mul(float4(_input.position, 1.f), g_WVP);
	output.color = _input.color;

	return output;
}