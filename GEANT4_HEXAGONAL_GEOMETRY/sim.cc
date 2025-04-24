#include <iostream>

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "QGSP_BERT.hh"

#include "construction.hh"
#include "physics.hh"
#include "action.hh"

int main(int argc, char** argv)
{
	// Initialize the run manager
	G4RunManager *runManager = new G4RunManager;
	runManager->SetUserInitialization(new Construction());
	runManager->SetUserInitialization(new MyPhysicsList());
	runManager->SetUserInitialization(new MyActionInitialization());
	runManager->Initialize();

	// Initialize the UI executive for interactive mode
	G4UIExecutive *ui = new G4UIExecutive(argc, argv);

	// Initialize the visualization manager
	G4VisManager *visManager = new G4VisExecutive();
	visManager->Initialize();

	// Get the UI manager to execute commands
	G4UImanager *UImanager = G4UImanager::GetUIpointer();
	UImanager->ApplyCommand("/vis/open OGL");  // Open OpenGL visualization
	UImanager->ApplyCommand("/vis/drawVolume");  // Draw all volumes
	UImanager->ApplyCommand("/vis/viewer/set/autoRefresh true");  // Enable auto-refresh for viewer
	UImanager->ApplyCommand("/vis/scene/add/trajectories smooth");  // Add smooth trajectories
	UImanager->ApplyCommand("/vis/scene/add/axes 0 0 0 2.5 m");  // Add coordinate axes at the origin with 5 meter length

	// Start the UI session
	ui->SessionStart();

	// Cleanup
	delete visManager;
	delete runManager;

	return 0;
}
