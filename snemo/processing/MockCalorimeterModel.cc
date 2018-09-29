#include "snemo/processing/MockCalorimeterModel.h"

#include "CLHEP/Random/RandGauss.h"
#include "CLHEP/Units/SystemOfUnits.h"
#include <cmath>

namespace {
  const double kFWHMINSIGMA{1.0 / (2 * std::sqrt(2 * std::log(2.0)))};
} // namespace

namespace snemo {
  MockCalorimeterModel::MockCalorimeterModel(double energyResolution,
                                             std::array<double, 3> alphaQuenchingPars,
                                             double relaxTime)
    : energyResolution_{energyResolution}
    , alphaQuenchingParameters_{alphaQuenchingPars}
    , relaxationTime_{relaxTime}
  {}

  double
  MockCalorimeterModel::getEnergySigma(double simEnergy) const
  {
    return energyResolution_ * kFWHMINSIGMA * std::sqrt(simEnergy / CLHEP::MeV);
  }

  double
  MockCalorimeterModel::smearEnergy(double simEnergy, CLHEP::HepRandomEngine& rng) const
  {
    // Need a source of randomness, then do
    const double smearedEnergy =
      CLHEP::RandGauss::shoot(&rng, simEnergy, this->getEnergySigma(simEnergy));
    return smearedEnergy < 0.0 ? 0.0 : smearedEnergy;
  }

  double
  MockCalorimeterModel::quenchAlphaEnergy(double simEnergy) const
  {
    const double modEnergy = 1.0 / (simEnergy * alphaQuenchingParameters_[1] + 1.0);

    const double quenchingFactor = -1.0 * alphaQuenchingParameters_[0] *
                                   (std::pow(modEnergy, alphaQuenchingParameters_[2]) -
                                    std::pow(modEnergy, alphaQuenchingParameters_[2] / 2.0));

    return simEnergy / quenchingFactor;
  }

  double
  MockCalorimeterModel::getTimeSigma(double simEnergy) const
  {
    return relaxationTime_ * energyResolution_ * kFWHMINSIGMA / std::sqrt(simEnergy / CLHEP::MeV);
  }

  double
  MockCalorimeterModel::smearTime(double simTime,
                                  double simEnergy,
                                  CLHEP::HepRandomEngine& rng) const
  {
    return CLHEP::RandGauss::shoot(&rng, simTime, this->getTimeSigma(simEnergy));
  }

} // namespace snemo
