// \file BTEventAction.cc
// \brief Implementation of the BTEventAction class

#include "BTEventAction.hh"
#include "BTRunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

BTEventAction::BTEventAction(BTRunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  fEdep(0.)
{} 

BTEventAction::~BTEventAction()
{}

void BTEventAction::BeginOfEventAction(const G4Event*)
{    
  fEdep = 0.;
}

void BTEventAction::EndOfEventAction(const G4Event*)
{   
  // accumulate statistics in run action
  fRunAction->AddEdep(fEdep);
}
