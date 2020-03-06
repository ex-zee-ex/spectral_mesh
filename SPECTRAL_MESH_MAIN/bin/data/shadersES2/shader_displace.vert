
// these are for the programmable pipeline system
uniform mat4 modelViewProjectionMatrix;

attribute vec4 position;
attribute vec2 texcoord;



varying vec2 texCoordVarying;
uniform sampler2D tex0;



uniform vec2 xyztheta;
uniform int bright_switch;
uniform float x_lfo;
uniform float x_lfo_arg;
uniform float x_lfo_amp;
uniform float y_lfo_arg;
uniform float y_lfo_amp;
uniform float z_lfo_arg;
uniform float z_lfo_amp;
uniform float x_lfo_other;
uniform float y_lfo_other;
uniform float z_lfo_other;

uniform vec2 xy_offset;

uniform int y_lfo_switch;
uniform int x_lfo_switch;
uniform int z_lfo_switch;

uniform int y_xmod_switch;
uniform int z_xmod_switch;
uniform int x_xmod_switch;

uniform int weird_switch;

float psuedo_random(float seed){
    
    return fract(sin(seed)*100.0);
}

float oscillate(float theta,int shape){
    float osc=0.0;
    
    if(shape==0){
        osc=sin(theta);
    }
    
    //squarewave..can add a dc offset for pwm too
    if(shape==1){
        osc=sign(sin(theta));
    }
    
    
    //haha tanwave
    //osc=tan(theta);
    
    //sawtooth??
    if(shape==2){
        osc=fract(theta/6.18);
    }
    
    
    //noise
    //notquite
    //osc=psuedo_random(sin(theta));
    
    
    return osc;
    
}

void main(void)
{
	
	int width=640;
    int height=480;
	texCoordVarying = texcoord;

	//vec4 new_position=ftransform();
    vec4 new_position=modelViewProjectionMatrix*position;
    vec4 color=texture2D(tex0,texCoordVarying);
    float bright =.33*color.r+.5*color.g+.16*color.b;
    
    bright=2.0*log(1.0+bright);
    
    
    if(bright_switch==1){
        bright=1.0-bright;
        
    }
    
	
    new_position.x=new_position.x+xy_offset.x;
    new_position.y=new_position.y+xy_offset.y;
    
    
    
    
    
   // float x_lfo=x_lfo_amp*oscillate(x_lfo_arg+new_position.y*x_lfo_other,x_lfo_switch);
	float x_lfo=0.0;
    float y_lfo=y_lfo_amp*oscillate(y_lfo_arg+new_position.x*y_lfo_other,y_lfo_switch);
    
    float z_lfo=z_lfo_amp*oscillate(z_lfo_arg+z_lfo_other*distance(abs(new_position.xy),vec2(xy_offset.x/2.0,xy_offset.y/2.0)+.005*float(z_xmod_switch)*y_lfo),z_lfo_switch);
    
    new_position.xy=new_position.xy*(1.0-z_lfo);
    
    x_lfo=x_lfo_amp*oscillate(x_lfo_arg+new_position.y*x_lfo_other+1.0*z_lfo,x_lfo_switch);
    //order of executing new_position.x is where the weird swtich comes in
    
    if(weird_switch==0){
		new_position.x=new_position.x+xyztheta.x*bright+x_lfo;
    }
    y_lfo=y_lfo_amp*oscillate(y_lfo_arg+new_position.x*y_lfo_other+.01*float(y_xmod_switch)*x_lfo,y_lfo_switch);
    
    if(weird_switch==1){
		new_position.x=new_position.x+xyztheta.x*bright+x_lfo;
    }
 
    
    new_position.y=new_position.y+xyztheta.y*bright+y_lfo;
    
    
    
    
   
    
    
    
	new_position.x=new_position.x-xy_offset.x;
	new_position.y=new_position.y-xy_offset.y;
    
    
   	gl_Position = new_position;
}
