#include "type.hlsl"

PS_OUTPUT_LIGHT_BUFFER TestPS(VS_OUTPUT_POSITION_TEX _input)
{
    PS_OUTPUT_LIGHT_BUFFER output = (PS_OUTPUT_LIGHT_BUFFER)0;

    output.ambient = float4(1.f, 1.f, 1.f, 1.f);
    output.diffuse = float4(1.f, 1.f, 1.f, 1.f);
    output.specular = float4(1.f, 1.f, 1.f, 1.f);

    //output.target = float4(1.f, 1.f, 1.f, 1.f);

    return output;
}
