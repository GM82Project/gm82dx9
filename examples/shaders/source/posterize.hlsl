SamplerState rSampler: register(s0);

struct PS_INPUT {
    float4 position: POSITION0;
    float2 texcoord: TEXCOORD0;
    float4 color: COLOR;
};

struct PS_OUTPUT {
    float4 color: COLOR;
};

uniform float levels;

PS_OUTPUT main(PS_INPUT input) {
    PS_OUTPUT output;

    float4 albedo = tex2D(rSampler,input.texcoord);

    output.color = float4(round(albedo.rgb * input.color * levels)/levels,albedo.a);
    
    return output;
}