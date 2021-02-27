#pragma once

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class BTEventAction;

class G4LogicalVolume;

class BTSteppingAction : public G4UserSteppingAction
{
public:
	BTSteppingAction(BTEventAction* eventaction);
	virtual ~BTSteppingAction();

	virtual void UserSteppingAction(const G4Step*);

private:
	BTEventAction* fEventAction;
	G4LogicalVolume* fScoringVolume;
};
