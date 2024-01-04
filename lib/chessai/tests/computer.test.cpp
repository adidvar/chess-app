#include "computer.hpp"

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

/*
TEST_CASE("Testing of computer search", "[computer][ai]") {
  Computer cmp(Color::kWhite);

  cmp.SetBoard(BitBoard("startpos"));

  cmp.Start();
  std::this_thread::sleep_for(std::chrono::milliseconds{500});
  cmp.Stop();

  CHECK(cmp.GetDepth() > 3);
  // CHECK(cmp.GetDepth() < 8);
  // CHECK(cmp.GetPV().front() == cmp.GetTurn());
}

TEST_CASE("Testing of a fast out for computer", "[computer][ai]") {
  Computer cmp(Color::kWhite);
  {
    cmp.SetBoard(BitBoard("startpos"));

    auto bbegin = std::chrono::high_resolution_clock::now();
    cmp.Start();
    auto bend = std::chrono::high_resolution_clock::now() - bbegin;

    auto sbegin = std::chrono::high_resolution_clock::now();
    cmp.Stop();
    auto send = std::chrono::high_resolution_clock::now() - sbegin;

    CHECK(std::chrono::duration_cast<std::chrono::milliseconds>(bend).count() <
          10);
    CHECK(std::chrono::duration_cast<std::chrono::milliseconds>(send).count() <
          20);
  }
  {
    cmp.SetBoard(BitBoard("startpos"));

    auto bbegin = std::chrono::high_resolution_clock::now();
    cmp.Start();
    auto bend = std::chrono::high_resolution_clock::now() - bbegin;

    auto sbegin = std::chrono::high_resolution_clock::now();
    cmp.Stop();
    auto send = std::chrono::high_resolution_clock::now() - sbegin;

    CHECK(std::chrono::duration_cast<std::chrono::milliseconds>(bend).count() <
          10);
    CHECK(std::chrono::duration_cast<std::chrono::milliseconds>(send).count() <
          20);
  }
}
*/
