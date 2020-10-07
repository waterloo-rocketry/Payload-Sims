/// \file BTDetectorConstruction.cpp
/// \brief Implementation of the BTDetectorConstruction class

#include "BTDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

BTDetectorConstruction::BTDetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(nullptr)
{}

BTDetectorConstruction::~BTDetectorConstruction()
{}

G4VPhysicalVolume* BTDetectorConstruction::Construct()
{
	// Get nist material manager
	G4NistManager* nist = G4NistManager::Instance();

	// World Parameters
	const G4double world_sizeX = 20 * cm;
	const G4double world_sizeY = 20 * cm;
	const G4double world_sizeZ = 30 * cm;
	G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

    G4Box* solidWorld =
        new G4Box("World",       // name
            world_sizeX, world_sizeX, world_sizeZ);     // size

    G4LogicalVolume* logicWorld =
        new G4LogicalVolume(solidWorld,  // the world is solid
            world_mat,                   // material
            "World");                    // name

    G4VPhysicalVolume* physWorld =
        new G4PVPlacement(0,       //no rotation
            G4ThreeVector(),       //at origin
            logicWorld,            //logical volume
            "World",               //name
            0,                     //mother  volume
            false,                 //no boolean operation
            0,                     //copy number
            checkOverlaps);        //check for any overlaps

	// Water Box Parameters
	G4double wb_sizeXY = 10 * cm, wb_sizeZ = 20 * cm;
	G4Material* wb_mat = nist->FindBuildMaterial("G4_WATER");

    G4Box* solidwb =
        new G4Box("Water Box",   //name
            wb_sizeXY, wb_sizeXY, wb_sizeZ); //size

    G4LogicalVolume* logicwb =
        new G4LogicalVolume(solidwb,       //box is solid
            wb_mat,                        //material
            "Water Box");                  //name

    new G4PVPlacement(0,         //no rotation
        G4ThreeVector(),         //at origin
        logicwb,                 //logical volume
        "Water Box",             //name
        logicWorld,              //mother volume
        false,                   //no boolean operation
        0,                       //copy number
        checkOverlaps);          //check for any overlaps

    fScoringVolume = logicwb;

    return physWorld;
}
