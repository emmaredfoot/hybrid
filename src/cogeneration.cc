#include "cogeneration.h"

#include <sstream>
#include <limits>

#include <boost/lexical_cast.hpp>

namespace hybrid {

Cogeneration::Cogeneration(cyclus::Context* ctx)
    : cyclus::Facility(ctx),
      throughput(std::numeric_limits<double>::max()),
      inventory_size(std::numeric_limits<double>::max()) {}

Cogeneration::~Cogeneration() {}

void Cogeneration::InitFrom(Cogeneration* m) {
  #pragma cyclus impl initfromcopy hybrid::Cogeneration
  cyclus::toolkit::CommodityProducer::Copy(m);
}

void Cogeneration::InitFrom(cyclus::QueryableBackend* b) {
  #pragma cyclus impl initfromdb hybrid::Cogeneration
  namespace tk = cyclus::toolkit;
  tk::CommodityProducer::Add(tk::Commodity(outcommod),
                             tk::CommodInfo(throughput, throughput));
}

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
  ss << cyclus::Facility::str() << " supplies commodity '" << outcommod
     << "' with recipe '" << outrecipe << "' at a throughput of "
     << throughput << " kg per time step "
     << " commod producer members: "
     << " produces " << outcommod << "?: " << ans
     << " throughput: " << cyclus::toolkit::CommodityProducer::Capacity(outcommod)
     << " cost: " << cyclus::toolkit::CommodityProducer::Cost(outcommod);
  return ss.str();
}

std::set<cyclus::BidPortfolio<cyclus::Material>::Ptr> Cogeneration::GetMatlBids(
    cyclus::CommodMap<cyclus::Material>::type& commod_requests) {
  using cyclus::Bid;
  using cyclus::BidPortfolio;
  using cyclus::CapacityConstraint;
  using cyclus::Material;
  using cyclus::Request;

  double max_qty = std::min(throughput, inventory_size);
  LOG(cyclus::LEV_INFO3, "Cogeneration") << prototype() << " is bidding up to "
                                   << max_qty << " kg of " << outcommod;
  LOG(cyclus::LEV_INFO5, "Cogeneration") << "stats: " << str();

  std::set<BidPortfolio<Material>::Ptr> ports;
  if (max_qty < cyclus::eps()) {
    return ports;
  } else if (commod_requests.count(outcommod) == 0) {
    return ports;
  }

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

  CapacityConstraint<Material> cc(max_qty);
  port->AddConstraint(cc);
  ports.insert(port);
  return ports;
}

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
    } else {
      response = Material::Create(this, qty, it->request->target()->comp());
    }
    responses.push_back(std::make_pair(*it, response));
    LOG(cyclus::LEV_INFO5, "Cogeneration") << prototype() << " sent an order"
                                     << " for " << qty << " of " << outcommod;
  }
}

extern "C" cyclus::Agent* ConstructCogeneration(cyclus::Context* ctx) {
  return new Cogeneration(ctx);
}

}  // namespace hybrid
