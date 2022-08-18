
// these are for the programmable pipeline system
uniform mat4 modelViewProjectionMatrix;

attribute vec4 position;
attribute vec2 texcoord;

varying vec2 texCoordVarying;
uniform sampler2D tex0;

uniform sampler2D x_noise_image;
uniform sampler2D y_noise_image;
uniform sampler2D z_noise_image;

uniform vec2 xy;
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

uniform int yLfoShape;
uniform int xLfoShape;
uniform int zLfoShape;

uniform int y_phasemod_switch;
uniform int z_phasemod_switch;
uniform int x_phasemod_switch;

uniform int y_ringmod_switch;
uniform int z_ringmod_switch;
uniform int x_ringmod_switch;

uniform int width;
uniform int height;

float oscillate(float theta,int shape,int xyz){
    float osc=0.0;
    
    if(shape==0){osc=sin(theta);}
    //squarewave..can add a dc offset for pwm too
    if(shape==1){osc=sign(sin(theta));}
    //sawtooth
    if(shape==2){osc=fract(theta/6.18);}
    if(shape==3){
        //0 is x, 1 is y, 2 is z
        if(xyz==0){osc=2.0*(texture2D(x_noise_image,texCoordVarying*.5).r-.5);}
        if(xyz==1){osc=2.0*(texture2D(y_noise_image,texCoordVarying*.5).r-.5);}
        if(xyz==2){osc=2.0*(texture2D(z_noise_image,texCoordVarying*.5).r-.5);}
    }
    return osc;
}
//--------------------------------------------------------------------
void main(void)
{
	texCoordVarying = texcoord;

	//vec4 new_position=ftransform();
    vec4 new_position=modelViewProjectionMatrix*position;
    vec4 color=texture2D(tex0,texCoordVarying);
    float bright =.33*color.r+.5*color.g+.16*color.b;
    
    //rescaling brightness in a more 'natural' way
    bright=2.0*log(1.0+bright);
    
    if(bright_switch==1){
        bright=1.0-bright;
    }
   
	//shift coordinates to (-.5,.5)
    new_position.x=new_position.x+xy_offset.x;
    new_position.y=new_position.y+xy_offset.y;
     
    //z oscillator
    float x_lfo=x_lfo_amp*oscillate(x_lfo_arg+new_position.y*x_lfo_other,xLfoShape,0);
    float y_lfo=(y_lfo_amp+float(y_ringmod_switch)*.01*x_lfo)*oscillate(
		y_lfo_arg+new_position.x*y_lfo_other+float(y_phasemod_switch)*.01*x_lfo,yLfoShape,1
		);
    
    float z_lfo_amp_dummy=z_lfo_amp+float(z_ringmod_switch)*.0025*y_lfo;
    float z_lfo_frequency=z_lfo_arg+z_lfo_other*distance(
		abs(new_position.xy),vec2(xy_offset.x/2.0,xy_offset.y/2.0)
		+float(z_phasemod_switch)*y_lfo
		);
    
    float z_lfo=z_lfo_amp_dummy*oscillate(z_lfo_frequency, zLfoShape,2);
   
	new_position.xy=new_position.xy*(1.0-z_lfo);
    
    //x oscillator
    float x_lfo_amp_dummy=x_lfo_amp+float(x_ringmod_switch)*1000.0*z_lfo;
    float x_lfo_frequency=x_lfo_arg+new_position.y*x_lfo_other+float(x_phasemod_switch)*10.0*z_lfo;
    x_lfo=x_lfo_amp_dummy*oscillate(x_lfo_frequency,xLfoShape,0);
    
    new_position.x=new_position.x+xy.x*bright+x_lfo;
    
    //y oscillator
    float y_lfo_amp_dummy=y_lfo_amp+float(y_ringmod_switch)*x_lfo;
    float y_lfo_frequency=y_lfo_arg+new_position.x*y_lfo_other+float(y_phasemod_switch)*.01*x_lfo;
    y_lfo=y_lfo_amp_dummy*oscillate(y_lfo_frequency,yLfoShape,1);
    
    new_position.y=new_position.y+xy.y*bright+y_lfo;
    
    //move coordinates back to (0,1)
	new_position.x=new_position.x-xy_offset.x;
	new_position.y=new_position.y-xy_offset.y;
    
   	gl_Position = new_position;
}
