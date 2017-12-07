#include "cogeneration.h"
#include <sstream>
#include <limits>
#include <boost/lexical_cast.hpp>

//namespace is like a package that encorporates all of the classes
namespace hybrid {

//Is this a defination of a new class?
//ctx is an instance (the timestep) that is passed throughout cyclus
//Not sure of what the syntax of the single and double semicolons represents. Means look in the namespace more or less
Cogeneration::Cogeneration(cyclus::Context* ctx) //Cogeneration inherits from Cogeneration with the instance of ctx. Context* seems to also be either an instance or a pointer. This is a definition of a new facility with the characteristics of reactor_size and inventory_size. I will probably need to change this to having multiple reactor_size values
    : cyclus::Facility(ctx),
    //Need to make two different reactor_size values for the cogeneration archetype.
    //To keep the ideas seperate, I am going to name it resource_allocated.
    //This portion of the code seems to just be creating the facility that has a
    //reactor_size. I want to first generate how much resource is made, then split that amount
      reactor_size(std::numeric_limits<double>::max()),
      inventory_size(std::numeric_limits<double>::max()),
      allocation_percent(std::numeric_limits<double>::max()) {}

//Cogeneration deconstructor is defined with no instances included
Cogeneration::~Cogeneration() {}

//Initialize the facility Cogeneration with no return value. Define it as having the same characteristics as producing a commodity.
void Cogeneration::InitFrom(Cogeneration* m) {
  //#pragma is used to access compiler-specific preprocessor extensions # represents something the system picks up before the processor
  #pragma cyclus impl initfromcopy hybrid::Cogeneration
  cyclus::toolkit::CommodityProducer::Copy(m);
}
//toolkit has a lot of tests without the actual commodity_producer.cc facility_tests

//Initialize a function without returning anything. This maybe the backend. I am not sure why there is an initialization from Cogeneration* and one from QueryableBackend*. InitFrom is a superclass. A superclass is the base class from which all other classes are derived.
void Cogeneration::InitFrom(cyclus::QueryableBackend* b) {
  #pragma cyclus impl initfromdb hybrid::Cogeneration
  //Bringing the classes from the toolkit and saving them to a variable in this library. Inheriting the commodity producer classes. The inputs are the outcommodity and the reactor_size as given by the xml file
  namespace tk = cyclus::toolkit;
  tk::CommodityProducer::Add(tk::Commodity(outcommod),
                             tk::CommodInfo(reactor_size, reactor_size));
}

//declaration of a new variable that is a string in the Cogeneration class. Test to see if there is a value in place for the outcommodity
std::string Cogeneration::str() {
  namespace tk = cyclus::toolkit;
  std::stringstream ss;
  std::string ans;
  if (cyclus::toolkit::CommodityProducer::Produces(
          cyclus::toolkit::Commodity(outcommod))) {
    ans = "yes";
  } else {
    ans = "no";
  }
  //<< means put the referenced variable in the string
  ss << cyclus::Facility::str() << " supplies commodity '" << outcommod
     << "' with recipe '" << outrecipe << "' at a reactor_size of "
     << reactor_size << " kg per time step "
     << " commod producer members: "
     << " produces " << outcommod << "?: " << ans
     << " reactor_size: " << cyclus::toolkit::CommodityProducer::Capacity(outcommod)
     << " cost: " << cyclus::toolkit::CommodityProducer::Cost(outcommod);
  return ss.str();
}

//std is a general purpose wrapper
std::set<cyclus::BidPortfolio<cyclus::Material>::Ptr> Cogeneration::GetMatlBids(
    cyclus::CommodMap<cyclus::Material>::type& commod_requests) {
//using makes the class visible
  using cyclus::Bid;
  using cyclus::BidPortfolio;
  using cyclus::CapacityConstraint;
  using cyclus::Material;
  using cyclus::Request;

//declaration of a new varioable with reactor_size and inventory_size as the inputs
  double max_qty = std::min(reactor_size, inventory_size);
  LOG(cyclus::LEV_INFO3, "Cogeneration") << prototype() << " is bidding up to "
                                   << max_qty << " kg of " << outcommod;
  LOG(cyclus::LEV_INFO5, "Cogeneration") << "stats: " << str();

//If the quantity being passed is too small to be taken into consideration it is ignored
  std::set<BidPortfolio<Material>::Ptr> ports;
  if (max_qty < cyclus::eps()) {
    return ports;
  } else if (commod_requests.count(outcommod) == 0) {
    return ports;
  }


//Point toward the amount that the sink is able to take, it is the iterator, iterate through the various bids
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
// access a member function or variable of an object through a pointer
    double qty = it->amt;
    inventory_size -= qty;

//The material is not particularly important in this case so this section is probably unnecessary
    Material::Ptr response;
    if (!outrecipe.empty()) {
      response = Material::Create(this, qty, context()->GetRecipe(outrecipe));
    } else {
      response = Material::Create(this, qty, it->request->target()->comp());
    }
    responses.push_back(std::make_pair(*it, response));
    LOG(cyclus::LEV_INFO5, "Cogeneration") << prototype() << " sent an order"
                                     << " for " << qty << " of " << outcommod;
  }
}
  //The compiler knows that there is some variable string C. Moves on to the next time step. I do not see where it iterates through the time step.
  extern "C" cyclus::Agent* ConstructCogeneration(cyclus::Context* ctx) {
    return new Cogeneration(ctx);
  }
}  // namespace hybrid
