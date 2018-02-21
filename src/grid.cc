
#line 1 "/home/cyc-user/cyclus/hybrid/src/grid.cc"
// Implements the Grid class
#include <algorithm>
#include <sstream>

#include <boost/lexical_cast.hpp>

#include "grid.h"

namespace hybrid {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Grid::Grid(cyclus::Context* ctx)
    : cyclus::Facility(ctx),
      capacity(std::numeric_limits<double>::max()) {
  SetMaxInventorySize(std::numeric_limits<double>::max());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Grid::~Grid() {}
std::string Grid::schema() {
  return ""
    "<interleave>\n"
    "    <element name=\"in_commods\">\n"
    "        <oneOrMore>\n"
    "            <element name=\"val\">\n"
    "                <data type=\"string\"/>\n"
    "            </element>\n"
    "        </oneOrMore>\n"
    "    </element>\n"
    "    <optional>\n"
    "        <element name=\"in_commod_prefs\">\n"
    "            <oneOrMore>\n"
    "                <element name=\"val\">\n"
    "                    <data type=\"double\"/>\n"
    "                </element>\n"
    "            </oneOrMore>\n"
    "        </element>\n"
    "    </optional>\n"
    "    <optional>\n"
    "        <element name=\"recipe_name\">\n"
    "            <data type=\"string\"/>\n"
    "        </element>\n"
    "    </optional>\n"
    "    <optional>\n"
    "        <element name=\"max_inv_size\">\n"
    "            <data type=\"double\"/>\n"
    "        </element>\n"
    "    </optional>\n"
    "    <optional>\n"
    "        <element name=\"capacity\">\n"
    "            <data type=\"double\"/>\n"
    "        </element>\n"
    "    </optional>\n"
    "</interleave>\n";
};
#line 20 "/home/cyc-user/cyclus/hybrid/src/grid.cc"

Json::Value Grid::annotations() {
  Json::Value root;
  Json::Reader reader;
  bool parsed_ok = reader.parse(
    "{\"name\":\"hybrid::Grid\",\"entity\":\"facility\",\"pare"
    "nts\":[\"cyclus::Facility\"],\"all_parents\":[\"cyclus::"
    "Agent\",\"cyclus::Facility\",\"cyclus::Ider\",\"cyclus::"
    "StateWrangler\",\"cyclus::TimeListener\",\"cyclus::Tra"
    "der\"],\"vars\":{\"in_commods\":{\"uitype\":[\"oneormore\","
    "\"incommodity\"],\"index\":0,\"doc\":\"commodities that "
    "the grid facility accepts\",\"tooltip\":[\"input "
    "commodities\",\"\"],\"uilabel\":[\"List of Input Commodi"
    "ties\",\"\"],\"alias\":[\"in_commods\",\"val\"],\"shape\":[-1"
    ",-1],\"type\":[\"std::vector\",\"std::string\"]},\"in_com"
    "mod_prefs\":{\"uitype\":[\"oneormore\",\"range\"],\"index\""
    ":1,\"default\":[],\"doc\":\"preferences for each of the"
    " given commodities, in the same order.Defauts to 1"
    " if unspecified\",\"shape\":[-1,-1],\"tooltip\":[\"in_co"
    "mmod_prefs\",\"\"],\"uilabel\":[\"In Commody Preferences"
    "\",\"\"],\"alias\":[\"in_commod_prefs\",\"val\"],\"range\":[n"
    "ull,[1e-299,1e+299]],\"type\":[\"std::vector\",\"double"
    "\"]},\"recipe_name\":{\"uitype\":\"inrecipe\",\"index\":2,\""
    "default\":\"\",\"doc\":\"name of recipe to use for "
    "Product requests, where the default (empty "
    "string) is to accept "
    "everything\",\"tooltip\":\"requested "
    "composition\",\"uilabel\":\"Input Recipe\",\"alias\":\"rec"
    "ipe_name\",\"shape\":[-1],\"type\":\"std::string\"},\"max_"
    "inv_size\":{\"uitype\":\"range\",\"index\":3,\"default\":1e"
    "+299,\"doc\":\"total maximum inventory size of grid "
    "facility\",\"shape\":[-1],\"tooltip\":\"grid maximum "
    "inventory size\",\"uilabel\":\"Maximum Inventory\",\"ali"
    "as\":\"max_inv_size\",\"range\":[0.0,1e+299],\"type\":\"do"
    "uble\"},\"capacity\":{\"uitype\":\"range\",\"index\":4,\"def"
    "ault\":1e+299,\"doc\":\"capacity the grid facility can"
    " accept at each time "
    "step\",\"shape\":[-1],\"tooltip\":\"grid "
    "capacity\",\"uilabel\":\"Maximum Throughput\",\"alias\":\""
    "capacity\",\"range\":[0.0,1e+299],\"type\":\"double\"},\"i"
    "nventory\":{\"type\":[\"cyclus::toolkit::ResBuf\",\"cycl"
    "us::Resource\"],\"shape\":[-1,-1],\"capacity\":\"max_inv"
    "_size\",\"index\":5}},\"doc\":\" A grid facility that "
    "accepts products with a fixed\\n "
    "throughput (per time step) capacity and a lifetime"
    " capacity defined by\\n a total inventory size. The"
    " inventory size and throughput capacity\\n both "
    "default to infinite. If a recipe is provided, it "
    "will request\\n Product with that recipe. Requests"
    " are made for any number of\\n specified "
    "commodities.\\n\"}", root);
  if (!parsed_ok) {
    throw cyclus::ValueError("failed to parse annotations for hybrid::Grid.");
  }
  return root;
};
#line 22 "/home/cyc-user/cyclus/hybrid/src/grid.cc"

void Grid::InfileToDb(cyclus::InfileTree* tree, cyclus::DbInit di) {
  cyclus::Facility::InfileToDb(tree, di);
  int rawcycpp_shape_in_commods[2] = {-1, -1};
  cycpp_shape_in_commods = std::vector<int>(rawcycpp_shape_in_commods, rawcycpp_shape_in_commods + 2);
  int rawcycpp_shape_in_commod_prefs[2] = {-1, -1};
  cycpp_shape_in_commod_prefs = std::vector<int>(rawcycpp_shape_in_commod_prefs, rawcycpp_shape_in_commod_prefs + 2);
  int rawcycpp_shape_recipe_name[1] = {-1};
  cycpp_shape_recipe_name = std::vector<int>(rawcycpp_shape_recipe_name, rawcycpp_shape_recipe_name + 1);
  int rawcycpp_shape_max_inv_size[1] = {-1};
  cycpp_shape_max_inv_size = std::vector<int>(rawcycpp_shape_max_inv_size, rawcycpp_shape_max_inv_size + 1);
  int rawcycpp_shape_capacity[1] = {-1};
  cycpp_shape_capacity = std::vector<int>(rawcycpp_shape_capacity, rawcycpp_shape_capacity + 1);
  int rawcycpp_shape_inventory[2] = {-1, -1};
  cycpp_shape_inventory = std::vector<int>(rawcycpp_shape_inventory, rawcycpp_shape_inventory + 2);
  cyclus::InfileTree* sub = tree->SubTree("config/*");
  int i;
  int n;
  {
    cyclus::InfileTree* bub = sub->SubTree("in_commods", 0);
    cyclus::InfileTree* sub = bub;
    int n1 = sub->NMatches("val");
    std::vector< std::string > in_commods_val;
    in_commods_val.resize(n1);
    for (int i1 = 0; i1 < n1; ++i1) {
      std::string elem;
      {
        std::string elem_in = cyclus::Query<std::string>(sub, "val", i1);
        elem = elem_in;
      }
      in_commods_val[i1] = elem;
    }
    in_commods = in_commods_val;
  }
  if (sub->NMatches("in_commod_prefs") > 0) {
    {
      cyclus::InfileTree* bub = sub->SubTree("in_commod_prefs", 0);
      cyclus::InfileTree* sub = bub;
      int n1 = sub->NMatches("val");
      std::vector< double > in_commod_prefs_val;
      in_commod_prefs_val.resize(n1);
      for (int i1 = 0; i1 < n1; ++i1) {
        double elem;
        {
          double elem_in = cyclus::Query<double>(sub, "val", i1);
          elem = elem_in;
        }
        in_commod_prefs_val[i1] = elem;
      }
      in_commod_prefs = in_commod_prefs_val;
    }
  } else {
    std::vector< double > in_commod_prefs_tmp;
    in_commod_prefs_tmp.resize(0);
    {
    }
    in_commod_prefs = in_commod_prefs_tmp;
  }
  if (sub->NMatches("recipe_name") > 0) {
    {
      std::string recipe_name_val = cyclus::Query<std::string>(sub, "recipe_name");
      recipe_name = recipe_name_val;
    }
  } else {
    std::string recipe_name_tmp("");
    recipe_name = recipe_name_tmp;
  }
  if (sub->NMatches("max_inv_size") > 0) {
    {
      double max_inv_size_val = cyclus::Query<double>(sub, "max_inv_size");
      max_inv_size = max_inv_size_val;
    }
  } else {
    double max_inv_size_tmp = 1e+299;
    max_inv_size = max_inv_size_tmp;
  }
  if (sub->NMatches("capacity") > 0) {
    {
      double capacity_val = cyclus::Query<double>(sub, "capacity");
      capacity = capacity_val;
    }
  } else {
    double capacity_tmp = 1e+299;
    capacity = capacity_tmp;
  }
  di.NewDatum("Info")
  ->AddVal("in_commods", in_commods, &cycpp_shape_in_commods)
  ->AddVal("in_commod_prefs", in_commod_prefs, &cycpp_shape_in_commod_prefs)
  ->AddVal("recipe_name", recipe_name, &cycpp_shape_recipe_name)
  ->AddVal("max_inv_size", max_inv_size, &cycpp_shape_max_inv_size)
  ->AddVal("capacity", capacity, &cycpp_shape_capacity)
  ->Record();
};
#line 24 "/home/cyc-user/cyclus/hybrid/src/grid.cc"

void Grid::Snapshot(cyclus::DbInit di) {
  di.NewDatum("Info")
  ->AddVal("in_commods", in_commods, &cycpp_shape_in_commods)
  ->AddVal("in_commod_prefs", in_commod_prefs, &cycpp_shape_in_commod_prefs)
  ->AddVal("recipe_name", recipe_name, &cycpp_shape_recipe_name)
  ->AddVal("max_inv_size", max_inv_size, &cycpp_shape_max_inv_size)
  ->AddVal("capacity", capacity, &cycpp_shape_capacity)
  ->Record();
};
#line 26 "/home/cyc-user/cyclus/hybrid/src/grid.cc"

cyclus::Inventories Grid::SnapshotInv() {
  cyclus::Inventories invs;
  invs["inventory"] = inventory.PopNRes(inventory.count());
  inventory.Push(invs["inventory"]);
  return invs;
};
#line 28 "/home/cyc-user/cyclus/hybrid/src/grid.cc"

void Grid::InitInv(cyclus::Inventories& inv) {
  inventory.Push(inv["inventory"]);

};
#line 30 "/home/cyc-user/cyclus/hybrid/src/grid.cc"

cyclus::Agent* Grid::Clone() {
  hybrid::Grid* m = new hybrid::Grid(context());
  m->InitFrom(this);
  return m;
};
#line 32 "/home/cyc-user/cyclus/hybrid/src/grid.cc"

void Grid::InitFrom(cyclus::QueryableBackend* b) {
  cyclus::Facility::InitFrom(b);
  int rawcycpp_shape_in_commods[2] = {-1, -1};
  cycpp_shape_in_commods = std::vector<int>(rawcycpp_shape_in_commods, rawcycpp_shape_in_commods + 2);
  int rawcycpp_shape_in_commod_prefs[2] = {-1, -1};
  cycpp_shape_in_commod_prefs = std::vector<int>(rawcycpp_shape_in_commod_prefs, rawcycpp_shape_in_commod_prefs + 2);
  int rawcycpp_shape_recipe_name[1] = {-1};
  cycpp_shape_recipe_name = std::vector<int>(rawcycpp_shape_recipe_name, rawcycpp_shape_recipe_name + 1);
  int rawcycpp_shape_max_inv_size[1] = {-1};
  cycpp_shape_max_inv_size = std::vector<int>(rawcycpp_shape_max_inv_size, rawcycpp_shape_max_inv_size + 1);
  int rawcycpp_shape_capacity[1] = {-1};
  cycpp_shape_capacity = std::vector<int>(rawcycpp_shape_capacity, rawcycpp_shape_capacity + 1);
  int rawcycpp_shape_inventory[2] = {-1, -1};
  cycpp_shape_inventory = std::vector<int>(rawcycpp_shape_inventory, rawcycpp_shape_inventory + 2);
  cyclus::QueryResult qr = b->Query("Info", NULL);
  in_commods = qr.GetVal<std::vector< std::string > >("in_commods");
  in_commod_prefs = qr.GetVal<std::vector< double > >("in_commod_prefs");
  recipe_name = qr.GetVal<std::string>("recipe_name");
  max_inv_size = qr.GetVal<double>("max_inv_size");
  capacity = qr.GetVal<double>("capacity");
  inventory.capacity(max_inv_size);
};
#line 34 "/home/cyc-user/cyclus/hybrid/src/grid.cc"

void Grid::InitFrom(hybrid::Grid* m) {
  cyclus::Facility::InitFrom(m);
  int rawcycpp_shape_in_commods[2] = {-1, -1};
  cycpp_shape_in_commods = std::vector<int>(rawcycpp_shape_in_commods, rawcycpp_shape_in_commods + 2);
  int rawcycpp_shape_in_commod_prefs[2] = {-1, -1};
  cycpp_shape_in_commod_prefs = std::vector<int>(rawcycpp_shape_in_commod_prefs, rawcycpp_shape_in_commod_prefs + 2);
  int rawcycpp_shape_recipe_name[1] = {-1};
  cycpp_shape_recipe_name = std::vector<int>(rawcycpp_shape_recipe_name, rawcycpp_shape_recipe_name + 1);
  int rawcycpp_shape_max_inv_size[1] = {-1};
  cycpp_shape_max_inv_size = std::vector<int>(rawcycpp_shape_max_inv_size, rawcycpp_shape_max_inv_size + 1);
  int rawcycpp_shape_capacity[1] = {-1};
  cycpp_shape_capacity = std::vector<int>(rawcycpp_shape_capacity, rawcycpp_shape_capacity + 1);
  int rawcycpp_shape_inventory[2] = {-1, -1};
  cycpp_shape_inventory = std::vector<int>(rawcycpp_shape_inventory, rawcycpp_shape_inventory + 2);
  in_commods = m->in_commods;
  in_commod_prefs = m->in_commod_prefs;
  recipe_name = m->recipe_name;
  max_inv_size = m->max_inv_size;
  capacity = m->capacity;
  inventory.capacity(m->inventory.capacity());
};
#line 38 "/home/cyc-user/cyclus/hybrid/src/grid.cc"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Grid::EnterNotify() {
  cyclus::Facility::EnterNotify();

  if (in_commod_prefs.size() == 0) {
    for (int i = 0; i < in_commods.size(); ++i) {
      in_commod_prefs.push_back(cyclus::kDefaultPref);
    }
  } else if (in_commod_prefs.size() != in_commods.size()) {
    std::stringstream ss;
    ss << "in_commod_prefs has " << in_commod_prefs.size()
       << " values, expected " << in_commods.size();
    throw cyclus::ValueError(ss.str());
  }

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string Grid::str() {
  using std::string;
  using std::vector;
  std::stringstream ss;
  ss << cyclus::Facility::str();

  string msg = "";
  msg += "accepts commodities ";
  for (vector<string>::iterator commod = in_commods.begin();
       commod != in_commods.end();
       commod++) {
    msg += (commod == in_commods.begin() ? "{" : ", ");
    msg += (*commod);
  }
  msg += "} until its inventory is full at ";
  ss << msg << inventory.capacity() << " kg.";
  return "" + ss.str();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::set<cyclus::RequestPortfolio<cyclus::Product>::Ptr>
Grid::GetProductRequests() {
  using cyclus::Product;
  using cyclus::RequestPortfolio;
  using cyclus::Request;

  std::set<RequestPortfolio<Product>::Ptr> ports;
  RequestPortfolio<Product>::Ptr port(new RequestPortfolio<Product>());
  double amt = RequestAmt();
  Product::Ptr mat;


  if (amt > cyclus::eps()) {
    std::vector<Request<Product>*> mutuals;
    for (int i = 0; i < in_commods.size(); i++) {
      mutuals.push_back(port->AddRequest(mat, this, in_commods[i], in_commod_prefs[i]));
    }
    port->AddMutualReqs(mutuals);
    ports.insert(port);
  }  // if amt > eps

  return ports;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::set<cyclus::RequestPortfolio<cyclus::Product>::Ptr>
Grid::GetGenRsrcRequests() {
  using cyclus::CapacityConstraint;
  using cyclus::Product;
  using cyclus::RequestPortfolio;
  using cyclus::Request;

  std::set<RequestPortfolio<Product>::Ptr> ports;
  RequestPortfolio<Product>::Ptr
      port(new RequestPortfolio<Product>());
  double amt = RequestAmt();

  if (amt > cyclus::eps()) {
    CapacityConstraint<Product> cc(amt);
    port->AddConstraint(cc);

    std::vector<std::string>::const_iterator it;
    for (it = in_commods.begin(); it != in_commods.end(); ++it) {
      std::string quality = "";  // not clear what this should be..
      Product::Ptr rsrc = Product::CreateUntracked(amt, quality);
      port->AddRequest(rsrc, this, *it);
    }

    ports.insert(port);
  }  // if amt > eps

  return ports;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Grid::AcceptProductTrades(
    const std::vector< std::pair<cyclus::Trade<cyclus::Product>,
                                 cyclus::Product::Ptr> >& responses) {
  std::vector< std::pair<cyclus::Trade<cyclus::Product>,
                         cyclus::Product::Ptr> >::const_iterator it;
  for (it = responses.begin(); it != responses.end(); ++it) {
    inventory.Push(it->second);
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Grid::AcceptGenRsrcTrades(
    const std::vector< std::pair<cyclus::Trade<cyclus::Product>,
                                 cyclus::Product::Ptr> >& responses) {
  std::vector< std::pair<cyclus::Trade<cyclus::Product>,
                         cyclus::Product::Ptr> >::const_iterator it;
  for (it = responses.begin(); it != responses.end(); ++it) {
    inventory.Push(it->second);
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Grid::Tick() {
  using std::string;
  using std::vector;
  LOG(cyclus::LEV_INFO3, "SnkFac") << prototype() << " is ticking {";

  double requestAmt = RequestAmt();
  // inform the simulation about what the grid facility will be requesting
  if (requestAmt > cyclus::eps()) {
    for (vector<string>::iterator commod = in_commods.begin();
         commod != in_commods.end();
         commod++) {
      LOG(cyclus::LEV_INFO4, "SnkFac") << " will request " << requestAmt
                                       << " kg of " << *commod << ".";
    }
  }
  LOG(cyclus::LEV_INFO3, "SnkFac") << "}";
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Grid::Tock() {
  LOG(cyclus::LEV_INFO3, "SnkFac") << prototype() << " is tocking {";

  // On the tock, the grid facility doesn't really do much.
  // Maybe someday it will record things.
  // For now, lets just print out what we have at each timestep.
  LOG(cyclus::LEV_INFO4, "SnkFac") << "Grid " << this->id()
                                   << " is holding " << inventory.quantity()
                                   << " units of Product at the close of month "
                                   << context()->time() << ".";
  LOG(cyclus::LEV_INFO3, "SnkFac") << "}";
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
extern "C" cyclus::Agent* ConstructGrid(cyclus::Context* ctx) {
  return new Grid(ctx);
}

}  // namespace hybrid
