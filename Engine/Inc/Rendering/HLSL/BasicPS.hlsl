#include "type.hlsl"

PS_OUTPUT_SINGLE_TARGET BasicPS(VS_OUTPUT_POSITION_COLOR _input)
{
	PS_OUTPUT_SINGLE_TARGET output = (PS_OUTPUT_SINGLE_TARGET)0;

	output.target = _input.color;

	return output;
}