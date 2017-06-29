#include "Vision_ControlFlow.h"
// Declare Enumerators for Control States
//enum steerMode {NOMINAL, HOMING};	// steering mode: NOMINAL uses road boundries to steer, HOMING uses stop marker as target
//enum TBMode {NOMINAL, STOP};		// throttle/brake mode: NOMINAL if stop marker is not detected the system continues at same speed, STOP stop marker is within distance threshold vehicle stops

struct steerMode
{
	int mode;
};

struct TBMode
{
	int mode;
};

struct cntrlSig
{
	float th;
	float br;
	float st;
};

//placeholder for ROS publisher function
void ROS_Pub(float throttle, float braking, float steerAng)
{
	std::cout << "Throttle: " << throttle << " Braking: " << braking << " SteeringAngle: " << steerAng << std::endl;
}

// control logic for vision system
void vision_Control(double markerDist)
{
	const cntrlSig stopcmd = { 0,1,0 };		// stop config
	const cntrlSig runcmd = { 0.1,0,0 };	// run config
	cntrlSig outcmd = runcmd;				// initialize
	steerMode st = { 0 };			// start in nominal
	TBMode th_br = { 0 };			// start in nominal

	bool mkr_notvis = false;

	// Check input and set visible flag
	if (markerDist <= 0 || markerDist >= 40)
		mkr_notvis = true; // ensure if reading is below zero or unusally high it will catch the error

	if (!mkr_notvis) // if marker is visible run this
	{
		// decide what command to send based on distance from marker
		if (markerDist <= 15)
		{
			// Stop the Vehicle
			outcmd = stopcmd;
			th_br.mode = 1;
		}
		else
		{
			// Move Forward
			outcmd = runcmd;
			th_br.mode = 0;
		}
	}
	else // if it is not visible keep moving? or stop it?
	{
		// Move Forward
		outcmd = runcmd;
		th_br.mode = 0;
	}

	ROS_Pub(outcmd.th, outcmd.br, outcmd.st); // sim publish to ROS
}

// code simulated distance input
void visionSim_ROSDemo()
{
	// this function simulates outputs to use for the ROS test
	const cntrlSig stopCond = { 0,1,0 };	// stop config
	const cntrlSig runCond = { 0.1,0,0 };	// run config
	const cntrlSig lstr = { 0.1,0,-0.5 };	// basic turn left
	const cntrlSig rstr = { 0.1,0,0.5 };	// basic turn right
	cntrlSig vcmd = stopCond;	// initialize output command with stopCond for safety

	double dist = 30; // to help visualize situation this represents distance from stop marker

		std::cout << "---TEST STARTING---" << std::endl << std::endl;
		// intial stop
		vcmd = stopCond;
		ROS_Pub(vcmd.th, vcmd.br, vcmd.st);
		delay(3000);

		for (int i = 0; i < 10; i++)
		{
			ROS_Pub(vcmd.th, vcmd.br, vcmd.st);
			delay(1000);
		}

		// drive straight for some time
		vcmd = runCond;
		for (int i = 0; i < 25; i++)
		{
			ROS_Pub(vcmd.th, vcmd.br, vcmd.st);
			delay(1000);
		}

		// zig zag a bit
		vcmd = lstr;
		for (int i = 0; i < 5; i++)
		{
			ROS_Pub(vcmd.th, vcmd.br, vcmd.st);
			delay(1000);
		}
		vcmd = rstr;
		for (int i = 0; i < 5; i++)
		{
			ROS_Pub(vcmd.th, vcmd.br, vcmd.st);
			delay(1000);
		}
		
		// run straight again
		vcmd = runCond;
		for (int i = 0; i < 10; i++)
		{
			ROS_Pub(vcmd.th, vcmd.br, vcmd.st);
			delay(1000);
		}

		// approaching marker
		std::cout << "Stop Marker Distance: " << dist << std::endl;
		for (int i = 0; i < 10; i++)
		{
			ROS_Pub(vcmd.th, vcmd.br, vcmd.st);
			dist = dist - 2;
			std::cout << "Stop Marker Distance: " << dist << std::endl;
			delay(1000);
		}
		
		// marker is detected close stop the vehicle
		vcmd = stopCond;
		std::cout << "STOP VEHICLE: CLOSE TO MARKER " << dist << std::endl;
		ROS_Pub(vcmd.th, vcmd.br, vcmd.st);
		for (int i = 0; i < 5; i++)
		{
			ROS_Pub(vcmd.th, vcmd.br, vcmd.st);
			delay(1000);
		}
		std::cout << std::endl << std::endl << "---TEST ENDED---" << std::endl << std::endl;
		std::cout << "SET BREAKPOINT HERE";
}

void visionSim_ControlSimulator()
{
	double dist = 50;
	// initial distance is 50
	std::cout << "Distance: " << dist << std::endl;
	vision_Control(dist);

	//marker is visible and far enough away
	dist = dist - 20;
	std::cout << "Distance: " << dist << std::endl;
	vision_Control(dist);

	for (int i = 0; i < 35; i++)
	{
		dist = dist--;
		std::cout << "Distance: " << dist << std::endl;
		vision_Control(dist);
	}

	////marker is visible and close
	//dist = dist - 20;
	//std::cout << "Distance: " << dist << std::endl;
	//vision_Control(dist);

	//// distance is less than 0
	//std::cout << "Distance: " << dist << std::endl;
	//vision_Control(dist);
}

//void strNOMINAL(float strAng)
//{
//	cntrlSig outcmd = { 0,0,strAng };
//	ROS_Pub(outcmd.th, outcmd.br, outcmd.st); // sim publish to ROS
//}

//void strHOMING()
//{
//	// NOT READY
// ROS_Pub(outcmd.th, outcmd.br, outcmd.st); // sim publish to ROS
//}

//void TBNOMINAL()
//{
//	cntrlSig outcmd = {0.1,0,0};
//	// ROS_Pub(outcmd.th, outcmd.br, outcmd.st); // sim publish to ROS
//}
//
//void TBSTOP()
//{
//	cntrlSig outcmd = {0,1,0};
//	ROS_Pub(outcmd.th, outcmd.br, outcmd.st); // sim publish to ROS
//}

//cntrlSig TBNOMINAL()
//{
//	cntrlSig outcmd = {0.1,0,0};
//	return outcmd;
//}
//
//cntrlSig TBSTOP()
//{
//	cntrlSig outcmd = {0,1,0};
//	return outcmd;
//}