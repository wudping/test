#include "stdafx.h"
#include "watershed.h"

#include <QImage>



static const POINT_W		FICTITIOUS(-1,-1);						// �����I
static const POINT_W		W(-1,-1);								// alg 4.8 �ΨӼХ� Watershed�Ϊ�. �����ݩ����@�ӹ�ڪ��I
//static const POINT		WW(-2,-2);
static const int		INIT	=	-1;							// minima detetor 
static const int		WSHED	=	0;							// mark �� watershed. ������L��label�_�Ĭ�
static const int		MASK	=	-2;
static const int		CON		=	4;							// �s�q��.  4 �� 8


/*=======================================================================================================
	struct point �� == �ާ@
/*=======================================================================================================*/
bool operator == ( POINT_W a, POINT_W b )
{
		if( a.x == b.x && a.y == b.y )
			return	true;
		else
			return	false;
}

/*=======================================================================================================
	= operator
/*=======================================================================================================*/
POINT_W		POINT_W::operator	=	( POINT_W b )
{
	x		=	b.x;
	y		=	b.y;

	return		*this;
}

/*=======================================================================================================
	!= operator
/*=======================================================================================================*/
bool operator	!=	( POINT_W a, POINT_W b )
{
	return	!(a == b);
}




/*=================================================================================
	�w�]function
===================================================================================*/
Watershed::~Watershed()
{}


#ifdef _OPENPIV_

/*=======================================================================================================
	�غc
	OPENPIV ���U�ϥ�
/*=======================================================================================================*/
Watershed::Watershed(Settings *settingsPass, DataContainer  *filedataPass)	
	: AndreaEngine()
{	
	progress	=	settingsPass->getProgress();	
	settings	=	settingsPass;
	filedata	=	filedataPass;

    mutex.lock();
    mutex.unlock();
}

#else

/*=======================================================================================================
	�غc
	�L�ǤJ��
/*=======================================================================================================*/
Watershed::Watershed()	
{
	image.load("test.bmp");
	final.load("test.bmp");
}

#endif




/*=======================================================================================================
	NG �D�����ǾF�~
/*=======================================================================================================*/
void	Watershed::NG( POINT_W p, POINT_W *pointlist, int &size, int width, int height )
{
	int		count	=	0;

	if( p.y > 0 )
		pointlist[count++]	=	POINT_W( p.x, p.y-1 );

	if( p.y < height-1 )
		pointlist[count++]	=	POINT_W( p.x, p.y+1 ); 

	if( p.x > 0 )
		pointlist[count++]	=	POINT_W( p.x-1, p.y );

	if( p.x < width-1 )
		pointlist[count++]	=	POINT_W( p.x+1, p.y );

	size	=	count;
}




/*=======================================================================================================
	��X
	�ɦW = output.bmp
/*=======================================================================================================*/
void	Watershed::output()
{
	final.save("output.bmp");
}



/*=======================================================================================================
	lower completion
	�ഫ��lower complete����
/*=======================================================================================================*/
void	Watershed::LowerCompletion()
{
	boost::queue<POINT_W>		fifo_queue;

	QRgb			rgb1,	rgb2;
	int				gray1,	gray2;

	POINT_W			point,	q;
	POINT_W			pointlist[4];				// �ΨӳB�zNG �Ҧ�NG�Φ���set ���p�n�Ҽ{8��V�h�ݭn�ק�


	lc.resize( image.width(), image.height() );

	int		i,	j;
	int		dist;
	int		size;

	for( i=0; i<image.width(); i++ )
		for( j=0; j<image.height(); j++ )
		{
			lc(i,j)		=	0;

			if( has_low_neib( i, j ) )
			{
				lc( i, j )	=	-1;
				point		=	POINT_W( i ,j );
				fifo_queue.push(point);
			}
		}


	dist	=	1;

	fifo_queue.push( FICTITIOUS );			// �����I

	while( !fifo_queue.empty() )
	{
		point	=	fifo_queue.top();		// remove���Ӱʧ@
		fifo_queue.pop();

		if( point == FICTITIOUS )
		{
			if(  !fifo_queue.empty() )
			{
				fifo_queue.push( FICTITIOUS );
				dist++;
			}
		}
		else
		{
			lc( point.x, point.y )	=	dist;
			NG( point, pointlist, size, image.width(), image.height() );

			for( i=0; i<size; i++ )
			{
				q		=	pointlist[i];

				gray1	=	qGray( image.pixel(q.x, q.y) );
				gray2	=	qGray( image.pixel(point.x, point.y ) );

				//if( image.gray_channel( q.x, q.y ) == image.gray_channel( point.x, point.y ) && lc( q.x, q.y ) == 0 )
				if( gray1 == gray2 && lc( q.x, q.y ) == 0 )
				{
					fifo_queue.push( q );
					lc( q.x, q.y )	=	-1;
				}
			}
		}
	}



	for( i=0; i<image.width(); i++ )
		for( j=0; j<image.height(); j++ )
		{
			gray1	=	qGray( image.pixel(i,j) );

			if( lc(i,j) != 0 )
				//lc(i,j)		=	dist * (int)image.gray_channel(i,j) + lc(i,j) - 1;// + max  ;
				lc(i,j)		=	dist * (int)gray1 + lc(i,j) - 1;// + max  ;
			else
				//lc(i,j)		=	dist * (int)image.gray_channel(i,j) ;//+ max;
				lc(i,j)		=	dist * (int)gray1 ;
		}


}


