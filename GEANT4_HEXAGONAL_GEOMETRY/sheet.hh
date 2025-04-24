#ifndef SHEET_HH
#define SHEET_HH
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include <G4LogicalVolume.hh>
#include <G4AssemblyVolume.hh>
#include <G4VisAttributes.hh>
#include <vector>

class Sheet {
public:
    Sheet(G4double side, G4int nStrips, G4double thickness, G4double spacing);
    ~Sheet();

    G4AssemblyVolume* Construct();
    void SetVisAttributes(G4VisAttributes* visAttributes);

private:
    G4double fSide;
    G4int fNStrips;
    G4double fThickness;
    G4double fSpacing;
    std::vector<G4LogicalVolume*> fStripLogicalVolumes;
};

#endif
