



#include "ofApp.h"


#include <iostream>

ofPolyline tetrahedron;

//0 is picapture, 1 is usbinput
bool inputswitch=1;

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


int scale=127;

int scale_key=0;

int y_lfo_switch=0;
int x_lfo_switch=0;
int z_lfo_switch=0;

bool y_freq0=FALSE;
bool y_freq1=FALSE;


bool x_freq0=FALSE;
bool x_freq1=FALSE;


bool z_freq0=FALSE;
bool z_freq1=FALSE;


bool y_xmod_switch=FALSE;
bool x_xmod_switch=FALSE;
bool z_xmod_switch=FALSE;

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

float stroke_weight=1;

//mesh types
//0 is trianglegrid
//1 is horizontal lines
//2 is vertical lines
int mesh_type=0;

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
    
    
   
    
    //framebufferbiz
    fbo0.allocate(width,height);
    fbo0.begin();
    ofClear(0,0,0,255);
    fbo0.end();
    
   
    
   
     trianglemesh(scale);
  

   
    
    
    
    
    
    
    
}

//--------------------------------------------------------------
void ofApp::update() {

  if(inputswitch==1){
		cam1.update();
	}
	
	if(inputswitch==0){
		
		omx_updates();
		
	}
    
    /*midimessagesbiz**/
    midibiz();

    
  
}

