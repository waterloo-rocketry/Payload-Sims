/// \file BTPrimaryGeneratorAction.hh
/// \brief Definition of the BTPrimaryGeneratorAction class

#pragma once

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class G4Box;

class BTPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
	BTPrimaryGeneratorAction();
	virtual ~BTPrimaryGeneratorAction();

	virtual void GeneratePrimaries(G4Event*);

	const G4ParticleGun* fParticleGun;
	G4Box* fEnvelopeBox;
};

#endif
