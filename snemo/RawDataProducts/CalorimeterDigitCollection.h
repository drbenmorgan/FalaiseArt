#ifndef SNEMO_RAWDATAPRODUCTS_CALORIMETERDIGITCOLLECTION_H
#define SNEMO_RAWDATAPRODUCTS_CALORIMETERDIGITCOLLECTION_H

#include "snemo/RawDataProducts/CalorimeterDigit.h"

#include <vector>

namespace snemo {
  namespace RawDataProducts {
    //! Collection of CalorimeterDigits
    using CalorimeterDigitCollection = std::vector<CalorimeterDigit>;
  }
}

#endif // SNEMO_RAWDATAPRODUCTS_CALORIMETERDIGITCOLLECTION_H
