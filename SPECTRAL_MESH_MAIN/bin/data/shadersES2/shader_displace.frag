precision highp float;


uniform sampler2D tex0;

varying vec2 texCoordVarying;

uniform float luma_key_level;

uniform int invert_switch;
uniform int luma_switch;
void main()
{
    vec4 color=texture2D(tex0,texCoordVarying);
    float bright =.33*color.r+.5*color.g+.16*color.b;
    if(invert_switch==1){
        color.rgb=1.0-color.rgb;
        }
   // color=pow(color,vec4(2.0));
    
    
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
