SamplerState Sampler : register(s0);

struct PS_INPUT {
    float4 position: POSITION0;
    float2 uv: TEXCOORD0;
    float4 color: COLOR;
};

struct PS_OUTPUT {
    float4 color: COLOR;
};

PS_OUTPUT main(PS_INPUT input) {
    PS_OUTPUT output;

    float4 albedo = tex2D(Sampler,input.uv);

    output.color = albedo * input.color;
    
    return output;
}