#include "qsearch.hpp"

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include "score.hpp"

class QSearchTest {
 public:
  void Test() {
    SECTION("Basic tests") {}
    SECTION("Stand pat tests") {}
    SECTION("Delta pruning tests") {}
    SECTION("Logic tests") {}
  }
};

TEST_CASE("QSearch tests", "[qsearch]", ) {
  QSearchTest test;
  test.Test();
}
