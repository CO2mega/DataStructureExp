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
    int bit[10000]; // ����
    int start; // �������ʼ�±�
    int weight; // �ַ���Ȩֵ
} Code;
typedef struct {
    char type[4]; //�ļ�����
    int length;	  //ԭ�ļ�����
    int weight[256]; //Ȩֵ����
} FileHead;
void HuffmanTree_encode(HuffmanNode* huffman_tree, int* weight, int n);
void Tree2Code(HuffmanNode* huffman_tree, Code* huffman_code, int n);