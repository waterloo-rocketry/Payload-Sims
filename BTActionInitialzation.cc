#include "BTActionInitialization.hh"
#include "BTPrimaryGeneratorAction.hh"
#include "BTRunAction.hh"
#include "BTEventAction.hh"
#include "BTSteppingAction.hh"

BTActionInitialization::BTActionInitialization()
 : G4VUserActionInitialization()
{}

BTActionInitialization::~BTActionInitialization()
{}

void BTActionInitialization::BuildForMaster() const
{
  BTRunAction* runAction = new BTRunAction;
  SetUserAction(runAction);
}

void BTActionInitialization::Build() const
{
  SetUserAction(new BTPrimaryGeneratorAction);

  BTRunAction* runAction = new BTRunAction;
  SetUserAction(runAction);
  
  BTEventAction* eventAction = new BTEventAction(runAction);
  SetUserAction(eventAction);
  
  SetUserAction(new BTSteppingAction(eventAction));
}
