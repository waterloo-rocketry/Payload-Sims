#include "BTDetectorConstruction.hh"
#include "BTActionInitialization.hh"
#include "G4RunManager.hh"

#include "G4UImanager.hh"
#include "QBBC.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

int main(int argc, char** argv)
{
	// detect interactive mode and define UI session
	G4UIExecutive* ui = nullptr;
	if (argc == 1) {
		ui = new G4UIExecutive(argc, argv);
	}

	// default run manager
	G4RunManager* runManager = new G4RunManager;


	// set user classes
	// detector construction
	runManager->SetUserInitialization(new BTDetectorConstruction());

	// physics list
	G4VModularPhysicsList* physicsList = new QBBC;
	physicsList->SetVerboseLevel(1);
	runManager->SetUserInitialization(physicsList);

	// user action initialization
	runManager->SetUserInitialization(new BTActionInitialization());

	// initialize visualization
	G4VisManager* visManager = new G4VisExecutive;
	visManager->Initialize();

	// Get the pointer to the User Interface manager
	G4UImanager* UImanager = G4UImanager::GetUIpointer();

	// Process macro or start UI session
	if (!ui) {
        // batch mode
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		UImanager->ApplyCommand(command + fileName);
	}
	else {
		// interactive mode
		UImanager->ApplyCommand("/control/execute init_vis.mac");
		ui->SessionStart();
		delete ui;
	}

	delete visManager;
	delete runManager;
}
