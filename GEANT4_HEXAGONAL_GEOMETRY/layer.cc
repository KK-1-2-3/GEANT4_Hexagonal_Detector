#include "layer.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

Layer::Layer(G4double side, G4int nStrips, G4double thickness, G4double spacing, G4double vSpacing, G4RotationMatrix* rotation)
: fVSpacing(vSpacing), fRotation(rotation), fThickness(thickness) {
    fSheet1 = new Sheet(side, nStrips, thickness, spacing);
    fSheet2 = new Sheet(side, nStrips, thickness, spacing);
    fSheet3 = new Sheet(side, nStrips, thickness, spacing);
}

Layer::~Layer() {
    delete fSheet1;
    delete fSheet2;
    delete fSheet3;
}

G4AssemblyVolume* Layer::Construct(G4LogicalVolume* parentLogical, const G4ThreeVector& position, G4RotationMatrix* rotation) {
    G4AssemblyVolume* assembly = new G4AssemblyVolume();

    // Create the assembly for the sheet
    G4AssemblyVolume* sheet1 = fSheet1->Construct();
    G4AssemblyVolume* sheet2 = fSheet2->Construct();
    G4AssemblyVolume* sheet3 = fSheet3->Construct();

    G4ThreeVector pos1(0, -fVSpacing - fThickness, 0);
    G4ThreeVector pos2(0, 0, 0);
    G4ThreeVector pos3(0, fVSpacing + fThickness, 0);

    G4RotationMatrix rot1;
    G4RotationMatrix rot2;
    rot2.rotateY(60 * deg);
    G4RotationMatrix rot3;
    rot3.rotateY(120 * deg);

    assembly->AddPlacedAssembly(sheet1, pos1, &rot1);
    assembly->AddPlacedAssembly(sheet2, pos2, &rot2);
    assembly->AddPlacedAssembly(sheet3, pos3, &rot3);

    // Use the provided rotation matrix, or default to identity if not provided
    G4RotationMatrix finalRotation = rotation ? *rotation : G4RotationMatrix();

    // Create a non-const local copy of the position
    G4ThreeVector nonConstPosition = position;

    // Place the layer in the parent logical volume with the specified rotation
    assembly->MakeImprint(parentLogical, nonConstPosition, &finalRotation);

    // Set visual attributes for each sheet
    G4VisAttributes* vis1 = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0, 1.0)); // Red with 50% transparency
    G4VisAttributes* vis2 = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 1.0)); // Green with 50% transparency
    G4VisAttributes* vis3 = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 1.0)); // Blue with 50% transparency

    // Apply visual attributes to the sheets
    fSheet1->SetVisAttributes(vis1);
    fSheet2->SetVisAttributes(vis2);
    fSheet3->SetVisAttributes(vis3);

    return assembly;
}
