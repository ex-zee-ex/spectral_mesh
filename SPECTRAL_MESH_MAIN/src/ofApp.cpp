

//add the midi info
//fix up the reset switch and parameter locks to include everything

#include "ofApp.h"


#include <iostream>



//0 is picapture, 1 is usbinput
//this is only relevant for folks with the now discontinued capture editions
//or their own picapture sd1s.  most people can just ignore this switch!
bool inputswitch=1;


int hdmi_aspect_ratio_switch=0;

float rescale=1;

float az=0.0;
float sx=0.0;
float dc=0.0;
float fv=.0;
float gb=.0;
float hn=.0;
float jm=.0;
float kk=.0;
float ll=.0;
float ylfo_amp=0.0;
float ty=.0;

float qw=0;
float er=0;
float ui=0;
float op=0;


float theta=0;

float x_lfo=0;
float x_lfo_arg=0;
float y_lfo=0;
float y_lfo_arg=0;
float z_lfo=0;
float z_lfo_arg=0;

float c1=0.0;
float c2=0.0;
float c3=0.0;
float c4=0.0;
float c5=0.0;
float c6=0.0;
float c7=0.0;
float c8=0.0;
float c9=0.0;
float c10=0.0;
float c11=0.0;
float c12=0.0;
float c13=0.0;
float c14=0.0;
float c15=.0;
float c16=.0;


//keyboard controls
//probbly shift all of these to a gui
float ss=1;
float dd=1;

float gg=1;
float hh=0;

float ee=1;
float pp=1;
float ii=.01;




int width=640;
int height=480;





bool wireframe_switch=0;
bool bright_switch=0;

bool invert_switch=0;
bool stroke_weight_switch=0;


int scale=100;

int scale_key=0;

int y_lfo_switch=0;
int x_lfo_switch=0;
int z_lfo_switch=0;

bool y_freq0=FALSE;
bool y_ringmod_switch=FALSE;
bool y_phasemod_switch=FALSE;

bool x_freq0=FALSE;
bool x_ringmod_switch=FALSE;
bool x_phasemod_switch=FALSE;

bool z_freq0=FALSE;
bool z_ringmod_switch=FALSE;
bool z_phasemod_switch=FALSE;

bool weird_switch=FALSE;

float global_x_displace=0.0;
float global_y_displace=0.0;

float center_x_displace=0.0;
float rotate_x=0.0;
float rotate_capture_z=0.0;

bool rotate_capture_z_switch=1;
bool rotate_x_switch=1;
bool global_x_displace_switch=1;
bool center_x_displace_switch=0;


float center_y_displace=0.0;
float rotate_y=0.0;



bool rotate_y_switch=1;
bool global_y_displace_switch=1;
bool center_y_displace_switch=0;
bool luma_switch=0;

bool b_w_switch=0;

float stroke_weight=1;

//mesh types
//0 is trianglegrid
//1 is horizontal lines
//2 is vertical lines
int mesh_type=0;


ofFbo aspect_fix_fbo;


const int p_lock_size=240;

bool p_lock_record_switch=0;

bool p_lock_erase=0;

//maximum number of p_locks available...maybe there can be one for every knob
//for whatever wacky reason the last member of this array of arrays has a glitch
//so i guess just make an extra array and forget about it for now
const int p_lock_number=17;

//so how we will organize the p_locks is in multidimensional arrays
//to access the data at timestep x for p_lock 2 (remember counting from 0) we use p_lock[2][x]
float p_lock[p_lock_number][p_lock_size];


//smoothing parameters(i think for all of these we can array both the arrays and the floats
//for now let us just try 1 smoothing parameter for everything.
float p_lock_smooth=.5;


//and then lets try an array of floats for storing the smoothed values
float p_lock_smoothed[p_lock_number];


//turn on and off writing to the array
bool p_lock_0_switch=1;

//global counter for all the locks
int p_lock_increment=0;


//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(30);
	ofSetVerticalSync(true);
	ofBackground(0);
	ofHideCursor();
	
	omx_settings();	
	if(inputswitch==0){
		videoGrabber.setup(settings);
	}

	
	if(inputswitch==1){
		cam1.setDesiredFrameRate(30);
		cam1.initGrabber(width,height);
	}
	//ofSetLogLevel(OF_LOG_VERBOSE);
	
    
    /**MIDIBIX***/
    
	// print input ports to console
	midiIn.listInPorts();
	
	// open port by number (you may need to change this)
	midiIn.openPort(1);
	//midiIn.openPort("IAC Pure Data In");	// by name
	//midiIn.openVirtualPort("ofxMidiIn Input"); // open a virtual port
	
	// don't ignore sysex, timing, & active sense messages,
	// these are ignored by default
	midiIn.ignoreTypes(false, false, false);
	
	// add ofApp as a listener
	midiIn.addListener(this);
	
	// print received messages to the console
	midiIn.setVerbose(true);
    
    
    //shadersbiz
    shader_displace.load("shadersES2/shader_displace");
    
    /*******/
    
    aspect_fix_fbo.allocate(width,height);
    aspect_fix_fbo.begin();
    ofClear(0,0,0,255);
    aspect_fix_fbo.end();
    
   
   
    
    //framebufferbiz
    fbo0.allocate(width,height);
    fbo0.begin();
    ofClear(0,0,0,255);
    fbo0.end();
    
   
    
   
     trianglemesh(scale);
  

   
    //so 1/4 size doggg
    x_noise_image.allocate(180,120, OF_IMAGE_GRAYSCALE);
	y_noise_image.allocate(180,120, OF_IMAGE_GRAYSCALE);
    z_noise_image.allocate(180,120, OF_IMAGE_GRAYSCALE);
    
    
    //p_lock biz
	for(int i=0;i<p_lock_number;i++){
        
        for(int j=0;j<p_lock_size;j++){
            
            p_lock[i][j]=0;
            
        }//endplocksize
    
    }//endplocknumber
    
    
}

