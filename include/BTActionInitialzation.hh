#pragma once

#include "G4VUserActionInitialization.hh"

/// Action initialization class.

class BTActionInitialization : public G4VUserActionInitialization
{
  public:
    BTActionInitialization();
    virtual ~BTActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
