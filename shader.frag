
uniform sampler2D texture;																					

uniform float min_x, max_x, min_y, max_y;
uniform int limit;

int color_panel[21] = {
		1, 0, 0,
		1, 1, 0,
		1, 1, 1,
		1, 0, 1,
		0, 0, 1,
		0, 1, 1,
		0, 1, 0 };

void main() {
    
    vec2 texCoord = gl_TexCoord[0].xy;
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

	vec2 coord = vec2(min_x + (max_x - min_x) * texCoord.x, min_y + (max_y - min_y) * texCoord.y);

	vec2 zCoord = vec2(coord.x, coord.y);
	vec2 coord2 = vec2(0.0, 0.0);


	int iter;
	for(iter=1; iter < limit; iter++){
		coord2.x = zCoord.x * zCoord.x;
		coord2.y = zCoord.y * zCoord.y;

		zCoord.y = 2.0f * zCoord.x * zCoord.y + coord.y;
		zCoord.x = coord2.x - coord2.y + coord.x;

		if(zCoord.x * zCoord.x + zCoord.y * zCoord.y > 4.0f){
			
			break;
		}
	}
	if(iter == limit){
		gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
		return;
	}    


	float index = (float(iter) / 40.0f);
	int i_index = (int(index) % 7);
	index = index - int(index);
	int next_index = ((i_index + 1) % 7) * 3;
	i_index *= 3;

	float r_ = color_panel[i_index];
	float g_ = color_panel[i_index + 1];
	float b_ = color_panel[i_index + 2];

	pixel.r = r_ + (index * (color_panel[next_index] - r_) );
	pixel.g = g_ + (index * (color_panel[next_index + 1] - g_));
	pixel.b = b_ + (index * (color_panel[next_index + 2] - b_));

    gl_FragColor = pixel;

}
