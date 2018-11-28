#include "pch.h"

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <bitset>
#include <stdio.h>
#include <random>
#include <algorithm>
#include <vector>

struct ListNode {
	ListNode* prev;
	ListNode* next;
	ListNode* rand;
	std::string data;
};

class List {
public:
	List()
	{
		head = nullptr;
		tail = nullptr;
		count = 0;
	}
	List(ListNode* const listHead , ListNode* const listTail, int const elementCount)
	{
		head = listHead;
		tail = listTail;
		count = elementCount;
	};
	void Serialize(FILE * file) 
	{
		if (!file)
			return;

		int* listNodes = new int[count];
		int index = 0;
		for (ListNode* currentNode = head; currentNode; currentNode = currentNode->next)
		{
			listNodes[index] = (int)currentNode;
			++index;
		}

		for (ListNode* currentNode = head; currentNode; currentNode = currentNode->next)
		{
			const char* buffer = currentNode->data.c_str();
			short bufferSize = (short)std::strlen(buffer);

			fwrite(&bufferSize, sizeof(short), 1, file);
			fwrite(buffer , bufferSize, 1, file);

			short index = (short)std::distance(listNodes, std::find(listNodes, listNodes + count, (int)currentNode->rand));
			fwrite(&index, sizeof(short), 1, file);
		}
		delete[] listNodes;
	};  //fopen(path, "wb"))

	void Deserialize(FILE * file)
	{
		if (!file)
			return;
		short strLenght = 0;
		std::vector<ListNode*> listNodes;
		std::vector<short> randomIndexes;

		while (fread(&strLenght, sizeof(short), 1, file))
		{
			ListNode* newListNode = new ListNode;

			if (listNodes.empty())
				newListNode->prev = nullptr;
			else
			{
				auto prevNode = listNodes[listNodes.size() - 1];
				prevNode->next = newListNode;
				newListNode->prev = prevNode;
			}

			std::string data(strLenght, '\0');
			fread(&data[0], sizeof(char), (size_t)strLenght, file);
			newListNode->data = data;

			short randomIndex = 0;
			fread(&randomIndex, sizeof(short), 1, file);
			randomIndexes.push_back(randomIndex);

			listNodes.push_back(newListNode);
		};

		listNodes.back()->next = nullptr;

		head = listNodes.front();
		tail = listNodes.back();
		count = listNodes.size();

		for (int i = 0; i < count; i++)
		{
			listNodes[i]->rand = listNodes[randomIndexes[i]];
		}
	};  // fopen(path, "rb"))

private:
	ListNode* head;
	ListNode* tail;
	int count;
};

int main()
{
	int count = 5;
	ListNode* listNodes[5];

	for (int i = 0; i < count; i++)
	{
		ListNode* listNode = new ListNode;
		listNode->data = "data" + std::to_string(i);
		listNodes[i] = listNode;

		if (i <= 0)
		{
			listNode->prev = nullptr;
			continue;
		}

		listNode->prev = listNodes[i - 1];

		if (i >= count - 1)
			listNode->next = nullptr;
	

		listNodes[i - 1]->next = listNode;
	}

	for (auto listNode: listNodes)
	{
		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_int_distribution<> distr(0, count - 1);
		listNode->rand = listNodes[distr(eng)];
	}

	List* list = new List(listNodes[0], listNodes[count - 1], count);
	const char* path = "file.txt";
	FILE* file = fopen(path, "wb");
	list->Serialize(file);
	fclose(file);
	file = fopen(path, "rb");
	List* newList = new List;
	newList->Deserialize(file);
	fclose(file);
	return 0;
}