//--------------------------------------------------------------
void ofApp::update() {

  if(inputswitch==1){
		cam1.update();
		
		 //corner crop and stretch to preserve hd aspect ratio
		if(hdmi_aspect_ratio_switch==1){
			aspect_fix_fbo.begin();
			cam1.draw(0,0,853,480);
			aspect_fix_fbo.end();
			
		}
	}
	
	if(inputswitch==0){
		
		omx_updates();
		
	}
    
    
    
    /*midimessagesbiz**/
    midibiz();
	
	x_noise_image=perlin_noise(x_lfo_arg,c5,x_noise_image);
    y_noise_image=perlin_noise(y_lfo_arg,c6,y_noise_image);
    z_noise_image=perlin_noise(z_lfo_arg,c4,z_noise_image);
    
    
    for(int i=0;i<p_lock_number;i++){
        p_lock_smoothed[i]=p_lock[i][p_lock_increment]*(1.0f-p_lock_smooth)+p_lock_smoothed[i]*p_lock_smooth;
        
        if(abs(p_lock_smoothed[i])<.05){p_lock_smoothed[i]=0;}
        
        
    }
  
}

//--------------------------------------------------------------
void ofApp::draw() {
    
    
    //dummy variables
    float d_luma_key_level=p_lock_smoothed[0]+1.01*az;
    float d_x=100.0*(p_lock_smoothed[1]+qw);
    float d_y=100.0*(p_lock_smoothed[2]+er);
    float d_z_other=.05*p_lock_smoothed[3]+sx;
    float d_x_other=height/15000.0*p_lock_smoothed[4]+gb;
    float d_y_other=width/15000.0*p_lock_smoothed[5]+kk;
    float d_zoom=p_lock_smoothed[6]*480+op;
    float d_scale=(1.0-p_lock_smoothed[7])*126.0+1.0+scale_key;

	float d_z_lfo_arg=p_lock_smoothed[10]+dc;
    float d_x_lfo_arg=p_lock_smoothed[12]+hn;
    float d_y_lfo_arg=p_lock_smoothed[14]+ll;

    float d_center_x=p_lock_smoothed[8]+ty;
    float d_center_y=p_lock_smoothed[9]+ui;
    float d_z_lfo_amp=.25*p_lock_smoothed[11]+fv;   
    float d_x_lfo_amp=ofGetWidth()*.25*p_lock_smoothed[13]+jm;   
    float d_y_lfo_amp=ofGetHeight()*.25*p_lock_smoothed[15]+ylfo_amp;
    //float d_y_lfo_amp=ofGetHeight()*.25*c6+ylfo_amp;
    
   
  
    fbo0.begin();
    
    shader_displace.begin();
    ofBackground(0);
   
    if(invert_switch==1){ofBackground(255);}
    
    shader_displace.setUniform1f("b_w_switch",b_w_switch);
    
    shader_displace.setUniformTexture("x_noise_image",x_noise_image.getTexture(),1);
    shader_displace.setUniformTexture("y_noise_image",y_noise_image.getTexture(),2);
    shader_displace.setUniformTexture("z_noise_image",z_noise_image.getTexture(),3);
    
    
    
    shader_displace.setUniform1f("luma_key_level",d_luma_key_level);
    //shader_displace.setUniform1f("amnt1",p_lock_smoothed[1]);
    
    ofVec2f xy;
    xy.set(d_x,d_y);
    shader_displace.setUniform2f("xyztheta",xy);
    shader_displace.setUniform1i("width",width);
    shader_displace.setUniform1i("height",height);
    shader_displace.setUniform1i("bright_switch",bright_switch);
    shader_displace.setUniform1f("invert_switch",invert_switch);
    
    
    
    //lfo1 for the xyz
    z_lfo_arg+=d_z_lfo_arg;
    x_lfo_arg+=d_x_lfo_arg;
    y_lfo_arg+=d_y_lfo_arg;
   // x_lfo=sin(x_lfo_arg)*(1+127*c14);
	shader_displace.setUniform1f("z_lfo_amp",d_z_lfo_amp);
    shader_displace.setUniform1f("z_lfo_arg",z_lfo_arg);
    shader_displace.setUniform1f("z_lfo_other",d_z_other);
   
    shader_displace.setUniform1f("x_lfo_amp",d_x_lfo_amp);
    shader_displace.setUniform1f("x_lfo_arg",x_lfo_arg);
    shader_displace.setUniform1f("x_lfo_other", d_x_other);
    
    shader_displace.setUniform1f("y_lfo_amp",d_y_lfo_amp);
    shader_displace.setUniform1f("y_lfo_arg",y_lfo_arg);
    shader_displace.setUniform1f("y_lfo_other", d_y_other);
    
    
    
    
    
    
    
    
    
    ofVec2f xy_offset;
    xy_offset.set(d_center_x,d_center_y);
    shader_displace.setUniform2f("xy_offset",xy_offset);
    
    
    shader_displace.setUniform1i("y_lfo_switch",y_lfo_switch);
    shader_displace.setUniform1i("x_lfo_switch",x_lfo_switch);
    shader_displace.setUniform1i("z_lfo_switch",z_lfo_switch);
    
    shader_displace.setUniform1i("y_phasemod_switch",y_phasemod_switch);
    shader_displace.setUniform1i("x_phasemod_switch",x_phasemod_switch);
    shader_displace.setUniform1i("z_phasemod_switch",z_phasemod_switch);
    
    shader_displace.setUniform1i("y_ringmod_switch",y_ringmod_switch);
    shader_displace.setUniform1i("x_ringmod_switch",x_ringmod_switch);
    shader_displace.setUniform1i("z_ringmod_switch",z_ringmod_switch);
    
    shader_displace.setUniform1i("weird_switch",weird_switch);
    shader_displace.setUniform1i("luma_switch",luma_switch);
   
    
    if(inputswitch==0){
			videoGrabber.getTextureReference().bind();
		}
		
	if(inputswitch==1){
			
		
			
			
		if(hdmi_aspect_ratio_switch==0){
			cam1.getTexture().bind();
		}
			
		if(hdmi_aspect_ratio_switch==1){
			aspect_fix_fbo.getTexture().bind();
		}
			
	
			
	}
    
  //  cam1.getTexture().bind();
    
    
    
  
    //ofSetLineWidth(10);
    
    glLineWidth(stroke_weight);
    
    ofPushMatrix();
    
    
    
    ofTranslate(0,0,d_zoom);
    //ofRotateXRad(rotate_x);
    if(wireframe_switch==0){
        vbo_mesh1.draw();
    }
    if(wireframe_switch==1){
        vbo_mesh1.drawWireframe();
    }
    
    ofPopMatrix();
  
    if(inputswitch==0){
			videoGrabber.getTextureReference().unbind();
		}
		
		if(inputswitch==1){
			
			if(hdmi_aspect_ratio_switch==0){
				cam1.getTexture().unbind();
			}
			
			if(hdmi_aspect_ratio_switch==1){
				aspect_fix_fbo.getTexture().unbind();
			}
		}
    
    
    //cam1.getTexture().unbind();
    
  
    
    shader_displace.end();
    
    
    
    
    
    
    fbo0.end();
	
	
   
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2,ofGetHeight()/2,100);
    ofRotateXRad(rotate_x);
    ofRotateYRad(rotate_y);
    ofRotateZRad(rotate_capture_z);
    fbo0.draw(-ofGetWidth()/2+global_x_displace,-ofGetHeight()/2+global_y_displace,720,480);
    ofPopMatrix();
   
    
    //y_noise_image.draw(0,0,720,480);
    //switch for this too maybe
    
    fbo0.begin();
   // ofClear(0,0,0,255);
    fbo0.end();
   
	
	
    //choose a different scale for the pi
    //for each mesh type have a rescaling option to prevent any slowdowns
    //each mesh has different scale ranges for optimizations
    //so just feed in c8+1 and rescale within
  
    
	scale=d_scale;
	
	
	if(scale>=127){scale=127;}
	if(scale<=1){
			scale=1;
			}
			
    //scale=50;
    
    ofSetColor(255);
    string msg="fps="+ofToString(ofGetFrameRate(),2)+" stroke"+ofToString(stroke_weight,2)+" stroke switch"+ofToString(stroke_weight_switch,0);
    //ofDrawBitmapString(msg,10,height-10);
    
    
    
    if(p_lock_record_switch==1){
        p_lock_increment++;
        p_lock_increment=p_lock_increment%p_lock_size;
    }
   
}

