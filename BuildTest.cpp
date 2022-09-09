
#include <vector>
#include <string>

#include "BTDetectorConstruction.hh"
#include "BTActionInitialization.hh"
#include "G4RunManager.hh"

#include "G4UImanager.hh"
#include "QBBC.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

const std::string DEFAULT_MACRO = "init_vis.mac";

int main(int argc, char** argv)
{
	// arg parsing
    int opt;
    bool nouiFlag = false;
    bool helpFlag = false;
    std::string macroFiles = "";

    while((opt = getopt(argc, argv, "uh")) != -1) {
        switch (opt) {
            case 'u':
                nouiFlag = true;
                break;
            case 'h':
                helpFlag = true;
                break;
            default:
                helpFlag = true;
        }
    }

    if (helpFlag) {
        std::cout << "BuildTest\n";
        return 0;
    }

    for (unsigned int i = optind; i < argc; ++i) {
        macroFiles = macroFiles + " " + std::string{argv[i]};
    }

	G4UIExecutive* ui = nullptr;
	if (!nouiFlag) {
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

    // run macros
    if (macroFiles.size() > 0) {
        auto command = "/control/execute" + macroFiles;
        std::cout << command << "\n";
        UImanager->ApplyCommand(command);
    }

    // start ui session if in ui mode
    if (!nouiFlag) {
		ui->SessionStart();
		delete ui;
    }

	delete visManager;
	delete runManager;
}
