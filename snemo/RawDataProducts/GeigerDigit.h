#ifndef SNEMO_RAWDATAPRODUCTS_GEIGERDIGIT_H
#define SNEMO_RAWDATAPRODUCTS_GEIGERDIGIT_H

namespace snemo {
namespace RawDataProducts {
//! Definition of raw digit from a tracker geiger cell
struct GeigerDigit {
  // ... must be some channel identification here?
  uint64_t   anodeTimeRegister_0; ///< Anodic timestamp from register 0
  uint64_t   anodeTimeRegister_1; ///< Anodic timestamp from register 1
  uint64_t   anodeTimeRegister_2; ///< Anodic timestamp from register 2
  uint64_t   anodeTimeRegister_3; ///< Anodic timestamp from register 3
  uint64_t   anodeTimeRegister_4; ///< Anodic timestamp from register 4
  uint64_t cathodeTimeRegister_5; ///< Anodic timestamp from register 5
  uint64_t cathodeTimeRegister_6; ///< Anodic timestamp from register 6
};
} // namespace RawDataProducts
} // namespace snemo

#endif // SNEMO_RAWDATAPRODUCTS_GEIGERDIGIT_H