//+---------------------------------------------------------------

ofImage ofApp::perlin_noise(float theta, float resolution, ofImage noise_image){
    
    //float noiseScale = ofMap(mouseX, 0, ofGetWidth(), 0, 0.1);
    //float noiseVel = ofGetElapsedTimef();
    resolution=resolution*.05;
    theta=theta*.1;
    ofPixels & pixels = noise_image.getPixels();
    int w = noise_image.getWidth();
    int h = noise_image.getHeight();
    for(int y=0; y<h; y++) {
        for(int x=0; x<w; x++) {
            int i = y * w + x;
            float noiseVelue = ofNoise(x * resolution, y * resolution, theta);
            pixels[i] = 255 * noiseVelue;
        }
    }
    
    
    noise_image.update();
    return noise_image;
}

//--------------------------------------------------------------
void ofApp::exit() {
	
	// clean up
	midiIn.closePort();
	midiIn.removeListener(this);
}

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& msg) {

	// add the latest message to the message queue
	midiMessages.push_back(msg);

	// remove any old messages if we have too many
	while(midiMessages.size() > maxMessages) {
		midiMessages.erase(midiMessages.begin());
	}
}


//--------------------------------------------------------------
void ofApp::omx_settings(){
	
	settings.sensorWidth = 640;
    settings.sensorHeight = 480;
    settings.framerate = 30;
    settings.enableTexture = true;
    settings.sensorMode=7;
    
    settings.whiteBalance ="Off";
    settings.exposurePreset ="Off";
    settings.whiteBalanceGainR = 1.0;
    settings.whiteBalanceGainB = 1.0;
	
	}
