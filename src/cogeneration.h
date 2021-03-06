//#ifndef If the function has been previously defined great, if not open it for this file
#ifndef CYCLUS_HYBRID_COGENERATION_H_
#define CYCLUS_HYBRID_COGENERATION_H_

#include <set>
#include <vector>
#include "/home/cyc-user/cyclus/cyclus/src/cyclus.h"

namespace hybrid {

/// This facility acts as a cogeneration of product with a (need to make this into a fluctuating amount) fixed reactor_heat (per
/// time step) capacity and a lifetime capacity defined by a total inventory
/// size(In this case the total inventory size will just be the thermal output of the reactor times the lifetime of the reactor).  It offers its product as a single commodity (Need to change this to two commodities, or have a commodity changer). If a composition (What does composition refer to in this context? Is it the isotopic makeup? Can it be parts heat and parts electricity?)
/// recipe is specified, it provides that single product composition to
/// requesters.  If unspecified, the cogeneration provides products with the exact
/// requested compositions.  The inventory size and reactor_heat both default to
/// infinite.  Supplies product results in corresponding decrease in
/// inventory, and when the inventory size reaches zero, the cogeneration can provide
/// no more product.

//Is Facility a method or a class?
//What do the purple and orange colors mean?
class Cogeneration : public cyclus::Facility,
  public cyclus::toolkit::CommodityProducer {
  //friend class -- nonmember function that can inherit the first class property from the function.  Good example in the C++ programming book
  friend class CogenerationTest;
 public:

//Constructs an instance of the class
  explicit Cogeneration(cyclus::Context* ctx);
//~Destructor class
  virtual ~Cogeneration();

  virtual std::string version() {return 0;}

  //Functions that Emma creates:
  double get_offer_amt();

  std::vector<std::string> offer_amt();



//I am going to copy the above syntax to create a new method that will split the generated resource into
//the quantities which the two sink locations demand. I am unsure as to whether I should create a new class
//or if it makes more sense to include this second method of splitting the resource generated in the same class?

  #pragma cyclus note { \
    "niche": "Cogeneration Facility",   \
    "doc":        \
    "This facility produces multiple commodities, cogenerating multiple resources.\n"\
     "The reactor_heat input is the size of the reactor in MWth, a constant value\n" \
     "The cycle_efficiency input determines the output of electricity given the heat which has been allocated\n" \
     "to electricity generation\n"\
     "The grid_demand input changes at every time step.\n " \
     "At each time step once the total inventory has been met, the time step has completed. \n" \
     "The lifetime capacity is defined by the total inventory size\n" \
     "There are two resources generated in the cogeneration agent.It offers its mater \n" \
     "as two commodities. In order to change the commodity, we will need to put the \n" \
     "commodity through a commodity changer.\n"\
     "There will not be a composition recipe specified.\n" \
     "The inventory size and reactor_heat both default to\n" \
     "infinite.  Supplies product results in corresponding decrease in\n" \
     "inventory, and when the inventory size reaches zero, the cogeneration can provide\n" \
     "no more product.", \
   }

//Use the default ( python's function def - define)
  #pragma cyclus def clone
  #pragma cyclus def schema
  #pragma cyclus def annotations
  #pragma cyclus def infiletodb
  #pragma cyclus def snapshot
  #pragma cyclus def snapshotinv
  #pragma cyclus def initinv

//The pointer Cogeneration (why would we call the pointer Cogeneration?) with a value m in the location Cogeneration
  virtual void InitFrom(Cogeneration* m);

//Is QueryableBackend just a made up pointer for this instance?
//If so, what is it looking for in the cyclus namespace?
  virtual void InitFrom(cyclus::QueryableBackend* b);

//Tick and Tock are declared and implemented to be used in the agent phase, or to search the environment
//when exchanges are occuring
  virtual void Tick() {};

  virtual void Tock() {};

  virtual std::string str();

/*
  #pragma cyclus var { \
  "doc": "Maximum amout of product that can be transferred in or out each time step",\
  "tooltip": "Maximum amout of product that can be transferred in or out each time step",\
  "units": "MW",\
  "uilabel": "Maximum throughput"\
  }
  double throughput;
  */

//Will need to first go through the agent phase before splitting the products
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
    "doc": "Output commodity on which the cogeneration offers product.", \
    "uilabel": "Output Commodity", \
    "uitype": "outcommodity", \
  }
  std::string outcommod;
/*
  #pragma cyclus var { \
    "tooltip": "name of product recipe to provide", \
    "doc": "Name of composition recipe that this cogeneration provides regardless " \
           "of requested composition. If empty, cogeneration creates and provides " \
           "whatever compositions are requested.", \
    "uilabel": "Output Recipe", \
    "default": "", \
    "uitype": "outrecipe", \
  }
  std::string outrecipe;
  */

  #pragma cyclus var { \
    "doc": "Total amount of product this cogeneration has remaining." \
           " Every trade decreases this value by the supplied product " \
           "quantity.  When it reaches zero, the cogeneration cannot provide any " \
           " more product.", \
    "default": 1e299, \
    "uitype": "range", \
    "range": [0.0, 1e299], \
    "uilabel": "Initial Inventory", \
    "units": "kg", \
  }
  double inventory_size;

  #pragma cyclus var {  \
    "default": 1e299, \
    "tooltip": "per time step MWth generation", \
    "units": "MWth/(time step)", \
    "uilabel": "Maximum Generation", \
    "uitype": "range", \
    "range": [0.0, 1e299], \
    "doc": "amount of commodity that can be supplied at each time step", \
  }
  double reactor_heat;

   #pragma cyclus var { \
     "default": [], \
     "uilabel": "Nuclear proportion of the electricity demand from the CA grid", \
     "doc": "Out commodity percent based on preferences " \
            "commodities (same order)." \
            " If unspecified, default is to use .5 for all "\
            "preferences.",                                                     \
   }
   std::vector<double> caiso_demand;

   #pragma cyclus var { \
     "doc": "The efficiency of the power cycle used for the reactor", \
     "default": .33, \
     "uitype": "range", \
     "range": [0.00, 1.00], \
     "uilabel": "cycle_efficiency", \
     "units": "kg", \
   }
   double cycle_efficiency;

};


}  // namespace hybrid

#endif // CYCLUS_HYBRID_COGENERATION_H_
