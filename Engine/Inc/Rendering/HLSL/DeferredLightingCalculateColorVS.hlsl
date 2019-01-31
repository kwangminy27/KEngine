#include "type.hlsl"

VS_OUTPUT_POSITION DeferredLightingCalculateColorVS(VS_INPUT_POSITION _input)
{
    VS_OUTPUT_POSITION output = (VS_OUTPUT_POSITION)0;

    output.position = float4(_input.position, 1.f);

    return output;
}
