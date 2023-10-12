#include "computer.hpp"

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

static bool TestComputerWorking(const char* fen) {
  Computer cmp(Color::kWhite);

  cmp.SetBoard(BitBoard(fen));

  cmp.Start();
  std::this_thread::sleep_for(std::chrono::milliseconds{500});
  cmp.Stop();

  CHECK(cmp.GetPV().size() > 3);
  CHECK(cmp.GetPV().size() < 8);
  CHECK(cmp.GetPV().front() == cmp.GetTurn());

  return false;
}

TEST_CASE("Testing of computer search", "[computer][ai]") {
  TestComputerWorking("startpos");
}
