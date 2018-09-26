#ifndef MOCKCALORIMETERMODEL_HH
#define MOCKCALORIMETERMODEL_HH

// Model for mock calibration of calorimeters from sim data
// We have one of these for each class of calo (calo,xcalo,gveto)

#include <array>

#include "CLHEP/Random/RandomEngine.h"

namespace snemo {
  class MockCalorimeterModel final {
  public:
    MockCalorimeterModel(double energyResolution,
                         std::array<double, 3> alphaQuenchingPars,
                         double relaxTime);

    //! Return energy resolution at given energy in sigma, assuming a Gaussian
    double getEnergySigma(double simEnergy) const;

    //! Return input energy smeared by energy resolution
    double smearEnergy(double simEnergy, CLHEP::HepRandomEngine& rng) const;

    //! Return energy multiplied by quenching factor for Alpha particles
    double quenchAlphaEnergy(double simEnergy) const;

    //! Return time resolution at a given energy in sigma, assuming a Gaussian
    // Taken from G. Pichenot thesis on Nemo2, parameterization
    // for Nemo3 by L. Simard
    double getTimeSigma(double simEnergy) const;

    //! Return input time smeared by time resolution.
    // Negative times can be returned because input time is relative.
    double smearTime(double simTime, double simEnergy, CLHEP::HepRandomEngine& rng) const;

  private:
    double energyResolution_;                        //!< energy resolution in % for 1MeV electrons
    std::array<double, 3> alphaQuenchingParameters_; //!< "Parameters for Alpha quenching function"
    double relaxationTime_;                          //!< Scintillator relaxation time.
  };

} // namespace snemo

#endif // MOCKCALORIMETERMODEL_HH
