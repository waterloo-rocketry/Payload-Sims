// \file BTEventAction.hh
// \brief Definition of the BTEventAction class

#pragma once

#include "G4UserEventAction.hh"
#include "globals.hh"

class BTRunAction;

// Event action class
//

class BTEventAction : public G4UserEventAction
{
  public:
    BTEventAction(BTRunAction* runAction);
    virtual ~BTEventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    void AddEdep(G4double edep) { fEdep += edep; }

  private:
    BTRunAction* fRunAction;
    G4double     fEdep;
};

    
