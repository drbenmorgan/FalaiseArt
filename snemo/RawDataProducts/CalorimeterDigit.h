#ifndef SNEMO_RAWDATAPRODUCTS_CALORIMETERDIGIT_H
#define SNEMO_RAWDATAPRODUCTS_CALORIMETERDIGIT_H

#include <array>

namespace snemo {
namespace RawDataProducts {

struct CalorimeterDigit
{
  // Types and Constants
  // Number of samples in signal
  static constexpr size_t kSamples {1024};
  using Signal = std::array<uint16_t, kSamples>;

  // Data Members
  // ... should there be a channel id here?
  uint64_t timestamp;
  // Front End Board data here (derived by board from waveform)?
  // e.g.
  // struct FEBOutput {
  // } febOutput;
  // Always want waveform to begin with, may drop
  // later on in "DataQuaility/Producer" processing stage
  Signal   waveform;
};

} // namespace RawDataProducts
} // namespace snemo

#endif // SNEMO_RAWDATAPRODUCTS_CALORIMETERDIGIT_H

