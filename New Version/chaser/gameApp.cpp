
//=============================================================================
// gameApp.cpp 
//
//Author: Doron Nussbaum (C) 2011 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. demonstrates how to use directx input 
// b. demonstrates how to use directx fonts
// c. demonstrates how to initialize direct3D
// d. provide a framework for creating games
// e. demonstrates how to create an egnine frame and provide a skeleton function of one.
// f. demonstarte the usage of a camera
// g. demonstrates the creation of a surface and of vertex and index buffers
// h. demonstartes how to set the differnt matrices. 
//
// Description:
//--------------
// A simple application that demonstrates how to use directx to surface and how to use the camea.  
// It uses a framework for a general purpose simple game engine
//
//
// License
//--------------
//
// Code can be used for instructional and educational purposes and for assignments in the gaming courses at 
// the School of Compuer Science at Carleton University.
// Usage of code for other purposes is not allowed without a given permission by the author.
//
//
// Disclaimer
//--------------
//
// The code is provided as is without any warranty

//=============================================================================


#include "gameApp.h"
#include "House.h"
#include "TargetHouse.h"
#include <algorithm>
#include "readFromTxt.h"
/********************************************************************************/


// DEFINE


#define FRAMES_PER_SECOND 30

#define NORMAL_SPEED 0.5
#define MAX_SPEED 2.0

// event types for processing game state
#define GAME_UPDATE_FRAME_STATE_EVENT 1
#define GAME_COLLISION_EVENT 2

//size of the surface(my defination)
#define	SURFACE_ROW 50
#define SURFACE_COL 50
#define SURFACE_WIDTH 100.0
#define SURFACE_HEIGHT 100.0

//number of obstacles
#define NUMBER_OF_HOUSES 10
#define NUMBER_OF_CHASERS 5

/*******************************************************************************/
// class static variables
camera *gameApp::cam = NULL;
camera *gameApp::preyFP = NULL;
camera *gameApp::preyTP = NULL;
camera *gameApp :: overPers = NULL;

meshSurface *gameApp::drawSurface = NULL;
Prey   *gameApp::preyRedKia = NULL;
Chaser *gameApp::chaserYellowKia = NULL;
TargetHouse *gameApp::th = NULL;

//net work related 
Truck *gameApp::truck = NULL;


gameApp * gameApp::myApp;
std::vector<gameObject *> gameApp::gameStaticEntities;	// these entities do not change with time
std::vector<gameObject *> gameApp::gameDynamicEntities; // these entities' attributes change with time





/******************************************************************/
/*
Purpose: constructor of the game application


Descripton: 

Return:


*/




gameApp::gameApp(void) 
{

	myApp = this;
	//initialize the net client
}





/******************************************************************/
/*
Purpose: destructor of the game application


Descripton: 

Return:
1 - if failed
0 - if successful

*/

gameApp::~gameApp(void)
{
}





/******************************************************************/
/*
Purpose: initializes the graphics device

Descripton: 

Return:
1 - if failed
0 - if successful

*/



