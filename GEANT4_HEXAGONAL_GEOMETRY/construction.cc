#include "construction.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "layer.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
Construction::Construction() : G4VUserDetectorConstruction() {}

Construction::~Construction() {}

G4VPhysicalVolume* Construction::Construct() {
    G4NistManager* nist = G4NistManager::Instance();

    // World volume
    G4Material* worldMaterial = nist->FindOrBuildMaterial("G4_AIR");
    G4Box* solidWorld = new G4Box("World", 5*m, 5*m, 5*m);
    logicWorld = new G4LogicalVolume(solidWorld, worldMaterial, "World");

    G4VPhysicalVolume* physWorld = new G4PVPlacement(
        0, G4ThreeVector(), logicWorld, "World", 0, false, 0, true);

    // Create and place the layer in the worldG4double side = 10*cm;
    G4double side =3.0*m;
    G4int nStrips = 20;
    G4double thickness = 1.0*cm;
    G4double spacing = 0.0*cm;
    G4double vSpacing = 0.0*cm;


    //Layer arrangement:
    G4double space = 1.0*m;
    G4double extra_space = 30.0  *cm;
    G4double dspace = 3.0*thickness+extra_space;
    // Layer rotation
    G4RotationMatrix* layerRotation = new G4RotationMatrix();
    layerRotation->rotateX(0*deg);  // Rotate 30 degrees around the X-axis

    G4ThreeVector layerPosition01(0, space+dspace, 0);  // Position for detector1
    G4ThreeVector layerPosition02(0, space, 0);
    G4ThreeVector layerPosition03(0, space-dspace, 0);

    // Material
    // Sample material
    G4Material* uraniumMaterial = nist->FindOrBuildMaterial("G4_U");
    G4Material* softMaterial = nist->FindOrBuildMaterial("G4_Fe");

    // Corrected dimensions
    G4double centerSize = 70 * cm;  // Fixed: Nonzero size
    G4double thick = 30 * cm;
    G4double armThickness = 70 * cm;
    G4double armLength = 1.0 * m;
    G4double smallUraniumSize = 70 * cm;

    // ✅ Center Uranium Block (No Overlaps)
    G4Box* uraniumBlock = new G4Box("UraniumBlock", centerSize/2, thick/2, centerSize/2);
    G4LogicalVolume* uraniumLogical = new G4LogicalVolume(uraniumBlock, uraniumMaterial, "UraniumBlock");
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), uraniumLogical, "UraniumBlock", logicWorld, false, 0, true);

    // ✅ Horizontal and Vertical Arms (Soft Material)
    G4Box* armX = new G4Box("ArmX", armLength/2, thick/2, armThickness/2);
    G4Box* armZ = new G4Box("ArmZ", armThickness/2, thick/2, armLength/2);

    G4LogicalVolume* armLogicalX = new G4LogicalVolume(armX, softMaterial, "ArmX");
    G4LogicalVolume* armLogicalZ = new G4LogicalVolume(armZ, softMaterial, "ArmZ");

    // ✅ Place arms ensuring they do not overlap with the uranium block
    G4double xOffset = (centerSize + armLength)/2 + 0.1 * cm;  // Small gap to avoid overlaps
    G4double zOffset = (centerSize + armLength)/2 + 0.1 * cm;

    new G4PVPlacement(nullptr, G4ThreeVector(xOffset, 0, 0), armLogicalX, "ArmX_Pos", logicWorld, false, 0, true);
    new G4PVPlacement(nullptr, G4ThreeVector(-xOffset, 0, 0), armLogicalX, "ArmX_Neg", logicWorld, false, 1, true);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zOffset), armLogicalZ, "ArmZ_Pos", logicWorld, false, 2, true);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -zOffset), armLogicalZ, "ArmZ_Neg", logicWorld, false, 3, true);

    // ✅ Small Uranium Block at the extreme end of one arm (placed beyond the arm)
    G4Box* smallUraniumBlock = new G4Box("SmallUraniumBlock", smallUraniumSize/2, thick/2, smallUraniumSize/2);
    G4LogicalVolume* smallUraniumLogical = new G4LogicalVolume(smallUraniumBlock, uraniumMaterial, "SmallUraniumBlock");

    // Position it **beyond the arm**, not inside it.
    G4double smallBlockOffset = xOffset + armLength/2 + smallUraniumSize/2 + 0.1 * cm;
    new G4PVPlacement(nullptr, G4ThreeVector(smallBlockOffset, 0, 0),
                      smallUraniumLogical, "SmallUraniumBlock", logicWorld, false, 4, true);

    // ✅ Set visualization attributes
    G4VisAttributes* visUr = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0, 0.8));
    visUr->SetForceSolid(true);
    uraniumLogical->SetVisAttributes(visUr);
    smallUraniumLogical->SetVisAttributes(visUr);

    G4VisAttributes* visArm1 = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.2));
    visArm1->SetForceSolid(true);
    armLogicalX->SetVisAttributes(visArm1);

    G4VisAttributes* visArm2 = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.2));
    visArm2->SetForceSolid(true);
    armLogicalZ->SetVisAttributes(visArm2);





    G4ThreeVector layerPosition11(0, -space-dspace, 0);  // Position for detector2
    G4ThreeVector layerPosition12(0, -space, 0);
    G4ThreeVector layerPosition13(0, -space+dspace, 0);

    Layer* layer01 = new Layer(side, nStrips, thickness, spacing, vSpacing, layerRotation);
    layer01->Construct(logicWorld, layerPosition01, layerRotation);
    Layer* layer02 = new Layer(side, nStrips, thickness, spacing, vSpacing, layerRotation);
    layer02->Construct(logicWorld, layerPosition02, layerRotation);
    Layer* layer03 = new Layer(side, nStrips, thickness, spacing, vSpacing, layerRotation);
    layer03->Construct(logicWorld, layerPosition03, layerRotation);

    Layer* layer11 = new Layer(side, nStrips, thickness, spacing, vSpacing, layerRotation);
    layer11->Construct(logicWorld, layerPosition11, layerRotation);
    Layer* layer12 = new Layer(side, nStrips, thickness, spacing, vSpacing, layerRotation);
    layer12->Construct(logicWorld, layerPosition12, layerRotation);
    Layer* layer13 = new Layer(side, nStrips, thickness, spacing, vSpacing, layerRotation);
    layer13->Construct(logicWorld, layerPosition13, layerRotation);

    return physWorld;
}
