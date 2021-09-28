/// \file BTPrimaryGeneratorAction
/// \brief Implementation of the BTPrimaryGeneratorAction class

#include "BTPrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

BTPrimaryGeneratorAction::BTPrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction(),
    generalParticleSource(nullptr),
    fEnvelopeBox(nullptr)
{

    generalParticleSource = new G4GeneralParticleSource();

}

BTPrimaryGeneratorAction::~BTPrimaryGeneratorAction()
{

    delete generalParticleSource;
    
}

void BTPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

    generalParticleSource->GeneratePrimaryVertex(anEvent);

}
