#pragma once
#include <iostream>
#include <queue>
#include <thread>
#include <condition_variable>
#include <shared_mutex>


struct Node
{
	int value;
	Node* next;
	std::mutex* node_mutex;
};

class FineGrainedQueue
{
	Node* head;
	std::mutex* list_mutex;
public:

	void insertIntoMiddle(int value, int pos)
	{
		int curPos = 2; // �������������� ������� ������� ������ � ������ (������ ��������� ��� ������� � �������� ������)
		Node* previous, * current; //  ��������� ���������� ��� ������ �����
		list_mutex->lock();               // ����� ���� ������  
		previous = this->head;        // �������� ���� ��������� �� "������" 
		current = this->head->next; // ������� ��������� �� ��������� �� "������" 
		previous->node_mutex->lock(); // ����� "������"
		list_mutex->unlock();         // ����������� ���� ������
		if (current) // ���� ��������� ������ ������ ����� ����� 
			current->node_mutex->lock();
		while (curPos < pos) // ������������ �� ������ ���� �� ����� ������ ������� �� ������� 
		{
			Node* oldPrevios = previous;
			previous = current;
			current = current->next;
			oldPrevios->node_mutex->unlock();
			if (current) // ��������� � ������ ����� ��������
			{
				current->node_mutex->lock();
			}
			else             // ���� �� ����� �������� ������� ������� ������ ������   
			{
				break;
			}
		}
		Node* newNode; // ������ ����� ���� 
		newNode->node_mutex->lock(); // �����  ��� 
		newNode->value = value;  // �������� ���������� ������
		previous->next = newNode; // ����� ���� ������ ��������� ��� ����������� 
		if (current)
			newNode->next = current; // ��������� ��� ������ ���� ���������� ������� 
		else
		{
			std::cout << "The list is smaller than the selected position. The data is recorded in " << curPos << " position" << std::endl;
			newNode->next = nullptr; // ���� ���� ���������
		}
		previous->node_mutex->unlock();
		newNode->node_mutex->unlock();
		current->node_mutex->unlock();
	}

};