#ifndef FL_DATAMODELS_RAWGEIGERHITCOLLECTION_H
#define FL_DATAMODELS_RAWGEIGERHITCOLLECTION_H

/// Collection of raw geiger hits
#include "FLArt/RawDataProducts/RawGeigerHit.h"

#include <vector>

namespace FL {
namespace RawDataProducts {
typedef std::vector<RawGeigerHit> RawGeigerHitCollection;
} // namespace data
} // namespace fl

#endif // FL_DATAMODELS_RAWGEIGERHIT_H

