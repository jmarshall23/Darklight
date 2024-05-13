// ================== VERTEX SHADER ===============================

// Vertex input structure
struct VertexInput {
    float3 pos : POSITION;  // Position
    float2 texCoord : TEXCOORD0;  // Texture coordinates
};

// Vertex output structure
struct VertexOutput {
    float4 pos : SV_POSITION;  // Position for rasterizer
    float2 texCoord : TEXCOORD0;  // Texture coordinates passed to pixel shader
};

// Constant buffer structure
cbuffer MatrixBuffer {
    float4x4 modelView;   // Modelview matrix
    float4x4 projection;  // Projection matrix
};

// Vertex shader function
VertexOutput VSMain(VertexInput input) {
    VertexOutput output;

    // Combine the modelView and projection matrices
    float4x4 modelViewProj = mul(projection, modelView);

    // Transform the vertex position
    output.pos = mul(float4(input.pos, 1.0), modelViewProj);

    // Pass through texture coordinates
    output.texCoord = input.texCoord;

    return output;
}

// ================== PIXEL SHADER ===============================

// Texture and sampler declaration
Texture2D tex : register(t0);
SamplerState texSampler : register(s0);

// Pixel shader output structure
struct PixelOutput {
    float4 color : SV_TARGET;  // Output color
};

// Pixel shader function
PixelOutput PSMain(VertexOutput input) {
    PixelOutput output;

    // Sample the texture
    output.color = tex.Sample(texSampler, input.texCoord);

    return output;
}