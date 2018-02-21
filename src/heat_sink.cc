// Implements the heat_sink class
#include <algorithm>
#include <sstream>

#include <boost/lexical_cast.hpp>

#include "heat_sink.h"

namespace hybrid {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
heat_sink::heat_sink(cyclus::Context* ctx)
    : cyclus::Facility(ctx),
      capacity(std::numeric_limits<double>::max()) {
  SetMaxInventorySize(std::numeric_limits<double>::max());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
heat_sink::~heat_sink() {}

#pragma cyclus def schema hybrid::heat_sink

#pragma cyclus def annotations hybrid::heat_sink

#pragma cyclus def infiletodb hybrid::heat_sink

#pragma cyclus def snapshot hybrid::heat_sink

#pragma cyclus def snapshotinv hybrid::heat_sink

#pragma cyclus def initinv hybrid::heat_sink

#pragma cyclus def clone hybrid::heat_sink

#pragma cyclus def initfromdb hybrid::heat_sink

#pragma cyclus def initfromcopy hybrid::heat_sink

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void heat_sink::EnterNotify() {
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
std::string heat_sink::str() {
  using std::string;
  using std::vector;
  std::stringstream ss;
  ss << cyclus::Facility::str();

  string msg = "";
  msg += "accepts heat commodity ";
  /*
  for (vector<string>::iterator commod = in_commods.begin();
       commod != in_commods.end();
       commod++) {
    msg += (commod == in_commods.begin() ? "{" : ", ");
    msg += (*commod);
  }
  */
  msg += "until its inventory is full at ";
  ss << msg << inventory.capacity() << " MWth.";
  return "" + ss.str();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::set<cyclus::RequestPortfolio<cyclus::Material>::Ptr>
heat_sink::GetMatlRequests() {
  using cyclus::Material;
  using cyclus::RequestPortfolio;
  using cyclus::Request;
  using cyclus::Composition;

  std::set<RequestPortfolio<Material>::Ptr> ports;
  RequestPortfolio<Material>::Ptr port(new RequestPortfolio<Material>());
  double amt = RequestAmt();
  Material::Ptr mat;

  if (recipe_name.empty()) {
    mat = cyclus::NewBlankMaterial(amt);
  } else {
    Composition::Ptr rec = this->context()->GetRecipe(recipe_name);
    mat = cyclus::Material::CreateUntracked(amt, rec);
  }

  if (amt > cyclus::eps()) {
    std::vector<Request<Material>*> mutuals;
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
heat_sink::GetGenRsrcRequests() {
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
void heat_sink::AcceptMatlTrades(
    const std::vector< std::pair<cyclus::Trade<cyclus::Material>,
                                 cyclus::Material::Ptr> >& responses) {
  std::vector< std::pair<cyclus::Trade<cyclus::Material>,
                         cyclus::Material::Ptr> >::const_iterator it;
  for (it = responses.begin(); it != responses.end(); ++it) {
    inventory.Push(it->second);
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void heat_sink::AcceptGenRsrcTrades(
    const std::vector< std::pair<cyclus::Trade<cyclus::Product>,
                                 cyclus::Product::Ptr> >& responses) {
  std::vector< std::pair<cyclus::Trade<cyclus::Product>,
                         cyclus::Product::Ptr> >::const_iterator it;
  for (it = responses.begin(); it != responses.end(); ++it) {
    inventory.Push(it->second);
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void heat_sink::Tick() {
  using std::string;
  using std::vector;
  LOG(cyclus::LEV_INFO3, "SnkFac") << prototype() << " is ticking {";

  double requestAmt = RequestAmt();
  // inform the simulation about what the heat_sink facility will be requesting
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
void heat_sink::Tock() {
  LOG(cyclus::LEV_INFO3, "SnkFac") << prototype() << " is tocking {";

  // On the tock, the heat_sink facility doesn't really do much.
  // Maybe someday it will record things.
  // For now, lets just print out what we have at each timestep.
  LOG(cyclus::LEV_INFO4, "SnkFac") << "heat_sink " << this->id()
                                   << " is holding " << inventory.quantity()
                                   << " units of material at the close of month "
                                   << context()->time() << ".";
  LOG(cyclus::LEV_INFO3, "SnkFac") << "}";
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
extern "C" cyclus::Agent* Constructheat_sink(cyclus::Context* ctx) {
  return new heat_sink(ctx);
};


}  // namespace hybrid
