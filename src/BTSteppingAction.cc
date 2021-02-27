#include "BTSteppingAction.hh"
#include "BTEventAction.hh"
#include "BTDetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

BTSteppingAction::BTSteppingAction(BTEventAction* eventAction)
	: G4UserSteppingAction(),
	fEventAction(eventAction),
	fScoringVolume(nullptr)
{}

BTSteppingAction::~BTSteppingAction()
{}

void BTSteppingAction::UserSteppingAction(const G4Step* step)
{
	if (!fScoringVolume) {
		auto detectorConstruction
			= static_cast<const BTDetectorConstruction*>
			(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
		fScoringVolume = detectorConstruction->GetScoringVolume();
	}

	///get volume of current step
	G4LogicalVolume* volume
		= step->GetPreStepPoint()->GetTouchableHandle()
		->GetVolume()->GetLogicalVolume();

	///check if in scoring volume
	if (volume != fScoringVolume) return;

	///collect energy deposited in this step
	G4double edepStep = step->GetTotalEnergyDeposit();
	fEventAction->AddEdep(edepStep);
}
