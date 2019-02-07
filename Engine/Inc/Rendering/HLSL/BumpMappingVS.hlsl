#include "type.hlsl"

VS_OUTPUT_3D BumpMappingVS(VS_INPUT_3D _input)
{
    VS_OUTPUT_3D output = (VS_OUTPUT_3D)0;

    output.position = mul(float4(_input.position, 1.f), g_WVP);
    output.positionV = mul(float4(_input.position, 1.f), g_WV).xyz;
    output.positionH = output.position;
    output.uv = _input.uv;
    output.normal = normalize(mul(float4(_input.normal, 0.f), g_WV)).xyz;
    output.binormal = normalize(mul(float4(_input.binormal, 0.f), g_WV)).xyz;
    output.tangent = normalize(mul(float4(_input.tangent, 0.f), g_WV)).xyz;

    return output;
}