/*=======================================================================================================
	�O�_����ۤv�p���F�~
/*=======================================================================================================*/
bool	Watershed::has_low_neib( int i, int j )
{
	QRgb	rgb1,	rgb2;
	int		gray1,	gray2;


	rgb1	=	image.pixel( i, j );
	gray1	=	qGray(rgb1);


	//int		self	=	image->gray_channel( i, j );
	int		self	=	gray1;

	POINT_W	up		=	POINT_W( i , j > 0 ? j-1 : j  );
	POINT_W	down	=	POINT_W( i , j < image.height()-1 ? j+1 : j   );
	POINT_W	left	=	POINT_W( i > 0 ? i-1 : i , j );
	POINT_W	right	=	POINT_W( i < image.width()-1 ? i+1 : i ,  j );

/*	if( image->gray_channel( up.x, up.y ) < self ||
		image->gray_channel( down.x, down.y ) < self ||
		image->gray_channel( left.x, left.y ) < self ||
		image->gray_channel( right.x, right.y ) < self  )*/
	if( qGray( image.pixel(up.x, up.y) ) < self ||
		qGray( image.pixel( down.x, down.y ) ) < self ||
		qGray( image.pixel( left.x, left.y ) ) < self ||
		qGray( image.pixel( right.x, right.y ) ) < self  )
			return	true;
	else
			return	false;

}


/*=======================================================================================================
	�O�_����ۤv�p���F�~
	alg 4.1 �ϥΪ���
/*=======================================================================================================*/
bool	Watershed::has_low_neib_2( QImage *image, int i, int j )
{
	if( j>0 )
	{
		if( lab(i,j-1) > 0 || lab(i,j-1) == WSHED )
			return	true;
		else
			return	false;
	}

	if( j<image->height()-1 )
	{
		if( lab(i,j+1) > 0 || lab(i,j+1) == WSHED )
			return	true;
		else
			return	false;
	}
	if( i>0 )
	{
		if( lab(i-1,j) > 0 || lab(i-1,j) == WSHED )
			return	true;
		else
			return	false;
	}
	if( i<image->width()-1 )
	{
		if( lab(i+1,j) > 0 || lab(i+1,j) == WSHED )
			return	true;
		else
			return	false;
	}

	return	false;
}



