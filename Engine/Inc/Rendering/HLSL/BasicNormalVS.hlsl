#include "type.hlsl"

VS_OUTPUT_POSITION_NORMAL_COLOR BasicNormalVS(VS_INPUT_POSITION_NORMAL_COLOR _input)
{
	VS_OUTPUT_POSITION_NORMAL_COLOR output = (VS_OUTPUT_POSITION_NORMAL_COLOR)0;

    output.position = mul(float4(_input.position, 1.f), g_WVP);
    output.normal = _input.normal;
    output.color = _input.color;

    return output;
}
