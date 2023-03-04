struct VS_INPUT {
   float4 position: POSITION;
   float2 texcoord: TEXCOORD0;
   float4 color: COLOR;
};

struct VS_OUTPUT {
   float4 position: POSITION;
   float2 texcoord: TEXCOORD0;
   float4 color: COLOR;
};

SamplerState rSampler: register(s0);
matrix rMatrixWVP: register(c0);
float scale;
float scroll;

float height(float4 color) {
    return (color.r+color.g+color.b)/3;
}

VS_OUTPUT main (VS_INPUT input) {
   VS_OUTPUT output;
   
   float4 col=tex2D(rSampler,input.texcoord+float2(scroll,scroll));
   
   input.position.z+=height(col)*scale;
   
   output.position = mul(rMatrixWVP, input.position);   
   output.texcoord = input.texcoord+float2(scroll,scroll);   
   output.color=input.color;

   return output;
}