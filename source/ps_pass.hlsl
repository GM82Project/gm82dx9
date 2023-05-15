struct PS_INPUT {
    float2 texcoord: TEXCOORD0;
    float4 color: COLOR0;
};

struct PS_OUTPUT {
    float4 color: COLOR0;
};

SamplerState rSampler: register(s0);

PS_OUTPUT main(PS_INPUT input) {
    PS_OUTPUT output;

    float4 albedo = tex2D(rSampler, input.texcoord);

    output.color = albedo * input.color;
    
    return output;
}
