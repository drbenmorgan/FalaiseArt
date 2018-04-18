#ifndef SNEMO_RAWDATAPRODUCTS_GEIGERDIGITCOLLECTION_H
#define SNEMO_RAWDATAPRODUCTS_GEIGERDIGITCOLLECTION_H
#define FL_DATAMODELS_RAWGEIGERHITCOLLECTION_H

#include "snemo/RawDataProducts/GeigerDigit.h"

#include <vector>

namespace snemo {
namespace RawDataProducts {
//! Collection of GeigerDigits
using GeigerDigitCollection = std::vector<GeigerDigit>;
}
}

#endif // SNEMO_RAWDATAPRODUCTS_GEIGERDIGITCOLLECTION_H

