#include "type.hlsl"

VS_OUTPUT_POSITION_NORMAL_COLOR GBufferVS(VS_INPUT_POSITION_NORMAL_COLOR _input)
{
    VS_OUTPUT_POSITION_NORMAL_COLOR output = (VS_OUTPUT_POSITION_NORMAL_COLOR)0;

    output.position = mul(float4(_input.position, 1.f), g_WVP);
    output.positionV = mul(float4(_input.position, 1.f), g_WV).xyz;
    output.normalV = normalize(mul(float4(_input.normal, 0.f), g_WV)).xyz;
    output.color = _input.color;

    return output;
}
