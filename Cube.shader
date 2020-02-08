#version 420

layout(location = 0) in vec3 vp;


uniform mat4 proj, view, model;



out vData{
	mat4 out_view;
}v_data;
void main() {
	gl_Position = proj * model * view* vec4(vp, 1.0);
	v_data.out_view = proj * model * view;
}
//@vertex

#version 420

layout(points) in;
in vData{
	mat4 out_view;
}v_data[];

layout(triangle_strip, max_vertices = 6) out;
void main() {
	for (int i = 0; i < gl_in.length(); i++) {		
		//emit cube
		
		gl_Position = gl_in[i].gl_Position + (v_data[i].out_view * vec4(0, 1, -1, 1));
		EmitVertex();
		gl_Position = vec4(gl_in[i].gl_Position);
		EmitVertex();
		gl_Position = gl_in[i].gl_Position + (v_data[i].out_view * vec4(0,0,-1,1));
		EmitVertex();
	}
	EndPrimitive();
}
//@geometry

#version 420


out vec4 frag_colour;
void main() {
	frag_colour = vec4(1,1,1,1);
}
//@fragment