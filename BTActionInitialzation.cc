#include "BTActionInitialization.hh"
#include "BTPrimaryGeneratorAction.hh"
#include "BTRunAction.hh"
#include "BTEventAction.hh"
#include "BTSteppingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BTActionInitialization::BTActionInitialization()
 : G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BTActionInitialization::~BTActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BTActionInitialization::BuildForMaster() const
{
  BTRunAction* runAction = new BTRunAction;
  SetUserAction(runAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BTActionInitialization::Build() const
{
  SetUserAction(new BTPrimaryGeneratorAction);

  BTRunAction* runAction = new BTRunAction;
  SetUserAction(runAction);
  
  BTEventAction* eventAction = new BTEventAction(runAction);
  SetUserAction(eventAction);
  
  SetUserAction(new BTSteppingAction(eventAction));
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
