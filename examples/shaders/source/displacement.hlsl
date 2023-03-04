sampler2D water_map : register(s0);
sampler2D displacement : register(s1);
float t : register(c0);

struct PS_INPUT {
    float2 tex : TEXCOORD;
};

struct PS_OUTPUT {
    float4 col : COLOR;
};

PS_OUTPUT main(PS_INPUT input) {
    PS_OUTPUT output;
    
    float2 disp_uv = float2(input.tex.x * 1.1 - 0.4 - t / 8, input.tex.y * 1.1 + t / 8) * 2 * 8;
    
    float2 uv = (input.tex + 0.2 + t / 4) * 4 * 8;
    uv += tex2D(displacement, disp_uv).r;
    
    output.col = tex2D(water_map, uv);
    
    return output;
}