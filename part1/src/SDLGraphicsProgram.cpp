#include "SDLGraphicsProgram.hpp"
#include "Camera.hpp"
#include "MarchingCubes.hpp"
#include "EmptyObject.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>

// Initialization function
// Returns a true or false value based on successful completion of setup.
// Takes in dimensions of window.
SDLGraphicsProgram::SDLGraphicsProgram(int w, int h){
	// Initialization flag
	bool success = true;
	// String to hold any errors that occur.
	std::stringstream errorStream;
	// The window we'll be rendering to
	m_window = NULL;

	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO)< 0){
		errorStream << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
		success = false;
	}
	else{
		//Use OpenGL 3.3 core
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
		// We want to request a double buffer for smooth updating.
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		//Create window
		m_window = SDL_CreateWindow( "Lab",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                w,
                                h,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

		// Check if Window did not create.
		if( m_window == NULL ){
			errorStream << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}

		//Create an OpenGL Graphics Context
		m_openGLContext = SDL_GL_CreateContext( m_window );
		if( m_openGLContext == NULL){
			errorStream << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}

		// Initialize GLAD Library
		if(!gladLoadGLLoader(SDL_GL_GetProcAddress)){
			errorStream << "Failed to iniitalize GLAD\n";
			success = false;
		}

		//Initialize OpenGL
		if(!InitGL()){
			errorStream << "Unable to initialize OpenGL!\n";
			success = false;
		}
  	}

    // If initialization did not work, then print out a list of errors in the constructor.
    if(!success){
        errorStream << "SDLGraphicsProgram::SDLGraphicsProgram - Failed to initialize!\n";
        std::string errors=errorStream.str();
        SDL_Log("%s\n",errors.c_str());
    }else{
        SDL_Log("SDLGraphicsProgram::SDLGraphicsProgram - No SDL, GLAD, or OpenGL, errors detected during initialization\n\n");
    }

	// SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN); // Uncomment to enable extra debug support!
	GetOpenGLVersionInfo();


    // Setup our Renderer
    m_renderer = new Renderer(w,h);    
}


// Proper shutdown of SDL and destroy initialized objects
SDLGraphicsProgram::~SDLGraphicsProgram(){
    if(m_renderer!=nullptr){
        delete m_renderer;
    }


    //Destroy window
	SDL_DestroyWindow( m_window );
	// Point m_window to NULL to ensure it points to nothing.
	m_window = nullptr;
	//Quit SDL subsystems
	SDL_Quit();
}


// Initialize OpenGL
// Setup any of our shaders here.
bool SDLGraphicsProgram::InitGL(){
	//Success flag
	bool success = true;

	return success;
}

float data_func(float x, float y, float z) {
    return sin(x * 35) + cos(y * 30) + sin(z * 33);
}

float* make_data(unsigned int xSegs, unsigned int ySegs, unsigned int zSegs) {
    float* result = new float[xSegs * ySegs * zSegs];
    for(unsigned int z = 0; z < zSegs; z++) {
        for(unsigned int y = 0; y < ySegs; y++) {
            for(unsigned int x = 0; x < xSegs; x++) {
                // result[((z * (xSegs * ySegs)) + (y * xSegs) + x)] = data_func(
                //     static_cast<float>(x) / static_cast<float>(xSegs),
                //     static_cast<float>(y) / static_cast<float>(ySegs),
                //     static_cast<float>(z) / static_cast<float>(zSegs)
                //     );
                unsigned int index = ((z * (xSegs * ySegs)) + (y * xSegs) + x);
                if (x == 0 || x == xSegs - 1
                || y == 0 || y == ySegs - 1
                || z == 0 || z == zSegs - 1) {
                    result[index] = 0.5999f;
                } else {
                    result[index] = 1;
                }
            }
        }
    }
    return result;
}

inline float clampf(float val, float min, float max) {
    return val < min ? min : (val > max ? max : val);
}

void dig(MarchingCubes* mc, float indicatorX, float indicatorY, float indicatorZ) {
    indicatorX += 1;
    indicatorX *= 12.5f;
    indicatorY += 1;
    indicatorY *= 12.5f;
    indicatorZ += 1;
    indicatorZ *= 12.5f;
    float* sphereData = mc->SphereExplosionData(3, 0, indicatorX, indicatorY, indicatorZ);
    mc->Subtract(sphereData);
    mc->Init(2, 2, 2, 0.6f);
    delete[] sphereData;
}