/*=======================================================================================================
	sln   (��ڤW�]�i�H���@���s�b�}�C�̭�)
/*=======================================================================================================*/
void	Watershed::sln_maker()
{
		// !!! �`�N�o�䪺index i �O 1~4  �Ӥ��O0~3
	int		width	=	lc.size1();
	int		height	=	lc.size2();
	int		min		=	INT_MAX;
	int		up,	down,	left,	right;
	int		i,	j;
	int		count;
	int		ii,	jj;


	POINT_W	p,	ptmp;
	POINT_W	plist[4];

	// !!! �`�N�o�䪺index i �O 1~4  �Ӥ��O0~3
	sln.resize( width*height, 5 );

	for( ii=0; ii<width; ii++ )
		for( jj=0; jj<height; jj++ )
		{
			min			=	INT_MAX;
			p			=	POINT_W(ii,jj);

			plist[0]	=	POINT_W(p.x,p.y-1);		// �W
			plist[1]	=	POINT_W(p.x,p.y+1);		// �U
			plist[2]	=	POINT_W(p.x-1,p.y);		// ��
			plist[3]	=	POINT_W(p.x+1,p.y);		// �k

			// sort plist by lc
			for( i=0; i<4; i++ )
				for( j=0; j<3-i; j++ )
				{
					// �I�b�d��
					if( plist[j].x >= 0 && plist[j].x < width && plist[j].y >= 0 && plist[j].y < height )
					{
						if( plist[j+1].x >= 0 && plist[j+1].x < width && plist[j+1].y >= 0 && plist[j+1].y < height )
						{
							if( lc( plist[j].x, plist[j].y ) > lc(plist[j+1].x, plist[j+1].y) )
							{
								ptmp		=	plist[j];
								plist[j]	=	plist[j+1];
								plist[j+1]	=	ptmp;
							}
						}
						//else{}   ��ӳ����,�~�����. ���M�N�����洫�F.
					}
					// �I�b�d��~ �������̤j�I
					else
					{
						ptmp		=	plist[j];
						plist[j]	=	plist[j+1];
						plist[j+1]	=	ptmp;	
					}

				}

			for( i=0; i<4; i++ )
			{
				if( plist[i].x >= 0 && plist[i].x < width && plist[i].y >= 0 && plist[i].y < height )
				{
					// !!! �`�N�o�䪺index i �O 1~4  �Ӥ��O0~3
					if( lc(p.x,p.y) > lc( plist[i].x,plist[i].y )  )
						sln( p.x*height + p.y, i+1 )	=	plist[i];
					else if( i == 0 )
						sln( p.x*height + p.y, 1 )		=	p;
					else
						sln( p.x*height + p.y, i+1 )	=	p;
				}
				else
					sln( p.x*height + p.y, i+1 )	=	p;
			}


#if 0
			up		=	p.y > 0             ?   lc( p.x, p.y-1 )        :   INT_MAX;
			down	=	p.y < height - 1    ?   lc( p.x, p.y+1 )	    :   INT_MAX;
			left	=	p.x > 0             ?   lc( p.x-1, p.y )        :   INT_MAX;
			right	=	p.x < width - 1     ?   lc( p.x+1, p.y )		:   INT_MAX;

			// �j�M�̤p��
			if( up < min )		min		=	up;
			if( down < min )	min		=	down;
			if( left < min )	min		=	left;
			if( right < min )	min		=	right;

			// �N��Ʀs��� sln�̭�
			count	=	1;
			// �קK�j�a���O INIT_MAX�����p. �H�άy��۹�C�I�����p
			/*if( min < INT_MAX && min < lc(p.x, p.y) )
			{
				if( up == min )		sln( p.x*height + p.y , count++ )	=	POINT(p.x, p.y-1 );
				if( down == min )	sln( p.x*height + p.y , count++ )	=	POINT(p.x, p.y+1 );
				if( left == min )	sln( p.x*height + p.y , count++ )	=	POINT(p.x-1, p.y );
				if( right == min )	sln( p.x*height + p.y , count++ )	=	POINT(p.x+1, p.y );
			}*/

			if( min < INT_MAX /*&& min < lc(p.x, p.y)*/ )
			{
				if( up < lc(p.x, p.y) )		sln( p.x*height + p.y , count++ )	=	POINT(p.x, p.y-1 );
				if( down < lc(p.x, p.y) )	sln( p.x*height + p.y , count++ )	=	POINT(p.x, p.y+1 );
				if( left < lc(p.x, p.y) )	sln( p.x*height + p.y , count++ )	=	POINT(p.x-1, p.y );
				if( right < lc(p.x, p.y) )	sln( p.x*height + p.y , count++ )	=	POINT(p.x+1, p.y );
			}

			// �ɨ��ѤU����.  (���V�ۤv  ��ܦۤv�N�O�ۤv��parent, �ۤv�N�O root)
			for( ; count<=4; count++ )
				sln( p.x*height + p.y , count )	=	p;
#endif
		}


	// !!! �`�N�o�䪺index i �O 1~4  �Ӥ��O0~3
}



