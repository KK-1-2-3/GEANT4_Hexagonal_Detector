#ifndef LAYER_HH
#define LAYER_HH

#include <G4LogicalVolume.hh>
#include <G4AssemblyVolume.hh>
#include "sheet.hh"
#include <G4VisAttributes.hh>

class Layer {
public:
    Layer(G4double side, G4int nStrips, G4double thickness, G4double spacing, G4double vSpacing, G4RotationMatrix* rotation = nullptr);
    ~Layer();

    G4AssemblyVolume* Construct(G4LogicalVolume* parentLogical, const G4ThreeVector& position, G4RotationMatrix* rotation);

private:
    Sheet* fSheet1;
    Sheet* fSheet2;
    Sheet* fSheet3;
    G4double fVSpacing;
    G4RotationMatrix* fRotation;
    G4double fThickness;
};

#endif
