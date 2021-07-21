/// \file BTDetectorConstruction.cpp
/// \brief Implementation of the BTDetectorConstruction class


//Next Step: Add an aluminium cover panel on top of the scintillator with the thickness of 2.38 mm and 

// Presumably something like this to define the material if needed.

// #include "G4Material.hh"
// #include "G4MaterialTable.hh"
// #include "G4Element.hh"
// #include "G4ElementTable.hh"
// ......
// G4Material* Al = 
// new G4Material("Aluminium", z=13., a= 26.98*g/mole, density= 2.7*g/cm3);

//Alternatively "G4_Al" can be used as a pre-defined material.

// Geometry-wise G4Box/G4PolyHedra should be enough to build the shape with the addition of 
// G4UnionSolid, G4SubtractionSolid, G4IntersectionSolid.
// http://geant4.in2p3.fr/IMG/pdf_Lecture-Geometry.pdf

#include "BTDetectorConstruction.hh"

#include "G4Material.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SubtractionSolid.hh"

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

    G4bool checkOverlaps = true;

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

	// Scintillator Parameters
	G4double sc_sizeXY = 2 * cm, sc_sizeZ = 1 * cm; //20x20x10 mm
	G4Material* sc_mat = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE"); //Scintillator Material
    
    G4Material* abs = new G4Material(
        "ABS Plastic",
        1.0 * g/cm3,
        3,
        kStateSolid, // Conditions in space
        225. * kelvin,
        0.01 * atmosphere
    );

	G4Element* C = nist->FindOrBuildElement("G4_C"); 
    abs->AddElement(C, 15);
	G4Element* H = nist->FindOrBuildElement("G4_H"); 
    abs->AddElement(H, 17);
	G4Element* N = nist->FindOrBuildElement("G4_N"); 
    abs->AddElement(N, 1);

    G4Box *outerHolder = new G4Box("Holder - Outer Rim", 36*mm, 36*mm, 10*mm);
    G4Box *innerHolder = new G4Box("Holder - Inner Rim", 25*mm, 25*mm, 10*mm);
    G4SubtractionSolid *Holder = new G4SubtractionSolid("Holder", outerHolder, innerHolder);
    G4LogicalVolume* logicHolder =
        new G4LogicalVolume(Holder,                 //scint is solid
            abs,                                    //material
            "Scintillator Holder");                 //name

    G4VPhysicalVolume* physHolder =
        new G4PVPlacement(0,       //no rotation
            G4ThreeVector(),       //at origin
            logicHolder,           //logical volume
            "Scintillator Holder", //name
            0,                     //mother  volume
            false,                 //no boolean operation
            0,                     //copy number
            checkOverlaps);        //check for any overlaps
    
    G4Box* scint =
        new G4Box("Scintillator",   //name
            sc_sizeXY, sc_sizeXY, sc_sizeZ); //size

    G4LogicalVolume* logicsc =
        new G4LogicalVolume(scint,         //scint is solid
            sc_mat,                        //material
            "Scintillator");               //name

    G4VPhysicalVolume* physScint = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(),         //at origin
            logicsc,                 //logical volume
            "Scintillator",          //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps

    fScoringVolume = logicsc;

    return physWorld;
}