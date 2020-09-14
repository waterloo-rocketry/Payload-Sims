/// \file BTDetectorConstruction.hh
/// \brief Definition of the BTDetectorConstruction class

#pragma once

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector Construction class to define materials and geometries of the build test

class BTDetectorConstruction : public G4VUserDetectorConstruction
{
public:
	BTDetectorConstruction();
	virtual ~BTDetectorConstruction();

	virtual G4VPhysicalVolume* Construct();

	G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }
protected:
	G4LogicalVolume* fScoringVolume;

};

#endif
