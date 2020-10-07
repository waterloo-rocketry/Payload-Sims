#include "BTRunAction.hh"
#include "BTPrimaryGeneratorAction.hh"
#include "BTDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

BTRunAction::BTRunAction()
	: G4UserRunAction(),
	fEdep(0.),
	fEdep2(0.)
{
	// add more units to describe dose
    const G4double milligray = 1.e-3 * gray;
    const G4double microgray = 1.e-6 * gray;
    const G4double nanogray = 1.e-9 * gray;
    const G4double picogray = 1.e-12 * gray;

    new G4UnitDefinition("milligray", "milliGy", "Dose", milligray);
    new G4UnitDefinition("microgray", "microGy", "Dose", microgray);
    new G4UnitDefinition("nanogray", "nanoGy", "Dose", nanogray);
    new G4UnitDefinition("picogray", "picoGy", "Dose", picogray);

    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->RegisterAccumulable(fEdep);
    accumulableManager->RegisterAccumulable(fEdep2);
}

BTRunAction::~BTRunAction()
{}

void BTRunAction::BeginOfRunAction(const G4Run*)
{
    G4RunManager::GetRunManager()->SetRandomNumberStore(false);

    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->Reset();
}

void BTRunAction::EndOfRunAction(const G4Run* run)
{
    G4int nofEvents = run->GetNumberOfEvent();
    if (nofEvents == 0) return;

    // Merge accumulables
    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->Merge();

    // Compute dose
    // dose = total energy deposit in a run and its variance
    const G4double edep = fEdep.GetValue();
    const G4double edep2 = fEdep2.GetValue();

    const G4double edepAvg = edep2 - edep * edep / nofEvents;
    if (edepAvg > 0.) rmsEdep = std::sqrt(edepAvg); else rmsEdep = 0.;

    const BTDetectorConstruction* detectorConstruction
        = static_cast<const BTDetectorConstrcution*>
        (G4RunManager::GetRunManager()->GerUserDetectorConstruction());
    const G4double mass = detectorConstruction->GetScoringVolume()->GetMass();
    const G4double dose = edep / mass;
    const G4double rmsDose = rmsEdep / mass;

    auto BTPrimaryGeneratorAction* generatorAction
        = static_cast<const BTPrimaryGeneratorAction*>
        (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAcion());
    G4String runCondition;
    if (generatorAction)
    {
        const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
        runCondition += particleGun->GetParticleDefinition()->GetParticleName();
        runCondition += " of ";
        G4double particleEnergy = particleGun->GetParticleEnergy();
        runCondition += G4BestUnit(particleEnergy, "Energy");
    }

    // tell user program has ended, and give dose and number of events

    if (IsMaster()) {
        G4cout
            << G4endl
            << "-------------------End of Global Run--------------------------";
    }
    else {
        G4cout
            << G4endl
            << "---------------------End of Local Run--------------------------";
    }

    G4cout
        << G4endl
        << "The run consists of " << nofEvents << " " << runCondition
        << G4endl
        << "Cumulated dose per run: "
        << G4BestUnit(dose, "Dose") << "rms = " << G4BestUnit(rmsDose, "Dose")
        << G4endl;
}

void BTRunAction::AddEdep(G4double edep)
{
    fEdep += edep;
    fEdep2 += edep * edep;
}
