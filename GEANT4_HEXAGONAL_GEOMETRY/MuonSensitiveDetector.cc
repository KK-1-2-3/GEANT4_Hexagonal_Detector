#include "MuonSensitiveDetector.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4TouchableHandle.hh"
#include <iomanip>

// Initialize static members
std::ofstream MuonSensitiveDetector::csvFile;
G4bool MuonSensitiveDetector::initialized = false;

MuonSensitiveDetector::MuonSensitiveDetector(const G4String& name)
: G4VSensitiveDetector(name), currentTrackID(-1), totalEnergyDeposited(0) {
    if (!initialized) {
        // Open the CSV file and write the header
        csvFile.open("muon_hits_output.csv");
        if (!csvFile.is_open()) {
            G4cerr << "Error opening CSV file for writing!" << G4endl;
        } else {
            WriteHeader();
        }
        initialized = true;
    }
}

MuonSensitiveDetector::~MuonSensitiveDetector() {
    if (csvFile.is_open()) {
        csvFile.close();
    }
}

void MuonSensitiveDetector::WriteHeader() {
    csvFile << "StripID,EnergyDeposited,MuonEnergy,PosX,PosY,PosZ,Time\n";
}

void MuonSensitiveDetector::WriteHitData(const HitData& hitData) {
    csvFile << hitData.stripID << ","
    << hitData.energyDeposited << ","
    << hitData.muonEnergy << ","
    << hitData.posX << ","
    << hitData.posY << ","
    << hitData.posZ << ","
    << hitData.time << "\n";
}

G4bool MuonSensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
    G4Track* track = aStep->GetTrack();

    // Check if the particle is a primary muon
    if (track->GetParticleDefinition()->GetParticleName() == "mu-" && track->GetParentID() == 0) {
        G4int trackID = track->GetTrackID();
        G4double energyDeposited = aStep->GetTotalEnergyDeposit();

        // Skip hits with no energy deposition
        if (energyDeposited == 0) {
            return false;
        }

        // Accumulate energy deposition per track
        if (trackID != currentTrackID) {
            currentTrackID = trackID;
            totalEnergyDeposited = 0;
        }
        totalEnergyDeposited += energyDeposited;

        // ✅ Check if the muon is exiting the volume
        if (aStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary) {
            // Retrieve copy numbers for unique identification
            const G4TouchableHandle& touchable = aStep->GetPreStepPoint()->GetTouchableHandle();
            G4int iScint = touchable->GetCopyNumber(1);
            G4int iSiPM = touchable->GetCopyNumber();

            // Construct a unique identifier for the hit
            HitData hitData;
            hitData.stripID = iScint * 100 + iSiPM;

            // Get the hit position
            G4ThreeVector hitPosition = aStep->GetPostStepPoint()->GetPosition();

            // Get muon kinetic energy at exit
            hitData.muonEnergy = aStep->GetPreStepPoint()->GetKineticEnergy() / MeV;

            // Populate hit data
            hitData.energyDeposited = totalEnergyDeposited/MeV;
            hitData.posX = hitPosition.x() / cm;
            hitData.posY = hitPosition.y() / cm;
            hitData.posZ = hitPosition.z() / cm;
            hitData.time = aStep->GetPostStepPoint()->GetGlobalTime() / ns;

            // Write the hit data to the CSV file
            WriteHitData(hitData);

            // Log hit details
            G4cout << "Muon exited at position: ("
            << hitData.posX << " cm, "
            << hitData.posY << " cm, "
            << hitData.posZ << " cm) "
            << "with total energy deposited: " << hitData.energyDeposited << " MeV" << G4endl;
            G4cout << "Muon energy at exit: " << hitData.muonEnergy << " MeV" << G4endl;
            G4cout << "Copy Number: " << iScint << " in SiPM: " << iSiPM << G4endl;
            G4cout << "Hit Time: " << hitData.time << " ns" << G4endl;

            // Reset for the next track
            totalEnergyDeposited = 0;
        }
    }
    return true;
}
