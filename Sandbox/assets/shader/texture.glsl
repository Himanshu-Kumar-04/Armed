#type vertex_Shader
#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float texIndex;
			
uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 v_TexCoord;
out vec4 v_Color;
out float v_TexIndex;

void main() {
	v_TexCoord = texCoord;
    v_Color = color;
    v_TexIndex = texIndex;
    gl_Position = u_ViewProjection * vec4(position, 1.0);
};

#type fragment_Shader
#version 460 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

uniform sampler2D u_Textures[32];

void main(){
    color = texture(u_Textures[int(v_TexIndex)], v_TexCoord) * v_Color;
};
