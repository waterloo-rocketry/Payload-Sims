/// \file BTPrimaryGeneratorAction.hh
/// \brief Definition of the BTPrimaryGeneratorAction class

#pragma once

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "globals.hh"

class G4GeneralParticleSource;
class G4Event;
class G4Box;

class BTPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
	BTPrimaryGeneratorAction();
	virtual ~BTPrimaryGeneratorAction();

	virtual void GeneratePrimaries(G4Event*);

	const G4GeneralParticleSource* GetParticleGun() const { return generalParticleSource; }
	G4GeneralParticleSource* generalParticleSource;

	G4Box* fEnvelopeBox;
};
