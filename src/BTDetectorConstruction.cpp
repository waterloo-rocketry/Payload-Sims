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
	G4double sc_sizeX = 35 * mm, sc_sizeY = 30 * mm, sc_sizeZ = 75 * mm;

    //Board Parameters
    G4double b_sizeX = 88 * mm, b_sizeY = 80 * mm, b_sizeZ = 2 * mm;

	G4Material* sc_mat = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE"); //Scintillator Material
    G4Material* pcb = nist->FindOrBuildMaterial("G4_Al"); // Temporarily Aluminium
    G4Material* gsk = nist->FindOrBuildMaterial("G4_Al"); // Temporarily Aluminium
    G4Material* alum_metal = nist->FindOrBuildMaterial("G4_Al"); // Aluminium
    G4Material* PLA = new G4Material(
        "PLA Plastic",
        1.25 * kg/cm3,
        4,
        kStateSolid, // Conditions in space
        225. * kelvin,
        0.01 * atmosphere
    );

	G4Element* C = nist->FindOrBuildElement("C"); 
    PLA->AddElement(C, 3);
	G4Element* H = nist->FindOrBuildElement("H"); 
    PLA->AddElement(H, 4);
	G4Element* O = nist->FindOrBuildElement("O"); 
    PLA->AddElement(H, 2);
	G4Element* N = nist->FindOrBuildElement("N"); 
    PLA->AddElement(N, 1);

    //One Module at the Center, from bottom to top
    G4double height = 0 * mm;

    //PCB
    G4Box* pcb_1 =
        new G4Box("PCB",   //name
            0.5*b_sizeX, 0.5*b_sizeY, 0.5*2.54 * mm); //size

    G4LogicalVolume* logicpcb_1 =
        new G4LogicalVolume(pcb_1,         //scint is solid
            pcb,                        //material
            "PCB");               //name

    G4VPhysicalVolume* physpcb_1 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(),         //at origin
            logicpcb_1,              //logical volume
            "PCB",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps

    height = 0.5*2.54 * mm;

    //Gasket (Lower)
    G4Box* gsk_l_1 =
        new G4Box("Lower Gasket",   //name
            0.5*b_sizeX, 0.5*b_sizeY, 0.5*b_sizeZ); //size

    G4LogicalVolume* logicgsk_l_1 =
        new G4LogicalVolume(gsk_l_1,         //scint is solid
            PLA,                        //material (temporarily PLA)
            "Lower Gasket");               //name

    G4VPhysicalVolume* physgsk_l_1 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0,0, height + 0.5 * b_sizeZ),         //dist from center (center of the top face)
            logicgsk_l_1,              //logical volume
            "Lower Gasket",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps
    
    height += b_sizeZ;

    //Scint
    G4Box* scint_1 =
        new G4Box("Scintillator",   //name
            0.5*sc_sizeX, 0.5*sc_sizeY, 0.5*sc_sizeZ); //size

    G4LogicalVolume* logicsc_1 =
        new G4LogicalVolume(scint_1,         //scint is solid
            sc_mat,                            //material (Placeholder ABS, not actually)
            "Scintillator 1");               //name

    G4VPhysicalVolume* physScint_1 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0.5*38 * mm, 0.5*32 * mm, height + 0.5*sc_sizeZ),         //at origin
            logicsc_1,                 //logical volume
            "Scintillator 1",          //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps

    G4Box* scint_2 =
        new G4Box("Scintillator 2",   //name
            0.5*sc_sizeX, 0.5*sc_sizeY, 0.5*sc_sizeZ); //size

    G4LogicalVolume* logicsc_2 =
        new G4LogicalVolume(scint_2,         //scint is solid
            sc_mat,                        //material
            "Scintillator 2");               //name

    G4VPhysicalVolume* physScint_2 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0.5*38 * mm, 0.5*(-32 * mm), height + 0.5*sc_sizeZ),         //at origin
            logicsc_2,                 //logical volume
            "Scintillator 2",          //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps

    G4Box* scint_3 =
        new G4Box("Scintillator 3",   //name
            0.5*sc_sizeX, 0.5*sc_sizeY, 0.5*sc_sizeZ); //size

    G4LogicalVolume* logicsc_3 =
        new G4LogicalVolume(scint_3,         //scint is solid
            sc_mat,                        //material
            "Scintillator 3");               //name

    G4VPhysicalVolume* physScint_3 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0.5*(-40 * mm), 0, height + 0.5*sc_sizeZ),         //at origin
            logicsc_3,                 //logical volume
            "Scintillator 3",          //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps
    
    //Scint Holder
    G4Box *outerHolder = new G4Box("Holder - Outer Rim", 0.5 * b_sizeX, 0.5 * b_sizeY, 0.5 * sc_sizeZ);
    G4SubtractionSolid *s1 = new G4SubtractionSolid("Temp Holder 1", outerHolder, scint_1, 0, 
            G4ThreeVector(0.5*38 * mm, 0.5*32 * mm, 0 * mm));
    G4SubtractionSolid *s2 = new G4SubtractionSolid("Temp Holder 2", s1, scint_2, 0, 
            G4ThreeVector(0.5*38 * mm, -0.5*32 * mm, 0 * mm));        
    G4SubtractionSolid *Holder = new G4SubtractionSolid("Holder", s2, scint_3, 0, 
            G4ThreeVector(0.5*(-40 * mm), 0 * mm, 0 * mm));

     G4LogicalVolume* logicHolder =
        new G4LogicalVolume(Holder,         //scint is solid
            PLA,                        //material (temporarily PLA)
            "Holder");               //name

    G4VPhysicalVolume* physHolder = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0,0, height + 0.5*sc_sizeZ),         //at origin
            logicHolder,              //logical volume
            "Holder",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps

    height += sc_sizeZ;
    //Gasket (Upper)
    G4Box* gsk_u_1 =
        new G4Box("Upper Gasket",   //name
            0.5*b_sizeX, 0.5*b_sizeY, 0.5*b_sizeZ); //size

    G4LogicalVolume* logicgsk_u_1 =
        new G4LogicalVolume(gsk_u_1,         //scint is solid
            PLA,                        //material (temporarily PLA)
            "Upper Gasket");               //name

    G4VPhysicalVolume* physgsk_u_1 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0,0, height + 0.5*b_sizeZ),         //at origin
            logicgsk_u_1,              //logical volume
            "Upper Gasket",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps
    
    height += b_sizeZ;

    //Lid
    G4Box* lid_1 =
        new G4Box("Lid",   //name
            0.5*b_sizeX, 0.5*b_sizeY, 0.5*b_sizeZ); //size

    G4LogicalVolume* logiclid_1 =
        new G4LogicalVolume(lid_1,         //scint is solid
            alum_metal,                        //material
            "Lid");               //name

    G4VPhysicalVolume* physlid_1 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0,0, height + 0.5*b_sizeZ),         //at origin
            logiclid_1,              //logical volume
            "Lid",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps
    
    fScoringVolume = logicsc_1;

    return physWorld;
}