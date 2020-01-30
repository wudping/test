#include "linklist.h"

#include <cassert>
#include <iostream>

using namespace std;



LinkList::LinkList()
{}


LinkList::~LinkList()
{
	Node	*node	=	head;

	while( node != nullptr )
	{
		node	=	node->next;
		delete	head;
		head	=	node;
	}

}
	

LinkList::LinkList(const LinkList& list)
{}


int		LinkList::size() const
{
	return _size;
}



void	 LinkList::push_back( int data )
{
	Node	*new_node	=	new Node;
	assert( new_node != nullptr );

	new_node->data	=	data;
	new_node->next	=	nullptr;

	if( head == nullptr )
		head	=	new_node;

	if( end == nullptr )
		end		=	new_node;
	else
	{
		end->next	=	new_node;
		end			=	end->next;
	}

	_size++;
}



void	LinkList::print()
{
	cout << "size = " << _size << endl;

	if( head == nullptr )
	{
		cout << "null list." << endl;
		return;
	}

	Node	*node	=	head;
	while( node != nullptr )
	{
		cout << node->data << " ";
		node = node->next;
	}	

	cout << endl << endl;

}



void	LinkList::insert( int index, int data )
{
	assert( index < _size );

	Node	*node	=	head;
	int		i;

	assert( node != nullptr );

	for( i = 0; i < index - 1; i++ )
		node	=	node->next;
	
	Node	*new_node	=	new	Node;
	new_node->data		=	data;
	new_node->next		=	node->next;
	node->next			=	new_node;

	_size++;
}