/*=======================================================================================================
	���j�ϥΪ� Resolve
/*=======================================================================================================*/
POINT_W	Watershed::Resolve( POINT_W p )
{
	int			i		=	1;	
	int			j;
	const int	index	=	p.x * lc.size2() + p.y;
	POINT_W		rep(0,0),	ptmp;

	while( i<=CON && rep != W )
	{
		ptmp	=	sln(index,i);

		/*if( ptmp == p )
		{
			if( i == 1 )
			{
				rep	=	sln(index,1);
				break;
			}
		}*/

		if( sln(index,i) != p && sln(index,i) != W )
			sln(index,i)	=	Resolve( sln(index,i) );

		POINT_W	ptmp2	=	sln(index,i);

		if( i == 1 )
			rep		=	sln( index, 1 );
		
		else if( ptmp2 == W )
		{
			rep		=	W;
			for( j=1; j<=CON; j++ )
				sln(index,j)	=	W;
		}

		else if( ptmp2 == p )
		{
			if( i == 1 )
			{
				rep		=	p;
				break;
			}
			/*else if( lab( ptmp2.x, ptmp2.y ) != lab( rep.x, rep.y ) )
			{
				rep		=	W;
				for( j=1; j<=CON; j++ )
					sln(index,j)	=	W;
			}*/
		}

		//else if( sln(index,i) != rep )
		else if( lab( ptmp2.x, ptmp2.y ) != lab( rep.x, rep.y ) )
		{
			rep		=	W;
			for( j=1; j<=CON; j++ )
				sln(index,j)	=	W;
		}

		i++;
	}

	return	rep;
}





/*=======================================================================================================
	����watershed
	�פ媺alg4.8  
	�λ��j���覡
/*=======================================================================================================*/
void	Watershed::doWatershed()
{
	int			width	=	lc.size1();
	int			height	=	lc.size2();
	int			i,	j;
	POINT_W		rep;
	POINT_W		p;


	// �إ�sln table
	sln_maker();

	for( i=0; i<width; i++ )
		for( j=0; j<height; j++ )
		{
			//if( i == 260 && j == 290 )
			//	std::cout << "doing " << i << " " << j << "\n";

			p		=	POINT_W(i,j);
			rep		=	Resolve( p );
			if( rep != W )
				lab( p.x, p.y )		=	lab( rep.x, rep.y );
			else
				lab( p.x, p.y )		=	WSHED;
		}

	//std::cout << "finished...\n";

	// �N��Ʀ^�g��Ϥ�
	for( i=0; i<image.width(); i++ )
		for( j=0; j<image.height(); j++ )
		{
			if( lab(i,j) == WSHED )
				//image.pixel_channel( i, j, 255, 0, 0 );
				final.setPixel( i, j, qRgb(255,0,0) );			// �Nwatershed�e������
				//	image.gray_channel( i, j, lab(i,j)*10 );
			//else
			//	final.gray_channel( i, j, lab(i,j)*10 );		// ��L���̷Ӥ��Plab�ӵe�X���P�C��

			// ��j�����Plab���]�e�W�C�� ( �קK�u�_��)
			if( i>=1 && i<image.width()-1 && j>=1 && j<image.height()-1 )
			{
				if( lab( i,j) != lab(i+1,j) || lab(i,j) != lab(i-1,j) || lab(i,j) != lab(i,j+1) || lab(i,j) != lab(i,j-1 ) )
					final.pixel_channel( i, j, 255, 0, 0 );
			}
		}

}


