
uniform sampler2D texture;

uniform float min_x, max_x, min_y, max_y;
uniform int limit;



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

	//pixel.r = float(iter)/limit;
	vec3 new_color = vec3(0, 0, 0);
	new_color.r = iter/255.0;
	new_color.b = (iter * iter)/255.0;
	new_color.g = (iter * iter * iter)/255.0;


	pixel.r = new_color.r - int(new_color.r);
	pixel.g = new_color.g - int(new_color.g);
	pixel.b = new_color.b - int(new_color.b);
    gl_FragColor = pixel;

    
}
