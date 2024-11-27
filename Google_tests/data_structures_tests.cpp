#include <gtest/gtest.h>
#include "../include/data_structures/AVLBinaryTree.h"
#include "../include/data_structures/ISetBinaryTree.h"
#include "../include/data_structures/IDictionaryBinaryTree.h"
#include "../include/data_structures/ISortedSequenceBinaryTree.h"
#include "../include/information_processing/Index.h"
#include "../include/information_processing/Histogram.h"


TEST(BinaryTree, InsertAndFind) {
    AVLBinaryTree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);

    ASSERT_NE(tree.find(5), false);
    ASSERT_NE(tree.find(3), false);
    ASSERT_NE(tree.find(7), false);
    ASSERT_EQ(tree.find(10), false);
}

TEST(BinaryTree, Remove) {
    AVLBinaryTree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    tree.remove(3);

    ASSERT_EQ(tree.find(3), false);
    ASSERT_NE(tree.find(5), false);
    ASSERT_NE(tree.find(7), false);
}

TEST(BinaryTree, Height) {
    AVLBinaryTree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    tree.insert(2);
    tree.insert(4);

    ASSERT_EQ(tree.height(), 3);
}

TEST(BinaryTree, Balance) {
    AVLBinaryTree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    tree.insert(2);
    tree.insert(4);
    tree.insert(6);
    tree.insert(8);

    ASSERT_EQ(tree.height(), 3);
}

TEST(BinaryTree, Iterator) {
    AVLBinaryTree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);

    std::vector<int> values;
    for (auto it = tree.begin(); it != tree.end(); ++it) {
        values.push_back(*it);
    }

    std::vector<int> expected = {3, 5, 7};
    ASSERT_EQ(values, expected);
}


TEST(SetBinaryTree, InsertAndFind) {
    ISetBinaryTree<int> set;
    set.insert(5);
    set.insert(3);
    set.insert(7);

    ASSERT_TRUE(set.find(5));
    ASSERT_TRUE(set.find(3));
    ASSERT_TRUE(set.find(7));
    ASSERT_FALSE(set.find(10));
}

TEST(SetBinaryTree, Erase) {
    ISetBinaryTree<int> set;
    set.insert(5);
    set.insert(3);
    set.insert(7);
    set.erase(3);

    ASSERT_FALSE(set.find(3));
    ASSERT_TRUE(set.find(5));
    ASSERT_TRUE(set.find(7));
}

TEST(SetBinaryTree, Union) {
    ISetBinaryTree<int> set1;
    set1.insert(1);
    set1.insert(2);

    ISetBinaryTree<int> set2;
    set2.insert(2);
    set2.insert(3);

    ISet<int>* result = set1.setUnion(set2);

    ASSERT_TRUE(result->find(1));
    ASSERT_TRUE(result->find(2));
    ASSERT_TRUE(result->find(3));
    delete result;
}

TEST(SetBinaryTree, Intersection) {
    ISetBinaryTree<int> set1;
    set1.insert(1);
    set1.insert(2);

    ISetBinaryTree<int> set2;
    set2.insert(2);
    set2.insert(3);

    ISet<int>* result = set1.intersection(set2);

    ASSERT_FALSE(result->find(1));
    ASSERT_TRUE(result->find(2));
    ASSERT_FALSE(result->find(3));
    delete result;
}

TEST(SetBinaryTree, Difference) {
    ISetBinaryTree<int> set1;
    set1.insert(1);
    set1.insert(2);

    ISetBinaryTree<int> set2;
    set2.insert(2);
    set2.insert(3);

    ISet<int>* result = set1.difference(set2);

    ASSERT_TRUE(result->find(1));
    ASSERT_FALSE(result->find(2));
    ASSERT_FALSE(result->find(3));
    delete result;
}

TEST(SetBinaryTree, SubSet) {
    ISetBinaryTree<int> set1;
    set1.insert(1);
    set1.insert(2);

    ISetBinaryTree<int> set2;
    set2.insert(1);
    set2.insert(2);
    set2.insert(3);

    ASSERT_TRUE(set1.subSet(set2));
    ASSERT_FALSE(set2.subSet(set1));
}