/*=======================================================================================================
	minima detetor
	��X�̤p��  �åB�Хܤ�
	(���ӬO��lower complete����)
/*=======================================================================================================*/
void	Watershed::LevelComponents()
{
	lab.resize( image.width(), image.height() );

	boost::queue<POINT_W>			fifo_queue;

	POINT_W		p,	q,	s,	pointlist[4];

	QRgb	rgb1,	rgb2;
	int		gray1,	gray2;


	int				i,	j,	k;
	int				size;
	unsigned int	curlab;			// current label

	for( i=0; i<image.width(); i++ )
		for( j=0; j<image.height(); j++ )
			lab(i,j)	=	INIT;

	curlab		=	1;

	for( i=0; i<image.width(); i++ )
		for( j=0; j<image.height(); j++ )
		{
			if( lab(i,j) == INIT )
			{
				lab(i,j)	=	curlab;
				p			=	POINT_W(i,j);
				fifo_queue.push( p );

				while( !fifo_queue.empty() )
				{
					s	=	fifo_queue.top();
					fifo_queue.pop();

					NG( s, pointlist, size, image.width(), image.height() );

					for( k=0; k<size; k++ )
					{
						q	=	pointlist[k];

						gray1	=	qGray( image.pixel(s.x,s.y) );
						gray2	=	qGray( image.pixel( q.x, q.y ) );

						//if( lc( s.x, s.y ) == lc( q.x, q.y ) )
						//if( image.gray_channel(s.x, s.y) == image.gray_channel( q.x, q.y ) )
						//if( abs( image.gray_channel(s.x, s.y) - image.gray_channel( q.x, q.y ) ) < 4 )
						if( gray1 == gray2 )
						//if( abs( gray1 - gray2 ) < 5 )
						{
							if( lab( q.x, q.y ) == INIT )
							{
								lab( q.x, q.y )	=	curlab;
								fifo_queue.push( q );
							}
						}
					}

				}
				curlab++;
			}
		}


	/*QImage	ILabel( image.width(), image.height(), QImage::Format_RGB888 );
	for( i = 0; i < ILabel.width(); i++ )
		for( j = 0; j < ILabel.height(); j++ )
			ILabel.setPixel( i, j, lab(i,j) );
	ILabel.save("label.bmp");*/

}



