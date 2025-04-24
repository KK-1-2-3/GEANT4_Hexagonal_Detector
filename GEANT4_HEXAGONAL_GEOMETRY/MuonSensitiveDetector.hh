#ifndef MUON_SENSITIVE_DETECTOR_HH
#define MUON_SENSITIVE_DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include <fstream>
#include <string>
#include "globals.hh"

class MuonSensitiveDetector : public G4VSensitiveDetector {
public:
    MuonSensitiveDetector(const G4String& name);
    virtual ~MuonSensitiveDetector();
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory*) override;

private:
    struct HitData {
        G4int stripID;
        G4double energyDeposited;
        G4double muonEnergy;
        G4double posX, posY, posZ;
        G4double time;
    };

    static std::ofstream csvFile;
    static G4bool initialized;

    G4int currentTrackID;
    G4double totalEnergyDeposited;

    void WriteHeader();
    void WriteHitData(const HitData& hitData);
};

#endif // MUON_SENSITIVE_DETECTOR_HH