int gameApp::initGraphics(int argc, char** argv, int winWidth, int winHeight, int winPosx, int winPosy)
{


	int rc = 0;

	GLenum err;

	//  initialize GLUT and pass it the command variables
	glutInit(&argc, argv);
	
	//glutInitDisplayMode(GLUT_DOUBLE);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	// Set the window
	// instruct openGL where to put the window on the screen
	glutInitWindowPosition(winPosx, winPosy);

	// instuct openGL what window size ot use
	glutInitWindowSize(winWidth, winHeight);		// no magic numbers

	////here i can initiate a second window, which is used to show the minimap of the game
	// careate the fist window and obtain a handle to it 
	//w2Id = glutCreateWindow("MiniMap");
	// Set the window
	// instruct openGL where to put the window on the screen
	glutInitWindowPosition(10, 10);

	// instuct openGL what window size ot use
	glutInitWindowSize(800, 600);		// no magic numbers

	//  instruct openGL to use a single display buffer and use 4 values for a colour RGB and Alpha
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

	//my window
	// careate the fist window and obtain a handle to it 
	w2Id = glutCreateWindow("My second Window");
	glutDisplayFunc(renderSceneFun);
	glutReshapeFunc(reshapeFun);
	//glutKeyboardFunc(keyboardFun);
	glutTimerFunc(10, timerFun, 1);


	
	// careate the fist window and obtain a handle to it 
	wId = glutCreateWindow("My First Window");

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	// set the colour buffer to black   
	glEnable(GL_DEPTH_TEST);				// Enable depth ubffer testing 
	glClearDepth(1.0f);						// Set depth buffer 
	glDepthFunc(GL_LEQUAL);					// depth test operation (this is the default)
	glShadeModel(GL_SMOOTH);				//  smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // highest quality perspective corrections

	// set the call back functions
	glutDisplayFunc(renderFrame);
	glutReshapeFunc(reshapeFun);
	//keyboardFun is a function parameter, which I should define to let OpenGL perform the related keyboard manipulation
	glutKeyboardFunc(keyboardFun);
	//register my preyKeyboardFun
	glutSpecialFunc(specialKeyboardFun);
	glutTimerFunc(10, timerFun, 1);






	// initialize GLEW
	err = glewInit();
	//GLenum err = glewInit();
	if (err != GLEW_OK)  fprintf(stderr, " Error initializing GLEW! \n");
	else fprintf(stderr, "Initializing GLEW succeeded!\n");


	return 0;


	return(0);
}

void gameApp::renderSceneFun(void)
{
	static float red = 0.0, green = 0.0, blue = 0.0;
	static float deltaColour = 0.05;


	glClearColor(1.0, 1.0, 1.0, 0.0);
	/*red += deltaColour;
	if (red > 1.0) {
		red = 0.0;
		green += deltaColour;
		if (green > 1.0) {
			green = 0.0;
			blue += deltaColour;
			if (blue > 1.0) blue = 0.0;
		}
	}*/

	glColor3f;
	glPointSize(4.0);
	glBegin(GL_LINE);
	glVertex2i(100, 50);
	glVertex2i(100, 130);

	glEnd();
	glFlush();


	glClearColor(red, green, blue, 0.0);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //Clear the screen

	glutSwapBuffers();
}






/******************************************************************/
/*
Purpose: executes the message loop

Descripton: 
This function is a virtual function and can be replaced by a similar function in the derived class.



Return:
1 - if failed
0 - if successful

*/


int gameApp::gameLoop(void)
{
	// enter glut main loop
	glutMainLoop();

	return 0;
}


/******************************************************************/
/*
Purpose: updates the game state

Descripton: 

Return:
1 - if failed
0 - if successful

*/




int gameApp::updateGameState(long event)
{

	static int frameCount = 0;
	static int dir = 1;
	unsigned int i;
	int rc;

	switch (event) {
	case GAME_UPDATE_FRAME_STATE_EVENT:
		
		frameCount++;	// increament frame count

		// this section should have the game logic with respect to time.
		// Then it calls each object with updateStateFunction

		for (i = 0; i < gameDynamicEntities.size(); i++) {
			rc = gameDynamicEntities.at(i)->updateState(frameCount);
			if (rc == GAME_OBJECT_DELETE_ENTITY) {
				// swap current object with the last one
				gameObject *temp = gameDynamicEntities.at(i);
				gameDynamicEntities.at(i) = gameDynamicEntities.back();
				delete temp;
				gameDynamicEntities.pop_back();
			}
		}

		
		break;
	case GAME_COLLISION_EVENT:
		//collision detection and handling
		handleCollisionCarWithCars(preyRedKia, gameDynamicEntities);
		//handleCollisionCarWithCars(chaserYellowKia, gameDynamicEntities);
		handleCoolisionCarWithHouses(preyRedKia, gameStaticEntities);
		handleCoolisionCarWithHouses(chaserYellowKia, gameStaticEntities);
		checkAndHandleCollisionWithBoundaries(gameDynamicEntities);
		checkAndHandleCollisionWithTarget();
		break;
	default:
		printf("another event occured \n");
		break;
	}



	return 0;
}


