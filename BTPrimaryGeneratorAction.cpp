/// \file BTPrimaryGeneratorAction
/// \brief Implementation of the BTPrimaryGeneratorAction class

#include "BTPrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

BTPrimaryGeneratorAction::BTPrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction(),
    fParticleGun(nullptr),
    nParticleGun(nullptr),
    fEnvelopeBox(nullptr)
{
    G4int n_particle = 1;
    fParticleGun = new G4ParticleGun(n_particle);

    //default particle specs
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName;
    G4ParticleDefinition* particle
        = particleTable->FindParticle(particleName = "gamma");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
    fParticleGun->SetParticleEnergy(10. * MeV);

    G4int n_particle2 = 1;
    nParticleGun = new G4ParticleGun(n_particle2);

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName;
    G4ParticleDefinition* particle2
        = particleTable->FindParticle(particleName = "neutron");
    nParticleGun->SetParticleDefinition(particle);
    nParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
    nParticleGun->SetParticleEnergy(1. * MeV);

}

BTPrimaryGeneratorAction::~BTPrimaryGeneratorAction()
{
    delete fParticleGun;

    delete nParticleGun;
}

void BTPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    //this function gets called at the start of an event
    //from the example codes: "In order to avoid dependence of PrimaryGeneratorAction
    //on DetectorConstruction class we get Envelope volume
    //from G4LogicalVolumeStore."

    G4double envSizeXY = 0;
    G4double envSizeZ = 0;

    if (fEnvelopeBox) {
        envSizeXY = fEnvelopeBox->GetXHalfLength() * 2.;
        envSizeZ = fEnvelopeBox->GetZHalfLength() * 2.;
    }

    else
    {
        G4LogicalVolume* envLV
            = G4LogicalVolumeStore::GetInstance()->GetVolume("Envelope");
        if (envLV) fEnvelopeBox = dynamic_cast<G4Box*>(envLV->GetSolid());
    }

    const G4double x0 = size * envSizeXY * (G4UniformRand() - 0.5);
    const G4double yo = size * envSizeXY * (G4UniformRand() - 0.5);
    const G4double z0 = -0.5 * envSizeZ;

    fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));

    fParticleGun->GeneratePrimaryVertex(anEvent);

    nParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));

    nParticleGun->GeneratePrimaryVertex(anEvent);

}
