#include "type.hlsl"

PS_OUTPUT_SINGLE_TARGET BasicTexPS(VS_OUTPUT_POSITION_TEX _input)
{
	PS_OUTPUT_SINGLE_TARGET output = (PS_OUTPUT_SINGLE_TARGET)0;

	output.target = g_texture.Sample(g_sampler, _input.uv) * g_diffuse;

	return output;
}