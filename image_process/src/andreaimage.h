#ifndef _ANDREAIMAGE_H
#define _ANDREAIMAGE_H

#include <cassert>
#include <string>




// class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class	AndreaImage
{
	public:
		typedef int (AndreaImage::*AndreaChannel)(int,int) const;									// �w�qŪ���q�D

		enum FORMAT
		{
			BGR		=	0,		// �w�]��
			RGB,
			GRAY
		};

		AndreaImage();
		AndreaImage( unsigned char *_ptr, int _w, int _h, FORMAT _format = BGR );					// �غc ��J�e ��
		AndreaImage( AndreaImage &a );																// �ƻs�غc
		~AndreaImage();

		void	operator = ( AndreaImage &a );														// ����B��
	
		void			set( unsigned char *_ptr, int _w, int _h, FORMAT _format = BGR );			// �]�w
		int				width();
		int				height();

		int				red_channel( int i, int j ) const;											// �^�Ǭ���q�D 
		int				green_channel( int i, int j ) const;										// �^�Ǻ��q�D
		int				blue_channel( int i, int j ) const;											// �^���Ŧ�q�D
		int				gray_channel( int i, int j ) const;											// �^�ǦǶ�
		void			red_channel( int i, int j, int r );											// �g�J����q�D
		void			green_channel( int i, int j, int g );										// �g�J���q�D
		void			blue_channel( int i, int j, int b );										// �g�J�Ŧ�q�D
		void 			gray_channel( int i, int j, int gray ); 									// �g�J�Ƕ�
		void			pixel_channel( int i, int j, int r, int g, int b );							// �g�JRGB
		void			fill( const unsigned char _value );											// �N��ƥ����]�����w����
		FORMAT			get_format();																// �^�Ǯ榡 (RGB BGR gray)

		void			save( char *filename );														// �g�J�ɮ�  �ثe�S���Ʊ�


	private:
		unsigned char	*ptr;
		int				w;
		int				h;
		int				size;
		FORMAT			format;


};







#endif