/*=======================================================================================================
	�̭�l��watershed   alg 4.1
/*=======================================================================================================*/
void	Watershed::doWatershed_4_1()
{
	int		curlab	=	0;
	int		i,	j;
	int		ii,	jj;
	
	boost::queue<POINT_W>		fifo_queue;

	int		width	=	image.width();
	int		height	=	image.height();

	lab.resize( width, height );
	dist.resize( width, height );

	for( i=0; i<width; i++ )
		for( j=0; j<height; j++ )
		{
			lab(i,j)	=	INIT;
			dist(i,j)	=	0;
		}

	// sort by gray value
	typedef boost::heap::priority_queue<int>	PriorityQueue;
	
	PriorityQueue	pq;

	//PriorityQueue::iterator		begin	=	sort_data.begin();
	//PriorityQueue::iterator		end		=	sort_data.end();
	//boost::heap::priority_queue<int>::iterator	itr;
	bool	is;
	for( i=0; i<width; i++ )
		for( j=0; j<height; j++ )
		{
			is	=	true;
			for( PriorityQueue::iterator itr = pq.begin();  itr!=pq.end();  itr++  )
			{
				//if( *itr == image.gray_channel(i,j) )
				if( *itr == qGray( image.pixel(i,j) ) )
				{
					is	=	false;
					break;
				}
			}

			if( is )
				pq.push(  qGray( image.pixel(i,j) ) );

		}

	sort_data.resize( pq.size() );

	//while( !pq.empty() )
	for( i=sort_data.size()-1; i>=0; i-- )
	{
		//std::cout << pq.top() << " ";
		sort_data(i)	=	pq.top();
		pq.pop();
	}
	// output
	for( i=0; i<sort_data.size(); i++ )
		std::cout << sort_data(i) << " ";
	std::cout << std::endl;
	




	int			curdist,	size;
	int			h;
	POINT_W		p,	q,	r;
	POINT_W		pointlist[4];

	for( ii=0; ii<sort_data.size(); ii++ )
	{

		h	=	sort_data(ii);
		std::cout << h << "... \n";

		for( i=0; i<width; i++ )
			for( j=0; j<height; j++ )
			{
				if(  qGray( image.pixel(i,j) ) == sort_data(ii) )
				{
					lab(i,j)	=	MASK;
					if( has_low_neib_2( &image, i,j ) )
					{
						dist(i,j)	=	1;
						fifo_queue.push( POINT_W(i,j) );
					}
				}
			}
		curdist	=	1;
		fifo_queue.push( FICTITIOUS);
		

		while(1)
		{
			p	=	fifo_queue.top();
			fifo_queue.pop();

			if( p == FICTITIOUS )
			{
				if( fifo_queue.empty() )
					break;
				else
				{
					fifo_queue.push( FICTITIOUS );
					curdist++;
					p	=	fifo_queue.top();
					fifo_queue.pop();
				}

			}


			NG( p, pointlist, size, width,  height );

			for( i=0; i<size; i++ )
			{
				q	=	pointlist[i];
				if( dist( q.x, q.y ) < curdist && ( lab(q.x,q.y)>0 || lab(q.x,q.y) == WSHED ) )
				{
					if( lab(q.x,q.y)>0 )
					{
						if( lab(p.x,p.y) == MASK || lab(p.x,p.y) == WSHED )
							lab(p.x,p.y)	=	lab(q.x,q.y);
						else if( lab(p.x,p.y) != lab(q.x,q.y) )
							lab(p.x,p.y)	=	WSHED;
					}
					else if( lab(p.x,p.y) == MASK )
						lab(p.x,p.y)	=	WSHED;
				}
				else if( lab(q.x,q.y) == MASK && dist(q.x,q.y) == 0 )
				{
					dist(q.x,q.y)	=	curdist + 1;
					fifo_queue.push(q);
				}
			}		
		}		// end whlie


	
		for( i=0; i<width; i++ )
		{
			for( j=0; j<height; j++ )
			{
				if( qGray( image.pixel(i,j) ) == h )
				{
					p	=	POINT_W(i,j);
					dist(p.x,p.y)	=	0;
					if( lab(p.x,p.y) == MASK )
					{
						curlab	=	curlab + 1;
						fifo_queue.push(p);
						lab(p.x,p.y)	=	curlab;

						while( !fifo_queue.empty() )
						{
							q	=	fifo_queue.top();
							fifo_queue.pop();

							NG( q, pointlist, size, width, height );
							for( jj=0; jj<size; jj++ )
							{
								r	=	pointlist[jj];
								if( lab(r.x,r.y) == MASK )
								{
									fifo_queue.push(r);
									lab(r.x,r.y)	=	curlab;
								}
							}
						}

					}
				}
			}
		}

	}	// end for( ii=0; ii<sort_data.size(); ii++ )




	for( i=0; i<width; i++ )
		for( j=0; j<height; j++ )
		{
			if( lab(i,j) == WSHED )
				final.setPixel( i, j, qRgb(255,0,0) );
				//image.pixel_channel(i,j,255,0,0);
			else
				final.setPixel( i, j,lab(i,j)*10);
			//	image.gray_channel( i, j, lab(i,j)*10 );

			// �N���P�϶������e�u (²���N�u�s�_��)
			if( i>=1 && i<width-1 && j>=1 && j<height-1 )
			{
			//	if( lab(i,j) != lab(i+1,j) || lab(i,j) != lab(i-1,j) || lab(i,j) != lab(i,j+1) || lab(i,j) != lab(i,j-1) )
			//		final.pixel_channel( i, j, 255, 0, 0 );
			}

		}


}







/*=======================================================================================================
	�ϥ�
/*=======================================================================================================*/
void	Watershed::highlight()
{
	int		gray;
	int		i,	j;

	for( i=0; i<image.width(); i++ )
	{
		for( j=0; j<image.height(); j++ )
		{
			gray	=	qGray( image.pixel(i,j) );
			gray	=	255 - gray;
			image.setPixel( i, j, qRgb(gray,gray,gray) );
		}
	}
}


/*=======================================================================================================
	�N�ϧΰ�truncate   
	i+-5 ���ܦ� i ������
	�n���X�Ӱ϶��h�ݭn�]�p
/*=======================================================================================================*/
QImage	Watershed::truncate( int n )
{
	QImage	img( image.width(), image.height(), QImage::Format_RGB888 );
	img.fill(0);

	const int	N	=	256/n;			// ���j�� 
	int			i,	j;
	int			mag;

	for( i=0; i<image.width(); i++ )
		for( j=0; j<image.height(); j++ )
		{
			mag		=	image.gray_channel( i, j );
			mag		=	(mag/N) * N;
			img.gray_channel( i, j, mag );
		}

	img.save("truncate.bmp");
	return	img;
}




