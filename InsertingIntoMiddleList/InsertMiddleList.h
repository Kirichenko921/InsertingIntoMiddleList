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
			else             // ���� �� ����� �������� ������� ������� ������ ������   (�������������� ��� ������ size ���)      
			{
				std::cout << "The list is over, select another insertion location" << std::endl;
				previous->node_mutex->unlock(); 
				current->node_mutex->unlock();
				return;
			}
		}
		Node* newNode; // ������ ����� ���� 
		newNode->node_mutex->lock(); // �����  ��� 
		newNode->value = value;  // �������� ���������� ������
		previous->next = newNode; // ����� ���� ������ ��������� ��� ����������� 
		newNode->next = current; // ��������� ��� ������ ���� ���������� ������� 
		previous->node_mutex->unlock();
		newNode->node_mutex->unlock();
		current->node_mutex->unlock();
	}

};