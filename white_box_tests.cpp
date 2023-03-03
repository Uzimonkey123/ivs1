//======== Copyright (c) 2022, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - test suite
//
// $NoKeywords: $ivs_project_1 $white_box_tests.cpp
// $Author:     Norman Babiak <xbabia01@stud.fit.vutbr.cz>
// $Date:       $2023-03-03
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author Norman Babiak
 * 
 * @brief Implementace testu hasovaci tabulky.
 */

#include <vector>

#include "gtest/gtest.h"

#include "white_box_code.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy hasovaci tabulky, testujte nasledujici:
// 1. Verejne rozhrani hasovaci tabulky
//     - Vsechny funkce z white_box_code.h
//     - Chovani techto metod testuje pro prazdnou i neprazdnou tabulku.
// 2. Chovani tabulky v hranicnich pripadech
//     - Otestujte chovani pri kolizich ruznych klicu se stejnym hashem 
//     - Otestujte chovani pri kolizich hashu namapovane na stejne misto v 
//       indexu
//============================================================================//

class EmptyHashMap : public ::testing::Test {
    
protected:
    hash_map_t *map = NULL;

};

class NonEmptyHashMap : public ::testing::Test {

protected:
    hash_map_t *map = hash_map_ctor();

    void SetUp() {
        hash_map_put(map, "Test", 1234);
        hash_map_put(map, "Test2", 5678);
        hash_map_put(map, "Test3", 9123);
    }

    void TearDown() {
        hash_map_clear(map);
        hash_map_dtor(map);
    }
    
};

class NonEmptyHashMap2 : public ::testing::Test {

protected:
    hash_map_t *map = hash_map_ctor();

    void SetUp() {
        hash_map_put(map, "Test", 1234);
        hash_map_put(map, "Test2", 5678);
        hash_map_put(map, "Test3", 9123);
    }
    
};

class EdgeCases : public ::testing::Test {

protected:
    hash_map_t *map = hash_map_ctor();

    void SetUp() {
        hash_map_put(map, "Test", 1234);
        hash_map_put(map, "Test2", 5678);
        hash_map_put(map, "Test3", 9123);
    }

    void TearDown() {
        hash_map_clear(map);
        hash_map_dtor(map);
    }
    
};

TEST_F(EmptyHashMap, ctor) {
    EXPECT_EQ(map, nullptr);

    hash_map_t* map2 = hash_map_ctor();
    EXPECT_FALSE(map2 == NULL);
    EXPECT_FALSE(map2->dummy == NULL);
    EXPECT_FALSE(hash_map_reserve(map2, map2->used) == MEMORY_ERROR);
    hash_map_dtor(map2);
}

TEST_F(NonEmptyHashMap, ctor) {
    EXPECT_TRUE(map != nullptr);
    EXPECT_TRUE(map->allocated == 8);

    EXPECT_FALSE(map->first == nullptr);
    EXPECT_FALSE(map->last == nullptr);
    EXPECT_FALSE(map->used == 0);
}

TEST_F(EmptyHashMap, dtor) {
    auto map = hash_map_ctor();
    EXPECT_TRUE(map != nullptr);

    hash_map_dtor(map);
    EXPECT_TRUE(map);
}

TEST_F(NonEmptyHashMap2, dtor) {
    EXPECT_TRUE(map != nullptr);

    EXPECT_FALSE(map->first == nullptr);
    EXPECT_FALSE(map->last == nullptr);
    EXPECT_FALSE(map->used == 0);

    hash_map_dtor(map);
    EXPECT_TRUE(map);
} 

TEST_F(EmptyHashMap, clear) {
    auto map = hash_map_ctor();
    EXPECT_TRUE(map != nullptr);
    hash_map_clear(map);

    EXPECT_EQ(map->first, nullptr);
    EXPECT_EQ(map->last, nullptr);
    EXPECT_EQ(map->used, 0);

    hash_map_dtor(map);
}

TEST_F(NonEmptyHashMap2, clear) {
    EXPECT_TRUE(map != nullptr);

    EXPECT_FALSE(map->first == nullptr);
    EXPECT_FALSE(map->last == nullptr);
    EXPECT_FALSE(map->used == 0);

    hash_map_clear(map);

    EXPECT_EQ(map->first, nullptr);
    EXPECT_EQ(map->last, nullptr);
    EXPECT_EQ(map->used, 0);

    hash_map_dtor(map);
    EXPECT_TRUE(map);
} 

TEST_F(EmptyHashMap, reserve) {
    auto map = hash_map_ctor();
    auto value = hash_map_reserve(map, 16);

    if(map->allocated < map->used) {
        ASSERT_EQ(value, VALUE_ERROR);
    } else {
        EXPECT_EQ(value, OK);
    }

    hash_map_dtor(map);
}

