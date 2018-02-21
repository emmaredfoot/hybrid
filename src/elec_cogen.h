#line 1 "elec_cogen.h"
//#ifndef CYCLUS_HYBRID_ELEC_COGEN_H_
//#define CYCLUS_HYBRID_ELEC_COGEN_H_

#include <set>
#include <vector>

#include "cyclus.h"

namespace hybrid {

/// This facility acts as a cogeneration of Product with a (need to make this into a fluctuating amount) fixed reactor_size (per
/// time step) capacity and a lifetime capacity defined by a total inventory
/// size(In this case the total inventory size will just be the thermal output of the reactor times the lifetime of the reactor).  It offers its Product as a single commodity (Need to change this to two commodities, or have a commodity changer). If a composition (What does composition refer to in this context? Is it the isotopic makeup? Can it be parts heat and parts electricity?)
/// recipe is specified, it provides that single Product composition to
/// requesters.  If unspecified, the cogeneration provides Products with the exact
/// requested compositions.  The inventory size and reactor_size both default to
/// infinite.  Supplies Product results in corresponding decrease in
/// inventory, and when the inventory size reaches zero, the cogeneration can provide
/// no more Product.

//Is Facility a method or a class?
//What do the purple and orange colors mean?
class ElecCogen : public cyclus::Facility,
  public cyclus::toolkit::CommodityProducer {
  //friend class -- nonmember function that can inherit the first class property from the function.  Good example in the C++ programming book
  friend class ElecCogenTest;
 public:

//Constructs an instance of the class
  explicit ElecCogen(cyclus::Context* ctx);
//~Destructor class
  virtual ~ElecCogen();

  virtual std::string version() {return 0;}

  //Functions that Emma creates:
  //double get_offer_amt();

  //std::vector<std::string> offer_amt();



//I am going to copy the above syntax to create a new method that will split the generated resource into
//the quantities which the two sink locations demand. I am unsure as to whether I should create a new class
//or if it makes more sense to include this second method of splitting the resource generated in the same class?

  #pragma cyclus note { \
    "doc": "This facility produces multiple commodities, cogenerating multiple resources.\n"\
           "The reactor_size input is the size of the reactor in MWth, a constant value\n" \
           "The cycle_efficiency input determines the output of electricity given the heat which has been allocated\n" \
           "to electricity generation\n"\
           "The grid_demand input changes at every time step.\n"\
           "At each time step once the total inventory has been met, the time step has completed. \n" \
           "The lifetime capacity is defined by the total inventory size\n" \
           "There are two resources generated in the cogeneration agent.It offers its mater \n" \
           "as two commodities. In order to change the commodity, we will need to put the \n" \
           "commodity through a commodity changer.\n"\
           "There will not be a composition recipe specified.\n" \
           "The inventory size and reactor_size both default to\n" \
           "infinite.  Supplies Product results in corresponding decrease in\n" \
           "inventory, and when the inventory size reaches zero, the cogeneration can provide\n" \
           "no more Product.\n" \
          }
  };

//Use the default ( python's function def - define)
  #pragma cyclus def clone
  #pragma cyclus def schema
  #pragma cyclus def annotations
  #pragma cyclus def infiletodb
  #pragma cyclus def snapshot
  #pragma cyclus def snapshotinv
  #pragma cyclus def initinv

//The pointer ElecCogen (why would we call the pointer ElecCogen?) with a value m in the location ElecCogen
  virtual void InitFrom(ElecCogen* m);

//Is QueryableBackend just a made up pointer for this instance?
//If so, what is it looking for in the cyclus namespace?
  virtual void InitFrom(cyclus::QueryableBackend* b);

//Tick and Tock are declared and implemented to be used in the agent phase, or to search the environment
//when exchanges are occuring
  virtual void Tick() {};

  virtual void Tock() {};

  virtual std::string str();

  #pragma cyclus var { \
  "doc": "Maximum amount of Product that can be transferred in or out each time step",\
  "tooltip": "Maximum amount of Product that can be transferred in or out each time step",\
  "units": "MW",\
  "uilabel": "Maximum throughput"\
  }
  double throughput;

  #pragma cyclus var { \
  "doc": "Power Cycle Efficiency",\
  "tooltip": "Power Cycle Efficiency",\
  "units": ""%"",\
  "uilabel": "Cycle Efficiency"\
  }
  double cycle_efficiency;

  #pragma cyclus var { \
  "doc": "The Size of the Reactor in MW thermal",\
  "tooltip": "The Size of the Reactor in MW thermal",\
  "units": "MWth",\
  "uilabel": "Reactor size"\
  }
  double reactor_size;

//Will need to first go through the agent phase before splitting the Products
//All of the logic in the archetype that I am building needs to serve these functions
//That run the simulation
  virtual std::set<cyclus::BidPortfolio<cyclus::Product>::Ptr>
      GetProductBids(cyclus::CommodMap<cyclus::Product>::type&
                  commod_requests);

//GetProductBids will need to include an optimization based on price.  Would you recommend
//changing GetProductBids or creating a new method that is not in Products, but instead in Resources

  virtual void GetProductTrades(
    const std::vector< cyclus::Trade<cyclus::Product> >& trades,
    std::vector<std::pair<cyclus::Trade<cyclus::Product>,
    //Class Ptr accesses Product in a similar way as a pointer
    cyclus::Product::Ptr> >& responses);

 private:
//pragma var define the characteristics for each of the inputs from the xml for cyclist2, the GUI.
  #pragma cyclus var { \
    "tooltip": "cogeneration output commodity", \
    "doc": "Output commodity on which the cogeneration offers Product.", \
    "uilabel": "Output Commodity", \
    "uitype": "outcommodity", \
  }
  std::string outcommod;

};


}  // namespace hybrid

//#endif // CYCLUS_ELEC_COGEN_H_
