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
		int curPos = 2; // инизиализируем текущую позицию второй в списке (первая возможная при вставке в середину списка)
		Node* previous, * current; //  временные переменные для обхода узлов
		list_mutex->lock();               // лочим весь список  
		previous = this->head;        // предущий узел указывает на "корень" 
		current = this->head->next; // текущий указывает на следующий за "корнем" 
		previous->node_mutex->lock(); // лочим "корень"
		list_mutex->unlock();         // разлочиваем весь список
		if (current) // если элементов больше одного тогда лочим 
			current->node_mutex->lock();
		while (curPos < pos) // перемещаемся по списку пока не найдём нужную позицию дя вставки 
		{
			Node* oldPrevios = previous;
			previous = current;
			current = current->next;
			oldPrevios->node_mutex->unlock();
			if (current) // проверили и только потом залочили
			{
				current->node_mutex->lock();
			}
			else             // если мы хотим добавить позицию которая больше списка   (предполагается что метода size нет)      
			{
				std::cout << "The list is over, select another insertion location" << std::endl;
				previous->node_mutex->unlock(); 
				current->node_mutex->unlock();
				return;
			}
		}
		Node* newNode; // создаём новый узел 
		newNode->node_mutex->lock(); // лочим  его 
		newNode->value = value;  // помещаем переданные данные
		previous->next = newNode; // новый узел делаем следующим для предыдущего 
		newNode->next = current; // следующим для нового узла становится текущий 
		previous->node_mutex->unlock();
		newNode->node_mutex->unlock();
		current->node_mutex->unlock();
	}

};