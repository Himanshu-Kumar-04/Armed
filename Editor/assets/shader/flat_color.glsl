#type vertex_Shader
#version 460 core

layout(location = 0) in vec3 position;
			
uniform mat4 u_ViewProjection;

void main() {
    gl_Position = u_ViewProjection * vec4(position, 1.0);
};

#type fragment_Shader
#version 460 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main(){
    color = u_Color;
};
