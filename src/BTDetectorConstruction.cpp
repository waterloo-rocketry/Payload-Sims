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
#include "G4Tubs.hh"
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
	const G4double world_sizeZ = 300 * cm;
	
    G4double air_density = 0.001225 * g/cm3; //air density at upper atmospher	
	G4Material* world_mat = nist->BuildMaterialWithNewDensity("Upper Atmosphere", "G4_AIR", air_density);
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
    G4Element* B = nist->FindOrBuildElement("B"); 
    G4Element* N = nist->FindOrBuildElement("N");
	G4Element* Si = nist->FindOrBuildElement("Si");
    G4Element* Mg = nist->FindOrBuildElement("Mg");
    G4Element* Cr = nist->FindOrBuildElement("Cr");
    G4Element* Al = nist->FindOrBuildElement("Al");
    G4Element* Cu = nist->FindOrBuildElement("Cu");
    G4Element* Fe = nist->FindOrBuildElement("Fe");
    G4Element* Mn = nist->FindOrBuildElement("Mn");
    G4Element* Gd = nist->FindOrBuildElement("Gd");

    G4Material* BunaN = new G4Material(
        "Buna-N Rubber",
        1.3 * g/cm3,
        3);
    BunaN -> AddElement(C, 7);
    BunaN -> AddElement(H, 9);
    BunaN -> AddElement(N, 1);

	//Scintillator Materials
    G4Material* sc_mat = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE"); 

    G4Material* BNNT = new G4Material(
        "BNNT",
        1.3 * g/cm3,
        2);
    BNNT->AddElement(B, 1);
    BNNT->AddElement(N, 1);

    G4Material* Gd2O3 = new G4Material(
        "Gadolinium Oxide",
        7.41 * g/cm3,
        2);
    Gd2O3->AddElement(Gd, 2);
    Gd2O3->AddElement(O, 3);

    G4Material* methyl_acrylate = new G4Material(
        "Methyl Acrylate",
        0.95 * g/cm3,
        3);
    methyl_acrylate->AddElement(C, 4); 
    methyl_acrylate->AddElement(H, 6);
    methyl_acrylate->AddElement(O, 2);
    
    G4Material* MO = new G4Material(
        "MO",
        (0.95 * 0.7 + 7.41 * 0.3) * g/cm3,
        2);
    
    MO->AddMaterial(Gd2O3, 0.3);
    MO->AddMaterial(methyl_acrylate, 0.7);
    
    G4Material* Cellulose = new G4Material(
        "Cellulose",
        1.5 * g/cm3,
        3);
    Cellulose->AddElement(C, 6);
    Cellulose->AddElement(H, 10);
    Cellulose->AddElement(O, 5);


    G4Material* pp7628 = new G4Material(
        "Pre-preg 7628",
        2.46 * g/cm3, //Borrowed from Wikipedia (S-2 Glass)
        2);
    pp7628->AddElement(Si, 1);
    pp7628->AddElement(O, 2);
    G4Material* gsk_mat = BunaN;
    G4Material* alum_metal = nist->FindOrBuildMaterial("G4_Al"); // Aluminium
    G4Material* copper_metal = nist->FindOrBuildMaterial("G4_Cu"); // Copper
    G4Material* PLA = new G4Material(
        "PLA Plastic",
        1.25 * kg/cm3,
        3);
    PLA->AddElement(C, 3);
    PLA->AddElement(H, 4);
    PLA->AddElement(O, 2);

    G4Material* Alum6061_T6 = new G4Material("6061 Aluminium T6", 2.7 * g/cm3, 5); 
    Alum6061_T6->AddElement(Al, 97.25*perCent); //Source: http://asm.matweb.com/search/SpecificMaterial.asp?bassnum=MA6061T 
    Alum6061_T6->AddElement(Mg, 1.2*perCent);
    Alum6061_T6->AddElement(Cr, 0.35*perCent);
    Alum6061_T6->AddElement(Cu, 0.4*perCent);
    Alum6061_T6->AddElement(Si, 0.8*perCent);

    G4Material* Steel1010 = new G4Material("ANSI Steel 1010", 7.87 * g/cm3, 3);
    Steel1010->AddElement(Fe, 99.3*perCent); //Source: https://www.azom.com/article.aspx?ArticleID=6539    
    Steel1010->AddElement(Mn, 0.6*perCent);
    Steel1010->AddElement(C, 0.1*perCent);

    G4Material* bodytubeMaterial = pp7628; //S-2 Glass Fiber

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

    //Enclosure

    // Scintillator Dimensions
	G4double sc_sizeX = 35 * mm, sc_sizeY = 30 * mm, sc_sizeZ = 75 * mm;
    G4double enclosure_sizeZ = sc_sizeZ + 0.5 * mm;

    G4Box* enclosure = //not an actual object, just scint + layer, used for the solid subtraction underneath
        new G4Box("Scintillator",   //name
            0.5*sc_sizeX, 0.5*sc_sizeY, 0.5*enclosure_sizeZ); //size

    G4Box* scint =
        new G4Box("Scintillator",   //name
            0.5*sc_sizeX, 0.5*sc_sizeY, 0.5*sc_sizeZ); //size
   
    G4double mo_sizeZ = 0.1 * mm;
    G4Box* mo_layer = 
        new G4Box("MO - Acrylic",
            0.5*sc_sizeX, 0.5*sc_sizeY, 0.5*mo_sizeZ);
    
    G4double bnnt_sizeZ = 0.13 * mm;
    G4Box* bnnt_layer = 
        new G4Box("BNNT layer",
            0.5*sc_sizeX, 0.5*sc_sizeY, 0.5*bnnt_sizeZ);
     
    G4double paper_sizeZ = 0.05 * mm;
    G4Box* parchment =
        new G4Box("Parchment Paper",
            0.5*sc_sizeX, 0.5*sc_sizeY, 0.5*paper_sizeZ);

    G4Box *outerHolder = new G4Box("Holder - Outer Rim", 0.5 * b_sizeX, 0.5 * b_sizeY, 0.5 * enclosure_sizeZ);
    G4SubtractionSolid *s1 = new G4SubtractionSolid("Temp Holder 1", outerHolder, enclosure, 0, 
            G4ThreeVector(0.5*38 * mm, 0.5*32 * mm, 0 * mm));
    G4SubtractionSolid *s2 = new G4SubtractionSolid("Temp Holder 2", s1, enclosure, 0, 
            G4ThreeVector(0.5*38 * mm, -0.5*32 * mm, 0 * mm));        
    G4SubtractionSolid *Holder = new G4SubtractionSolid("Holder", s2, enclosure, 0, 
            G4ThreeVector(0.5*(-40 * mm), 0 * mm, 0 * mm)); //Use this one for the actual holder

    //Front/Back Face / Cover Panel Dimensions
    G4double railThick = 8.5 * mm, railLength = 100 * mm, railHeight = 327.5 * mm, topRailHeight = 10 * mm;
    G4double cpThick = 2.66 * mm, cpLength = 83 * mm, cpHeight = 307.5 * mm;
    G4double topFaceCut = 70 * mm;

    G4Box *outerFBFace = new G4Box("Outer FBFace", 0.5 * railThick, 0.5 * railLength, 0.5 * railHeight);
    G4Box *FBFaceCutOut = new G4Box("FBFace Cutout", 0.5 * railThick, 0.5 * cpLength, 0.5 * cpHeight);
    G4SubtractionSolid *FBFace = new G4SubtractionSolid("CubeSat FBFace", outerFBFace, FBFaceCutOut, 0, 
            G4ThreeVector(0,0,0));
            //G4ThreeVector(0.5 * railThick, 0.5 * railLength, 0.5 * railHeight)); //Use this one for the actual holder, cut out from center
    
    G4Box* cpanel =
        new G4Box("Cover Panel",   //name
            0.5*cpThick, 0.5*cpLength, 0.5*cpHeight); //size
    
    G4Box* OutertopFace = new G4Box("Outer Top Face", 0.5 * cpLength, 0.5 * railLength, 0.5 * topRailHeight);
    G4Box *tfCutOut = new G4Box("Top Face Cutout", 0.5 * topFaceCut, 0.5 * topFaceCut, 0.5 * topRailHeight);
    G4SubtractionSolid *topFace = new G4SubtractionSolid("Top Panel", OutertopFace, tfCutOut, 0, 
            G4ThreeVector(0,0,0)); //Use this one for the actual holder, cut out from center
    
    G4double tubeOuter = 76.2 * mm, tubeInner = 74.6015 * mm, couplerR = 69.85 * mm; //radius
    G4double btPZ = (16.28*mm + railHeight), btRZ = 381*mm, coupZ = 7.94*mm;
    G4VSolid* btP = new G4Tubs("Body Tube - Payload", tubeInner, tubeOuter, 0.5*btPZ, 0, 360*deg);  
    G4VSolid* btR = new G4Tubs("Body Tube - Recovery", tubeInner, tubeOuter, 0.5*btRZ, 0, 360*deg);
    G4VSolid* coupler = new G4Tubs("Coupler", 0, tubeOuter, 0.5*coupZ, 0, 360*deg);  


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

    /*G4LogicalVolume* logicsc =
        new G4LogicalVolume(scint,         //scint is solid
            sc_mat,                            //material (Placeholder ABS, not actually)
            "Scintillator Scoring Volume");                   //name
   */
    G4LogicalVolume* logicsc_duplicate = 
        new G4LogicalVolume(scint,
            sc_mat,
            "Scintillator");

    G4LogicalVolume* logic_bnnt = 
        new G4LogicalVolume(bnnt_layer,
            BNNT,
            "BNNT layer");
    
    G4LogicalVolume* logic_mo = 
        new G4LogicalVolume(mo_layer,
            MO,
            "MO layer");
      
    G4LogicalVolume* logicpaper =
        new G4LogicalVolume(parchment,
            Cellulose,
            "Parchment Paper");

    G4LogicalVolume* logicHolder =
        new G4LogicalVolume(Holder,         //scint is solid
            PLA,                        //material (temporarily PLA)
            "Holder");               //name
    
    G4LogicalVolume* logiclid =
        new G4LogicalVolume(lid,         //scint is solid
            PLA,                        //material
            "Lid");               //name

    G4LogicalVolume* logicFBFace =
        new G4LogicalVolume(FBFace,         //scint is solid
            Alum6061_T6,                        //material
            "Front/Back Face");               //name
    
    G4LogicalVolume* logiccpanel =
        new G4LogicalVolume(cpanel,         //scint is solid
            Steel1010,                        //material
            "Cover Panel");               //name

    G4LogicalVolume* logictface =
        new G4LogicalVolume(topFace,         //scint is solid
            alum_metal,                        //material
            "Top Face");               //name

    G4LogicalVolume* logicbtP =
        new G4LogicalVolume(btP,         //scint is solid
            bodytubeMaterial,                        //material
            "Body Tube - Payload");               //name

    G4LogicalVolume* logicbtR =
        new G4LogicalVolume(btR,         //scint is solid
            bodytubeMaterial,                        //material
            "Body Tube - Payload");               //name

    G4LogicalVolume* logicCoupler =
        new G4LogicalVolume(coupler,         //scint is solid
            Alum6061_T6,                        //material
            "Coupler");               //name

    //Physical Definition
    
    //Lower Module at the Center, from bottom to top
    G4double height = 0 * mm;

    ///*
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
            logicsc_duplicate,                 //logical volume
            "Scintillator 1",          //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps
    
    G4VPhysicalVolume* physPaper_1 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0.5*38 * mm, 0.5*32 * mm, height + sc_sizeZ + 0.5*paper_sizeZ),         //at origin
            logicpaper,                 //logical volume
            "Scintillator 1 - Paper",          //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps
    
    G4VPhysicalVolume* physScint_2 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0.5*38 * mm, 0.5*(-32 * mm), height + 0.5*sc_sizeZ),         //at origin
            logicsc_duplicate,                 //logical volume
            "Scintillator 2",        //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps
    
    G4VPhysicalVolume* physMO_2_1 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0.5*38 * mm, 0.5*(-32 * mm), height + sc_sizeZ + 0.5*mo_sizeZ),         //at origin
            logic_mo,                 //logical volume
            "Scintillator 2 - MO - bottom",          //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps

    G4VPhysicalVolume* physPaper_2 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0.5*38 * mm, 0.5*(-32 * mm), height + sc_sizeZ + mo_sizeZ + 0.5 * paper_sizeZ),//at origin
            logicpaper,                 //logical volume
            "Scintillator 2 - Paper",          //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps


    G4VPhysicalVolume* physMO_2_2 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0.5*38 * mm, 0.5*(-32 * mm), height + sc_sizeZ + 1.5* mo_sizeZ + paper_sizeZ), //at origin
            logic_mo,                 //logical volume
            "Scintillator 2 - MO - top",          //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps


    G4VPhysicalVolume* physScint_3 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0.5*(-40 * mm), 0, height + 0.5*sc_sizeZ),         //at origin
            logicsc_duplicate,                 //logical volume
            "Scintillator 3",          //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps
   
    G4VPhysicalVolume* physPaper_3 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0.5*(-40 * mm), 0, height + sc_sizeZ + 0.5 * paper_sizeZ),         //at origin
            logicpaper,                 //logical volume
            "Scintillator 3 - Paper",          //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps
    
    G4VPhysicalVolume* physBNNT_3 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0.5*(-40 * mm), 0, height + sc_sizeZ + paper_sizeZ + 0.5 * bnnt_sizeZ),   //at origin
            logic_bnnt,                 //logical volume
            "Scintillator 3 - BNNT",          //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps
    
 
    //Scint Holder
    G4VPhysicalVolume* physHolder_1 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0,0, height + 0.5*enclosure_sizeZ),         //at origin
            logicHolder,              //logical volume
            "Holder",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps

    height += enclosure_sizeZ;    
    
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
            logicsc_duplicate,                 //logical volume
            "Scintillator 4",          //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps
    
    G4VPhysicalVolume* physPaper_4 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0.5*38 * mm, 0.5*32 * mm, height + sc_sizeZ + 0.5*paper_sizeZ),         //at origin
            logicpaper,                 //logical volume
            "Scintillator 4 - Paper",          //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps
    
    G4VPhysicalVolume* physScint_5 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0.5*38 * mm, 0.5*(-32 * mm), height + 0.5*sc_sizeZ),         //at origin
            logicsc_duplicate,                 //logical volume
            "Scintillator 5",        //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps
    
    G4VPhysicalVolume* physMO_5_1 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0.5*38 * mm, 0.5*(-32 * mm), height + sc_sizeZ + 0.5*mo_sizeZ),         //at origin
            logic_mo,                 //logical volume
            "Scintillator 5 - MO - bottom",          //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps

    G4VPhysicalVolume* physPaper_5 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0.5*38 * mm, 0.5*(-32 * mm), height + sc_sizeZ + mo_sizeZ + 0.5 * paper_sizeZ),//at origin
            logicpaper,                 //logical volume
            "Scintillator 5 - Paper",          //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps


    G4VPhysicalVolume* physMO_5_2 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0.5*38 * mm, 0.5*(-32 * mm), height + sc_sizeZ + 1.5* mo_sizeZ + paper_sizeZ), //at origin
            logic_mo,                 //logical volume
            "Scintillator 5 - MO - top",          //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps


    G4VPhysicalVolume* physScint_6 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0.5*(-40 * mm), 0, height + 0.5*sc_sizeZ),         //at origin
            logicsc_duplicate,                 //logical volume
            "Scintillator 6",          //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps
   
    G4VPhysicalVolume* physPaper_6 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0.5*(-40 * mm), 0, height + sc_sizeZ + 0.5 * paper_sizeZ),         //at origin
            logicpaper,                 //logical volume
            "Scintillator 6 - Paper",          //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps
    
    G4VPhysicalVolume* physBNNT_6 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0.5*(-40 * mm), 0, height + sc_sizeZ + paper_sizeZ + 0.5 * bnnt_sizeZ),   //at origin
            logic_bnnt,                 //logical volume
            "Scintillator 6 - BNNT",          //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps
    
    //Scint Holder
    
    G4VPhysicalVolume* physHolder_2 = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0,0, height + 0.5*enclosure_sizeZ),         //at origin
            logicHolder,              //logical volume
            "Holder",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps
    
    height += enclosure_sizeZ;

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

    height += 10 * mm; // from top of lid to bottom of Top FBFace Panel
    //

    // Top Face
    G4VPhysicalVolume* phystface = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0,0, height + 0.5*topRailHeight),         //at origin
            logictface,              //logical volume
            "Top Face",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps
    
    //Side Cover Panel: Off (outwards) on the Y-axis for ~0.5 mm, but I am overlooking it for now
    //Rotate around Z-axis
    G4RotationMatrix* rotationMatrix = new G4RotationMatrix();
    rotationMatrix->rotateZ(90.*deg);
    
    G4VPhysicalVolume* physlcpanel = 
        new G4PVPlacement(rotationMatrix,         //no rotation
            G4ThreeVector(0, 0.5*railLength, height - 0.5*cpHeight),         //at origin
            logiccpanel,              //logical volume
            "Left Cover Panel",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps

    G4VPhysicalVolume* physrcpanel = 
        new G4PVPlacement(rotationMatrix,         //no rotation
            G4ThreeVector(0, -0.5*railLength, height - 0.5*cpHeight),         //at origin
            logiccpanel,              //logical volume
            "Right Cover Panel",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps

    height += topRailHeight;
    // Front Face
    G4VPhysicalVolume* physfface = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0.5*cpLength + 0.5 * railThick,0, height - 0.5 * railHeight),         //at origin
            logicFBFace,              //logical volume
            "Front Face",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps
    
    G4VPhysicalVolume* physfcpanel = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector((0.5*railThick-0.5*cpThick) + 0.5*cpLength + 0.5 * railThick, 0, height - topRailHeight - 0.5 * cpHeight),         //at origin
            logiccpanel,              //logical volume
            "Front Cover Panel",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps

    //Back Face
    G4VPhysicalVolume* physbface = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(-(0.5*cpLength + 0.5 * railThick),0, height - 0.5 * railHeight),         //at origin
            logicFBFace,              //logical volume
            "Back Face",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps
    
    G4VPhysicalVolume* physbcpanel = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(-(0.5*railThick-0.5*cpThick) - (0.5*cpLength + 0.5 * railThick), 0, height - topRailHeight - 0.5 * cpHeight),         //at origin
            logiccpanel,              //logical volume
            "Back Cover Panel",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps

    height += 16.28 * mm;
    
    G4VPhysicalVolume* physbtP = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0, 0, height - 0.5 * btPZ),         //at origin
            logicbtP,              //logical volume
            "Body Tube - Payload",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps

    G4VPhysicalVolume* physbtR = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0, 0, height + 0.5 * btRZ),         //at origin
            logicbtR,              //logical volume
            "Body Tube - Recovery",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps
    
    height += btRZ;

    G4VPhysicalVolume* physCoupler = 
        new G4PVPlacement(0,         //no rotation
            G4ThreeVector(0, 0, height + 0.5 * coupZ),         //at origin
            logicCoupler,              //logical volume
            "Coupler",                   //name
            logicWorld,              //mother volume
            false,                   //no boolean operation
            0,                       //copy number
            checkOverlaps);          //check for any overlaps
    

    fScoringVolume = logicsc_duplicate;

    return physWorld;
}
