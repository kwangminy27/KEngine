#include "type.hlsl"

#define DIRECTIONAL 0
#define POINT 1
#define SPOT 2

VS_OUTPUT_POSITION DeferredLightingVS(VS_INPUT_POSITION _input)
{
    VS_OUTPUT_POSITION output = (VS_OUTPUT_POSITION)0;

    switch(g_light_type)
    {
        case DIRECTIONAL:
            output.position = float4(_input.position, 1.f);
            break;

        case POINT:
        case SPOT:
            output.position = mul(float4(_input.position, 1.f), g_WVP);
            break;
    }

    return output;
}