TEST_F(NonEmptyHashMap, reserve) {
    auto value = hash_map_reserve(map, 2);

    if(map->allocated < map->used) {
        ASSERT_EQ(value, VALUE_ERROR);
    }

    auto value2 = hash_map_reserve(map, 16);
    EXPECT_TRUE(value2 == OK);

}

TEST_F(EmptyHashMap, size) {
    EXPECT_EQ(map, nullptr);
    auto map = hash_map_ctor();

    auto size = hash_map_size(map);
    
    EXPECT_TRUE(size < map->allocated);
    EXPECT_EQ(size, 0);

    hash_map_dtor(map);
}

TEST_F(NonEmptyHashMap, size) {
    auto size = hash_map_size(map);
    
    EXPECT_TRUE(size < map->allocated);
    EXPECT_TRUE(size != 0);
    EXPECT_EQ(size, 3);

    hash_map_put(map, "Test4", 1111);
    auto size2 = hash_map_size(map);
    EXPECT_EQ(size2, 4);
}

TEST_F(EmptyHashMap, capacity) {
    auto map = hash_map_ctor();
    EXPECT_FALSE(hash_map_capacity(map) == 0);
    EXPECT_TRUE(hash_map_size(map) != hash_map_capacity(map));

    hash_map_dtor(map);
}

TEST_F(NonEmptyHashMap, capacity) {
    EXPECT_FALSE(hash_map_capacity(map) == 0);
    EXPECT_TRUE(hash_map_size(map) != hash_map_capacity(map));

    hash_map_put(map, "1", 1);
    hash_map_put(map, "2", 2);
    hash_map_put(map, "3", 3);
    hash_map_put(map, "4", 4);
    hash_map_put(map, "5", 5);
    EXPECT_TRUE(hash_map_capacity(map) > 8);
}

TEST_F(EmptyHashMap, contains) {
    auto map = hash_map_ctor();
    auto value = hash_map_contains(map, "Random");

    EXPECT_EQ(value, 0);

    hash_map_dtor(map);
}

TEST_F(NonEmptyHashMap, contains) {
    auto value = hash_map_contains(map, "Test");
    EXPECT_TRUE(value != 0);

    auto value2 = hash_map_contains(map, "Test4");
    EXPECT_TRUE(value2 == 0);
}

TEST_F(EmptyHashMap, put) {
    auto map = hash_map_ctor();
    auto value = hash_map_put(map, "Test", 1234);

    EXPECT_EQ(value, OK);
    EXPECT_FALSE(value == KEY_ALREADY_EXISTS);

    hash_map_dtor(map);
}

TEST_F(NonEmptyHashMap, put) {
    auto value = hash_map_put(map, "Test4", 5555);

    EXPECT_EQ(value, OK);
    EXPECT_FALSE(value == KEY_ALREADY_EXISTS);

    auto value2 = hash_map_put(map, "Test", 1234);

    EXPECT_EQ(value2, KEY_ALREADY_EXISTS);

}

TEST_F(EmptyHashMap, get) {
    auto map = hash_map_ctor();

    int value;
    auto error = hash_map_get(map, "Test", &value);
    EXPECT_EQ(error, KEY_ERROR);
    EXPECT_FALSE(error == OK);

    hash_map_dtor(map);
}

TEST_F(NonEmptyHashMap, get) {
    int value;
    auto error1 = hash_map_get(map, "Test", &value);
    EXPECT_EQ(error1, OK);

    auto error2 = hash_map_get(map, "Test123", &value);
    EXPECT_EQ(error2, KEY_ERROR);
}

TEST_F(EmptyHashMap, pop) {
    auto map = hash_map_ctor();

    int value;
    auto error = hash_map_pop(map, "Test", &value);
    EXPECT_EQ(error, KEY_ERROR);
    EXPECT_FALSE(error == OK);

    hash_map_dtor(map);
}

TEST_F(NonEmptyHashMap, pop) {
    int value;
    auto error1 = hash_map_pop(map, "Test", &value);
    EXPECT_FALSE(hash_map_contains(map, "Test"));

    auto error2 = hash_map_pop(map, "Test123", &value);
    EXPECT_EQ(error2, KEY_ERROR);
}

TEST_F(EmptyHashMap, remove) {
    auto map = hash_map_ctor();

    auto value = hash_map_remove(map, "Test");
    ASSERT_EQ(value, KEY_ERROR);
    EXPECT_FALSE(value == OK);

    hash_map_dtor(map);
}

TEST_F(NonEmptyHashMap, remove) {
    auto value1 = hash_map_remove(map, "Test");
    EXPECT_EQ(value1, OK);
    auto value2 = hash_map_remove(map, "Test2");
    EXPECT_EQ(value2, OK);

    auto value3 = hash_map_remove(map, "Test123");
    EXPECT_EQ(value3, KEY_ERROR);

}

/*** Konec souboru white_box_tests.cpp ***/