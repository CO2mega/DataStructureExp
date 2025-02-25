#pragma once
typedef struct {
    int left_child;
    int right_child;
    int parent;
    int weight;
    int flag;
} HuffmanNode;

typedef struct
{
    int bit[10000]; // 数组
    int start; // 编码的起始下标
    int weight; // 字符的权值
} Code;
typedef struct {
    char type[4]; //文件类型
    int length;	  //原文件长度
    int weight[256]; //权值数组
} FileHead;
void HuffmanTree_encode(HuffmanNode* huffman_tree, int* weight, int n);
void Tree2Code(HuffmanNode* huffman_tree, Code* huffman_code, int n);