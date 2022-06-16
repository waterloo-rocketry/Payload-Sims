#pragma once

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"

class G4Run;

///Run action class
/// calculates dose to the water box and displays it on the screen

class BTRunAction : public G4UserRunAction
{
public:
	BTRunAction();
	virtual ~BTRunAction();

	virtual void BeginOfRunAction(const G4Run*);
	virtual void EndOfRunAction(const G4Run*);

	void AddEdep(G4double edep);

private:
	G4Accumulable<G4double> fEdep;
	G4Accumulable<G4double> fEdep2;
    G4int   fpCount;
};
