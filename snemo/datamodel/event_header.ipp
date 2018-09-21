// -*- mode: c++ ; -*-
/// \file falaise/snemo/datamodels/event_header.ipp

#ifndef FALAISE_SNEMO_DATAMODEL_EVENT_HEADER_IPP
#define FALAISE_SNEMO_DATAMODEL_EVENT_HEADER_IPP 1

// Ourselves:
#include "snemo/datamodel/event_header.h"

// Third party:
// - Boost:
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/nvp.hpp>
// - Bayeux/datatools:
#include <datatools/event_id.ipp>
#include <datatools/i_serializable.ipp>
#include <datatools/properties.ipp>

// This project:
#include "snemo/datamodel/timestamp.ipp"

namespace snemo {

namespace datamodel {

template <class Archive>
void event_header::serialize(Archive& ar_, const unsigned int version_) {
  if (version_ > 0) {
    ar_& DATATOOLS_SERIALIZATION_I_SERIALIZABLE_BASE_OBJECT_NVP;
  }
  ar_& boost::serialization::make_nvp("id", _id_);
  ar_& boost::serialization::make_nvp("generation", _generation_);
  ar_& boost::serialization::make_nvp("timestamp", _timestamp_);
  ar_& boost::serialization::make_nvp("properties", _properties_);
}

}  // end of namespace datamodel

}  // end of namespace snemo

#endif  // FALAISE_SNEMO_DATAMODEL_EVENT_HEADER_IPP
