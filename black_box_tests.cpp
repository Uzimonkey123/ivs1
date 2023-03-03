//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     Norman Babiak <xbabia01@stud.fit.vutbr.cz>
// $Date:       $2023.02.XX 
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author Norman Babiak
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//

class EmptyTree : public ::testing::Test{

protected:
    BinaryTree tree;
};

class NonEmptyTree : public ::testing::Test{

protected:
    void SetUp() {

        int nodes[] = { 1, 2, 3, 5, 10, 15, 20, 25, 50, 60, 70};
        for(auto node : nodes) {
            tree.InsertNode(node);
        }
    }

    BinaryTree tree;
};

class TreeAxioms : public ::testing::Test{

protected:
    void SetUp() {

        int nodes[] = { 1, 2, 3, 5, 10, 15, 20, 25, 50, 60, 70};
        for(auto node : nodes) {
            tree.InsertNode(node);
        }
    }

    BinaryTree tree;
};

TEST_F(EmptyTree, InsertNode_1) {
    auto tmp = tree.InsertNode(10);
    EXPECT_TRUE(tmp.first);
    EXPECT_EQ(tmp.second->key, 10);

    auto tmp2 = tree.InsertNode(10);
    EXPECT_FALSE(tmp2.first);
}

TEST_F(EmptyTree, DeleteNode_1) {
    EXPECT_FALSE(tree.DeleteNode(5));

    tree.InsertNode(5);
    EXPECT_TRUE(tree.DeleteNode(5));
}

TEST_F(EmptyTree, FindNode_1) {
    EXPECT_TRUE(tree.FindNode(5) == NULL);

    tree.InsertNode(10);
    EXPECT_TRUE(tree.FindNode(10) != NULL);
}

TEST_F(NonEmptyTree, InsertNode_1) {
    auto tmp = tree.InsertNode(10);
    EXPECT_FALSE(tmp.first);

    auto tmp2 = tree.InsertNode(150);
    EXPECT_TRUE(tmp2.first);
}

TEST_F(NonEmptyTree, DeleteNode_1) {
    EXPECT_TRUE(tree.DeleteNode(1));
    EXPECT_TRUE(tree.DeleteNode(50));

    EXPECT_FALSE(tree.DeleteNode(150));
}

TEST_F(NonEmptyTree, FindNode_1) {
    EXPECT_TRUE(tree.FindNode(10) != NULL);
    EXPECT_FALSE(tree.FindNode(50) == NULL);

    EXPECT_FALSE(tree.FindNode(150) != NULL);
}

TEST_F(TreeAxioms, Axiom1) {
    std::vector<Node_t *> leafsArr {};
    tree.GetLeafNodes(leafsArr);

    for(auto node : leafsArr) {
        EXPECT_EQ(BinaryTree::BLACK, node->color);

        EXPECT_FALSE(BinaryTree::RED == node->color);
    }
}

TEST_F(TreeAxioms, Axiom2) {
    std::vector<Node_t *> array {};
    tree.GetAllNodes(array);

    for(auto node : array) {
        if(node->color == BinaryTree::RED) {
            EXPECT_EQ(BinaryTree::BLACK, node->pLeft->color);
            EXPECT_EQ(BinaryTree::BLACK, node->pRight->color);
        }
    }
}

TEST_F(TreeAxioms, Axiom3) {
    std::vector<Node_t *> leafsArr {};
    tree.GetLeafNodes(leafsArr);

    int previousCount = -1;
    BinaryTree::Node_t *currentNode = nullptr;

    for(auto node : leafsArr) {
        int blackCount = 0;
        currentNode = node;
        
        for(currentNode = node; currentNode != nullptr; currentNode = currentNode->pParent) {
            if(BinaryTree::BLACK == currentNode->color) {
                blackCount++;
            }
        }

        if(previousCount > -1) {
            EXPECT_EQ(previousCount, blackCount);
        }

        previousCount = blackCount;
    }
}

/*** Konec souboru black_box_tests.cpp ***/
