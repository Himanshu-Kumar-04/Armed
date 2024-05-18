#type vertex_Shader
#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 tintColor;
			
uniform mat4 u_ViewProjection;

out vec2 v_TexCoord;
out vec4 v_TintColor;

void main() {
	v_TexCoord = texCoord;
    v_TintColor = tintColor;
    gl_Position = u_ViewProjection * vec4(position, 1.0);
};

#type fragment_Shader
#version 460 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_TintColor;

//uniform sampler2D u_Texture;

void main(){
    //color = texture(u_Texture, v_TexCoord) * v_TintColor;
    color = v_TintColor;
};
