#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH
#include <G4VUserDetectorConstruction.hh>
#include <G4LogicalVolume.hh>

class Construction : public G4VUserDetectorConstruction {
public:
    Construction();
    virtual ~Construction();

    virtual G4VPhysicalVolume* Construct();

private:
    G4LogicalVolume* logicWorld;
};

#endif
