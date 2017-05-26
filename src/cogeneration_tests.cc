#include <gtest/gtest.h>

#include "cogeneration.h"

#include "agent_tests.h"
#include "context.h"
#include "facility_tests.h"
#include "pyhooks.h"

using hybrid::Cogeneration;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class CogenerationTest : public ::testing::Test {
 protected:
  cyclus::TestContext tc;
  Cogeneration* facility;

  virtual void SetUp() {
    cyclus::PyStart();
    facility = new Cogeneration(tc.get());
  }

  virtual void TearDown() {
    delete facility;
    cyclus::PyStop();
  }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TEST_F(CogenerationTest, InitialState) {
  // Test things about the initial state of the facility here
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TEST_F(CogenerationTest, Print) {
  EXPECT_NO_THROW(std::string s = facility->str());
  // Test Cogeneration specific aspects of the print method here
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TEST_F(CogenerationTest, Tick) {
  ASSERT_NO_THROW(facility->Tick());
  // Test Cogeneration specific behaviors of the Tick function here
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TEST_F(CogenerationTest, Tock) {
  EXPECT_NO_THROW(facility->Tock());
  // Test Cogeneration specific behaviors of the Tock function here
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Do Not Touch! Below section required for connection with Cyclus
cyclus::Agent* CogenerationConstructor(cyclus::Context* ctx) {
  return new Cogeneration(ctx);
}
// Required to get functionality in cyclus agent unit tests library
#ifndef CYCLUS_AGENT_TESTS_CONNECTED
int ConnectAgentTests();
static int cyclus_agent_tests_connected = ConnectAgentTests();
#define CYCLUS_AGENT_TESTS_CONNECTED cyclus_agent_tests_connected
#endif  // CYCLUS_AGENT_TESTS_CONNECTED
INSTANTIATE_TEST_CASE_P(Cogeneration, FacilityTests,
                        ::testing::Values(&CogenerationConstructor));
INSTANTIATE_TEST_CASE_P(Cogeneration, AgentTests,
                        ::testing::Values(&CogenerationConstructor));
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