//------------------------------------------------------------

void ofApp::omx_updates(){
	
		videoGrabber.setSharpness(100);
		videoGrabber.setBrightness(40);
		videoGrabber.setContrast(100);
		videoGrabber.setSaturation(0);
	
	}


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

    
    
   
    //if(key=='q'){sw1==0;}
    
    if(key=='a'){az+=.01;}
    if(key=='z'){az-=.01;}
    
    //zflo biz
    if(key=='s'){sx+=.0001;}
    if(key=='x'){sx-=.0001;}
    if(key=='d'){dc+=0.001;}
    if(key=='c'){dc-=0.001;}
    if(key=='f'){fv+=0.001;}
    if(key=='v'){fv-=0.001;}
    
    //ylfo biz
    if(key=='g'){gb+=0.001;}
    if(key=='b'){gb-=0.001;}
    if(key=='h'){hn+=0.001;}
    if(key=='n'){hn-=0.001;}
    if(key=='j'){jm+=0.1;}
    if(key=='m'){jm-=0.1;}
    
    if(key=='k'){kk+=0.001;}
    if(key==','){kk-=0.001;}
    if(key=='l'){ll+=0.001;}
    if(key=='.'){ll-=0.001;}
    if(key==';'){ylfo_amp+=0.1;}
    if(key=='/'){ylfo_amp-=0.1;}
    
    
    if(key=='t'){ty+=5;}
    if(key=='y'){ty-=5;}
    
    if(key=='u'){ui+=5;}
    if(key=='i'){ui-=5;}
    
    if(key=='o'){op+=5;}
    if(key=='p'){op-=5;}
    
    if(key=='e'){er+=0.01;}
    if(key=='r'){er-=0.01;}
    
    if(key=='q'){qw+=0.01;}
    if(key=='w'){qw-=0.01;}
    
    
    if(key==']'){scale_key+=1;}
    if(key=='['){
		scale_key-=1;
		
		}
    
    if(key=='1'){luma_switch=!luma_switch;}
    if(key=='2'){bright_switch=!bright_switch;}
    if(key=='3'){invert_switch=!invert_switch;}
    if(key=='4'){}
    if(key=='5'){b_w_switch=!b_w_switch;}
    
    if(key=='6'){
		z_lfo_switch++;
		z_lfo_switch=z_lfo_switch%4;
		
		}
		
	if(key=='7'){
		x_lfo_switch++;
		x_lfo_switch=x_lfo_switch%4;
		
		}
		
	if(key=='8'){
		y_lfo_switch++;
		y_lfo_switch=y_lfo_switch%4;
		
		}
		
	
    
    if(key=='9'){vertical_linemesh(scale);}
    if(key=='0'){horizontal_linemesh(scale);}
    if(key=='-'){
		trianglemesh(scale);
		if(wireframe_switch==1){
			wireframe_switch=0;
			}
		}
    if(key=='='){
		trianglemesh(scale);
		if(wireframe_switch==0){
			wireframe_switch=1;
			}
		
		}
    
 
    
    
    //ring mod
   if(key=='!'){z_ringmod_switch=!z_ringmod_switch;}
   if(key=='@'){x_ringmod_switch=!x_ringmod_switch;}
   if(key=='#'){y_ringmod_switch=!y_ringmod_switch;}

   if(key=='$'){z_phasemod_switch=!z_phasemod_switch;}
   if(key=='%'){x_phasemod_switch=!x_phasemod_switch;}
   if(key=='^'){y_phasemod_switch=!y_phasemod_switch;}
}
//--------------
//this makes a mesh of a grid
//but not quite
void ofApp:: gridmesh(int gridsize){
    mesh1.clearVertices();
    mesh1.clearTexCoords();
    ofVec3f Q_vertex1;
    ofVec3f Q_vertex2;
    ofVec3f Q_vertex3;
    ofVec3f Q_vertex4;
    
    ofVec2f Q_tex1;
    ofVec2f Q_tex2;
    ofVec2f Q_tex3;
    ofVec2f Q_tex4;
    
    for(int i=0;i<gridsize;i++){
        for(int j=0;j<gridsize;j++){
            int x0=j*width/gridsize;
            int x1=(j+1)*width/gridsize;
            int y0=i*height/gridsize;
            int y1=(i+1)*height/gridsize;
            
            Q_vertex1.set(x0,y0,0);
            Q_vertex2.set(x1,y0,0);
            Q_vertex3.set(x1,y1,0);
            Q_vertex4.set(x0,y1,0);
            
            Q_tex1.set(x0,y0);
            Q_tex2.set(x1,y0);
            Q_tex3.set(x1,y1);
            Q_tex4.set(x0,y1);
            
            
            
            mesh1.addVertex(Q_vertex1);
            mesh1.addVertex(Q_vertex2);
            mesh1.addVertex(Q_vertex4);
            mesh1.addVertex(Q_vertex3);
            
            //then corresponding texture coordinates one for each vertext
            mesh1.addTexCoord(Q_tex1);
            mesh1.addTexCoord(Q_tex2);
            mesh1.addTexCoord(Q_tex4);
            mesh1.addTexCoord(Q_tex3);
           
        }//endxfor
    }//endyfor
    mesh1.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
   vbo_mesh1= mesh1;
    
    
}
//_---------------
//this makes a mesh of some loose triangles
void ofApp:: trianglemesh(int gridsize){
    mesh1.clearVertices();
    mesh1.clearTexCoords();
    ofVec3f Q_vertex1;
    ofVec3f Q_vertex2;
    ofVec3f Q_vertex3;
    ofVec3f Q_vertex4;
    
    ofVec2f Q_tex1;
    ofVec2f Q_tex2;
    ofVec2f Q_tex3;
    ofVec2f Q_tex4;
    
    for(int i=0;i<gridsize;i++){
        for(int j=0;j<gridsize;j++){
            int x0=j*width/gridsize;
            int x1=(j+1)*width/gridsize;
            int y0=i*height/gridsize;
            int y1=(i+1)*height/gridsize;
            
            Q_vertex1.set(x0,y0,0);
            Q_vertex2.set(x1,y0,0);
            Q_vertex3.set(x1,y1,0);
            Q_vertex4.set(x0,y1,0);
            
            
            //so if i go from 0-1 then 
            //the tex coords are still only in the 
            //bottom left hand corner
            //only integer values make adifference 
            //for rescaling
            /*
            float tex_x0=j/gridsize*rescale;
            float tex_x1=(j+1)/gridsize*rescale;
            float tex_y0=i/gridsize*rescale;
            float tex_y1=(i+1)/gridsize*rescale;\
            */
            
            //if i do this then it moves to the 
            //upper right hand corner
            // then if rescale=1/gridsize it fills
            rescale=1.0/gridsize;
            float tex_x0=j*rescale;
            float tex_x1=(j+1)*rescale;
            float tex_y0=i*rescale;
            float tex_y1=(i+1)*rescale;
            
            
            
            Q_tex1.set(tex_x0,tex_y0);
            Q_tex2.set(tex_x1,tex_y0);
            Q_tex3.set(tex_x1,tex_y1);
            Q_tex4.set(tex_x0,tex_y1);
            
           
            
            mesh1.addVertex(Q_vertex1);
            mesh1.addVertex(Q_vertex2);
            mesh1.addVertex(Q_vertex3);
            
        
            
            mesh1.addVertex(Q_vertex3);
            mesh1.addVertex(Q_vertex4);
            mesh1.addVertex(Q_vertex1);
            
            //then corresponding texture coordinates one for each vertext
            mesh1.addTexCoord(Q_tex1);
            mesh1.addTexCoord(Q_tex2);
            mesh1.addTexCoord(Q_tex3);
            
            mesh1.addTexCoord(Q_tex3);
            mesh1.addTexCoord(Q_tex4);
            mesh1.addTexCoord(Q_tex1);
        }//endxfor
    }//endyfor
    mesh1.setMode(OF_PRIMITIVE_TRIANGLES);
    vbo_mesh1= mesh1;
    
    
}


