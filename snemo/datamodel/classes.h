#include "canvas/Persistency/Common/Wrapper.h"

// Need to include entire inheritance chain
#include "bayeux/datatools/i_clear.h"
#include "bayeux/datatools/i_serializable.h"
#include "bayeux/datatools/i_tree_dump.h"

#include "bayeux/datatools/event_id.h"
#include "bayeux/datatools/properties.h"
#include "bayeux/geomtools/geom_id.h"

// Datatools workarounds
#include "snemo/datamodel/MultiProperties.h"

// MC
#include "snemo/datamodel/GenBBPrimaryEvent.h"
#include "snemo/datamodel/StepHit.h"
#include "snemo/datamodel/StepHitCollection.h"

// Processing
#include "snemo/datamodel/CalorimeterHit.h"
#include "snemo/datamodel/CalorimeterHitCollection.h"
#include "snemo/datamodel/GeigerHit.h"
#include "snemo/datamodel/GeigerHitCollection.h"
