
#include <stdlib.h>
#include <iostream>

typedef struct Node
{
	int id;
	int age;
	char name[20];
	Node* next;
}*Node_Ptr;

//PROTOTYPES
void Traverse_List(Node_Ptr head);
Node_Ptr Insert_Node(int id, int age, char* name);
int Delete_Node(int id);

//GLOBALS
Node_Ptr head = nullptr, //head pointer to list
		 tail = nullptr; //tail pointer to list

//traverse the list, and print infomations of nodes
void Traverse_List(Node_Ptr head) {
	if (head == nullptr) {
		printf("\nLinked List is empty!\n");
		return;
	}
	while (head!=nullptr)
	{
		printf("\nNode Data: id=%d", head->id);
		printf("\nage=%d", head->age);
		printf("\nname=%s\n", head->name);

		head = head->next;
	}
}

//insert a new node after tail
Node_Ptr Insert_Node(int id, int age, char* name) {
	Node_Ptr new_node = (Node_Ptr)malloc(sizeof(Node));

	new_node->id = id;
	new_node->age = age;
	strcpy_s(new_node->name, name);
	new_node->next = nullptr;

	if (head == nullptr) {
		head = tail = new_node;
		return new_node;
	}
	else {
		tail->next = new_node;
		tail = new_node;
		return new_node;
	}
}

//this function deletes a node from the linked list given its id
int Delete_Node(int id) {
	Node_Ptr curr_ptr = head,//search the list
			 prev_ptr = head;//previous record

	if (head == nullptr) {
		return -1;
	}

	while (curr_ptr != nullptr&&curr_ptr->id != id) {
		prev_ptr = curr_ptr;
		curr_ptr = curr_ptr->next;
	}

	if (curr_ptr == nullptr)
		return -1;//could't find record

	if (tail == head) {
		head = nullptr;
		tail = nullptr;
		free(curr_ptr);
		return id;
	}
	else if (curr_ptr == head) {
		head = curr_ptr->next;
		free(curr_ptr);
		return id;
	}
	else if (curr_ptr == tail) {
		prev_ptr->next = nullptr;
		free(curr_ptr);
		return id;
	}
	else {
		prev_ptr->next = curr_ptr->next;
		free(curr_ptr);
		return id;
	}
}

// MAIN //////////////////////////////////////////////
void main(void)
{

	int done = 0, // exit flag
		node_num = 0, // node number, used for id
		sel = 0; // used for input

				 // main event loop
	while (!done)
	{
		// display menu
		printf("\nLinked List Manipulation Menu\n");
		printf("\n1 - Display linked list.");
		printf("\n2 - Insert new node.");
		printf("\n3 - Delete node.");
		printf("\n4 - Exit Program.");
		printf("\n\nSelect one please?");

		// get selection
		std::cin >> sel;

		// what to do
		switch (sel)
		{
		case 1:
		{
			// print out the list
			Traverse_List(head);
		}  break;

		case 2:
		{
			// locals to do the insertion
			char name[32];
			int age;

			// get the info
			printf("\nNew node entry form:\n");
			printf("\nName?");
			std::cin >> name;
			printf("\nAge?");
			std::cin >> age;

			// insert the node
			Insert_Node(node_num++, age, name);

		} break;

		case 3:
		{
			int id; // id to delete
			printf("\nEnter id number of node to delete?");
			std::cin >> id;

			// delete the node
			Delete_Node(id);

		} break;


		case 4:
		{
			done = 1;
		} break;

		default: break;

		} // end switch

	} // end while

} // end main

