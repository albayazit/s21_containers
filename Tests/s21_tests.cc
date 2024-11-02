#include <gtest/gtest.h>

#include <map>
#include <set>

#include "../Map/s21_map.h"
#include "../Multiset/s21_multiset.h"
#include "../Set/s21_set.h"

// __________SET__________ //

TEST(SetTest, InsertTest) {
  s21::set<int> my_set;
  std::set<int> std_set;
  auto res1 = my_set.insert(10);
  auto res2 = std_set.insert(10);
  EXPECT_EQ(*res1.first, *std_set.find(10));
  EXPECT_EQ(res1.second, res2.second);
  res1 = my_set.insert(10);
  res2 = std_set.insert(10);
  EXPECT_EQ(res1.second, res2.second);
}

TEST(SetTest, InsertManyTest) {
  s21::set<int> my_set;
  std::set<int> std_set;
  auto my_results = my_set.insert_many(5, 10, 15, 20);
  std_set.insert(5);
  std_set.insert(10);
  std_set.insert(15);
  std_set.insert(20);
  for (int i = 0; i < 4; ++i) {
    EXPECT_EQ(*my_results[i].first, *std_set.find(*my_results[i].first));
    EXPECT_EQ(my_results[i].second, true);
  }
  auto my_results_dup = my_set.insert_many(5, 10);
  for (const auto &result : my_results_dup) {
    EXPECT_EQ(result.second, false);
  }
}

TEST(SetTest, EraseTest) {
  s21::set<int> my_set = {1, 2, 3};
  std::set<int> std_set = {1, 2, 3};
  auto it_my = my_set.find(2);
  auto it_std = std_set.find(2);
  my_set.erase(it_my);
  std_set.erase(it_std);
  EXPECT_EQ(my_set.size(), std_set.size());
  EXPECT_EQ(my_set.contains(2), false);
  EXPECT_EQ(std_set.find(2), std_set.end());
}

TEST(SetTest, FindTest) {
  s21::set<int> my_set = {10, 20, 30};
  std::set<int> std_set = {10, 20, 30};
  auto it_my = my_set.find(20);
  auto it_std = std_set.find(20);
  EXPECT_EQ(*it_my, *it_std);
  it_my = my_set.find(40);
  it_std = std_set.find(40);
  EXPECT_EQ(it_my, my_set.end());
  EXPECT_EQ(it_std, std_set.end());
}

TEST(SetTest, MergeTest) {
  s21::set<int> my_set{1, 2, 3};
  s21::set<int> other_set{3, 4, 5};
  std::set<int> std_set{1, 2, 3};
  std::set<int> std_other_set{3, 4, 5};
  my_set.merge(other_set);
  std_set.insert(std_other_set.begin(), std_other_set.end());
  ASSERT_EQ(my_set.size(), std_set.size());
  auto my_it = my_set.begin();
  auto std_it = std_set.begin();
  for (; my_it != my_set.end() && std_it != std_set.end(); ++my_it, ++std_it) {
    EXPECT_EQ(*my_it, *std_it);
  }
}

TEST(SetTest, SwapTest) {
  s21::set<int> my_set1;
  my_set1.insert(1);
  my_set1.insert(2);
  my_set1.insert(3);
  s21::set<int> my_set2;
  my_set2.insert(4);
  my_set2.insert(5);
  my_set2.insert(6);
  std::set<int> std_set1 = {1, 2, 3};
  std::set<int> std_set2 = {4, 5, 6};
  my_set1.swap(my_set2);
  std_set1.swap(std_set2);
  EXPECT_EQ(my_set1.size(), std_set1.size());
  EXPECT_EQ(my_set2.size(), std_set2.size());
  auto it1 = my_set1.begin();
  auto std_it1 = std_set1.begin();
  while (it1 != my_set1.end() && std_it1 != std_set1.end()) {
    EXPECT_EQ(*it1, *std_it1);
    ++it1;
    ++std_it1;
  }
  auto it2 = my_set2.begin();
  auto std_it2 = std_set2.begin();
  while (it2 != my_set2.end() && std_it2 != std_set2.end()) {
    EXPECT_EQ(*it2, *std_it2);
    ++it2;
    ++std_it2;
  }
}

TEST(SetTest, EmptyAndClearTest) {
  s21::set<int> my_set = {1, 2, 3};
  std::set<int> std_set = {1, 2, 3};
  EXPECT_EQ(my_set.empty(), std_set.empty());
  my_set.clear();
  std_set.clear();
  EXPECT_EQ(my_set.empty(), std_set.empty());
  EXPECT_EQ(my_set.size(), std_set.size());
}