//Loops forever!
void SDLGraphicsProgram::Loop(){

    // Main loop flag
    // If this is quit = 'true' then the program terminates.
    bool quit = false;
    // Event handler that handles various events in SDL
    // that are related to input and output
    SDL_Event e;
    // Enable text input
    SDL_StartTextInput();

	float data3[] = {
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
        
        0, 0, 0,
        0, 1, 0,
        0, 0, 0,

        0, 0, 0,
        0, 0, 0,
        0, 0, 0
    };
	float data5[] = {
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 1, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        
        0, 0, 0, 0, 0,
        0, 0, 1, 0, 0,
        0, 1, 0, 1, 0,
        0, 0, 1, 0, 0,
        0, 0, 0, 0, 0,
        
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 1, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0
    };
    float* bigData = make_data(25, 25, 25);
    MarchingCubes* mc = new MarchingCubes(25, 25, 25, 2, 2, 2, bigData, 0.6f);
	SceneNode* mcNode = new SceneNode(mc);

    float indicatorX = 0;
    float indicatorY = 0;
    float indicatorZ = 0;
    float indicatorSpeed = 0.04f;
    MarchingCubes* indicator = new MarchingCubes(3, 3, 3, 1, 1, 1, data3, 0.5f);
    SceneNode* indicatorNode = new SceneNode(indicator);

    EmptyObject* root = new EmptyObject();
    SceneNode* rootNode = new SceneNode(root);
    rootNode->AddChild(mcNode);
    rootNode->AddChild(indicatorNode);

	m_renderer->setRoot(rootNode);

    m_renderer->GetCamera(0)->SetCameraEyePosition(0.0f,0.0f,3.5f);

    float rotate = 0.0f;
    float x = -1;

    float yRot = 0;
    float xRot = 0;

    // While application is running
    while(!quit){

        //Handle events on queue
        while(SDL_PollEvent( &e ) != 0){
            // User posts an event to quit
            // An example is hitting the "x" in the corner of the window.
            if(e.type == SDL_QUIT){
                quit = true;
            }
            // Handle keyboad input for the camera class
            if(e.type==SDL_MOUSEMOTION){
                // Handle mouse movements
                if (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1)) {
                    xRot += e.motion.xrel * 0.005f;
                    yRot += e.motion.yrel * 0.005f;
                    yRot = clampf(yRot, -0.625, 0.625f);
                }
                // m_renderer->GetCamera(0)->MouseLook(mouseX, mouseY);
            }
            switch(e.type){
                // Handle keyboard presses
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym){
                        case SDLK_x:
                            {
                                float* explodeData = mc->SphereExplosionData(1.0f + (x / 3), 1.0f, x, 12, 5);
                                x += 0.1f;
                                bigData = mc->Subtract(explodeData);
                                delete[] explodeData;
                                mc->Init(2, 2, 2, 0.6f);
                            }
                            break;
                        case SDLK_r:
                            {
                                float* resetData = make_data(25, 25, 25);
                                mc->SetData(resetData);
                                delete[] resetData;
                                mc->Init(2, 2, 2, 0.6f);
                                x = -1;
                            }
                            break;
                        case SDLK_LEFT:
                            indicatorX -= indicatorSpeed;
                            dig(mc, indicatorX, indicatorY, indicatorZ);
                            break;
                        case SDLK_RIGHT:
                            indicatorX += indicatorSpeed;
                            dig(mc, indicatorX, indicatorY, indicatorZ);
                            break;
                        case SDLK_DOWN:
                            indicatorZ += indicatorSpeed;
                            dig(mc, indicatorX, indicatorY, indicatorZ);
                            break;
                        case SDLK_UP:
                            indicatorZ -= indicatorSpeed;
                            dig(mc, indicatorX, indicatorY, indicatorZ);
                            break;
                        case SDLK_RSHIFT:
                            indicatorY += indicatorSpeed;
                            dig(mc, indicatorX, indicatorY, indicatorZ);
                            break;
                        case SDLK_RCTRL:
                            indicatorY -= indicatorSpeed;
                            dig(mc, indicatorX, indicatorY, indicatorZ);
                            break;
                    }
                break;
            }
        } // End SDL_PollEvent loop.
		
        rootNode->GetLocalTransform().LoadIdentity();
        rootNode->GetLocalTransform().Rotate(yRot, 1, 0, 0);
        rootNode->GetLocalTransform().Rotate(xRot, 0, 1, 0);

        indicatorNode->GetLocalTransform().LoadIdentity();
        indicatorNode->GetLocalTransform().Translate(indicatorX, indicatorY, indicatorZ);

        rotate += 0.04f;

        // Update our scene through our renderer
        m_renderer->Update();
        // Render our scene using our selected renderer
        m_renderer->Render();
        // Delay to slow things down just a bit!
        SDL_Delay(25);  // TODO: You can change this or implement a frame
                        // independent movement method if you like.
      	//Update screen of our specified window
      	SDL_GL_SwapWindow(GetSDLWindow());
	}
    //Disable text input
    SDL_StopTextInput();
}


// Get Pointer to Window
SDL_Window* SDLGraphicsProgram::GetSDLWindow(){
  return m_window;
}

// Helper Function to get OpenGL Version Information
void SDLGraphicsProgram::GetOpenGLVersionInfo(){
	SDL_Log("(Note: If you have two GPU's, make sure the correct one is selected)");
	SDL_Log("Vendor: %s",(const char*)glGetString(GL_VENDOR));
	SDL_Log("Renderer: %s",(const char*)glGetString(GL_RENDERER));
	SDL_Log("Version: %s",(const char*)glGetString(GL_VERSION));
	SDL_Log("Shading language: %s",(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}
