#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "HuffmanTree.h"
using namespace std;


void getHead(char* filename, FileHead* head) {
    FILE* in = fopen(filename, "rb");
    if (in == NULL)
    {
        printf("打开文件失败");
        return;
    }
    fread(head, sizeof(FileHead), 1, in);
    fclose(in);
}

void write2File(Code* huffman_code,FileHead* head,) {

}
int main()
{
    cout << "--------------Huffman文件压缩编码---------------" << endl;
    cout << "请输入文件名:";
    char filename[256]; // 文件名
    cin >> filename;

    char ch;
    int weight[256] = {0};
    FileHead head;
    getHead(filename, &head);
    // 以二进制流的方式打开文件
    FILE* in = fopen(filename, "rb");
    if (in == NULL)
    {
        printf("打开文件失败");
        return 0;
    }
    // 扫描文件，获得权重
    while ((ch = getc(in)) != EOF)
    {
        weight[(unsigned char)ch]++;
    }
    // 关闭文件
    fclose(in);

    // 创建哈夫曼树
    HuffmanNode* myHaffNode = (HuffmanNode*)malloc(sizeof(HuffmanNode) * (2 * 256 - 1));
    if (myHaffNode == NULL)
    {
        printf("内存分配失败");
        return 0;
    }
    HuffmanTree_encode(myHaffNode, weight, 256);
    Code* myHaffCode = (Code*)malloc(sizeof(Code) * 256);
    if (myHaffCode == NULL)
    {
        printf("内存分配失败");
        return 0;
    }
    Tree2Code(myHaffNode, myHaffCode, 256);
    /*cout << "--------------Huffman编码表---------------" << endl;
    printf("%-10s%-10s%-10s%-10s\n", "字符", "权重", "编码", "编码长度");
    for (int i = 0; i < 256; i++)
    {
        if (myHaffNode[i].weight != 0)
        {
            printf("%-10c%-10d ", i, myHaffNode[i].weight);
            for (int j = myHaffCode[i].start; j < 10000; j++)
            {
                printf("%d", myHaffCode[i].bit[j]);
            }
            printf(" %-10d\n", 10000 - myHaffCode[i].start);
        }
    }*/
    //输出压缩文件
    
    system("pause");
    return 0;
}