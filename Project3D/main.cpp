#include "Application3D.h"

/// <summary>
/// The main() entry simply instantiates a new Application3D object and calls run on it, which initialises
/// an application window of size 1280x720, and continually triggers the application's update loop each frame
/// the user has triggered the exit by pressing escape
/// </summary>
/// <returns></returns>
int main() {
	
	// Instantiate an Application3D object
	auto app = new Application3D();

	// Initialise and loop until close
	app->run("Graphics Engine Demonstration - Ronan Richardson", 1280, 720, false);

	// Deallocation of application memory
	delete app;

	return 0;
}