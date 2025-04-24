#include "generator.hh"
#include "G4RandomTools.hh"  // For random number generation
#include "G4ThreeVector.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include <cmath>

MyPrimaryGenerator::MyPrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1);  // Generate one particle per event
    fMuonRate = 1000;  // Number of muons per second (can be adjusted)
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
{
    // Access the particle table to retrieve the muon definition
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = particleTable->FindParticle("mu-"); // "mu-" for negative muons

    // Define beam's cross-sectional area (width and height)
    G4double beamWidth = 5.0 * m;   // Horizontal size of the beam
    G4double beamHeight = 5.0 * m;  // Vertical size of the beam

    // Randomize position within the beam's cross-section
    G4double x = (G4UniformRand() - 0.5) * beamWidth;
    G4double z = (G4UniformRand() - 0.5) * beamHeight;
    G4ThreeVector position(x, 6.0 * m, z);  // Set initial y-coordinate to 6 m

    // Generate theta using cos²(theta) probability distribution
    G4double thetaMax = 5.0 * deg;
    G4double theta;
    do {
        theta = G4UniformRand() * thetaMax;  // Generate theta in range [0, 15°]
    } while (G4UniformRand() > std::pow(std::cos(theta), 2));  // Apply rejection sampling

    // Generate a random azimuthal angle phi in [0, 2π]
    G4double phi = G4UniformRand() * 2.0 * M_PI;

    // Convert to Cartesian coordinates (direction vector)
    G4ThreeVector momentumDirection(sin(theta) * cos(phi), -cos(theta), sin(theta) * sin(phi));
    momentumDirection = momentumDirection.unit(); // Normalize the vector

    // Set particle properties (position, direction, energy)
    fParticleGun->SetParticlePosition(position);
    fParticleGun->SetParticleMomentumDirection(momentumDirection);
    fParticleGun->SetParticleMomentum(4.0 * GeV);  // Set the energy of the muon
    fParticleGun->SetParticleDefinition(particle);

    // Generate the primary vertex (this emits a single particle per event)
    fParticleGun->GeneratePrimaryVertex(anEvent);
}