//--------------------------------------------------------------
void ofApp::draw() {
    
    
   
  
    fbo0.begin();
    
    shader_displace.begin();
    ofBackground(0);
   
    if(invert_switch==1){ofBackground(255);}
    
    
    shader_displace.setUniform1f("luma_key_level",c1+1.01*az);
    shader_displace.setUniform1f("amnt1",c2*100);
    
    ofVec2f xyztheta;
    xyztheta.set(200*(c2+qw),200*(c3+er));
    shader_displace.setUniform2f("xyztheta",xyztheta);
    shader_displace.setUniform1i("width",width);
    shader_displace.setUniform1i("height",height);
    shader_displace.setUniform1i("bright_switch",bright_switch);
    shader_displace.setUniform1i("invert_switch",invert_switch);
    
    //lfo1 for the xyz
    z_lfo_arg+=c11+dc;
    x_lfo_arg+=c13+hn;
    y_lfo_arg+=c15+ll;
   // x_lfo=sin(x_lfo_arg)*(1+127*c14);
	shader_displace.setUniform1f("z_lfo_amp",c12*.25+fv);
    shader_displace.setUniform1f("z_lfo_arg",z_lfo_arg);
    shader_displace.setUniform1f("z_lfo_other",c4*.05+sx);
   
    shader_displace.setUniform1f("x_lfo_amp",ofGetWidth()*c14+jm);
    shader_displace.setUniform1f("x_lfo_arg",x_lfo_arg);
    shader_displace.setUniform1f("x_lfo_other",c5*height/10000.0+gb);
    
    shader_displace.setUniform1f("y_lfo_amp",ofGetHeight()*c16+ylfo_amp);
    shader_displace.setUniform1f("y_lfo_arg",y_lfo_arg);
    shader_displace.setUniform1f("y_lfo_other",c6*width/10000.0+kk);
    
    
    
    
    
    
    
    ofVec2f xy_offset;
    xy_offset.set(center_x_displace+ty,center_y_displace+ui);
    shader_displace.setUniform2f("xy_offset",xy_offset);
    
    
    shader_displace.setUniform1i("y_lfo_switch",y_lfo_switch);
    shader_displace.setUniform1i("x_lfo_switch",x_lfo_switch);
    shader_displace.setUniform1i("z_lfo_switch",z_lfo_switch);
    
    shader_displace.setUniform1i("y_xmod_switch",y_xmod_switch);
    shader_displace.setUniform1i("x_xmod_switch",x_xmod_switch);
    shader_displace.setUniform1i("z_xmod_switch",z_xmod_switch);
    
    shader_displace.setUniform1i("weird_switch",weird_switch);
    shader_displace.setUniform1i("luma_switch",luma_switch);
   
    
    if(inputswitch==0){
			videoGrabber.getTextureReference().bind();
		}
		
		if(inputswitch==1){
			
		
			cam1.getTexture().bind();
			
		}
    
  //  cam1.getTexture().bind();
    
    
    
  
    //ofSetLineWidth(10);
    
    glLineWidth(stroke_weight);
    
    ofPushMatrix();
    ofTranslate(0,0,c7*480 +op);
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
			cam1.getTexture().unbind();
		}
    
    
    //cam1.getTexture().unbind();
    
  
    
    shader_displace.end();
    
    
    
    
    
    
    fbo0.end();
	
	
   
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2,ofGetHeight()/2,100);
    ofRotateXRad(rotate_x);
    ofRotateYRad(rotate_y);
    ofRotateZRad(rotate_capture_z);
    fbo0.draw(-ofGetWidth()/2+global_x_displace,-ofGetHeight()/2+global_y_displace-80,720,640);
    ofPopMatrix();
   
    
    //switch for this too maybe
    
    fbo0.begin();
    ofClear(0,0,0,255);
    fbo0.end();
   
	
	
    //choose a different scale for the pi
    //for each mesh type have a rescaling option to prevent any slowdowns
    //each mesh has different scale ranges for optimizations
    //so just feed in c8+1 and rescale within
  
	scale=(1.0-c8)*127+1.0 +scale_key;
	
	if(scale>=127){scale=127;}
	if(scale<=1){
			scale=1;
			}
    //scale=50;
    
    ofSetColor(255);
    string msg="fps="+ofToString(ofGetFrameRate(),2)+" stroke"+ofToString(stroke_weight,2)+" stroke switch"+ofToString(stroke_weight_switch,0);
    //ofDrawBitmapString(msg,10,height-10);
    
    
   
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
    if(key=='4'){y_xmod_switch=!y_xmod_switch;}
    if(key=='5'){weird_switch=!weird_switch;}
    
    if(key=='6'){
		z_lfo_switch++;
		z_lfo_switch=z_lfo_switch%3;
		
		}
		
	if(key=='7'){
		x_lfo_switch++;
		x_lfo_switch=x_lfo_switch%3;
		
		}
		
	if(key=='8'){
		y_lfo_switch++;
		y_lfo_switch=y_lfo_switch%3;
		
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
    mesh1.clearVertices();
    mesh1.clearTexCoords();
    ofVec3f Q_vertex1;
    ofVec3f Q_vertex2;

    
    ofVec2f Q_tex1;
    ofVec2f Q_tex2;
   
    for(int i=0;i<gridsize;i++){
        for(int j=0;j<gridsize;j++){
            int x0=j*width/gridsize;
            int x1=(j+1)*width/gridsize;
            int y0=i*height/gridsize;
           // int y1=(i+1)*height/gridsize;
            
            Q_vertex1.set(x0,y0,0);
            Q_vertex2.set(x1,y0,0);
            
            
            rescale=1.0/gridsize;
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
    mesh1.clearVertices();
    mesh1.clearTexCoords();
    ofVec3f Q_vertex1;
    ofVec3f Q_vertex2;
    
    
    ofVec2f Q_tex1;
    ofVec2f Q_tex2;
    
    for(int i=0;i<gridsize;i++){
        for(int j=0;j<gridsize;j++){
            int x0=i*width/gridsize;
           // int x1=(i+1)*width/gridsize;
            int y0=j*height/gridsize;
            int y1=(j+1)*height/gridsize;
            
            Q_vertex1.set(x0,y0,0);
            Q_vertex2.set(x0,y1,0);
            
            rescale=1.0/gridsize;
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
                
               
                
                
                //nanokontrol2 controls
                if(message.control==16){
                    //  c1=(message.value-63.0)/63.0;
                    c1=(message.value)/127.00;
                    
                }
                
                
                if(message.control==17){
                     c2=(message.value-63.0)/63.0;
                    //c2=(message.value)/127.00;
                    
                }
                
                
                if(message.control==18){
                    c3=(message.value-63.0)/63.00;
                    //  c3=(message.value)/127.00;
                }
                
                
                if(message.control==19){
                   // c4=(message.value-63.0)/63.00;
                     c4=(message.value)/127.00;
                    
                }
                
                
                if(message.control==20){
                    c5=(message.value-63.0)/63.00;
                    //  c5=(message.value)/127.00;
                    
                }
                
                
                if(message.control==21){
                    c6=(message.value-63.0)/63.0;
                    // c6=(message.value)/127.00;
                }
                
                
                if(message.control==22){
                    c7=(message.value-63.0)/63.0;
                     //  c7=(message.value)/127.00;
                }
                
                
                if(message.control==23){
                    // c8=(message.value-63.0)/63.00;
                    c8=(message.value)/127.0;
                    
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
                    c11=.01*(message.value-63.0)/63.00;
                    //c11=(message.value)/127.00;
                    
                    if(z_freq0==TRUE){
                        c11=.1*(message.value-63.0)/63.0;
                    }
                    if(z_freq1==TRUE){
                        c11=(message.value-63.0)/63.0;
                    }
                   

                }
                
                
                if(message.control==123){
                     c12=(message.value-63.0)/63.00;
                    //c12=(message.value)/127.00;
                    
                }
                
                if(message.control==124){
                    c13=.01*(message.value-63.0)/63.00;
                    //c13=(message.value)/127.00;
                    
                    if(x_freq0==TRUE){
                        c13=.1*(message.value-63.0)/63.0;
                    }
                    if(x_freq1==TRUE){
                        c13=(message.value-63.0)/63.0;
                    }
                   

                    
                }
                
                if(message.control==125){
                    c14=(message.value-63.0)/63.0;
                   // c14=(message.value)/127.00;
                }
                
                if(message.control==126){
                    c15=.01*(message.value-63.0)/63.0;
                    //c15=(message.value)/127.00;
                    
                    if(y_freq0==TRUE){
                        c15=.1*(message.value-63.0)/63.0;
                    }
                    if(y_freq1==TRUE){
                        c15=(message.value-63.0)/63.0;
                    }
                    
                    
                }
                
                if(message.control==127){
                    c16=(message.value-63.0)/63.00;
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
                        y_freq1=TRUE;
                    }
                    
                    if(message.value==0){
                        y_freq1=FALSE;
                    }
                }
                
                if(message.control==70){
                    if(message.value==127){
                        y_xmod_switch=TRUE;
                    }
                    
                    if(message.value==0){
                        y_xmod_switch=FALSE;
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
                        x_freq1=TRUE;
                    }
                    
                    if(message.value==0){
                        x_freq1=FALSE;
                    }
                }
                
                if(message.control==68){
                    if(message.value==127){
                        x_xmod_switch=TRUE;
                    }
                    
                    if(message.value==0){
                        x_xmod_switch=FALSE;
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
                        z_freq1=TRUE;
                    }
                    
                    if(message.value==0){
                        z_freq1=FALSE;
                    }
                }
                
                if(message.control==66){
                    if(message.value==127){
                        z_xmod_switch=TRUE;
                    }
                    
                    if(message.value==0){
                        z_xmod_switch=FALSE;
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
                
                
                
                
                //weird_switch
                if(message.control==46){
                    if(message.value==127){
                        weird_switch=1;
                        
                    }
                    if(message.value==0){
                        weird_switch=0;
                        
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
                        
                        luma_switch=1;
                    }
                    
                    if(message.value==0){
                        
                        luma_switch=0;
                    }
                    
                }

                
                
                
              
                
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


