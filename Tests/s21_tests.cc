#include <gtest/gtest.h>
#include <set>
#include "../Set/s21_set.h"
#include <map>
#include "../Map/s21_map.h"

// _____SET_____ //

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

    // Оригинальный std::set для сравнения
    std::set<int> std_set{1, 2, 3};
    std::set<int> std_other_set{3, 4, 5};

    // Выполняем merge на пользовательских и стандартных наборах
    my_set.merge(other_set);
    std_set.insert(std_other_set.begin(), std_other_set.end());

    // Проверяем, что размеры совпадают
    ASSERT_EQ(my_set.size(), std_set.size());

    // Проверяем, что элементы совпадают
    auto my_it = my_set.begin();
    auto std_it = std_set.begin();
    for (; my_it != my_set.end() && std_it != std_set.end(); ++my_it, ++std_it) {
        EXPECT_EQ(*my_it, *std_it);
    }
}

TEST(SetTest, SwapTest) {
    // Создаем два набора с разными элементами
    s21::set<int> my_set1;
    my_set1.insert(1);
    my_set1.insert(2);
    my_set1.insert(3);

    s21::set<int> my_set2;
    my_set2.insert(4);
    my_set2.insert(5);
    my_set2.insert(6);

    // Создаем аналогичные std::set для сравнения
    std::set<int> std_set1 = {1, 2, 3};
    std::set<int> std_set2 = {4, 5, 6};

    // Выполняем swap для пользовательских и стандартных наборов
    my_set1.swap(my_set2);
    std_set1.swap(std_set2);

    // Проверяем, что все элементы поменялись корректно
    EXPECT_EQ(my_set1.size(), std_set1.size());
    EXPECT_EQ(my_set2.size(), std_set2.size());

    // Сравниваем элементы после swap
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

// _____MAP_____ //

TEST(MapTest, InsertTest) {
    s21::map<int, std::string> my_map;
    std::map<int, std::string> std_map;

    auto result1 = my_map.insert({1, "one"});
    auto result2 = std_map.insert({1, "one"});

    EXPECT_EQ(result1.second, result2.second); // Проверка на вставку
    EXPECT_EQ(my_map.at(1), std_map.at(1));    // Проверка значений
}

// Тест для метода at
TEST(MapTest, AtTest) {
    s21::map<int, std::string> my_map;
    std::map<int, std::string> std_map;

    my_map.insert({1, "one"});
    std_map.insert({1, "one"});

    EXPECT_EQ(my_map.at(1), std_map.at(1)); // Проверка значения по ключу
    EXPECT_THROW(my_map.at(2), std::out_of_range); // Проверка на исключение
}

// Тест для оператора []
TEST(MapTest, OperatorBracketTest) {
    s21::map<int, std::string> my_map;
    std::map<int, std::string> std_map;

    my_map[1] = "one";
    std_map[1] = "one";

    EXPECT_EQ(my_map[1], std_map[1]); // Проверка вставки и получения значения
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


    EXPECT_EQ(test_map.size(), std_map.size());  // Размер должен быть 1
    EXPECT_FALSE(test_map.contains(1));  // Ключ 1 должен быть удален
}

TEST(MapTest, EraseNonExistentElement) {
    s21::map<int, std::string> test_map;
    test_map.insert({1, "one"});
    test_map.insert({2, "two"});

    auto it = test_map.find(3);  // 3 не существует
    test_map.erase(it);  // Не должно вызвать ошибок
}

// Тест для метода swap
TEST(MapTest, SwapTest) {
    s21::map<int, std::string> my_map1;
    s21::map<int, std::string> my_map2;

    my_map1.insert({1, "one"});
    my_map2.insert({2, "two"});

    my_map1.swap(my_map2);

    EXPECT_EQ(my_map1.at(2), "two");
    EXPECT_EQ(my_map2.at(1), "one");
}

// Тест для метода merge
TEST(MapTest, MergeTest) {
    s21::map<int, std::string> my_map1;
    s21::map<int, std::string> my_map2;

    my_map1.insert({1, "one"});
    my_map2.insert({2, "two"});

    my_map1.merge(my_map2);

    EXPECT_EQ(my_map1.contains(1), true);
    EXPECT_EQ(my_map1.contains(2), true);
    EXPECT_EQ(my_map2.contains(2), false); // my_map2 должен быть пустым после merge
}

// Тест для метода contains
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

    // Вставка нескольких элементов
    auto results = test_map.insert_many(std::make_pair(1, "one"), std::make_pair(2, "two"), std::make_pair(3, "three"));

    EXPECT_TRUE(results[0].second);         // Первый элемент должен быть вставлен
    EXPECT_TRUE(results[1].second);         // Второй элемент должен быть вставлен
    EXPECT_TRUE(results[2].second);         // Третий элемент должен быть вставлен

    // Проверка значений в map
    EXPECT_EQ(test_map[1], "one");
    EXPECT_EQ(test_map[2], "two");
    EXPECT_EQ(test_map[3], "three");
}