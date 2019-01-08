#include "type.hlsl"

VS_OUTPUT_POSITION ColliderVS(VS_INPUT_POSITION _input)
{
	VS_OUTPUT_POSITION output = (VS_OUTPUT_POSITION)0;

	output.position = mul(float4(_input.position, 1.f), g_WVP);

	return output;
}