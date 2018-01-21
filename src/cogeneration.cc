#include "cogeneration.h"
#include <sstream>
#include <limits>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
int x=-1;

//namespace is like a package that encorporates all of the classes
namespace hybrid {

//Is this a definition  of new type cogeneration
//ctx is an instance (the timestep) that is passed throughout
Cogeneration::Cogeneration(cyclus::Context* ctx)
    : cyclus::Facility(ctx),
      reactor_heat(std::numeric_limits<double>::max()),
      cycle_efficiency(std::numeric_limits<double>::max()) {}

//Cogeneration destructor
Cogeneration::~Cogeneration() {}

//Initialize the facility Cogeneration with no return value. Define it as
//having the same characteristics as producing a commodity.
void Cogeneration::InitFrom(Cogeneration* m) {
  //#pragma is used to access compiler-specific preprocessor extensions
  //# represents something the system picks up before the processor
  #pragma cyclus impl initfromcopy hybrid::Cogeneration
  cyclus::toolkit::CommodityProducer::Copy(m);
}

//I am not sure why there is an initialization from Cogeneration* and one
//from QueryableBackend*. InitFrom is a superclass.
//A superclass is the base class from which all other classes are derived.
void Cogeneration::InitFrom(cyclus::QueryableBackend* b) {
  #pragma cyclus impl initfromdb hybrid::Cogeneration
  //Bringing the classes from the toolkit and saving them to a variable in this
  //library. Inheriting the commodity producer classes. The inputs are the outcommodity
  //and the reactor_heat as given by the xml file
  namespace tk = cyclus::toolkit;
  tk::CommodityProducer::Add(tk::Commodity(outcommod),
                             tk::CommodInfo(reactor_heat, cycle_efficiency));
  //I am trying adding the CommodityProducer twice in order to have two outcommods
  tk::CommodityProducer::Add(tk::Commodity(outcommod),
                             tk::CommodInfo(reactor_heat, cycle_efficiency));
}

//declaration of a new variable that is a string in the Cogeneration class.
//Test to see if there is a value in place for the outcommodity
std::string Cogeneration::str() {
  namespace tk = cyclus::toolkit;
  std::stringstream ss;
  std::string ans;
  if (cyclus::toolkit::CommodityProducer::Produces(
          cyclus::toolkit::Commodity(outcommod))) {
    ans = "yes";
  }
  else {
    ans = "no";
  }

  ss << cyclus::Facility::str() << " supplies commodity '" << outcommod
     << "' with recipe '" << outrecipe << "' at a reactor_heat of "
     << reactor_heat << " MWth per time step "
     << " commod producer members: "
     << " produces " << outcommod << "?: " << ans
     << " reactor_heat: " << cyclus::toolkit::CommodityProducer::Capacity(outcommod)
     << " cost: " << cyclus::toolkit::CommodityProducer::Cost(outcommod);
  return ss.str();
}

std::vector<std::string> Cogeneration::offer_amt(){
  std::string sdemand;
  std::vector<std::string> demand;
  std::ifstream myfile("CAISO-demand.csv");

  if(!myfile.is_open()) std::cout << "Error File Open" << '\n';

  for(int i=0; i<100; i++){
    getline(myfile, sdemand, ',');

    //std::stringstream convert(sdemand);
    demand.push_back(sdemand);
    //std::cout << demand[i] << std::endl;
  }
  return demand;
}

double Cogeneration::get_offer_amt(){
  std::vector<std::string> caiso;
  double nuke_demand;
  x++;
  caiso = offer_amt();
  std::stringstream convert(caiso[x]);
  int y;
  convert >> y;
  nuke_demand = y*.056;
  return nuke_demand;
}



std::set<cyclus::BidPortfolio<cyclus::Material>::Ptr> Cogeneration::GetMatlBids(
    cyclus::CommodMap<cyclus::Material>::type& commod_requests) {

  using cyclus::Bid;
  using cyclus::BidPortfolio;
  using cyclus::CapacityConstraint;
  using cyclus::Material;
  using cyclus::Request;

  LOG(cyclus::LEV_INFO3, "Cogeneration") << prototype() << " is bidding up "
                                   << reactor_heat*cycle_efficiency << " MWe of " << outcommod;
  LOG(cyclus::LEV_INFO5, "Cogeneration") << "stats: " << str();

//If the quantity being passed is too small to be taken into consideration it is ignored
  double max_qty = reactor_heat;
  std::set<BidPortfolio<Material>::Ptr> ports;
  if (max_qty < cyclus::eps()) {
    return ports;
  }
  if (commod_requests.count(outcommod) == 0) {
    return ports;
  }

//Point toward the amount that the sink is able to take, it is the iterator,
//iterate through the various bids
  BidPortfolio<Material>::Ptr port(new BidPortfolio<Material>());
  std::vector<Request<Material>*>& requests = commod_requests[outcommod];
  std::vector<Request<Material>*>::iterator it;
  for (it = requests.begin(); it != requests.end(); ++it) {
    Request<Material>* req = *it;
    Material::Ptr target = req->target();
    double qty = std::min(target->quantity(), max_qty);
    Material::Ptr m = Material::CreateUntracked(qty, target->comp());
    if (!outrecipe.empty()) {
      m = Material::CreateUntracked(qty, context()->GetRecipe(outrecipe));
    }
    port->AddBid(req, m, this);
  }

//Constrain by the maximum the sink is able to take at any given timestep
  CapacityConstraint<Material> cc(max_qty);
  port->AddConstraint(cc);
  ports.insert(port);
  return ports;
}

//This might be where it declares that there is a constant amount being traded at each time step
void Cogeneration::GetMatlTrades(
    const std::vector<cyclus::Trade<cyclus::Material> >& trades,
    std::vector<std::pair<cyclus::Trade<cyclus::Material>,
                          cyclus::Material::Ptr> >& responses) {
  using cyclus::Material;
  using cyclus::Trade;
  std::vector<cyclus::Trade<cyclus::Material> >::const_iterator it;
  for (it = trades.begin(); it != trades.end(); ++it) {
    double qty = it->amt;
    inventory_size -= qty;
    Material::Ptr response;
    if (!outrecipe.empty()) {
      response = Material::Create(this, qty, context()->GetRecipe(outrecipe));
    }
    else {
      response = Material::Create(this, qty, it->request->target()->comp());
    }
    responses.push_back(std::make_pair(*it, response));
    LOG(cyclus::LEV_INFO5, "Cogeneration") << prototype() << " sent an order"
                                     << " for " << qty << " of " << outcommod;
  }
}
  //The compiler knows that there is some variable string C. Moves on to the
  //next time step. I do not see where it iterates through the time step.
  extern "C" cyclus::Agent* ConstructCogeneration(cyclus::Context* ctx) {
    return new Cogeneration(ctx);
  }
} // namespace hybrid