//_-_---_-----------
//this makes a mesh of a line horizontal
void ofApp:: horizontal_linemesh(int gridsize){
	int new_gridsize=gridsize*2;
    mesh1.clearVertices();
    mesh1.clearTexCoords();
    ofVec3f Q_vertex1;
    ofVec3f Q_vertex2;

    
    ofVec2f Q_tex1;
    ofVec2f Q_tex2;
   
    for(int i=0;i<new_gridsize;i++){
        for(int j=0;j<new_gridsize;j++){
            int x0=j*width/new_gridsize;
            int x1=(j+1)*width/new_gridsize;
            int y0=i*height/new_gridsize;
           // int y1=(i+1)*height/gridsize;
            
            Q_vertex1.set(x0,y0,0);
            Q_vertex2.set(x1,y0,0);
            
            
            rescale=1.0/new_gridsize;
            float tex_x0=j*rescale;
            float tex_x1=(j+1)*rescale;
            float tex_y0=i*rescale;
            Q_tex1.set(tex_x0,tex_y0);
            Q_tex2.set(tex_x1,tex_y0);
            
            
            
            mesh1.addVertex(Q_vertex1);
            mesh1.addVertex(Q_vertex2);
            
            
            //then corresponding texture coordinates one for each vertext
            mesh1.addTexCoord(Q_tex1);
            mesh1.addTexCoord(Q_tex2);
            
        }//endxfor
    }//endyfor
    mesh1.setMode(OF_PRIMITIVE_LINES);
    
    vbo_mesh1= mesh1;
    
    
}

//----------------------------

