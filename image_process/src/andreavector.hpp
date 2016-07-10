#ifndef _ANDREAVECTOR_HPP
#define _ANDREAVECTOR_HPP

#include <assert.h>

#define	AND_VECTOR_OFFSET	1			// �i�H�ɭȪ��ƶq

namespace andrea{



template<class T>
class	vector
{
	private:
		T		*ptr;
		int		_size;
#if ( AND_VECTOR_OFFSET == 1 )
		bool	add_flag;			// �O�_���ɭȪ� flag
#else
		�ݭn��g�o�䪺code!!
#endif

	public:
		// �w�]�غc
		vector()
		{
			ptr			=	0;
			_size		=	0;
			add_flag	=	false;
		}
		// �Ѻc
		~vector()
		{
			delete [] ptr;
			ptr			=	0;
			_size		=	0;
			add_flag	=	false;
		}
		// �ƻs�غc
		vector( const andrea::vector<T> &v )
		{
			ptr			=	0;
			_size		=	0;
			add_flag	=	false;
			*this		=	v;
		}
		// ��size�غc
		vector( const int _s )
		{
			ptr			=	0;
			_size		=	0;
			add_flag	=	false;
			resize(_s);
		}
		// ����
		void	operator = ( const andrea::vector<T> &v )
		{
			if( _size != v._size )
				resize( v._size );
			if( _size > 0 )
				memcpy( ptr, v.ptr, _size*sizeof(T) );		// �����offset ���M�|��bug (�����|�ϥΨ�D�k�O����)
			// add_flag�bresize���a�谵��s.
		}
		// Ū����m
		T&	operator () ( int index )
		{
			assert( index < _size && ptr != 0 );
			return	*(ptr + index);		// �o�䤣�[�P�_ �O�]���o�˰��t�׷|����� (debug���U��assert�ױ�)
		}
		// Ū����m
		T&	operator [] ( int index )
		{
			assert( index < _size && ptr != 0 );
			return	*(ptr + index);
		}
		// size
		int		size()				{	return	_size;	}
		// ���s���o�O����Ŷ�
		void	resize( int _s )
		{
			assert( _s >= 0 );

			if( _s == 0 )
			{
				delete	[] ptr;
				ptr			=	0;
				_size		=	0;
				add_flag	=	false;
			}
			else if( _size != _s )
			{
				delete	[]	ptr;
				ptr			=	0;
				_size		=	_s;	// ���s���o size
				ptr			=	new	T[ AND_VECTOR_OFFSET + _size ];		// size���P���ɭԤ~���s���o�Ŷ�
				add_flag	=	false;									// ���s���o�Ŷ����ɭԤ~�Noffset_count�_��.  ���M�u��z�Lclear���ʧ@�ӲM���ɭ�.
				assert( ptr != 0 );
			}
		}
		// �k�s
		void	clear()
		{
			memset( ptr, 0,  _size*sizeof(T) );				// �u�bsize�d�򤺰��ʧ@ �קK�O����X��
		}
		// �ɭ�
		void	add_value( T _d )
		{
			assert( _size > 0 && ptr != 0 );		// �S��Ʊ��p����ɭ�
			assert( !add_flag );			// �S�ɭȤ~��i�ӳo��  �t��k�]�p�W  �T��ɭȨ⦸

			// �S�ɭ�  �i�H�ɭ�
			if( !add_flag )
			{
				*(ptr + _size)	=	_d;
				_size++;
				add_flag		=	true;
			}
		}
		// �M���ɭ�
		void	clear_add_value()
		{
			assert( _size > 0 && ptr != 0 );		// �S��Ʊ��p����ɭ�

// �p�G���\�h���ɭ� �o��ݭn�ק�!!
#if (AND_VECTOR_OFFSET == 1)		
			if( add_flag )
			{
				_size--;
				add_flag	=	false;
			}
#else
			THIS IS AVOID ADD_VECTOR_OFFSET > 0  need compile error!!
#endif
		}
		// ���O�@�ӽd�� ���w��
		void	set_value( int start, int end, T value )
		{
			assert( start >= 0 && end < _size && start < end && ptr != 0 );
			memset( ptr + start, value, (end-start)*sizeof(T) );
		}
		// �N�@�Ӱ}�C����Ʊ��b�᭱
		void	add_vector( int start, const andrea::vector<T> &v )
		{
			assert( v._size > 0 && start + v._size <= _size && start >= 0 );
			assert( v.ptr != 0 && ptr != 0 );

			memcpy( ptr + start, v.ptr, v._size*sizeof(T) );
		}
		// �N�@�Ӱ}�C����Ʊ��b�᭱  (���w�d��)  �Φb�R���ɭȸ��
		void	add_vector( andrea::vector<T> &v, int v_start, int v_end )
		{
			//assert( v._size > 0 && start + v._size <= _size && start >= 0 );
			assert( v.ptr != 0 && ptr != 0 );
			assert( v_start >= 0 && v_end <= v.size() && (v_end-v_start) <= _size );

			//memcpy( ptr + start, v.ptr, v._size*sizeof(T) );
			memcpy( ptr, v.ptr + v_start, (v_end-v_start)*sizeof(T) );
		}

};

















}

#endif