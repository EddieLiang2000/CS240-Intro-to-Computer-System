#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/catch.hpp"
#include "lib/mstats-utils.h"

// Testers 1-5
TEST_CASE("tester1", "[weight=1][part=5][suite=week2]") {
  system("make -s");
  system("./mstats tests/testers_exe/tester1 evaluate");
  mstats_result * result = read_mstats_result("mstats_result.txt");
  system("rm mstats_result.txt");
  REQUIRE(result->status == 1);
}

TEST_CASE("tester2", "[weight=1][part=5][suite=week2]") {
  system("make -s");
  system("./mstats tests/testers_exe/tester2 evaluate");
  mstats_result * result = read_mstats_result("mstats_result.txt");
  system("rm mstats_result.txt");
  REQUIRE(result->status == 1);
}

TEST_CASE("tester3", "[weight=1][part=5][suite=week2]") {
  system("make -s");
  system("./mstats tests/testers_exe/tester3 evaluate");
  mstats_result * result = read_mstats_result("mstats_result.txt");
  system("rm mstats_result.txt");
  REQUIRE(result->status == 1);
}

TEST_CASE("tester4", "[weight=1][part=5][suite=week2]") {
  system("make -s");
  system("./mstats tests/testers_exe/tester4 evaluate");
  mstats_result * result = read_mstats_result("mstats_result.txt");
  system("rm mstats_result.txt");
  REQUIRE(result->status == 1);
}

TEST_CASE("tester5", "[weight=1][part=5][suite=week2]") {
  system("make -s");
  system("./mstats tests/testers_exe/tester5 evaluate");
  mstats_result * result = read_mstats_result("mstats_result.txt");
  system("rm mstats_result.txt");
  REQUIRE(result->status == 1);
}

TEST_CASE("tester6", "[weight=1][part=5][suite=week2]") {
  system("make -s");
  system("./mstats tests/testers_exe/tester6 evaluate");
  mstats_result * result = read_mstats_result("mstats_result.txt");
  system("rm mstats_result.txt");
  REQUIRE(result->status == 1);
}

TEST_CASE("tester7", "[weight=1][part=5][suite=week2]") {
  system("make -s");
  system("./mstats tests/testers_exe/tester7 evaluate");
  mstats_result * result = read_mstats_result("mstats_result.txt");
  system("rm mstats_result.txt");
  REQUIRE(result->status == 1);
}

TEST_CASE("tester8", "[weight=1][part=5][suite=week2]") {
  system("make -s");
  system("./mstats tests/testers_exe/tester8 evaluate");
  mstats_result * result = read_mstats_result("mstats_result.txt");
  system("rm mstats_result.txt");
  REQUIRE(result->status == 1);
}

TEST_CASE("tester9", "[weight=1][part=5][suite=week2]") {
  system("make -s");
  system("./mstats tests/testers_exe/tester9 evaluate");
  mstats_result * result = read_mstats_result("mstats_result.txt");
  system("rm mstats_result.txt");
  REQUIRE(result->status == 1);
}

TEST_CASE("tester10", "[weight=1][part=5][suite=week2]") {
  system("make -s");
  system("./mstats tests/testers_exe/tester10 evaluate");
  mstats_result * result = read_mstats_result("mstats_result.txt");
  system("rm mstats_result.txt");
  REQUIRE(result->status == 1);
}

TEST_CASE("tester11", "[weight=1][part=5][suite=week2]") {
  system("make -s");
  system("./mstats tests/testers_exe/tester11 evaluate");
  mstats_result * result = read_mstats_result("mstats_result.txt");
  system("rm mstats_result.txt");
  REQUIRE(result->status == 1);
}

TEST_CASE("tester12", "[weight=1][part=5][suite=week2]") {
  system("make -s");
  system("./mstats tests/testers_exe/tester12 evaluate");
  mstats_result * result = read_mstats_result("mstats_result.txt");
  system("rm mstats_result.txt");
  REQUIRE(result->status == 1);
}

TEST_CASE("tester13", "[weight=1][part=5][suite=week2]") {
  system("make -s");
  system("./mstats tests/testers_exe/tester13 evaluate");
  mstats_result * result = read_mstats_result("mstats_result.txt");
  system("rm mstats_result.txt");
  REQUIRE(result->status == 1);
}

TEST_CASE("tester14", "[weight=1][part=5][suite=week2]") {
  system("make -s");
  system("./mstats tests/testers_exe/tester14 evaluate");
  mstats_result * result = read_mstats_result("mstats_result.txt");
  system("rm mstats_result.txt");
  REQUIRE(result->status == 1);
}