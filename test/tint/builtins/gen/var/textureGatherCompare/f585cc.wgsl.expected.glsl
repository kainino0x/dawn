//
// fragment_main
//
#version 310 es
precision highp float;
precision highp int;

layout(binding = 0, std430)
buffer f_prevent_dce_block_ssbo {
  vec4 inner;
} v;
uniform highp sampler2DArrayShadow f_arg_0_arg_1;
vec4 textureGatherCompare_f585cc() {
  vec2 arg_2 = vec2(1.0f);
  int arg_3 = 1;
  float arg_4 = 1.0f;
  vec2 v_1 = arg_2;
  float v_2 = arg_4;
  vec4 res = textureGatherOffset(f_arg_0_arg_1, vec3(v_1, float(arg_3)), v_2, ivec2(1));
  return res;
}
void main() {
  v.inner = textureGatherCompare_f585cc();
}
//
// compute_main
//
#version 310 es

layout(binding = 0, std430)
buffer prevent_dce_block_1_ssbo {
  vec4 inner;
} v;
uniform highp sampler2DArrayShadow arg_0_arg_1;
vec4 textureGatherCompare_f585cc() {
  vec2 arg_2 = vec2(1.0f);
  int arg_3 = 1;
  float arg_4 = 1.0f;
  vec2 v_1 = arg_2;
  float v_2 = arg_4;
  vec4 res = textureGatherOffset(arg_0_arg_1, vec3(v_1, float(arg_3)), v_2, ivec2(1));
  return res;
}
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
  v.inner = textureGatherCompare_f585cc();
}
//
// vertex_main
//
#version 310 es


struct VertexOutput {
  vec4 pos;
  vec4 prevent_dce;
};

uniform highp sampler2DArrayShadow v_arg_0_arg_1;
layout(location = 0) flat out vec4 tint_interstage_location0;
vec4 textureGatherCompare_f585cc() {
  vec2 arg_2 = vec2(1.0f);
  int arg_3 = 1;
  float arg_4 = 1.0f;
  vec2 v = arg_2;
  float v_1 = arg_4;
  vec4 res = textureGatherOffset(v_arg_0_arg_1, vec3(v, float(arg_3)), v_1, ivec2(1));
  return res;
}
VertexOutput vertex_main_inner() {
  VertexOutput v_2 = VertexOutput(vec4(0.0f), vec4(0.0f));
  v_2.pos = vec4(0.0f);
  v_2.prevent_dce = textureGatherCompare_f585cc();
  return v_2;
}
void main() {
  VertexOutput v_3 = vertex_main_inner();
  gl_Position = vec4(v_3.pos.x, -(v_3.pos.y), ((2.0f * v_3.pos.z) - v_3.pos.w), v_3.pos.w);
  tint_interstage_location0 = v_3.prevent_dce;
  gl_PointSize = 1.0f;
}