/*=======================================================================================================
	����gradient image    sqrt( fx^2 + fy^2 )
/*=======================================================================================================*/
QImage	Watershed::gradient()
{
	int		i,	j;
	double	fx,	fy;
	double	mag;

	QImage	img( image.width(), image.height(), QImage::Format_RGB888 );
	img.fill(0);	// �k�s

	for( i=1; i<image.width()-1; i++ )
		for( j=1; j<image.height()-1; j++ )
		{
			fx	=	0.5 * ( image.gray_channel(i+1,j) - image.gray_channel(i-1,j) );
			fy	=	0.5 * ( image.gray_channel(i,j+1) - image.gray_channel(i,j-1) );
			mag	=	sqrt( fx*fx + fy*fy );

			img.gray_channel( i, j, mag );
		}
	img.save("gradient.bmp");
	return	img;
}

/*=======================================================================================================
	�N�ϧΤ��}��  
	��Х�
/*=======================================================================================================*/
QImage	Watershed::divide( QImage	imagePass )
{
	int		i,	j;

	GaussFilter_2D	gf(1,1);		// ��filter

	//Watershed	watershed;
	image	=	imagePass;
	final	=	image;
	
	image	=	gf.do_filter( image, progress );
	image	=	gradient();			// �N�ϧΰ�gradient  ��X��

	// ���U�O�쥻��4.8  �e���O�w�B�z
	LowerCompletion();
	LevelComponents();
	doWatershed();


	// ����label��T  (������˴��ϥ�)
	for( i=0; i<image.width(); i++ )
		for( j=0; j<image.height(); j++ )
		{
			if( lab(i,j) == WSHED )
				final.pixel_channel( i, j, 255, 255, 255 );				// ���pcurrenlab�]��զ�  ���N�@�w�|�o�Ϳ��~
			else
				final.setPixel( i, j, lab(i,j) );
				//final.pixel_channel( i, j, lab(i,j) , 0, 0 );
		}
	//final.save("watersh48.bmp");

	return	final;
}



#ifdef _OPENPIV_
/*=======================================================================================================
	�Q�γo����Nmain  ����U�ذʧ@ 
/*=======================================================================================================*/
void	Watershed::dosomething()
{
	GaussFilter_2D	gf(1,1);		// ��filter

	//Watershed	watershed;
	image	=	filedata->get_image();
	final	=	image;

	// �O�_�n�ϥ�
	if( settings->isHighlight )
		highlight();

	// ��gradient
	//gradient();
	// �o�䪺�t��k�O  ���N�ϧΰ���gradient map, �A�Q��gradient�h��watershed. �Ω�4.8 ���A��MSER
	//image	=	gradient();
	

	switch( settings->watershed )
	{
		case OpenPIV::i4_8 :

			image	=	gf.do_filter( image, progress );
			//image	=	truncate(8);		// �N�ϧΰ�truncate, �קK�L�h�����T   �ǤJ�Ȫ�ܧƱ���Φ��X��level
			image	=	gradient();			// �N�ϧΰ�gradient  ��X��

			// ���U�O�쥻��4.8  �e���O�w�B�z
			LowerCompletion();
			LevelComponents();
			doWatershed();
			break;

		case OpenPIV::i4_1 :

			image	=	gf.do_filter( image, progress );
			//image	=	truncate(8);		// �N�ϧΰ�truncate, �קK�L�h�����T   �ǤJ�Ȫ�ܧƱ���Φ��X��level
			image	=	gradient();			// �N�ϧΰ�gradient  ��X��

			// ���U�O�쥻4.1
			doWatershed_4_1();
			break;

		case OpenPIV::MSER :
			final	=	mser_process(image);
			break;

		default:
			break;
	}

	//watershed.doWatershed_4_1();
	//	mser_process();
	//emit( filedata->output(final) );
	emit( progress->outputImage( 0, final ) );
	progress->debuger("<font color=\"Red\"> finish watershed blob detector... </font>");

}

#endif