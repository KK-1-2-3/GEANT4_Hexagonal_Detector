#include "sheet.hh"
#include "MuonSensitiveDetector.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4AssemblyVolume.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include <cmath>

// Constructor
Sheet::Sheet(G4double side, G4int nStrips, G4double thickness, G4double spacing)
: fSide(side), fNStrips(nStrips), fThickness(thickness), fSpacing(spacing) {}

// Destructor
Sheet::~Sheet() {}

G4AssemblyVolume* Sheet::Construct() {
    G4NistManager* nist = G4NistManager::Instance();
    G4Material* material = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

    G4double stripWidth = (std::sqrt(3.0) / 2.0) * fSide / fNStrips;
    G4AssemblyVolume* assembly = new G4AssemblyVolume();

    MuonSensitiveDetector* sensitiveDetector = new MuonSensitiveDetector("MuonSensitiveDetector");
    G4SDManager::GetSDMpointer()->AddNewDetector(sensitiveDetector);

    for (G4int i = 0; i < fNStrips; i++) {

        G4double stripLength = 2.0 * fSide - fSide * (i + 0.5) / fNStrips - fSpacing;
        G4double effectiveStripWidth = stripWidth - fSpacing;

        G4Box* stripSolid = new G4Box("Strip", effectiveStripWidth / 2, fThickness / 2, stripLength / 2);
        G4LogicalVolume* stripLogical = new G4LogicalVolume(stripSolid, material, "Strip");
        stripLogical->SetSensitiveDetector(sensitiveDetector);
        fStripLogicalVolumes.push_back(stripLogical);

        G4ThreeVector stripPositionP(stripWidth / 2.0 + i * stripWidth, 0., 0.);
        G4ThreeVector stripPositionN(-stripWidth / 2.0 - i * stripWidth, 0., 0.);

        assembly->AddPlacedVolume(stripLogical, stripPositionP, 0);
        assembly->AddPlacedVolume(stripLogical, stripPositionN, 0);
    }

    return assembly;
}

void Sheet::SetVisAttributes(G4VisAttributes* visAttributes) {
    for (auto& stripLogical : fStripLogicalVolumes) {
        stripLogical->SetVisAttributes(visAttributes);
    }
}