/******************************************************************/
/*
Purpose: renders one frame

Descripton: 

Return:
1 - if failed
0 - if successful

*/



void gameApp::renderFrame(void)
{
	static int count = 0;
	static int dir = 1;
	unsigned int i;

	count++;
	glClearColor(0, 0, 0, 0.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //Clear the colour and depth buffers

	for (i = 0; i < gameDynamicEntities.size(); i++) {
		gameObject *g = gameDynamicEntities.at(i);
		gameDynamicEntities.at(i)->render(NULL, gameApp::cam);
	}
	for (i = 0; i < gameStaticEntities.size(); i++) {
		gameStaticEntities.at(i)->render(NULL, gameApp::cam);
	}


	// swap the buffers
	glutSwapBuffers();

	

}


/******************************************************************/
/*
Purpose: initialize the game

Descripton: 

Return:
1 - if failed
0 - if successful

*/

int gameApp::initGame(void)
{
	// create game objects
	//Car *truck;
	int rc;
	House *house1, *house2;		// may want to set them as static for collision

	house1 = new House();
	house2 = new House();
	preyRedKia = new Prey();// define the user car's logic in class Prey
	chaserYellowKia = new Chaser(preyRedKia);//define the NPC's logic in class Chaser
	th = new TargetHouse();
	truck = new Truck(preyRedKia);

	Shader *tmpShader = NULL;
	GLuint shaderId;
	drawSurface = new meshSurface();
	tmpShader = new Shader();		
	
	/* genewrating one shader program only - consider changing it to a class shader DN*/
	if (tmpShader == NULL) {
		printf("error in creating a shader obeject \n");
		assert(0);
	}
	rc = tmpShader->createShaderProgram("general.vert", "general.frag", &shaderId);
	if (rc != 0) {
		printf("error in generating shader vs=%s, fs=%s \n", "general.vert", "general.frag");
		delete tmpShader;
		tmpShader = NULL;
		assert(0);
	}

	chaserYellowKia->setShader(tmpShader);
	th->setShader(tmpShader);
	truck->setShader(tmpShader); // load shader
	preyRedKia->setShader(tmpShader);
	house1->setShader(tmpShader);
	house2->setShader(tmpShader);
	drawSurface->setShader(tmpShader);
	
	// load the gemoetry
	preyRedKia->loadModelOBJ("kia\\kia_rio.obj",&preyRedKia->mVtxBuf, &preyRedKia->mNumVtx,&preyRedKia->mIndBuf, &preyRedKia->mNumInd);
	// load the textures
	preyRedKia->loadTexture("kia\\rio_red.bmp");
	// set the initial position attributes - align the object wth the z-axis if needed

	preyRedKia->setScale((float) 0.2, (float) 0.2, (float) 0.2);
	preyRedKia->setPositionOrientation(Vector3f(5, 0, 5), Vector3f(1, 0, 1), Vector3f(0, 1, 0));
	preyRedKia->mYaw=90;		// orient it so that it faces that +zaxis

	

	//shadowPrey related(net work--deadReckoning model)

	// load the gemoetry
	truck->loadModelOBJ("truck\\L200_OBJ_DOS.obj", &truck->mVtxBuf, &truck->mNumVtx, &truck->mIndBuf, &truck->mNumInd);
	// load the textures
	truck->loadTexture("truck\\truck_color_clean_256.jpg");
	// set the initial position attributes - align the object wth the z-axis if needed
	truck->setScale((float) 0.05, (float)0.05, (float)0.05);
	truck->mYaw = 0;		// orient it so that it faces that +zaxis
	
	// load the gemoetry model
	chaserYellowKia->loadModelOBJ("kia\\kia_rio.obj",&chaserYellowKia->mVtxBuf, &chaserYellowKia->mNumVtx,&chaserYellowKia->mIndBuf, &chaserYellowKia->mNumInd);
	// load the textures
	chaserYellowKia->loadTexture("kia\\rio_yellow.bmp");
	// set the initial position attributes - align the object wth the z-axis if needed
	chaserYellowKia->setScale((float) 0.2, (float) 0.2, (float) 0.2);
	chaserYellowKia->mYaw=90;		// orient it so that it faces that +zaxis
	chaserYellowKia->setPositionOrientation(Vector3f(-15,0,25), Vector3f(0,0,1),Vector3f(0,1,0));



	// set the house object
	

	house1->loadModelOBJ("house_obj\\house_obj.obj",&house1->mVtxBuf, &house1->mNumVtx,&house1->mIndBuf, &house1->mNumInd);
	// load the textures
	house1->loadTexture("house_obj\\house_diffuse_256.tga");
	// set attributes (scale, position, and initial orientation towards the (0,0,1)
	house1->setScale(0.006,0.006,0.006);
	//this is the obstacle's location
	house1->setPositionOrientation(Vector3f(5,0,1.2), Vector3f(1,0,1),Vector3f(0,1,0));


	house2->loadModelOBJ("house_obj\\house_obj.obj",&house2->mVtxBuf, &house2->mNumVtx,&house2->mIndBuf, &house2->mNumInd);
	// load the textures
	house2->loadTexture("house_obj\\house_diffuse_256.tga");
	// set attributes (scale, position, and initial orientation towards the (0,0,1)
	house2->setScale(0.006,0.006,0.006);
	house2->setPositionOrientation(Vector3f(15,0,20.2), Vector3f(-1,0,1),Vector3f(0,1,0));
	

	//Here I will try to generate 20 houses at random places on the surface
	/*
	House* houses[NUMBER_OF_HOUSES];
	for (int i = 0; i < NUMBER_OF_HOUSES; i++){
		houses[i] = new House();
		houses[i]->createShaderProg("general.vert", "general.frag");
		houses[i]->loadModelOBJ("house_obj\\house_obj.obj", &houses[i]->mVtxBuf, &houses[i]->mNumVtx, &houses[i]->mIndBuf, &houses[i]->mNumInd);
		// load the textures
		houses[i]->loadTexture("house_obj\\house_diffuse_256.tga");
		// set attributes (scale, position, and initial orientation towards the (0,0,1)
		houses[i]->setScale(0.006, 0.006, 0.006);
		houses[i]->setPositionOrientation(Vector3f((rand() % SURFACE_ROW)*2-SURFACE_ROW, 0, (rand() % SURFACE_COL)*2-SURFACE_COL), Vector3f(-1, 0, 1), Vector3f(0, 1, 0));
	
		//push back dynamic entities
		gameStaticEntities.push_back(houses[i]);
	}
	*/
	/*
		Chaser* chaser[NUMBER_OF_CHASERS];
	for (int i = 0; i < NUMBER_OF_CHASERS; i++){
		chaser[i] = new Chaser(preyRedKia);
		// set the chaser object
		chaser[i]->createShaderProg("general.vert", "general.frag");
		// load the gemoetry model
		chaser[i]->loadModelOBJ("kia\\kia_rio.obj", &chaser[i]->mVtxBuf, &chaser[i]->mNumVtx, &chaser[i]->mIndBuf, &chaser[i]->mNumInd);
		// load the textures
		chaser[i]->loadTexture("kia\\rio_yellow.bmp");
		// set the initial position attributes - align the object wth the z-axis if needed
		chaser[i]->setScale((float) 0.2, (float) 0.2, (float) 0.2);
		chaser[i]->mYaw = 90;		// orient it so that it faces that +zaxis
		chaser[i]->setPositionOrientation(Vector3f((rand() % SURFACE_ROW) * 4 - SURFACE_ROW, 0, (rand() % SURFACE_COL) * 4 - SURFACE_COL), Vector3f(-1, 0, 1), Vector3f(0, 1, 0));

		//push back dynamic entities
		gameDynamicEntities.push_back(chaser[i]);

	}
	*/



	//load the target house

	th->loadModelOBJ("house_obj\\house_obj.obj", &th->mVtxBuf, &th->mNumVtx, &th->mIndBuf, &th->mNumInd);
	// load the textures
	//th->loadTexture("house_obj\\house_diffuse_256.tga");
	// set attributes (scale, position, and initial orientation towards the (0,0,1)
	th->setScale(0.006, 0.006, 0.006);
	//this is the obstacle's location
	th->setPositionOrientation(Vector3f(0, 0, 15), Vector3f(1, 0, 1), Vector3f(0, 1, 0));


	// set the surface

	//here we can modify the surface's size: rows, cols, width, height
	drawSurface->createSurface(SURFACE_ROW, SURFACE_COL, SURFACE_WIDTH, SURFACE_HEIGHT);
	drawSurface->loadTexture("surface\\grass_texture_256.tga");

	// add the objects to the list of game objects
	gameDynamicEntities.push_back(chaserYellowKia);
	gameDynamicEntities.push_back(preyRedKia);
	
	gameDynamicEntities.push_back(truck);

	gameStaticEntities.push_back(drawSurface);
	gameStaticEntities.push_back(house1);
	gameStaticEntities.push_back(house2);
	gameStaticEntities.push_back(th);


	// set the global camera
	cam = new camera();
	cam->setCamera(Vector3f(0, 30, 60), Vector3f(0, -1, 0), Vector3f(0, 1, 0));
	cam->setPerspectiveView(45, 1, (float) 0.2, 1000);

	//initialize the oldCam
	preyFP = new camera();
	*preyFP = *(preyRedKia->getFirstPersonPers());
	
	preyTP = new camera();
	*preyTP = *(preyRedKia->getThirdPersonPers());

	overPers = new camera();
	*overPers = *cam;
	
	
	return 0;
}



/****************************************************************************/



void gameApp::keyboardFun(unsigned char key, int x, int y) 
{
	switch (key) {
		case 033:
		case 'Q':
		case 'q':
			exit(1);
			break;
		case 'w': 
			overPers->moveForward(NORMAL_SPEED); 
			*cam = *overPers;
			break;
		case 'W': 
			overPers->moveForward(MAX_SPEED); 
			*cam = *overPers;
			break;
		case 's':
			overPers->moveForward(-NORMAL_SPEED);
			*cam = *overPers;
			break;
		case 'S':
			overPers->moveForward(-MAX_SPEED);
			*cam = *overPers;
			break;
		case 'a': 
			overPers->yaw((float) .2);
			*cam = *overPers;
			break;
		case 'A': 
			overPers->yaw(1);
			*cam = *overPers;
			break;
		case 'D':
			overPers->yaw(-1);
			*cam = *overPers;
			break;
		case 'd': 
			overPers->yaw((float)-.2);
			*cam = *overPers;
			break;
		case 'z': 
			overPers->zoomIn();
			*cam = *overPers;
			break;
		case 'Z': 
			overPers->zoomOut();
			*cam = *overPers;
			break;
		case 'r': 
//			angle += 1.0;
			break;
		case 'R': 
//			angle -= 1.0;
			break;
		case 't': 
//			refractFlag = (refractFlag == 0) ? 1 : 0;
			break;

		//the following are the cases that I define
		
		case 'c':
			//first person perspective
			//chage the perspective of the prey, which should be change it back and forth. So the logic should be defined in class Prey
			changePerspective(preyRedKia,key);
			break;
		case 'v':
			changePerspective(preyRedKia,key);
			break;
		case 'b':
			//overview perspective
			//toggle to overview camera, which is already provided. 
			changePerspective(preyRedKia, key);
			break;
			
		//i,k,j,l
		case 'i':
			preyRedKia->moveForward(CAR_SPEED);

					
			break;
		//case 'k':



		case 'j':
			//left
				//preyRedKia->yaw(10);
			preyRedKia->yaw(10);


			break;
		case 'l':

			preyRedKia->yaw(-10);


			
			break;
	}
}

void gameApp::specialKeyboardFun(int key, int x, int y) 
{
	switch (key) {
		case 033:
		case 'Q':
		case 'q':
			exit(1);
			break;
		case GLUT_KEY_LEFT:
			overPers->roll((float) .2);
			*cam = *overPers;
			break;
		case GLUT_KEY_UP:
			overPers->pitch((float) .2);
			*cam = *overPers;
			break;
		case GLUT_KEY_RIGHT:
			overPers->roll((float) -.2);
			*cam = *overPers;
			break;
		case GLUT_KEY_DOWN:
			overPers->pitch((float) -.2);
			*cam = *overPers;
			break;
	}
}


/************************************************************************************************/
/*
This functions in invoked when ther
e is a change to the window by the user
 Here a new setting of the rendering paramters are set. In most cases it consists of two things:
a. changeing the view port dimensions
b. changing the aspect ratio

input 
w, h - the width and height of the new window in the pixel coordinates

*/

void gameApp::reshapeFun(int w, int h)
{

	static GLfloat aspect;			// aspect ratio of height and width

	// change the view port
	glViewport (0 , 0, (GLsizei) w, (GLsizei) h);

	// Compute aspect ratio of the new window
	aspect = (float)w / (float)h;


	// Set the state to accept projection information
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             
	// Enable perspective projection with fovy, aspect, zNear and zFar
	gluPerspective(45.0f, aspect, 0.1f, 100.0f);
	cam->setPerspectiveView(cam->fieldOfView,aspect,cam->nearPlane,cam->farPlane);

}
 


/***************************************************************************************************/

void gameApp::timerFun(int timerEvent)
{
	static int frameNumber = 0;
	frameNumber++;


//	renderSceneFun();
	glutPostRedisplay();
	// get next event from Queue  (future implementation) 
	// the queue should have return an event and the event timer

	myApp->updateGameState((long) GAME_UPDATE_FRAME_STATE_EVENT);
	myApp->updateGameState((long) GAME_COLLISION_EVENT);
	
	//constantly set the first person camera
	*preyFP = *(preyRedKia->getFirstPersonPers());
	//constantly set the third preson camera
	*preyTP = *(preyRedKia->getThirdPersonPers());
	
	glutTimerFunc(5, timerFun, frameNumber);
}


//here I will define a function changePerspective to switch perspective between general and firstPerson
//I need to elaborate this function so that the camera can move with the car, and see what it is supposed to see
void gameApp::changePerspective(const Prey *p, unsigned char key){
	switch (key){
		//c prey's perspective
	case'c':
		//first person perspective
		cam = preyFP;
		break;
		//v overview camera
	case 'v':
		//third person perspective
		cam = preyTP;
		break;
	case 'b':
		//overview perspective
		cam = overPers;
		break;
		
	}
	
	return;
}

/*
	Utility: instance of
	NewType* v=dynamic_cast<NewType*>(old);
	if(v!=0){
		v->doSomething
	}
*/

//check collision: prey and chaser
void gameApp::handleCollisionCarWithCars( Car* c1,  std::vector<gameObject*> otherCars){//we should pass gameDynamicEntities to otherCars
	//check the validity of c1 and otherCars
	if (c1 == NULL || otherCars.size()==0 ){
		return ;
	}
	
	//there are two situations:
	
	//1.prey collide with chaser
	Prey* p = dynamic_cast<Prey*>(c1);
	if (p != 0){
		//p is now representing the prey
		for (int i = 0; i < otherCars.size(); i++){
			//otherCars[i] do something
			Chaser* cs = dynamic_cast<Chaser*>(otherCars[i]);
			
			 checkAndHandleCollisionBetween(p, cs);

		}
	}

	//2.chaser collide with chaser
	Chaser* cs1 = dynamic_cast<Chaser*>(c1);
	if (cs1 != 0){
		//cs1 is now representing chaser1
		for (int i = 0; i < otherCars.size(); i++){
			//otherCars[i] do something
			Chaser* cs2 = dynamic_cast<Chaser*>(otherCars[i]);
			
			 checkAndHandleCollisionBetween(cs1, cs2);
		}
	}
	return;
}

//check collision: prey/chaser and house
void gameApp::handleCoolisionCarWithHouses(Car* car, std::vector<gameObject*> staticEntities){
	
	//check the validity of car and staticEntities
	if (car == NULL || staticEntities.size()==0){
		//std::cout << staticEntities.size() << std::endl;
		return ;
	}

	//there are two situations:

	//1.prey collide with house
	Prey* p = dynamic_cast<Prey*>(car);
	if (p != 0){
		//p is now representing the prey
		for (int i = 0; i < staticEntities.size(); i++){
			
			;
			//std::cout << "check house# " << i << std::endl;
			
			//staticEntities[i] do something
			House* h = dynamic_cast<House*>(staticEntities[i]);
			
			
			 checkAndHandleCollisionBetween(p, h);

		}
	}

	//2.chaser collide with houses
	Chaser* cs = dynamic_cast<Chaser*>(car);
	
	if (cs != 0){
		//cs is now representing the chaser
		for (int i = 0; i < staticEntities.size(); i++){

			//staticEntities[i] do something
			House* h = dynamic_cast<House*>(staticEntities[i]);
			 checkAndHandleCollisionBetween(cs, h);
			 //std:: cout << "chaser<-->house checked" << std::endl;
		}
	}
	return ;
}

//check collision between two car/house objects
bool gameApp::checkAndHandleCollisionBetween(Car* c1, Car* c2){
	float distance;//r, the space distance between the two objects
	float c1R;//boundary of the c1 
	float c2R;//boundary of the c2
	float limit;// minimum distance for the two objects not collide
	//we only check the horizental direction, so the distance represent the distance 
	//between two objects in x and z dimension
	if (c1 == nullptr || c2 == nullptr){
		return false;
	}
	distance = sqrt(pow((c1->mPosition.x - c2->mPosition.x), 2) + pow((c1->mPosition.z - c2->mPosition.z), 2));

	//I need to find out the bounding radius of the car
	c1R = c1->boundaryRadius*fminf(c1->mScaleX,c1->mScaleZ);
	c2R = c2->boundaryRadius*fminf(c2->mScaleX,c2->mScaleZ);
	limit = c1R + c2R;

	if (distance <= limit){ //collide
		//temporary handles the collision
		//std::cout << "collision happens: " << std::endl;
		//c1->moveForward(-1);
		c1->collisionReaction(c2);
		return true;
	}
	else                   //not collide
		return false;
}


void gameApp::checkAndHandleCollisionWithBoundaries(std::vector<gameObject*> dynamicEntities){
	float boundaryMargin = 4;
	if (dynamicEntities.size() == 0){
		return;
	}
	
	//p is now representing the prey
	for (int i = 0; i < dynamicEntities.size(); i++){
		Car* c = dynamic_cast<Car*>(dynamicEntities[i]);
		if (c != 0){
			//check whether it collide with boundaries
			if (c->mPosition.x >(SURFACE_WIDTH / 2 - boundaryMargin)){
				c->yaw(5);
			}
			if (c->mPosition.x <(-SURFACE_WIDTH / 2 + boundaryMargin)){
				c->yaw(5);
			}
			if (c->mPosition.z > (SURFACE_HEIGHT / 2 - boundaryMargin)){
				c->yaw(5);
			}
			if (c->mPosition.z < (-SURFACE_HEIGHT / 2 + boundaryMargin)){
				c->yaw(5);
			}


			//std::cout << "boundary checked" << std::endl;
			
		}
	}
	
	return;
}

void gameApp::checkAndHandleCollisionWithTarget(){
	checkAndHandleCollisionBetween(preyRedKia, th);
}