//this makes a mesh of a line vertical
void ofApp:: vertical_linemesh(int gridsize){
	int new_gridsize=gridsize*2;
    mesh1.clearVertices();
    mesh1.clearTexCoords();
    ofVec3f Q_vertex1;
    ofVec3f Q_vertex2;
    
    
    ofVec2f Q_tex1;
    ofVec2f Q_tex2;
    
    for(int i=0;i<new_gridsize;i++){
        for(int j=0;j<new_gridsize;j++){
            int x0=i*width/new_gridsize;
           // int x1=(i+1)*width/gridsize;
            int y0=j*height/new_gridsize;
            int y1=(j+1)*height/new_gridsize;
            
            Q_vertex1.set(x0,y0,0);
            Q_vertex2.set(x0,y1,0);
            
            rescale=1.0/new_gridsize;
            float tex_x0=i*rescale;
            float tex_y0=j*rescale;
            float tex_y1=(j+1)*rescale;
            
            Q_tex1.set(tex_x0,tex_y0);
            Q_tex2.set(tex_x0,tex_y1);
            
            
            
            mesh1.addVertex(Q_vertex1);
            mesh1.addVertex(Q_vertex2);
            
            
            //then corresponding texture coordinates one for each vertext
            mesh1.addTexCoord(Q_tex1);
            mesh1.addTexCoord(Q_tex2);
            
        }//endxfor
    }//endyfor
    mesh1.setMode(OF_PRIMITIVE_LINES);
    
    vbo_mesh1= mesh1;
    
    
}



