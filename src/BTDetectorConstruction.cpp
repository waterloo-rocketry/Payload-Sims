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

    G4Element* C = nist->FindOrBuildElement("C"); 
	G4Element* H = nist->FindOrBuildElement("H"); 
	G4Element* O = nist->FindOrBuildElement("O"); 
	G4Element* Si = nist->FindOrBuildElement("Si");

	G4Material* sc_mat = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE"); //Scintillator Material
    G4Material* pp7628 = new G4Material(
        "Pre-preg 7628",
        2.46 * g/cm3, //Borrowed from Wikipedia (S-2 Glass)
        2,
        kStateSolid, // Conditions in space
        225. * kelvin,
        0.01 * atmosphere
    );
    pp7628->AddElement(Si, 1);
    pp7628->AddElement(O, 2);
    G4Material* gsk_mat = nist->FindOrBuildMaterial("G4_Al"); // Temporarily Aluminium
    G4Material* alum_metal = nist->FindOrBuildMaterial("G4_Al"); // Aluminium
    G4Material* copper_metal = nist->FindOrBuildMaterial("G4_Cu"); // Copper
    G4Material* PLA = new G4Material(
        "PLA Plastic",
        1.25 * kg/cm3,
        3,
        kStateSolid, // Conditions in space
        225. * kelvin,
        0.01 * atmosphere
    );
    PLA->AddElement(C, 3);
    PLA->AddElement(H, 4);
    PLA->AddElement(O, 2);

    //Geometry Definition
    //Scint Module

    //Lid/Gasket/PCB Dimensions
    G4double b_sizeX = 88 * mm, b_sizeY = 80 * mm, b_sizeZ = 2 * mm;

    //PCB Thickness
    G4double pcbtb_Z = 0.035 * mm, pcbpp_Z = 0.2 * mm, pcbcore_Z = 1.1 * mm;

    G4Box* pcb_tb =
        new G4Box("PCB T/B",   //name
            0.5*b_sizeX, 0.5*b_sizeY, 0.5*pcbtb_Z); //size

    G4Box* pcb_pp =
        new G4Box("PCB PP",   //name (PP = pre-preg)
            0.5*b_sizeX, 0.5*b_sizeY, 0.5*pcbpp_Z); //size

    G4Box* pcb_c =
        new G4Box("PCB Core",   //name
            0.5*b_sizeX, 0.5*b_sizeY, 0.5*pcbcore_Z); //size

    G4Box* gsk =
        new G4Box("Gasket",   //name
            0.5*b_sizeX, 0.5*b_sizeY, 0.5*b_sizeZ); //size
            
    G4Box* lid =
        new G4Box("Lid",   //name
            0.5*b_sizeX, 0.5*b_sizeY, 0.5*b_sizeZ); //size

    G4Box *HolderBottom = 
        new G4Box("Holder Bottom", //name (The bottom layer of the holder that actually occpies where SiPM is)
            0.5 * b_sizeX, 0.5 * b_sizeY, 0.5 * 9.5 * mm); //size

    // Scintillator Dimensions
	G4double sc_sizeX = 35 * mm, sc_sizeY = 30 * mm, sc_sizeZ = 75 * mm;

    G4Box* scint =
        new G4Box("Scintillator",   //name
            0.5*sc_sizeX, 0.5*sc_sizeY, 0.5*sc_sizeZ); //size

    G4Box *outerHolder = new G4Box("Holder - Outer Rim", 0.5 * b_sizeX, 0.5 * b_sizeY, 0.5 * sc_sizeZ);
    G4SubtractionSolid *s1 = new G4SubtractionSolid("Temp Holder 1", outerHolder, scint, 0, 
            G4ThreeVector(0.5*38 * mm, 0.5*32 * mm, 0 * mm));
    G4SubtractionSolid *s2 = new G4SubtractionSolid("Temp Holder 2", s1, scint, 0, 
            G4ThreeVector(0.5*38 * mm, -0.5*32 * mm, 0 * mm));        
    G4SubtractionSolid *Holder = new G4SubtractionSolid("Holder", s2, scint, 0, 
            G4ThreeVector(0.5*(-40 * mm), 0 * mm, 0 * mm)); //Use this one for the actual holder

    //Strut/Cover Panel Dimensions
    G4double strutThick = 8.5 * mm, strutLength = 100 * mm, strutHeight = 327.5 * mm; topstrutHeight = 10 * mm
    G4double cpThick = 2.66 * mm, cpLength = 83 * mm, cpHeight = 307.5 * mm;
    G4double topPanelCut = 70 * mm, 

    G4Box *outerStrut = new G4Box("Outer Strut", 0.5 * strutThick, 0.5 * strutLength, 0.5 * strutHeight);
    G4Box *strutCutOut = new G4Box("Strut Cutout", 0.5 * strutThick, 0.5 * cpLength, 0.5 * cpHeight);
    G4SubtractionSolid *strut = new G4SubtractionSolid("CubeSat Strut", outerStrut, strutCutOut, 0, 
            G4ThreeVector(0.5 * strutThick, 0.5 * strutLength, 0.5 * strutHeight)); //Use this one for the actual holder, cut out from center
    
    G4Box* cpanel =
        new G4Box("Cover Panel",   //name
            0.5*cpThick, 0.5*cpLength, 0.5*cpHeight); //size
    
    G4Box* OutertopPanel = new G4Box("Outer Top Panel", 0.5 * cpLength, 0.5 * strutLength, 0.5 * topstrutHeight);
    G4Box *tpCutOut = new G4Box("Top Panel Cutout", 0.5 * topPanelCut, 0.5 * topPanelCut, 0.5 * topstrutHeight);
    G4SubtractionSolid *topPanel = new G4SubtractionSolid("Top Panel", OutertopPanel, tpCutOut, 0, 
            G4ThreeVector(0.5 * cpLength, 0.5 * strutLength, 0.5 * topstrutHeight)); //Use this one for the actual holder, cut out from center
    


    //Logical Volumes (mat defns)
    G4LogicalVolume* logicpcb_tb =
        new G4LogicalVolume(pcb_tb,         //scint is solid
            copper_metal,                        //material
            "PCB T/B");               //name

    G4LogicalVolume* logicpcb_pp =
        new G4LogicalVolume(pcb_pp,         //scint is solid
            pp7628,                        //material
            "PCB PP");               //name
    
    G4LogicalVolume* logicpcb_c =
        new G4LogicalVolume(pcb_c,         //scint is solid
            copper_metal,                        //material
            "PCB Core");               //name

    G4LogicalVolume* logicgskl =
        new G4LogicalVolume(gsk,         //scint is solid
            gsk_mat,                        //material (temporarily PLA)
            "Lower Gasket");               //name

    G4LogicalVolume* logicgsku =
        new G4LogicalVolume(gsk,         //scint is solid
            gsk_mat,                        //material (temporarily PLA)
            "Upper Gasket");               //name

    G4LogicalVolume* logicHB =
        new G4LogicalVolume(HolderBottom,         //scint is solid
            PLA,                        //material (temporarily PLA)
            "Holder Bottom");               //name

    G4LogicalVolume* logicsc =
        new G4LogicalVolume(scint,         //scint is solid
            sc_mat,                            //material (Placeholder ABS, not actually)
            "Scintillator");                   //name
    
     G4LogicalVolume* logicHolder =
        new G4LogicalVolume(Holder,         //scint is solid
            PLA,                        //material (temporarily PLA)
            "Holder");               //name
    
    G4LogicalVolume* logiclid =
        new G4LogicalVolume(lid,         //scint is solid
            alum_metal,                        //material
            "Lid");               //name

    //Physical Definition
    
    //Lower Module at the Center, from bottom to top
    G4double height = 0 * mm;

    //Lower Module
    G4VPhysicalVolume* physpcb_b1 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0,0,height + 0.5*pcbtb_Z),         //at origin
            logicpcb_tb,              //logical volume
            "PCB L4",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps

    height += pcbtb_Z;

    G4VPhysicalVolume* physpcb_pp_b1 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0,0, height + 0.5*pcbpp_Z),//at origin
            logicpcb_pp,              //logical volume
            "PCB PP-b",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps

    height += pcbpp_Z;

    G4VPhysicalVolume* physpcb_c_1 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0,0, height + 0.5*pcbcore_Z),         //at origin
            logicpcb_c,              //logical volume
            "PCB Core",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps

    height += pcbcore_Z;

    G4VPhysicalVolume* physpcb_pp_t1 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0,0, height + 0.5*pcbpp_Z),         //at origin
            logicpcb_pp,              //logical volume
            "PCB PP-t",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps

    height += pcbpp_Z;

    G4VPhysicalVolume* physpcb_t1 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0,0,height + 0.5*pcbtb_Z),         //at origin
            logicpcb_tb,              //logical volume
            "PCB L1",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps

    height += pcbtb_Z;

    //Gasket
    G4VPhysicalVolume* physgsk_l_1 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0,0, height + 0.5 * b_sizeZ),         //dist from center (center of the top face)
            logicgskl,              //logical volume
            "Lower Gasket",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps
    
    height += b_sizeZ;

    //Holder (bottom layer, occupying the sipm spaces, filled by PLA in sim)
    G4VPhysicalVolume* physHB_1 =
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0,0, height + 0.5 * 9.5 * mm),         //at origin
            logicHB,              //logical volume
            "Holder Bottom",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps
    
    height += 9.5 * mm;

    //Scint
    G4VPhysicalVolume* physScint_1 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0.5*38 * mm, 0.5*32 * mm, height + 0.5*sc_sizeZ),         //at origin
            logicsc,                 //logical volume
            "Scintillator 1",          //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps

    G4VPhysicalVolume* physScint_2 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0.5*38 * mm, 0.5*(-32 * mm), height + 0.5*sc_sizeZ),         //at origin
            logicsc,                 //logical volume
            "Scintillator 2",          //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps

    G4VPhysicalVolume* physScint_3 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0.5*(-40 * mm), 0, height + 0.5*sc_sizeZ),         //at origin
            logicsc,                 //logical volume
            "Scintillator 3",          //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps
    
    //Scint Holder
    G4VPhysicalVolume* physHolder_1 = 
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
    G4VPhysicalVolume* physgsk_u_1 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0,0, height + 0.5*b_sizeZ),         //at origin
            logicgsku,              //logical volume
            "Upper Gasket",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps
    
    height += b_sizeZ;

    //Lid
    G4VPhysicalVolume* physlid_1 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0,0, height + 0.5*b_sizeZ),         //at origin
            logiclid,              //logical volume
            "Lid",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps

    height += b_sizeZ;

    height += 10 * mm;

    //Upper Module
    G4VPhysicalVolume* physpcb_b2 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0,0,height + 0.5*pcbtb_Z),         //at origin
            logicpcb_tb,              //logical volume
            "PCB L4",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps

    height += pcbtb_Z;

    G4VPhysicalVolume* physpcb_pp_b2 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0,0, height + 0.5*pcbpp_Z),//at origin
            logicpcb_pp,              //logical volume
            "PCB PP-b",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps

    height += pcbpp_Z;

    G4VPhysicalVolume* physpcb_c_2 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0,0, height + 0.5*pcbcore_Z),         //at origin
            logicpcb_c,              //logical volume
            "PCB Core",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps

    height += pcbcore_Z;

    G4VPhysicalVolume* physpcb_pp_t2 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0,0, height + 0.5*pcbpp_Z),         //at origin
            logicpcb_pp,              //logical volume
            "PCB PP-t",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps

    height += pcbpp_Z;

    G4VPhysicalVolume* physpcb_t2 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0,0,height + 0.5*pcbtb_Z),         //at origin
            logicpcb_tb,              //logical volume
            "PCB L1",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps

    height += pcbtb_Z;

    //Gasket
    G4VPhysicalVolume* physgsk_l_2 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0,0, height + 0.5 * b_sizeZ),         //dist from center (center of the top face)
            logicgskl,              //logical volume
            "Lower Gasket",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps
    
    height += b_sizeZ;

    //Holder (bottom layer, occupying the sipm spaces, filled by PLA in sim)
    G4VPhysicalVolume* physHB_2 =
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0,0, height + 0.5 * 9.5 * mm),         //at origin
            logicHB,              //logical volume
            "Holder Bottom",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps
    
    height += 9.5 * mm;

    //Scint
    G4VPhysicalVolume* physScint_4 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0.5*38 * mm, 0.5*32 * mm, height + 0.5*sc_sizeZ),         //at origin
            logicsc,                 //logical volume
            "Scintillator 4",          //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps

    G4VPhysicalVolume* physScint_5 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0.5*38 * mm, 0.5*(-32 * mm), height + 0.5*sc_sizeZ),         //at origin
            logicsc,                 //logical volume
            "Scintillator 5",          //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps

    G4VPhysicalVolume* physScint_6 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0.5*(-40 * mm), 0, height + 0.5*sc_sizeZ),         //at origin
            logicsc,                 //logical volume
            "Scintillator 6",          //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps
    
    //Scint Holder
    G4VPhysicalVolume* physHolder_2 = 
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
    G4VPhysicalVolume* physgsk_u_2 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0,0, height + 0.5*b_sizeZ),         //at origin
            logicgsku,              //logical volume
            "Upper Gasket",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps
    
    height += b_sizeZ;

    //Lid
    G4VPhysicalVolume* physlid_2 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0,0, height + 0.5*b_sizeZ),         //at origin
            logiclid,              //logical volume
            "Lid",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps

    height += b_sizeZ;

    G4VPhysicalVolume* physCP_1 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(placeholder, placeholder, -127.5 * mm + 0.5*sc_sizeZ),//at origin
            logicsc,                 //logical volume
            "Scintillator 6",          //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps
    
    fScoringVolume = logicsc;

    return physWorld;
}