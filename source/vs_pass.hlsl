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

matrix rMatrixWVP: register(c0);

VS_OUTPUT main (VS_INPUT input) {
   VS_OUTPUT output;
   
   output.position = mul(rMatrixWVP, input.position);   
   output.texcoord = input.texcoord;   
   output.color=input.color;

   return output;
}