precision highp float;


uniform sampler2D tex0;

varying vec2 texCoordVarying;

uniform float luma_key_level;

uniform float invert_switch;
uniform float b_w_switch;
uniform int luma_switch;
void main()
{
    vec4 color=texture2D(tex0,texCoordVarying);
    float bright =.33*color.r+.5*color.g+.16*color.b;
    
   // color.w=bright;
    
    //greyscale try
    color=b_w_switch*vec4(bright)+(1.0-b_w_switch)*color;
    
     
      color.rgb=invert_switch*(1.0-color.rgb)+(1.0-invert_switch)*color.rgb;   
  
    
    
    //make sure to offer bright switch for this too
    if(luma_switch==0){
		if(bright<luma_key_level){
			color.w=0.0;
			}
    }
    
    if(luma_switch==1){
		if(bright>luma_key_level){
			color.w=0.0;
			}
    }
	
	
	//color.w=bright;
    gl_FragColor =color;
}