TEST(SetBinaryTree, Equal) {
    ISetBinaryTree<int> set1;
    set1.insert(1);
    set1.insert(2);

    ISetBinaryTree<int> set2;
    set2.insert(1);
    set2.insert(2);

    ASSERT_TRUE(set1.equal(set2));
    set2.insert(3);
    ASSERT_FALSE(set1.equal(set2));
}

TEST(IDictionaryBinaryTree, AddAndGet) {
    IDictionaryBinaryTree<int, std::string> dict;
    dict.Add(1, "one");
    dict.Add(2, "two");

    ASSERT_EQ(dict.Get(1), "one");
    ASSERT_EQ(dict.Get(2), "two");
}

TEST(IDictionaryBinaryTree, Remove) {
    IDictionaryBinaryTree<int, std::string> dict;
    dict.Add(1, "one");
    dict.Add(2, "two");
    dict.Remove(1);

    ASSERT_THROW(dict.Get(1), std::out_of_range);
    ASSERT_EQ(dict.Get(2), "two");
}

TEST(IDictionaryBinaryTree, ContainsKey) {
    IDictionaryBinaryTree<int, std::string> dict;
    dict.Add(1, "one");
    dict.Add(2, "two");

    ASSERT_TRUE(dict.ContainsKey(1));
    ASSERT_TRUE(dict.ContainsKey(2));
    ASSERT_FALSE(dict.ContainsKey(3));
}

TEST(IDictionaryBinaryTree, GetKeys) {
    IDictionaryBinaryTree<int, std::string> dict;
    dict.Add(1, "one");
    dict.Add(2, "two");

    auto keys = dict.GetKeys();
    ASSERT_EQ(keys.getLength(), 2);
    ASSERT_EQ(keys.get(0), 1);
    ASSERT_EQ(keys.get(1), 2);
}

TEST(IDictionaryBinaryTree, GetValues) {
    IDictionaryBinaryTree<int, std::string> dict;
    dict.Add(1, "one");
    dict.Add(2, "two");

    auto values = dict.GetValues();
    ASSERT_EQ(values.getLength(), 2);
    ASSERT_EQ(values.get(0), "one");
    ASSERT_EQ(values.get(1), "two");
}

TEST(ISortedSequenceBinaryTree, AddAndGet) {
    ISortedSequenceBinaryTree<int> seq;
    seq.Add(3);
    seq.Add(1);
    seq.Add(2);

    ASSERT_EQ(seq.Get(0), 1);
    ASSERT_EQ(seq.Get(1), 2);
    ASSERT_EQ(seq.Get(2), 3);
}

TEST(ISortedSequenceBinaryTree, GetFirstAndLast) {
    ISortedSequenceBinaryTree<int> seq;
    seq.Add(3);
    seq.Add(1);
    seq.Add(2);

    ASSERT_EQ(seq.GetFirst(), 1);
    ASSERT_EQ(seq.GetLast(), 3);
}

TEST(ISortedSequenceBinaryTree, IndexOf) {
    ISortedSequenceBinaryTree<int> seq;
    seq.Add(3);
    seq.Add(1);
    seq.Add(2);

    ASSERT_EQ(seq.IndexOf(1), 0);
    ASSERT_EQ(seq.IndexOf(2), 1);
    ASSERT_EQ(seq.IndexOf(3), 2);
    ASSERT_EQ(seq.IndexOf(4), -1);
}

TEST(ISortedSequenceBinaryTree, GetSubsequence) {
    ISortedSequenceBinaryTree<int> seq;
    seq.Add(3);
    seq.Add(1);
    seq.Add(2);

    auto* subseq = seq.GetSubsequence(0, 1);
    ASSERT_EQ(subseq->GetLength(), 2);
    ASSERT_EQ(subseq->Get(0), 1);
    ASSERT_EQ(subseq->Get(1), 2);
    delete subseq;
}

TEST(ISortedSequenceBinaryTree, IsEmptyAndGetLength) {
    ISortedSequenceBinaryTree<int> seq;
    ASSERT_TRUE(seq.IsEmpty());
    ASSERT_EQ(seq.GetLength(), 0);

    seq.Add(1);
    ASSERT_FALSE(seq.IsEmpty());
    ASSERT_EQ(seq.GetLength(), 1);
}