// __________MAP__________ //

TEST(MapTest, InsertTest) {
  s21::map<int, std::string> my_map;
  std::map<int, std::string> std_map;
  auto result1 = my_map.insert({1, "one"});
  auto result2 = std_map.insert({1, "one"});
  EXPECT_EQ(result1.second, result2.second);
  EXPECT_EQ(my_map.at(1), std_map.at(1));
}

TEST(MapTest, AtTest) {
  s21::map<int, std::string> my_map;
  std::map<int, std::string> std_map;
  my_map.insert({1, "one"});
  std_map.insert({1, "one"});
  EXPECT_EQ(my_map.at(1), std_map.at(1));
  EXPECT_THROW(my_map.at(2), std::out_of_range);
}

TEST(MapTest, OperatorBracketTest) {
  s21::map<int, std::string> my_map;
  std::map<int, std::string> std_map;
  my_map[1] = "one";
  std_map[1] = "one";
  EXPECT_EQ(my_map[1], std_map[1]);
}

TEST(MapTest, EraseExistingElement) {
  s21::map<int, std::string> test_map;
  test_map.insert({1, "one"});
  test_map.insert({2, "two"});
  std::map<int, std::string> std_map;
  std_map.insert({1, "one"});
  std_map.insert({2, "two"});
  EXPECT_EQ(test_map.size(), std_map.size());
  auto it = test_map.find(1);
  auto it_std = std_map.find(1);
  test_map.erase(it);
  std_map.erase(it_std);
  EXPECT_EQ(test_map.size(), std_map.size());
  EXPECT_FALSE(test_map.contains(1));
}

TEST(MapTest, EraseNonExistentElement) {
  s21::map<int, std::string> test_map;
  test_map.insert({1, "one"});
  test_map.insert({2, "two"});

  auto it = test_map.find(3);
  test_map.erase(it);
}

TEST(MapTest, SwapTest) {
  s21::map<int, std::string> my_map1;
  s21::map<int, std::string> my_map2;
  my_map1.insert({1, "one"});
  my_map2.insert({2, "two"});
  my_map1.swap(my_map2);
  EXPECT_EQ(my_map1.at(2), "two");
  EXPECT_EQ(my_map2.at(1), "one");
}

TEST(MapTest, MergeTest) {
  s21::map<int, std::string> my_map1;
  s21::map<int, std::string> my_map2;
  my_map1.insert({1, "one"});
  my_map2.insert({2, "two"});
  my_map1.merge(my_map2);
  EXPECT_EQ(my_map1.contains(1), true);
  EXPECT_EQ(my_map1.contains(2), true);
  EXPECT_EQ(my_map2.contains(2), false);
}

TEST(MapTest, ContainsTest) {
  s21::map<int, std::string> my_map;
  std::map<int, std::string> std_map;
  my_map.insert({1, "one"});
  std_map.insert({1, "one"});
  EXPECT_EQ(my_map.contains(1), std_map.count(1));
  EXPECT_EQ(my_map.contains(2), std_map.count(2));
}

TEST(MapTest, SimpleInsertion) {
  s21::map<int, std::string> test_map;
  auto results =
      test_map.insert_many(std::make_pair(1, "one"), std::make_pair(2, "two"),
                           std::make_pair(3, "three"));
  EXPECT_TRUE(results[0].second);
  EXPECT_TRUE(results[1].second);
  EXPECT_TRUE(results[2].second);
  EXPECT_EQ(test_map[1], "one");
  EXPECT_EQ(test_map[2], "two");
  EXPECT_EQ(test_map[3], "three");
}

// __________MULTISET__________ //

TEST(MultisetTest, InsertAndSize) {
  s21::multiset<int> my_multiset;
  std::multiset<int> std_multiset;
  my_multiset.insert(10);
  my_multiset.insert(20);
  my_multiset.insert(10);
  my_multiset.insert(30);
  my_multiset.insert(20);
  std_multiset.insert(10);
  std_multiset.insert(20);
  std_multiset.insert(10);
  std_multiset.insert(30);
  std_multiset.insert(20);
  EXPECT_EQ(my_multiset.size(), std_multiset.size());
}

