#ifndef CYCLUS_HYBRID_DUAL_GENERATION_H_
#define CYCLUS_HYBRID_DUAL_GENERATION_H_

#include <set>
#include <vector>

#include "cyclus.h"
#include "hybrid_version.h"

namespace hybrid {

class Context;

/// This facility acts as a dual_generation of material with a fixed throughput (per
/// time step) capacity and a lifetime capacity defined by a total inventory
/// size.  It offers its material as a single commodity. If a composition
/// recipe is specified, it provides that single material composition to
/// requesters.  If unspecified, the dual_generation provides materials with the exact
/// requested compositions.  The inventory size and throughput both default to
/// infinite.  Supplies material results in corresponding decrease in
/// inventory, and when the inventory size reaches zero, the dual_generation can provide
/// no more material.
class DualGeneration : public cyclus::Facility,
  public cyclus::toolkit::CommodityProducer {
  friend class DualGenerationTest;
 public:

  DualGeneration(cyclus::Context* ctx);

  virtual ~DualGeneration();

  virtual std::string version() { return HYBRID_VERSION; }

  #pragma cyclus note { \
    "doc": "This facility acts as a dual_generation of material with a fixed throughput (per\n" \
           "time step) capacity and a lifetime capacity defined by a total inventory\n" \
           "size.  It offers its material as a single commodity. If a composition\n" \
           "recipe is specified, it provides that single material composition to\n" \
           "requesters.  If unspecified, the dual_generation provides materials with the exact\n" \
           "requested compositions.  The inventory size and throughput both default to\n" \
           "infinite.  Supplies material results in corresponding decrease in\n" \
           "inventory, and when the inventory size reaches zero, the dual_generation can provide\n" \
           "no more material.\n" \
           "", \
  }

  #pragma cyclus def clone
  #pragma cyclus def schema
  #pragma cyclus def annotations
  #pragma cyclus def infiletodb
  #pragma cyclus def snapshot
  #pragma cyclus def snapshotinv
  #pragma cyclus def initinv

  virtual void InitFrom(DualGeneration* m);

  virtual void InitFrom(cyclus::QueryableBackend* b);

  virtual void Tick() {};

  virtual void Tock() {};

  virtual std::string str();

  virtual std::set<cyclus::BidPortfolio<cyclus::Material>::Ptr>
      GetMatlBids(cyclus::CommodMap<cyclus::Material>::type&
                  commod_requests);

  virtual void GetMatlTrades(
    const std::vector< cyclus::Trade<cyclus::Material> >& trades,
    std::vector<std::pair<cyclus::Trade<cyclus::Material>,
    cyclus::Material::Ptr> >& responses);

 private:
  #pragma cyclus var { \
    "tooltip": "dual_generation output commodity", \
    "doc": "Output commodity on which the dual_generation offers material.", \
    "uilabel": "Output Commodity", \
    "uitype": "outcommodity", \
  }
  std::string outcommod;

  #pragma cyclus var { \
    "tooltip": "name of material recipe to provide", \
    "doc": "Name of composition recipe that this dual_generation provides regardless " \
           "of requested composition. If empty, dual_generation creates and provides " \
           "whatever compositions are requested.", \
    "uilabel": "Output Recipe", \
    "default": "", \
    "uitype": "outrecipe", \
  }
  std::string outrecipe;

  #pragma cyclus var { \
    "doc": "Total amount of material this dual_generation has remaining." \
           " Every trade decreases this value by the supplied material " \
           "quantity.  When it reaches zero, the dual_generation cannot provide any " \
           " more material.", \
    "default": 1e299, \
    "uitype": "range", \
    "range": [0.0, 1e299], \
    "uilabel": "Initial Inventory", \
    "units": "kg", \
  }
  double inventory_size;

  #pragma cyclus var {  \
    "default": 1e299, \
    "tooltip": "per time step throughput", \
    "units": "kg/(time step)", \
    "uilabel": "Maximum Throughput", \
    "uitype": "range", \
    "range": [0.0, 1e299], \
    "doc": "amount of commodity that can be supplied at each time step", \
  }
  double throughput;

};

}  // namespace hybrid

#endif  // HYBRID_SRC_SOURCE_H_
