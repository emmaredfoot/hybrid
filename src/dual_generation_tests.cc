#include <gtest/gtest.h>

#include "dual_generation.h"

#include "agent_tests.h"
#include "context.h"
#include "facility_tests.h"
#include "pyhooks.h"

using hybrid::DualGeneration;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class DualGenerationTest : public ::testing::Test {
 protected:
  cyclus::TestContext tc;
  DualGeneration* facility;

  virtual void SetUp() {
    cyclus::PyStart();
    facility = new DualGeneration(tc.get());
  }

  virtual void TearDown() {
    delete facility;
    cyclus::PyStop();
  }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TEST_F(DualGenerationTest, InitialState) {
  // Test things about the initial state of the facility here
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TEST_F(DualGenerationTest, Print) {
  EXPECT_NO_THROW(std::string s = facility->str());
  // Test DualGeneration specific aspects of the print method here
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TEST_F(DualGenerationTest, Tick) {
  ASSERT_NO_THROW(facility->Tick());
  // Test DualGeneration specific behaviors of the Tick function here
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TEST_F(DualGenerationTest, Tock) {
  EXPECT_NO_THROW(facility->Tock());
  // Test DualGeneration specific behaviors of the Tock function here
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Do Not Touch! Below section required for connection with Cyclus
cyclus::Agent* DualGenerationConstructor(cyclus::Context* ctx) {
  return new DualGeneration(ctx);
}
// Required to get functionality in cyclus agent unit tests library
#ifndef CYCLUS_AGENT_TESTS_CONNECTED
int ConnectAgentTests();
static int cyclus_agent_tests_connected = ConnectAgentTests();
#define CYCLUS_AGENT_TESTS_CONNECTED cyclus_agent_tests_connected
#endif  // CYCLUS_AGENT_TESTS_CONNECTED
INSTANTIATE_TEST_CASE_P(DualGeneration, FacilityTests,
                        ::testing::Values(&DualGenerationConstructor));
INSTANTIATE_TEST_CASE_P(DualGeneration, AgentTests,
                        ::testing::Values(&DualGenerationConstructor));
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
