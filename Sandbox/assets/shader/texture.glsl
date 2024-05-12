#type vertex_Shader
#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
			
uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 v_TexCoord;

void main() {
	v_TexCoord = texCoord;
    gl_Position = u_ViewProjection * u_Transform * vec4(position, 1.0);
};

#type fragment_Shader
#version 460 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;
uniform vec4 u_Color;
uniform float u_TilingFactor;

void main(){
    color = texture(u_Texture,v_TexCoord * u_TilingFactor) * u_Color;
};
