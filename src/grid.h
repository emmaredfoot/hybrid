
#line 1 "grid.h"
#ifndef CYCLUS_HYBRID_GRID_H_
#define CYCLUS_HYBRID_GRID_H_

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "cyclus.h"

namespace hybrid {

class Context;

/// This facility acts as a grid of the product electricity with a fluctuating
/// throughput (per time step) capacity (the demand).
///  The throughput capacity
/// defaults to infinite. This archetype only takes in electricity.
/// Requests are made only for electricity
class Grid : public cyclus::Facility  {
 public:
  Grid(cyclus::Context* ctx);

  virtual ~Grid();

  #pragma cyclus note {     "doc":     " A grid facility that accepts electricity with a fluctuating\n"
    "throughput (per time step) capacity \n"
    " The throughput capacity\n"
    " defaults to infinite. If a recipe is provided, it will request\n"
    " Product with that recipe. Requests are made for any number of\n"    " specified commodities.\n"     }
#line 38 "/home/cyc-user/cyclus/hybrid/src/grid.h"

  virtual void InitFrom(hybrid::Grid* m);

  virtual void InitFrom(cyclus::QueryableBackend* b);

  virtual void InfileToDb(cyclus::InfileTree* tree, cyclus::DbInit di);

  virtual cyclus::Agent* Clone();

  virtual std::string schema();

  virtual Json::Value annotations();

  virtual void InitInv(cyclus::Inventories& inv);

  virtual cyclus::Inventories SnapshotInv();

  virtual void Snapshot(cyclus::DbInit di);
#line 40 "/home/cyc-user/cyclus/hybrid/src/grid.h"

  virtual std::string str();

  virtual void EnterNotify();

  virtual void Tick();

  virtual void Tock();

  /// @brief GridFacilities request Products of their given commodity. Note
  /// that it is assumed the Grid operates on a single resource type!
  virtual std::set<cyclus::RequestPortfolio<cyclus::Product>::Ptr>
      GetProductRequests();

  /// @brief GridFacilities request Products of their given
  /// commodity. Note that it is assumed the Grid operates on a single
  /// resource type!
  virtual std::set<cyclus::RequestPortfolio<cyclus::Product>::Ptr>
      GetGenRsrcRequests();

  /// @brief GridFacilities place accepted trade Products in their Inventory
  virtual void AcceptProductTrades(
      const std::vector< std::pair<cyclus::Trade<cyclus::Product>,
      cyclus::Product::Ptr> >& responses);

  /// @brief GridFacilities place accepted trade Products in their Inventory
  virtual void AcceptGenRsrcTrades(
      const std::vector< std::pair<cyclus::Trade<cyclus::Product>,
      cyclus::Product::Ptr> >& responses);


///Not including anything to do with the Inventory Size, only throughput matters

  ///  sets the size of the storage inventory for received Product
  ///  @param size the storage size
  /*
  inline void SetMaxInventorySize(double size) {
    max_inv_size = size;
    inventory.capacity(size);
  }

  /// @return the maximum inventory storage size
  inline double MaxInventorySize() const { return inventory.capacity(); }

  /// @return the current inventory storage size
  inline double InventorySize() const { return inventory.quantity(); }
*/

  /// determines the amount to request
  inline double RequestAmt() const {
    return capacity;
  }

  /// sets the capacity of a Product generated at any given time step
  /// @param capacity the reception capacity
  inline void Capacity(double cap) { capacity = cap; }

  /// @return the reception capacity at any given time step
  inline double Capacity() const { return capacity; }

  /// @return the input commodities
  inline const std::vector<std::string>&
      input_commodities() const { return in_commods; }

  /// @return the input commodities preferences
  inline const std::vector<double>&
      input_commodity_preferences() const { return in_commod_prefs; }

 private:
  /// all facilities must have at least one input commodity
  #pragma cyclus var {"tooltip": "input commodities",                       "doc": "commodities that the grid facility accepts",                       "uilabel": "List of Input Commodities",                       "uitype": ["oneormore", "incommodity"]}
#line 113 "/home/cyc-user/cyclus/hybrid/src/grid.h"
  std::vector<std::string> in_commods;
  std::vector<int> cycpp_shape_in_commods;
#line 114 "/home/cyc-user/cyclus/hybrid/src/grid.h"


  #pragma cyclus var {"default": [],                      "doc":"preferences for each of the given commodities, in the same order."                      "Defauts to 1 if unspecified",                      "uilabel":"In Commody Preferences",                       "range": [None, [1e-299, 1e299]],                       "uitype":["oneormore", "range"]}
#line 121 "/home/cyc-user/cyclus/hybrid/src/grid.h"
  std::vector<double> in_commod_prefs;
  std::vector<int> cycpp_shape_in_commod_prefs;
#line 122 "/home/cyc-user/cyclus/hybrid/src/grid.h"


/*
  #pragma cyclus var {"default": "",                       "tooltip": "requested composition",                       "doc": "name of recipe to use for Product requests, "                              "where the default (empty string) is to accept "                              "everything",                       "uilabel": "Input Recipe",                       "uitype": "inrecipe"}
#line 130 "/home/cyc-user/cyclus/hybrid/src/grid.h"
  std::string recipe_name;
  std::vector<int> cycpp_shape_recipe_name;


#line 131 "/home/cyc-user/cyclus/hybrid/src/grid.h"

  /// max inventory size
  #pragma cyclus var {"default": 1e299,                       "tooltip": "grid maximum inventory size",                       "uilabel": "Maximum Inventory",                       "uitype": "range",                       "range": [0.0, 1e299],                       "doc": "total maximum inventory size of grid facility"}
#line 139 "/home/cyc-user/cyclus/hybrid/src/grid.h"
  double max_inv_size;
  std::vector<int> cycpp_shape_max_inv_size;

#line 140 "/home/cyc-user/cyclus/hybrid/src/grid.h"
*/
  /// timestep acceptance capacity
  #pragma cyclus var {"default": 1e299,                       "tooltip": "grid capacity",                       "uilabel": "Maximum Throughput",                       "uitype": "range",                       "range": [0.0, 1e299],                       "doc": "capacity the grid facility can "                              "accept at each time step"}
#line 149 "/home/cyc-user/cyclus/hybrid/src/grid.h"
  double capacity;
  std::vector<int> cycpp_shape_capacity;

#line 150 "/home/cyc-user/cyclus/hybrid/src/grid.h"

  /// this facility holds Product in storage.
  #pragma cyclus var {'capacity': 'max_inv_size'}
  cyclus::toolkit::ResBuf<cyclus::Resource> inventory;
  std::vector<int> cycpp_shape_inventory;

#line 154 "/home/cyc-user/cyclus/hybrid/src/grid.h"
};

}  // namespace hybrid

#endif  // CYCAMORE_SRC_SINK_H_
