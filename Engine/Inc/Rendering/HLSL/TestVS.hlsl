#include "type.hlsl"

VS_OUTPUT_POSITION_TEX TestVS(VS_INPUT_POSITION_TEX _input)
{
	VS_OUTPUT_POSITION_TEX output = (VS_OUTPUT_POSITION_TEX)0;

	output.position = float4(_input.position, 1.f);
	output.uv = _input.uv;

	return output;
}