TEST(MultisetTest, DuplicateValues) {
  s21::multiset<int> my_multiset;
  std::multiset<int> std_multiset;
  my_multiset.insert(15);
  my_multiset.insert(15);
  my_multiset.insert(25);
  my_multiset.insert(15);
  my_multiset.insert(25);
  std_multiset.insert(15);
  std_multiset.insert(15);
  std_multiset.insert(25);
  std_multiset.insert(15);
  std_multiset.insert(25);
  auto my_it = my_multiset.begin();
  auto std_it = std_multiset.begin();
  for (; my_it != my_multiset.end() && std_it != std_multiset.end();
       ++my_it, ++std_it) {
    EXPECT_EQ(*my_it, *std_it);
  }
  EXPECT_EQ(my_it, my_multiset.end());
  EXPECT_EQ(std_it, std_multiset.end());
}

TEST(MultisetTest, EraseSingleElement) {
  s21::multiset<int> my_multiset;
  std::multiset<int> std_multiset;
  my_multiset.insert(5);
  my_multiset.insert(10);
  my_multiset.insert(5);
  std_multiset.insert(5);
  std_multiset.insert(10);
  std_multiset.insert(5);
  my_multiset.erase(my_multiset.find(5));
  std_multiset.erase(std_multiset.find(5));
  EXPECT_EQ(my_multiset.size(), std_multiset.size());
  auto my_it = my_multiset.begin();
  auto std_it = std_multiset.begin();
  for (; my_it != my_multiset.end() && std_it != std_multiset.end();
       ++my_it, ++std_it) {
    EXPECT_EQ(*my_it, *std_it);
  }
  EXPECT_EQ(my_it, my_multiset.end());
  EXPECT_EQ(std_it, std_multiset.end());
}

TEST(MultisetTest, InsertMany) {
  s21::multiset<int> my_multiset;
  std::multiset<int> std_multiset;
  my_multiset.insert_many(10, 20, 10, 30, 20);
  std_multiset.insert(10);
  std_multiset.insert(20);
  std_multiset.insert(10);
  std_multiset.insert(30);
  std_multiset.insert(20);
  EXPECT_EQ(my_multiset.size(), std_multiset.size());
  auto my_it = my_multiset.begin();
  auto std_it = std_multiset.begin();
  for (; my_it != my_multiset.end() && std_it != std_multiset.end();
       ++my_it, ++std_it) {
    EXPECT_EQ(*my_it, *std_it);
  }
  EXPECT_EQ(my_it, my_multiset.end());
  EXPECT_EQ(std_it, std_multiset.end());
}

TEST(MultisetTest, CountTest) {
  s21::multiset<int> ms = {1, 2, 2, 3, 3, 3};
  std::multiset<int> std_ms = {1, 2, 2, 3, 3, 3};
  EXPECT_EQ(ms.count(2), std_ms.count(2));
  EXPECT_EQ(ms.count(3), std_ms.count(3));
}

TEST(MultisetTest, ContainsTest) {
  s21::multiset<int> ms = {10, 20, 30};
  EXPECT_TRUE(ms.contains(10));
  EXPECT_FALSE(ms.contains(15));
  EXPECT_TRUE(ms.contains(30));
  std::multiset<int> std_ms = {10, 20, 30};
  EXPECT_EQ(ms.contains(10), std_ms.count(10) > 0);
  EXPECT_EQ(ms.contains(15), std_ms.count(15) > 0);
}

TEST(MultisetTest, EqualRangeTest) {
  s21::multiset<int> ms = {5, 10, 10, 15, 15, 15, 20};
  auto range = ms.equal_range(15);
  int count = 0;
  for (auto it = range.first; it != range.second; ++it) {
    ++count;
  }
  EXPECT_EQ(count, 3);
  std::multiset<int> std_ms = {5, 10, 10, 15, 15, 15, 20};
  auto std_range = std_ms.equal_range(15);
  int std_count = 0;
  for (auto it = std_range.first; it != std_range.second; ++it) {
    ++std_count;
  }
  EXPECT_EQ(count, std_count);
}

TEST(MultisetTest, LowerBoundTest) {
  s21::multiset<int> ms = {1, 2, 4, 5, 7};
  auto it = ms.lower_bound(4);
  EXPECT_EQ(*it, 4);
  it = ms.lower_bound(3);
  EXPECT_EQ(*it, 4);
  std::multiset<int> std_ms = {1, 2, 4, 5, 7};
  EXPECT_EQ(*it, *std_ms.lower_bound(3));
}

TEST(MultisetTest, UpperBoundTest) {
  s21::multiset<int> ms = {1, 2, 4, 5, 7};
  auto it = ms.upper_bound(4);
  EXPECT_EQ(*it, 5);
  it = ms.upper_bound(7);
  EXPECT_EQ(it, ms.end());
  std::multiset<int> std_ms = {1, 2, 4, 5, 7};
  EXPECT_EQ(it == ms.end(), std_ms.upper_bound(7) == std_ms.end());
}