//#ifndef If the function has been previously defined great, if not open it for this file
#ifndef CYCLUS_HYBRID_COGENERATION_H_
#define CYCLUS_HYBRID_COGENERATION_H_

#include <set>
#include <vector>

#include "cyclus.h"

namespace hybrid {

/// This facility acts as a cogeneration of material with a (need to make this into a fluctuating amount) fixed throughput (per
/// time step) capacity and a lifetime capacity defined by a total inventory
/// size(In this case the total inventory size will just be the thermal output of the reactor times the lifetime of the reactor).  It offers its material as a single commodity (Need to change this to two commodities, or have a commodity changer). If a composition (What does composition refer to in this context? Is it the isotopic makeup? Can it be parts heat and parts electricity?)
/// recipe is specified, it provides that single material composition to
/// requesters.  If unspecified, the cogeneration provides materials with the exact
/// requested compositions.  The inventory size and throughput both default to
/// infinite.  Supplies material results in corresponding decrease in
/// inventory, and when the inventory size reaches zero, the cogeneration can provide
/// no more material.
class Cogeneration : public cyclus::Facility,
  public cyclus::toolkit::CommodityProducer {
  //friend class -- nonmember function that can inherit the first property from the function.  Good example in the C++ programming book
  friend class CogenerationTest;
 public:

  explicit Cogeneration(cyclus::Context* ctx);

  virtual ~Cogeneration();

  #pragma cyclus note { \
    "doc": "This facility produces multiple commodities, cogenerating multiple resources.\n" \
           "The throughput changes based on optimization and preferences at each time step. \n" \
           "At each time step once the total inventory has been met, the time step has completed. \n" \
           "The lifetime capacity is defined by the total inventory size\n" \
           "There are two resources generated in the cogeneration agent.It offers its mater \n" \
           "as two commodities. In order to change the commodity, we will need to put the \n" \
           "commodity through a commodity changer.\n"\
           "There will not be a composition recipe specified.\n" \
           "The inventory size and throughput both default to\n" \
           "infinite.  Supplies material results in corresponding decrease in\n" \
           "inventory, and when the inventory size reaches zero, the cogeneration can provide\n" \
           "no more material.\n" \
           "", \
  }
//Use the default ( python's function def) 
  #pragma cyclus def clone
  #pragma cyclus def schema
  #pragma cyclus def annotations
  #pragma cyclus def infiletodb
  #pragma cyclus def snapshot
  #pragma cyclus def snapshotinv
  #pragma cyclus def initinv

  virtual void InitFrom(Cogeneration* m);

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
    //Class Ptr accesses material in a similar way as a pointer
    cyclus::Material::Ptr> >& responses);

 private:
//pragma var define the characteristics for each of the inputs from the xml for cyclist2, the GUI.
  #pragma cyclus var { \
    "tooltip": "cogeneration output commodity", \
    "doc": "Output commodity on which the cogeneration offers material.", \
    "uilabel": "Output Commodity", \
    "uitype": "outcommodity", \
  }
  std::string outcommod;

  #pragma cyclus var { \
    "tooltip": "name of material recipe to provide", \
    "doc": "Name of composition recipe that this cogeneration provides regardless " \
           "of requested composition. If empty, cogeneration creates and provides " \
           "whatever compositions are requested.", \
    "uilabel": "Output Recipe", \
    "default": "", \
    "uitype": "outrecipe", \
  }
  std::string outrecipe;

  #pragma cyclus var { \
    "doc": "Total amount of material this cogeneration has remaining." \
           " Every trade decreases this value by the supplied material " \
           "quantity.  When it reaches zero, the cogeneration cannot provide any " \
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

#endif // CYCLUS_HYBRID_COGENERATION_H_