//---------------------
void ofApp:: midibiz(){
    
    for(unsigned int i = 0; i < midiMessages.size(); ++i) {
        
        ofxMidiMessage &message = midiMessages[i];
        
        
        //cout << "envcount="<< envcount<< endl;
        
        if(message.status < MIDI_SYSEX) {
            //text << "chan: " << message.channel;
            if(message.status == MIDI_CONTROL_CHANGE) {
                //  cout << "message.control"<< message.control<< endl;
                //  cout << "message.value"<< message.value<< endl;
                
               
                
                if(message.control==60){
                    if(message.value==127){
                        p_lock_record_switch=1;
                        // p_lock_increment=0;
                        //cout<<"HEY"<<endl;
                        
                        for(int i=0;i<p_lock_number;i++){
								p_lock_smoothed[i]=0;
								for(int j=0;j<p_lock_size;j++){
                                
									p_lock[i][j]=p_lock[i][p_lock_increment];
                                
								}//endplocksize
                            
							}//endplocknumber
                        
                    }
                    
                    if(message.value==0){
                        p_lock_record_switch=0;
                    }
                    
                }
                
                if(message.control==58){
                    if(message.value==127){
                        for(int i=0;i<p_lock_number;i++){
                            
                            for(int j=0;j<p_lock_size;j++){
                                
                                p_lock[i][j]=0;
                                
                            }//endplocksize
                            
                        }//endplocknumber
                        
                        global_y_displace=global_x_displace=rotate_y=rotate_x=0;
                        
                        
                    }
                  
                    
                }
                //nanokontrol2 controls
                if(message.control==16){
                    //  c1=(message.value-63.0)/63.0;
                    c1=(message.value)/127.00;
                    p_lock[0][p_lock_increment]=message.value/127.0f;
                    
                }
                
                
                if(message.control==17){
                     c2=(message.value-63.0)/63.0;
                    //c2=(message.value)/127.00;
                    p_lock[1][p_lock_increment]=(message.value-63.0)/63.0;
                }
                
                
                if(message.control==18){
                    c3=(message.value-63.0)/63.00;
                    //  c3=(message.value)/127.00;
                    p_lock[2][p_lock_increment]=(message.value-63.0)/63.0;
                }
                
                
                if(message.control==19){
                   // c4=(message.value-63.0)/63.00;
                     c4=(message.value)/127.00;
                    p_lock[3][p_lock_increment]=message.value/127.0f;
                }
                
                
                if(message.control==20){
                    c5=(message.value-63.0)/63.00;
                    //  c5=(message.value)/127.00;
                    p_lock[4][p_lock_increment]=(message.value-63.0)/63.0;
                }
                
                
                if(message.control==21){
                    c6=(message.value-63.0)/63.0;
                    // c6=(message.value)/127.00;
                    p_lock[5][p_lock_increment]=(message.value-63.0)/63.0;
                }
                
                
                if(message.control==22){
                    c7=(message.value-63.0)/63.0;
                     //  c7=(message.value)/127.00;
                     p_lock[6][p_lock_increment]=(message.value-63.0)/63.0;
                }
                
                
                if(message.control==23){
                    // c8=(message.value-63.0)/63.00;
                    c8=(message.value)/127.0;
                    p_lock[7][p_lock_increment]=(message.value)/127.0;
                    
                    if(stroke_weight_switch==1){
						stroke_weight=5*(message.value)/127.0;
						}
                    
                }
                
                
                //multi_x_stuffs
                if(message.control==32){
					if(message.value==127){
						global_x_displace_switch=0;
						center_x_displace_switch=1;
					}
					
					if(message.value==0){
						global_x_displace_switch=1;
						center_x_displace_switch=0;
					}
                }
                
                if(message.control==48){
					if(message.value==127){
						center_x_displace_switch=1;
						rotate_x_switch=0;
						global_x_displace_switch=1;
					}
					
					if(message.value==0){
						rotate_x_switch=1;
						center_x_displace_switch=0;
					}
                }
                
                 if(message.control==64){
					if(message.value==127){
						rotate_capture_z_switch=0;
						center_x_displace_switch=1;
						rotate_x_switch=1;
						global_x_displace_switch=1;
						
					}
					
					if(message.value==0){
						rotate_capture_z_switch=1;
						center_x_displace_switch=0;
					}
                }
                
                //so c9 and c10 will be multipurpose switches
                if(message.control==120){
                    c9=(message.value-63.0)/63.0;
                    
                    //c9=(message.value)/127.00;
                    
                    if(center_x_displace_switch==0){
						center_x_displace=-960*c9;
						
						p_lock[8][p_lock_increment]=-960.0*(message.value-63.0)/63.0;
						}
                    
                    if(global_x_displace_switch==0){
						global_x_displace=ofGetWidth()/4*c9;
						}
						
					if(rotate_x_switch==0){
						rotate_x=c9*3.14;
					}
					
					if(rotate_capture_z_switch==0){
						rotate_capture_z=c9*3.14;
					}
                    
                }
                
                
                
                 //multi_y_stuffs
                if(message.control==33){
					if(message.value==127){
						global_y_displace_switch=0;
						center_y_displace_switch=1;
					}
					
					if(message.value==0){
						global_y_displace_switch=1;
						center_y_displace_switch=0;
					}
                }
                
                if(message.control==49){
					if(message.value==127){
						center_y_displace_switch=1;
						rotate_y_switch=0;
						global_y_displace_switch=1;
					}
					
					if(message.value==0){
						rotate_y_switch=1;
						center_y_displace_switch=0;
					}
                }
                /*
                 if(message.control==65){
					if(message.value==127){
						rotate_capture_z_switch=0;
						center_y_displace_switch=1;
						rotate_y_switch=1;
						global_y_displace_switch=1;
						
					}
					
					if(message.value==0){
						rotate_capture_z_switch=1;
						center_y_displace_switch=0;
					}
					
                }
                */
                
                if(message.control==121){
                   c10=(message.value-63.0)/63.0;
                   
                   
                   if(center_y_displace_switch==0){
						center_y_displace=-960*c10;
						p_lock[9][p_lock_increment]=-960.0*(message.value-63.0)/63.0;
						}
                    
                    if(global_y_displace_switch==0){
						global_y_displace=ofGetHeight()/4*c10;
						}
						
					if(rotate_y_switch==0){
						rotate_y=c10*3.14;
					}
					/*
					if(rotate_capture_z_switch==0){
						rotate_capture_z=c10*3.14;
					}
                    */
                }
                
                if(message.control==122){
                    c11=.1*(message.value-63.0)/63.00;
                    //c11=(message.value)/127.00;
                    
                    p_lock[10][p_lock_increment]=.1*(message.value-63.0)/63.0;
                    
                    if(z_freq0==TRUE){
                        c11=(message.value-63.0)/63.0;
                    }
                  
                   

                }
                
                
                if(message.control==123){
                     c12=(message.value-63.0)/63.00;
                    //c12=(message.value)/127.00;
                    p_lock[11][p_lock_increment]=(message.value-63.0)/63.0;
                    
                }
                
                if(message.control==124){
                    c13=.1*(message.value-63.0)/63.00;
                    //c13=(message.value)/127.00;
                    p_lock[12][p_lock_increment]=.1*(message.value-63.0)/63.0;
                    
                    if(x_freq0==TRUE){
                        c13=(message.value-63.0)/63.0;
                    }
                   
                   

                    
                }
                
                if(message.control==125){
                    c14=(message.value-63.0)/63.0;
                    p_lock[13][p_lock_increment]=(message.value-63.0)/63.0;
                   // c14=(message.value)/127.00;
                }
                
                if(message.control==126){
                    c15=.1*(message.value-63.0)/63.0;
                    p_lock[14][p_lock_increment]=.1*(message.value-63.0)/63.0;
                    //c15=(message.value)/127.00;
                    
                    if(y_freq0==TRUE){
                        c15=(message.value-63.0)/63.0;
                    }
                   
                    
                    
                }
                
                if(message.control==127){
                    c16=(message.value-63.0)/63.00;
                    p_lock[15][p_lock_increment]=(message.value-63.0)/63.0;
                    //c16=(message.value)/127.0;
                    
                }
                
                
                //frequency switches
                
                //y
                if(message.control==38){
                    if(message.value==127){
                        y_freq0=TRUE;
                    }
                    
                    if(message.value==0){
                        y_freq0=FALSE;
                    }

                }
                
                if(message.control==54){
                    if(message.value==127){
                        y_ringmod_switch=TRUE;
                    }
                    
                    if(message.value==0){
                        y_ringmod_switch=FALSE;
                    }
                }
                
                if(message.control==70){
                    if(message.value==127){
                        y_phasemod_switch=TRUE;
                    }
                    
                    if(message.value==0){
                        y_phasemod_switch=FALSE;
                    }
                }

                //x
                if(message.control==36){
                    if(message.value==127){
                        x_freq0=TRUE;
                    }
                    
                    if(message.value==0){
                        x_freq0=FALSE;
                    }
                    
                }
                
                if(message.control==52){
                    if(message.value==127){
                        x_ringmod_switch=TRUE;
                    }
                    
                    if(message.value==0){
                        x_ringmod_switch=FALSE;
                    }
                }
                
                if(message.control==68){
                    if(message.value==127){
                        x_phasemod_switch=TRUE;
                    }
                    
                    if(message.value==0){
                        x_phasemod_switch=FALSE;
                    }
                }
                
                //z
                if(message.control==34){
                    if(message.value==127){
                        z_freq0=TRUE;
                    }
                    
                    if(message.value==0){
                        z_freq0=FALSE;
                    }
                    
                }
                
                if(message.control==50){
                    if(message.value==127){
                        z_ringmod_switch=TRUE;
                    }
                    
                    if(message.value==0){
                        z_ringmod_switch=FALSE;
                    }
                }
                
                if(message.control==66){
                    if(message.value==127){
                        z_phasemod_switch=TRUE;
                    }
                    
                    if(message.value==0){
                        z_phasemod_switch=FALSE;
                    }
                }


                
                
                //y_lfo_switches
                //y_lfo_switch=0;
                if(message.control==39){
                   
                    if(message.value==127){
                        y_lfo_switch=1;
                      
                    }
                    if(message.value==0){
                        y_lfo_switch=0;
                      
                    }
                    
                }
                
                if(message.control==55){
                    if(message.value==127){
                        y_lfo_switch=2;
                       
                    }
                    if(message.value==0){
                        y_lfo_switch=0;
                      
                    }
                    
                }
                
                if(message.control==71){
                    if(message.value==127){
                        y_lfo_switch=3;
                       
                    }
                    if(message.value==0){
                        y_lfo_switch=0;
                       
                    }
                    
                }
                
                if(message.control==37){
                    
                    if(message.value==127){
                        x_lfo_switch=1;
                       
                    }
                    if(message.value==0){
                        x_lfo_switch=0;
                        
                    }
                    
                }
                
                if(message.control==53){
                    if(message.value==127){
                        x_lfo_switch=2;
                        
                    }
                    if(message.value==0){
                        x_lfo_switch=0;
                        
                    }
                    
                }
                
                
                if(message.control==69){
                    if(message.value==127){
                        x_lfo_switch=3;
                       
                    }
                    if(message.value==0){
                        x_lfo_switch=0;
                       
                    }
                    
                }
                
                if(message.control==35){
                    
                    if(message.value==127){
                        z_lfo_switch=1;
                       
                    }
                    if(message.value==0){
                        z_lfo_switch=0;
                        
                    }
                    
                }
                
                if(message.control==51){
                    if(message.value==127){
                        z_lfo_switch=2;
                        
                    }
                    if(message.value==0){
                        z_lfo_switch=0;
                        
                    }
                    
                }
                
                 if(message.control==67){
                    if(message.value==127){
                        z_lfo_switch=3;
                        
                    }
                    if(message.value==0){
                        z_lfo_switch=0;
                        
                    }
                    
                }
                
                
                
                //weird_switch
                if(message.control==46){
                    if(message.value==127){
                        b_w_switch=1;
                        
                    }
                    if(message.value==0){
                        b_w_switch=0;
                        
                    }
                    
                }
                
                if(message.control==43){
                    if(message.value==127){
                        trianglemesh(scale);
                        wireframe_switch=FALSE;
                    }
                    
                }
                
                if(message.control==44){
                    if(message.value==127){
                        horizontal_linemesh(scale);
                    }
                    
                }
                
                if(message.control==42){
                    if(message.value==127){
                        vertical_linemesh(scale);
                    }
                    
                }
                
                if(message.control==41){
                    if(message.value==127){
                        trianglemesh(scale);
                        wireframe_switch=TRUE;
                    }
                    
                }
                
                if(message.control==45){
                    if(message.value==127){
                        
                        stroke_weight_switch=1;
                    }
                    
                    if(message.value==0){
                        
                        stroke_weight_switch=0;
                    }
                    
                }
                
                if(message.control==61){
                    if(message.value==127){
                        
                        bright_switch=1;
                    }
                    
                    if(message.value==0){
                        
                        bright_switch=0;
                    }
                    
                }
                
                if(message.control==59){
                    if(message.value==127){
                        
                        invert_switch=1;
                    }
                    
                    if(message.value==0){
                        
                        invert_switch=0;
                    }
                    
                }
                
                if(message.control==62){
                    if(message.value==127){
                        hdmi_aspect_ratio_switch=1;
                        //luma_switch=1;
                    }
                    
                    if(message.value==0){
                        hdmi_aspect_ratio_switch=0;
                        //luma_switch=0;
                    }
                    
                }

                
                //add a reset switch for parameters
                //and parameter locks dang it
                
              
                
                //text << "\tctl: " << message.control;
                //ofDrawRectangle(x + ofGetWidth()*0.2, y + 12,
                //	ofMap(message.control, 0, 127, 0, ofGetWidth()*0.2), 10);
            }
            
            
            //text << " "; // pad for delta print
        }//
        
        
    }
    
    
    /******* endmidimessagesbiz*********/
    
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
  
}


//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseReleased() {
}



//--------------------------------------------------------------




//--------------------------------------------------------------


