#ifndef SNEMO_RAWDATAPRODUCTS_CALORIMETERDIGIT_H
#define SNEMO_RAWDATAPRODUCTS_CALORIMETERDIGIT_H

#include <array>

namespace snemo {
namespace RawDataProducts {

struct CalorimeterDigit
{
  static constexpr size_t kSamples {1024};
  using Signal = std::array<uint16_t, kSamples>;

  uint64_t timestamp;
  // Front End Board data here (derived by board from waveform)?
  Signal   waveform;
};

} // namespace RawDataProducts
} // namespace snemo

#endif // SNEMO_RAWDATAPRODUCTS_CALORIMETERDIGIT_H

