#include "type.hlsl"

PS_OUTPUT_SINGLE_TARGET ColliderPS(VS_OUTPUT_POSITION _input)
{
	PS_OUTPUT_SINGLE_TARGET output = (PS_OUTPUT_SINGLE_TARGET)0;

	output.target = g_color;

	return output;
}