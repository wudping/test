/*=================================================================================
	
	�ΨӰ���blob

===================================================================================*/

#include "stdafx.h"
#include "blob.h"


using namespace std;


#ifdef _OPENPIV_
/*=================================================================================
	�غc�禡
===================================================================================*/
SUBPIX_BLOB::SUBPIX_BLOB( Settings *settingsPass, DataContainer  *filedataPass )
	: AndreaEngine()

{
	settings	=	settingsPass;

	scale_w		=	settings->get_scale_w();	//_w;   �w�]��
	scale_l		=	settings->get_scale_l();	//_l;
	progress	=	settings->getProgress();	

	filedata	=	filedataPass;

    mutex.lock();
    mutex.unlock();

}
#else
/*=================================================================================
	�غc�禡
===================================================================================*/
SUBPIX_BLOB::SUBPIX_BLOB(  )
{}
#endif



/*=================================================================================
	�w�]function
===================================================================================*/
SUBPIX_BLOB::~SUBPIX_BLOB()
{}


/*=================================================================================
	����curvature  
	���w�d��  
	���W �k�U
===================================================================================*/
void		SUBPIX_BLOB::curvature( PointInt lu, PointInt rd )
{
	int			i,	j;
	int			x,	y;
	int			gray1,	gray2;
	double		rx,	ry;
	double		dtmp;

	curva_map.resize( image.width(), image.height() );
	curva_map.clear();

	for( x = lu.x+1;  x <= rd.x-1;   x++ )
	{
		for(  y = lu.y+1;   y <= rd.y-1 ; y++ )
		{
			gray1	=	image.gray_channel( x+1, y );
			gray2	=	image.gray_channel( x-1, y );
			rx		=	0.5 * ( gray1 - gray2 );

			gray1	=	image.gray_channel( x, y+1 );
			gray2	=	image.gray_channel( x, y-1 );
			ry		=	0.5 * ( gray1 - gray2 );

			dtmp	=	sqrt( rx*rx + ry*ry );

			curva_map( x, y )	=	dtmp;

		}
	}
	
	output( curva_map, "curvature.bmp");
}


/*=================================================================================
	���ե�
	��corner
===================================================================================*/
void		SUBPIX_BLOB::corner( int index )
{
	int			i,		j;
	int			cx,		cy;				// �����I
	int			max_i,	max_j;
	int			point_x[4];
	int			point_y[4];
	double		sub_x[4];
	double		sub_y[4];
	double		fvv,	fw;
	double		dx,		dy;
	double		dtmp1,	dtmp2;
	double		max;
	double		sum;

	ifstream	in("BMP/center.txt");
	in >> dtmp1 >> dtmp2;
	in.close();

	cx	=	(int) dtmp1;
	cy	=	(int) dtmp2;


	boost::numeric::ublas::matrix<double>	cor;
	cor.resize( image.width(), image.height() );
	cor.clear();

	// �Q�� njet �����k�Ӱ���corner
	Njet	njet(image);
	for( i = cx - scale_w - 10;   i <= cx + scale_w + 10;   i++ )
		for( j = cy - scale_w - 10;   j <= cy + scale_w + 10;   j++ )
		{
			fvv		=	njet.get_fvv( i, j );
			fw		=	njet.get_fw( i, j );
			cor( i, j )		=	-fvv * fw*fw*fw;
		}
	output( cor, "corner.bmp");

	// �}�l�j�Mlocal max   �Ĥ@��local max    �o��O���եΪ�²���k
	max		=	0;
	for( i = cx - scale_w - 10;   i <= cx + scale_w + 10;   i++ )
		for( j = cy - scale_w - 10;   j <= cy + scale_w + 10;   j++ )
		{
			if( max < cor( i, j ) )
			{
				max		=	cor( i, j );
				max_i	=	i;
				max_j	=	j;
			}
		}
	// �N�P���k�s
	point_x[0]	=	max_i;
	point_y[0]	=	max_j;
	cor( max_i + 1, max_j + 1 )		=	0;
	cor( max_i - 1, max_j + 1 )		=	0;
	cor( max_i + 1, max_j - 1 )		=	0;
	cor( max_i - 1, max_j - 1 )		=	0;
	cor( max_i + 1, max_j     )		=	0;
	cor( max_i    , max_j + 1 )		=	0;
	cor( max_i - 1, max_j     )		=	0;
	cor( max_i    , max_j - 1 )		=	0;
	cor( max_i	  , max_j     )		=	0;

	// �}�l�j�Mlocal max  �ĤG��
	max		=	0;
	for( i = cx - scale_w - 10;   i <= cx + scale_w + 10;   i++ )
		for( j = cy - scale_w - 10;   j <= cy + scale_w + 10;   j++ )
		{
			if( max < cor( i, j ) )
			{
				max		=	cor( i, j );
				max_i	=	i;
				max_j	=	j;
			}
		}
	// �N�P���k�s
	point_x[1]	=	max_i;
	point_y[1]	=	max_j;
	cor( max_i + 1, max_j + 1 )		=	0;
	cor( max_i - 1, max_j + 1 )		=	0;
	cor( max_i + 1, max_j - 1 )		=	0;
	cor( max_i - 1, max_j - 1 )		=	0;
	cor( max_i + 1, max_j     )		=	0;
	cor( max_i    , max_j + 1 )		=	0;
	cor( max_i - 1, max_j     )		=	0;
	cor( max_i    , max_j - 1 )		=	0;
	cor( max_i	  , max_j     )		=	0;

	// �}�l�j�Mlocal max �ĤT��
	max		=	0;
	for( i = cx - scale_w - 10;   i <= cx + scale_w + 10;   i++ )
		for( j = cy - scale_w - 10;   j <= cy + scale_w + 10;   j++ )
		{
			if( max < cor( i, j ) )
			{
				max		=	cor( i, j );
				max_i	=	i;
				max_j	=	j;
			}
		}
	// �N�P���k�s
	point_x[2]	=	max_i;
	point_y[2]	=	max_j;
	cor( max_i + 1, max_j + 1 )		=	0;
	cor( max_i - 1, max_j + 1 )		=	0;
	cor( max_i + 1, max_j - 1 )		=	0;
	cor( max_i - 1, max_j - 1 )		=	0;
	cor( max_i + 1, max_j     )		=	0;
	cor( max_i    , max_j + 1 )		=	0;
	cor( max_i - 1, max_j     )		=	0;
	cor( max_i    , max_j - 1 )		=	0;
	cor( max_i	  , max_j     )		=	0;

	// �}�l�j�Mlocal max  �ĥ|��
	max		=	0;
	for( i = cx - scale_w - 10;   i <= cx + scale_w + 10;   i++ )
		for( j = cy - scale_w - 10;   j <= cy + scale_w + 10;   j++ )
		{
			if( max < cor( i, j ) )
			{
				max		=	cor( i, j );
				max_i	=	i;
				max_j	=	j;
			}
		}
	// �N�P���k�s
	point_x[3]	=	max_i;
	point_y[3]	=	max_j;
	cor( max_i + 1, max_j + 1 )		=	0;
	cor( max_i - 1, max_j + 1 )		=	0;
	cor( max_i + 1, max_j - 1 )		=	0;
	cor( max_i - 1, max_j - 1 )		=	0;
	cor( max_i + 1, max_j     )		=	0;
	cor( max_i    , max_j + 1 )		=	0;
	cor( max_i - 1, max_j     )		=	0;
	cor( max_i    , max_j - 1 )		=	0;
	cor( max_i	  , max_j     )		=	0;


	// �i�� subpixel��corner�p��
	// �p�ⰾ�t���覡��²�� �������@���I��ѤU�T���I���Z�� �ݶZ���~�t
	QImage		QQ	=	image;
	Facet		facet(3);
	for( i=0; i<4; i++ )
	{
		njet.get_corner( point_x[i], point_y[i], dx, dy );

		/*if( facet.edge( point_x[i] + 0.5, point_y[i] + 0.5, image, dx, dy ) == false )
		{
			dx	=	-1;
			dy	=	-1;
		}*/

		cornerSubPix( image, point_x[i] , point_y[i] , dx, dy );
		//cout << "center = " << dx << " " << dy << endl;
		sub_x[i]	=	dx;
		sub_y[i]	=	dy;

		QQ.pixel_channel( (int)(dx), (int)(dy), 0, 0, 255 );

	}

	// ���͸��
	FILE	*p	=	fopen( "BMP/rectangle.txt", "a+" );
	FILE	*q	=	fopen( "BMP/pixel.txt", "a+" );
	for( i=0; i<4; i++ )
	{
		fprintf( p, "%lf %lf\n", sub_x[i], sub_y[i] );
		fprintf( q, "%d %d\n", point_x[i], point_y[i] );
	}
	fprintf( p, "\n" );
	fprintf( q, "\n" );
	fclose(p);
	fclose(q);

	char	str[100];
	sprintf( str, "10/o_%d.bmp", index );
	QQ.save( str );

}


/*=================================================================================
	����curvature
===================================================================================*/
void		SUBPIX_BLOB::curvature()
{
	int			i,	j;
	int			x,	y;
	double		rx,	ry;
	double		dtmp;

	QRgb		rgb1,	rgb2;
	int			gray1,	gray2;

	curva_map.resize( image.width(), image.height() );

	QImage	IQ	=	image;

	//cout << "\nfind curvature...\n" ;
	for( x = 1;  x < image.width() - 1;   x++ )
	{
		//cout << "~";
		for( y=1; y<image.height() -1; y++ )
		{
			rgb1				=	image.pixel( x+1, y );
			rgb2				=	image.pixel( x-1, y );
			gray1				=	qGray( rgb1 );
			gray2				=	qGray( rgb2 );
			rx					=	0.5 * ( gray1 - gray2 );

			rgb1				=	image.pixel( x, y+1 );
			rgb2				=	image.pixel( x, y-1 );
			gray1				=	qGray( rgb1 );
			gray2				=	qGray( rgb2 );
			ry					=	0.5 * ( gray1 - gray2 );

			dtmp				=	sqrt( rx*rx + ry*ry );
			curva_map( x, y )	=	dtmp;

			IQ.setPixel( x, y, qRgb( dtmp, dtmp, dtmp ) );

		}
	}
	
	IQ.save("curvature.bmp");

}

/*=================================================================================
	�D rx, ry, rxx, ryy, rxy  ���w�d��
===================================================================================*/
void		SUBPIX_BLOB::diff( PointInt lu, PointInt rd )
{
	int		i,	j;
	int		x,	y;
	int		gray1,	gray2,	gray3,	gray4;
	double	dtmp;


	r_x.resize ( image.width(), image.height() );
	r_y.resize ( image.width(), image.height() );
	r_xx.resize( image.width(), image.height() );
	r_yy.resize( image.width(), image.height() );
	r_xy.resize( image.width(), image.height() );

	for(  x = lu.x+1;   x <= rd.x-1;   x++  )
	{
		for(  y = lu.y+1;   y <= rd.y-1;   y++  )
		{
			gray1			=	filter.gray_channel( x+1, y );
			gray2			=	filter.gray_channel( x-1, y );
			r_x( x, y )		=	0.5 * ( gray1 - gray2 );

			gray1			=	filter.gray_channel( x, y+1 );
			gray2			=	filter.gray_channel( x, y-1 );
			r_y( x, y )		=	0.5 * ( gray1 - gray2 );

			gray1			=	filter.gray_channel( x+1, y );
			gray2			=	filter.gray_channel( x-1, y );
			gray3			=	filter.gray_channel( x, y );
			r_xx( x, y )	=	gray1 + gray2 - 2.0*gray3;

			gray1			=	filter.gray_channel( x, y+1 );
			gray2			=	filter.gray_channel( x, y-1 );
			gray3			=	filter.gray_channel( x, y );
			r_yy( x, y )	=	gray1 + gray2 - 2.0*gray2;

			gray1			=	filter.gray_channel( x+1, y+1 );
			gray2			=	filter.gray_channel( x-1, y-1 );
			gray3			=	filter.gray_channel( x+1, y-1 );
			gray4			=	filter.gray_channel( x-1, y+1 );
			r_xy( x, y )	=	0.25 * ( gray1 + gray2 - gray3 - gray4 );
		}
	}

}



/*=================================================================================
	�D rx, ry, rxx, ryy, rxy
===================================================================================*/
void		SUBPIX_BLOB::diff()
{
	int		i,	j;
	int		x,	y;
	double	dtmp;

	QRgb		rgb1,	rgb2,	rgb3,	rgb4;
	int			gray1,	gray2,	gray3,	gray4;

	r_x.resize ( image.width(), image.height() );
	r_y.resize ( image.width(), image.height() );
	r_xx.resize( image.width(), image.height() );
	r_yy.resize( image.width(), image.height() );
	r_xy.resize( image.width(), image.height() );

		
	// �H�U�O���] h = 1   (����ݭn���nnormalize  �]���Ȼ~�t�L�j)
	for( x=1; x<image.width()-1; x++ )
	{
		for( y=1; y<image.height()-1; y++ )
		{

			rgb1			=	filter.pixel( x+1, y );
			rgb2			=	filter.pixel( x-1, y );
			gray1			=	qGray(rgb1);
			gray2			=	qGray(rgb2);
			r_x( x, y )		=	0.5 * ( gray1 - gray2 );

			rgb1			=	filter.pixel( x, y+1 );
			rgb2			=	filter.pixel( x, y-1 );
			gray1			=	qGray(rgb1);
			gray2			=	qGray(rgb2);
			r_y( x, y )		=	0.5 * ( gray1 - gray2 );

			rgb1			=	filter.pixel( x+1, y );
			rgb2			=	filter.pixel( x-1, y );
			rgb3			=	filter.pixel( x  , y );
			gray1			=	qGray(rgb1);
			gray2			=	qGray(rgb2);
			gray3			=	qGray(rgb3);
			r_xx( x, y )	=	gray1 + gray2 - 2.0*gray3;
	
			rgb1			=	filter.pixel( x, y+1 );
			rgb2			=	filter.pixel( x, y-1 );
			rgb3			=	filter.pixel( x, y   );
			gray1			=	qGray(rgb1);
			gray2			=	qGray(rgb2);
			gray3			=	qGray(rgb3);
			r_yy( x, y )	=	gray1 + gray2 - 2.0*gray2;

			rgb1			=	filter.pixel( x+1, y+1 );
			rgb2			=	filter.pixel( x-1, y-1 );
			rgb3			=	filter.pixel( x+1, y-1 );
			rgb4			=	filter.pixel( x-1, y+1 );
			gray1			=	qGray(rgb1);
			gray2			=	qGray(rgb2);
			gray3			=	qGray(rgb3);
			gray4			=	qGray(rgb4);
			r_xy( x, y )	=	0.25 * ( gray1 + gray2 - gray3 - gray4 );
		}
	}

}




/*=================================================================================
	�D�X�b�Ӥ�V, �h�ֶZ�����X�{ edge. �䤣�쪺�� tmax = -1
	vector (vx, vy)
	point  (px, py)
===================================================================================*/
double		SUBPIX_BLOB::find_edge( double vx, double vy, double px, double py, double scale )
{
	int			sx,	sy;
	int			X,	Y;
	int			a,	b;
	double		max;
	double		t,	tmax;
	double		t1,	t2;
	double		limit;	// �P�_ t �n���h��

	limit	=	1.5 * scale;	// 2.5 * scale / sqrt(3)  
	//limit	=	2.5 * scale;

	if( vx > 0 )		sx	=	1;
	else if( vx == 0 )	sx	=	0;
	else				sx	=	-1;

	if( vy > 0 )		sy	=	1;
	else if( vy == 0 )	sy	=	0;
	else				sy	=	-1;

	X	=	(int)(px) + sx;
	Y	=	(int)(py) + sy;

	max	=	0;
	t	=	0;
	while( vx*t <= limit && vy*t <= limit)
	{
		if( sqrt( vx*vx*t*t + vy*vy*t*t ) > limit )										break;
		if( X < 0 || Y < 0 || X >= curva_map.size1() || Y >= curva_map.size2() )		break;

		t1	=	vx != 0 ? ((double)X - px) / vx : 100000000;
		t2	=	vy != 0 ? ((double)Y - py) / vy : 100000000;

		//if( vx == 0 && vy == 0 )	error_msg("SUBPIX_BLOB::find_edge");

		t	=	t1 < t2 ? t1 : t2;

		//if( t < 0 )		error_msg("find_edge");

		if( t1 == t2 )
		{
			a	=	(int)(vx*t1+px);
			b	=	(int)(vy*t1+py);

			if(   curva_map( a, b ) > max )
			{
				tmax	=	t1;
				max		=	curva_map( a, b );
			}

			X	+=	sx;
			Y	+=	sy;
		}

		else if( t1 < t2 )
		{
			a	=	(int)(vx*t1+px);
			b	=	(int)(vy*t1+py);

			if( curva_map( a, b ) > max )
			{
				tmax	=	t1;	
				max		=	curva_map( a, b );
			}

			X	+=	sx;
		}

		else if( t1 > t2 )
		{
			int		a	=	(int)(vx*t2+px);
			int		b	=	(int)(vy*t2+py);

			//if( curva_map( (int)(vx*t2+px), (int)(vy*t2+py) ) > max )
			//QString	str	=	"" + atoi(curva_map(a,b));
			//progress->debuger(str);
			if( curva_map( a, b ) > max )
			{
				tmax	=	t2;	
				max		=	curva_map( a, b );
			}

			Y	+=	sy;
		}
	}


	// �M�w�j�M��T�ת��ѼƤ��@
	if( max < EDGE_THRESHOLD )
		return	-1;
	else
		return	tmax;

}


/*=================================================================================
	�p�⤤�ߪ��j��
===================================================================================*/
double		SUBPIX_BLOB::center_value( int x, int y )
{
	double		rx,		ry;
	double		rxx,	ryy,	rxy;		
	double		dtmp;
	double		nx,			ny;							// normalized vector    ��ڤW���ӬO�u���V
	double		tx,			ty;							// ���u�V�q			    ��ڤW���ӬO�����V
	double		t1,	t2,	t3,	t4;							// �P�_�|�Ӥ�V����b����.
	double		px,	py;
	double		delta_x,	delta_y;					// x,y �������q
	double		detH;									// det(H)
	double		trH;									// trace(H)
	double		mag;
	int			scale;

	rx		=	r_x ( x, y );
	ry		=	r_y ( x, y );
	rxx		=	r_xx( x, y );
	ryy		=	r_yy( x, y );
	rxy		=	r_xy( x, y );

	// �D�����I�����q delta
	detH	=	rxx*ryy - rxy*rxy;
	trH		=	rxx + ryy;


	if( detH == 0 )
		detH	=	0.0000000001;

	// �쥻���Ҽ{��o�ǨB�J��֨�is_center�̭�  ��ӵo�{���n�B�z  �Ҽ{��t�׮t�����j  �Ȯɥ����}�B�z.
	delta_x	=	( rxy*ry - ryy*rx ) / detH;
	delta_y	=	( rxx*ry - rxy*rx ) / detH;

	mag		=	1.0 / sqrt( delta_x*delta_x + delta_y*delta_y + 0.0000000001 );

	if ( ! is_center(  x,  y, &delta_x, &delta_y ) )
	{
		mag	=	0;
	}

	return	mag;
}



/*=================================================================================
	�Q��non maximal suppression �h�o�줤���I
===================================================================================*/
boost::numeric::ublas::matrix<int>		SUBPIX_BLOB::center_detector()
{
	int		i,	j;
	int		x,	y;
	int		scale;

	scale	=	scale_w > scale_l ? scale_l : scale_w;
	scale	/=	2;

	boost::numeric::ublas::matrix<int>		mask,	skip;
	boost::numeric::ublas::matrix<double>	map;

	map.resize( image.width(), image.height() );
	map.clear();

	mask.resize( image.width(), image.height() );
	mask.clear();

	skip.resize( image.width(), image.height() );
	skip.clear();

	for( i=scale_w; i<image.width()-scale_w; i++ )
		for( j=scale_l; j<image.height()-scale_l; j++ )
			map( i, j )		=	center_value( i, j );

	//output( map, "output_test2.bmp");

	// n*n �� non maximum suppression   �Q��scale�ӨM�wwindow�j�p
	for( i=0; i<image.width(); i++ )
		for( j=0; j<image.height(); j++ )
		{
			if( skip( i, j ) == 1 )
				continue;

			// �M�wlocal max
			for( x=i-scale; x<=i+scale; x++ )
			{
				for( y=j-scale; y<=j+scale; y++ )
				{
					if( i == x && j == y )		continue;

					if( x >= 0 && x < image.width() && y>=0 && y<image.height() )
					{
						if( map( i, j ) >= map( x, y ) )
						{
							skip( x, y )	=	1;
							mask( x, y )	=	-1;
						}
						else
						{
							mask( i, j )	=	-1;
							break;
						}

					}
				}

				if( mask( i, j ) == -1 )
					break;
			}

			if( mask( i, j ) == 0 )
				mask( i, j )	=	1;

		}

	//output( mask );

	return	mask;
}


/*=================================================================================
	�ˬd�O�_�O�����I
===================================================================================*/
bool		SUBPIX_BLOB::is_center( int x, int y, double *subpx, double *subpy )
{
	double		rx,		ry;
	double		rxx,	ryy,	rxy;		
	double		dtmp;
	double		nx,			ny;							// normalized vector    ��ڤW���ӬO�u���V
	double		tx,			ty;							// ���u�V�q			    ��ڤW���ӬO�����V
//	double		t,	tmax;
	double		t1,	t2,	t3,	t4;							// �P�_�|�Ӥ�V����b����.
	double		px,	py;
	double		delta_x,	delta_y;					// x,y �������q
	double		detH;									// det(H)
	double		trH;									// trace(H)
//	int			xtmp,	ytmp;
//	int			max;
	int			scale;

	rx		=	r_x ( x, y );
	ry		=	r_y ( x, y );
	rxx		=	r_xx( x, y );
	ryy		=	r_yy( x, y );
	rxy		=	r_xy( x, y );

	// �D�����I�����q delta
	detH	=	rxx*ryy - rxy*rxy;
	trH		=	rxx + ryy;

	
	// �k�G  �Q�� traceH / degH �h�P�_
	/*if( detH == 0 )		return	false;
	else
	{
		//cout << trH*trH/detH << endl;

		if( fabs( trH*trH/detH  ) > 0.01 )
			return	false;
		else
		{
			cout << trH*trH/detH << endl;
			return	true;
		}
	}*/


	if( detH == 0 )
	{
		return	false;

		*subpx	=	0.5 + x;
		*subpy	=	0.5 + y;
		px		=	0.5 + x;
		py		=	0.5 + y;
		nx		=	1.;
		ny		=	0.;
		//return	true;
	}
	else
	{
		delta_x	=	( rxy*ry - ryy*rx ) / detH;
		delta_y	=	( rxx*ry - rxy*rx ) / detH;

		if( fabs(delta_x) > 0.5 || fabs(delta_y) > 0.5 )	return	false;
		//if( delta_x <= -0.5 || delta_x > 0.5 || delta_y <= -0.5 || delta_y > 0.5 )		return false;

		// �]�����]�����I�O  (1.5,2.5) �o�خ榡.
		*subpx	=	0.5 + x + delta_x;
		px		=	0.5 + x + delta_x;
		*subpy	=	0.5 + y + delta_y;	
		py		=	0.5 + y + delta_y;

		// �Deigenvector �p��쥪�k���䪺�Z���O�_�۵�
		eigenvector( x, y, &nx, &ny, rx, ry, rxx, ryy, rxy );
	}

	// ���u�V�q
	tx	=	ny;
	ty	=	-nx;

	// ���|�Ӥ�V�ӷj�M   (���G�]�i�H�令�p��X�Ӫ�t)
	// �k�V�q
	// �o�䦳bug   Ū����index   nx*t + x �i��|�W�X�Ϥ��d��
	// �o���k�i�H�ץ�,�F��[�t���ĪG  �٦��ܦhbug  �Ϊ̵{���ק�Ŷ�.

	// �k�V�q
	scale	=	scale_w < scale_l ? scale_w : scale_l;
	t1		=	find_edge( nx, ny, px, py, scale );
	
	// �t�k�V�q
	scale	=	scale_w < scale_l ? scale_w : scale_l;
	t2		=	find_edge( -nx, -ny, px, py,scale );

	// ���u�V�q
	scale	=	scale_w > scale_l ? scale_w : scale_l;
	t3		=	find_edge( tx, ty, px, py, scale );

	// �t���u�V�q
	scale	=	scale_w > scale_l ? scale_w : scale_l;
	t4		=	find_edge( -tx, -ty, px, py, scale );

	if( t1 < 0 || t2 < 0 || t3 < 0 || t4 < 0 )
		return	false;

#if 0
	max	=	0;
	for( t=0; t<=1.444*scale_w; t+=0.1 )
		if( bmp_cur->gray_channel( (int)(nx*t + px), (int)(ny*t + py) )  >=  max  )
		{
			max		=	 bmp_cur->gray_channel( (int)(nx*t + px), (int)(ny*t + py) );
			tmax	=	t;
		}
	if( max < 5 )
		return	false;
	t1	=	tmax;

	// �t�k�V�q
	max	=	0;
	for( t=0; t<=1.444*scale_w; t+=0.1 )
		if( bmp_cur->gray_channel( (int)(-nx*t + px), (int)(-ny*t + py) )  >=  max  )
		{
			max		=	 bmp_cur->gray_channel( (int)(-nx*t + px), (int)(-ny*t + py) );
			tmax	=	t;
		}
	if( max < 5 )
		return	false;
	t2	=	tmax;

	// ���u
	max	=	0;
	for( t=0; t<=1.444*scale_l; t+=0.1 )
		if( bmp_cur->gray_channel( (int)(tx*t + px), (int)(ty*t + py) )  >=  max  )
		{
			max		=	 bmp_cur->gray_channel( (int)(tx*t + px), (int)(ty*t + py) );
			tmax	=	t;
		}
	if( max < 5 )
		return	false;
	t3	=	tmax;

	// �t���u
	max	=	0;
	for( t=0; t<=1.444*scale_l; t+=0.1 )
		if( bmp_cur->gray_channel( (int)(-tx*t + px), (int)(-ty*t + py) )  >=  max  )
		{
			max		=	 bmp_cur->gray_channel( (int)(-tx*t + px), (int)(-ty*t + py) );
			tmax	=	t;
		}
	if( max < 5 )
		return	false;
	t4	=	tmax;

#endif

	// �o��]�i�H�ΤثרӨM�w��������T��
	double	leng1	=	sqrt( nx*nx*t1*t1 + ny*ny*t1*t1 );
	double	leng2	=	sqrt( nx*nx*t2*t2 + ny*ny*t2*t2 );
	double	leng3	=	sqrt( tx*tx*t3*t3 + ty*ty*t3*t3 );
	double	leng4	=	sqrt( tx*tx*t4*t4 + ty*ty*t4*t4 );

	scale			=	scale_w < scale_l ? scale_w : scale_l;
	if( fabs( leng1 - leng2 ) >= 0.5*scale )	return	false;
	if( leng1 <= 0.5*scale )					return	false;
	if( leng2 <= 0.5*scale )					return	false;


	scale	=	scale_w > scale_l ? scale_w : scale_l;
	if( fabs( leng3 - leng4 ) > 0.5*scale  )	return	false;
	if( leng3 <= 0.5*scale )					return	false;
	if( leng4 <= 0.5*scale )					return	false;


	//eigenvector2(
	eigenvector2( x, y, &nx, &ny, rx, ry, rxx, ryy, rxy );

	return	true;
}



/*=================================================================================
	�M��Xblob  
	���w�d��
===================================================================================*/
void		SUBPIX_BLOB::find_blob( PointInt lu, PointInt rd )
{
	int			i,	j;
	int			count;
	double		subpx,		subpy;		// �s sub-pixel �������I�y�� 
	double		delta_x,	delta_y;
	double		mag;
	bool		isfind;

	boost::numeric::ublas::matrix<int>	map;
	map.resize( image.width(), image.height() );
	map.clear();

	isfind	=	false;

	// �����ͤ@��map  �ح��]�t�F�O�_�����ߪ���T
	map		=	center_detector();

	count	=	0;

	for(  i  =  lu.x+1;   i <= rd.x-1;   i++  )
	{

#ifdef _OPENPIV_
		progress->setGress( 100.0*i/image.width() );
#endif

		for(  j = lu.y+1;   j <= rd.y-1;   j++  )
		{

			//if( i == 823 && j == 71 )	continue;
			//if( i == 809 && j == 73 ) continue;


			// ���g�Q�L�n��ֳo�䪺�p�� ��Ӧ]�����n��֩ҥH�����B�z
			if( is_center( i, j,  &subpx,  &subpy ) && map(i,j) == 1 )
			//if( is_center( i, j,  &subpx,  &subpy ) )
			{
				count ++;

				isfind	=	true;

				// �аO�X��
				final.setPixel( i, j, qRgb( 255, 0, 0 ) );

				final.pixel_channel( i, j, 255, 0, 0 );
				final.save("final.bmp");

				polygen( subpx, subpy );

				final.save("output.bmp");

				// �w�g��X���F  �}�l�e��
				ellipse_draw();

			}

		}
	}

	//output( map, "center.bmp");

#ifdef _OPENPIV_
	char	ss[100];
	sprintf(ss,"Find %d blobs...",count );
	progress->debuger( ss , qRgb(100,100,0) );
#endif
	
}



/*=================================================================================
	�i�氻��blob���ʧ@
	�����X�Ӷ��q�ӳB�z
===================================================================================*/
void		SUBPIX_BLOB::find_blob()
{

	int			i,	j;
	int			count;
	double		subpx,		subpy;		// �s sub-pixel �������I�y�� 
	double		delta_x,	delta_y;
	double		mag;
	bool		isfind;

	boost::numeric::ublas::matrix<int>	map;
	map.resize( image.width(), image.height() );
	map.clear();


	isfind	=	false;

	// �����ͤ@��map  �ح��]�t�F�O�_�����ߪ���T
	map		=	center_detector();
	count	=	0;

	for(  i = scale_w; i<image.width() - scale_w; i++ )
	{

#ifdef _OPENPIV_
		progress->setGress( 100.0*i/image.width() );
#endif

		for( j = scale_l; j<image.height() - scale_l; j++ )
		{
			// ���g�Q�L�n��ֳo�䪺�p�� ��Ӧ]�����n��֩ҥH�����B�z
			if( is_center( i, j,  &subpx,  &subpy ) && map(i,j) == 1 )
			//if( is_center( i, j,  &subpx,  &subpy ) )
			{
				count ++;

				isfind	=	true;

				// �аO�X��
				final.setPixel( i, j, qRgb( 255, 0, 0 ) );

				// �w�惡�����I  �h���P��j�M  ��Xpolygen
				polygen( subpx, subpy );

				// �w�g��X���F  �}�l�e��
				ellipse_draw();

			}
		}
	}

#ifdef _OPENPIV_
	char	ss[100];
	sprintf(ss,"Find %d blobs...",count );
	progress->debuger( ss , qRgb(100,100,0) );
#endif
	
}


/*=================================================================================
	�e�X���
===================================================================================*/ 
void		SUBPIX_BLOB::ellipse_draw()
{
	FILE	*p;
	p	=	fopen( "BMP/ellipse.txt", "a+" );

	ifstream	in("ellipse.txt");

	double		A,	B,	C,	D,	E,	F;
	in >> A >> B >> C >> D >> E >> F;
	in.close();

	//fprintf( p, "\n\nThe ellpse is : \n%lf x^2 + %lf xy + %lf y^2 + %lf x + %lf y + %lf\n" ,A ,B ,C ,D ,E ,F );

	double	vx,	vy,	wx,	wy,	t;
	double	h,	k;
	double	x,	y;
	double	lambda1,	lambda2;
	double	mu;
	double	tmp;
	double	m11,	m12,	m22;
	double	a,	b;
	

	// �p�⤤��
	tmp	=	1.0 / ( 2.0 * 0.25 * B * B - 2.0 * A * C );
	h	=	C*D - 0.5*B*E;
	k	=	A*E - 0.5*B*D;
	h	*=	tmp;
	k	*=	tmp;


	ofstream	out("BMP/center.txt");
	out << h << " " << k << endl;


	//fprintf( p, "The center is (%lf, %lf)\n" , h , k );

	eigenvector( A, 0.5*B, C, vx, vy, wx, wy );

	// �p����b�u�b
	tmp		=	A*h*h + B*h*k + C*k*k - F;
	if( tmp != 0 )
		mu	=	1 / tmp;
	else
		mu	=	1;
	m11	=	mu * A;
	m12	=	mu * 0.5 * B;
	m22	=	mu * C;

	lambda1	=	0.5 * ( (m11+m22) + sqrt( (m11-m22)*(m11-m22)+4.0*m12*m12 ) );
	lambda2	=	0.5 * ( (m11+m22) - sqrt( (m11-m22)*(m11-m22)+4.0*m12*m12 ) );

	if( lambda1 != 0 )
		lambda1	=	1 / sqrt(lambda1);
	else
		lambda1	=	1;
	if( lambda2 != 0 )
		lambda2	=	1 / sqrt( lambda2 );
	else
		lambda2	=	1;

	a	=	lambda2;		// �ѼƦ� ���u�b
	b	=	lambda1;

	//fprintf( p, "The a = %lf, b = %lf\n", a , b );
	fprintf( p, "%lf %lf\n", a , b );

	//---
	tmp	=	sqrt( vx*vx+vy*vy );
	vx	=	vx / tmp;
	vy	=	vy / tmp;

	//fprintf( p,  "The direction is (%lf, %lf)\n" , vx, vy );

	wx	=	vy;
	wy	=	-vx;

	//QImage	QI	=	image;


	for( t = 0; t<2*3.1415926; t+=0.01 )
	{
		x	=	h + a * cos(t) * wx - b * sin(t) * wy;
		y	=	k + b * sin(t) * wx + a * cos(t) * wy;
		//x	=	h + cos(t)*vx - sin(t)*wx;
		//y	=	k + cos(t)*vy + sin(t)*wy;

		if( x >= 0 && x < image.width() && y > 0 && y < image.height() )
			final.pixel_channel( (int)x, (int)y, 0, 0, 255 );

	}


	fclose(p);

}


/*=================================================================================
	�p��eivenvector�Ϊ��lfunction ���@�ǳB�z
===================================================================================*/
void		SUBPIX_BLOB::eigenvector_1( double A, double B, double C, double &nx, double &ny , double eigen )
{
	double	a,	c;
	double	dtmp;

	// �D�Ĥ@��eigenvector
	a		=	A - eigen;
	c		=	C - eigen;
	
	// ���U�b�D eigen vector
	dtmp	=	B*B - (eigen-A)*(eigen-A);
	if( dtmp > 0 )
	{
		nx		=	B / sqrt(dtmp) ;
		ny		=	(eigen-A) / sqrt(dtmp) ;
	}
	else
	{
		dtmp	=	B*B - (eigen-C)*(eigen-C);
		if( dtmp <= 0 )
		{

			// �o��O�S�Ұ�	 �p�߳o�䪺�P�_  �����D���ɭԦ^���ˬd�o��O�_�y��bug
			/*
				case 1   
				 a -a
				-a  a
			*/
			if( fabs(a-c) < 0.0001 && fabs(a+B) < 0.0001 && fabs(a) > 0.0001 )
			{
				nx	=	1.0 / sqrt(2.0);
				ny	=	1.0 / sqrt(2.0);
			}
			/*
				case 2
				 a  0
				 0  0
			*/
			else if( fabs(B) < 0.0001 && fabs(a) >= 0.0001 && fabs(c) < 0.0001 )
			{
				nx	=	0;
				ny	=	1;
			}
			/*
				case 3
				 0  0
				 0  a
			*/
			else if( fabs(B) < 0.0001 && fabs(c) >= 0.0001 && fabs(a) < 0.0001 )
			{
				nx	=	1;
				ny	=	0;
			}
			/*
				case 4 
				 a a
				 a a
			*/
			else if( fabs(a-c) < 0.0001 && fabs(a-B) < 0.0001 && fabs(a) > 0.0001 )
			{
				nx	=	 1.0 / sqrt(2.0);
				ny	=	-1.0 / sqrt(2.0);
			}
			else
			{
				/*nx	=	0;
				ny	=	0;*/
				// ��ڤW ���ӭn��0,0   ���F�p���K����(1,0)����
				nx	=	1;
				ny	=	0;
			}
		}
		else
		{
			nx		=	(eigen-C) / sqrt( dtmp );
			ny		=	B / sqrt( dtmp );
		}
	}





/*	dtmp	=	nx*nx + ny*ny ;

	//if( fabs(dtmp) > 0.001 )
	if( dtmp != 0 )
	{
		nx	/=	sqrt(dtmp);
		ny	/=	sqrt(dtmp);
	}
	//else	error_msg("eigenvector = 0");*/
}



/*=================================================================================
	�p��eigenvector 
===================================================================================*/ 
void		SUBPIX_BLOB::eigenvector( double A, double B, double C, double &vx, double &vy, double &wx, double &wy )
{
	double		dtmp;
	double		eigen1,		eigen2;						// �s eigenvalue
	double		eigen;
	double		D;
	double		nx,			ny;							// normalized vector
	double		a,		c;

	// ���U�O�Deigen value������
	D		=	(A+C)*(A+C) - 4.*(A*C-B*B);
	//if( D < 0 )		error_msg("SUBPIX_BLOB::eigenvector D < 0");
	
	eigen1	=	0.5 * ((A+C) + sqrt( D )) ;
	eigen2	=	0.5 * ((A+C) - sqrt( D )) ;


	// �D�Ĥ@��eigen vector

	eigenvector_1( A, B, C, nx, ny, eigen1 );

	vx	=	nx;
	vy	=	ny;

	// �D�Ĥ@��eigen vector

	eigenvector_1( A, B, C, nx, ny, eigen2 );

	wx	=	nx;
	wy	=	ny;
}


/*=================================================================================
	�D�Xpolygen
===================================================================================*/ 
void		SUBPIX_BLOB::polygen( double subpx, double subpy )
{
	/*
		����x�}  M = cos    -sin
		              sin     cos
	    M * [x,y] = 
		cos * x - sin * y
		sin * x + cos * y
	*/

	Facet		facet(3);		// �Τ@�� 3rd �� facet �ӧ�X subpixel ���䪺�y��.

	double		theta;
	double		vx,	vy;		// �j�M��V
	double		px,	py;
//	double		dtmp1,	dtmp2;
	double		t;  //,	tmax;
	//double		scale;
	int			i;
	int			max;
	int			scale;
	int			count;
	double		X[1000]	=	{0};
	double		Y[1000]	=	{0};
	double		tmp1,	tmp2;
	int			x,	y;
	bool		is;

	ofstream	out;

	scale	=	scale_w > scale_l ? scale_w : scale_l;

	ofstream	outxy("xy.txt");

	vx	=	1;
	vy	=	0;

	count	=	0;


	for( theta = 0; theta <= 2*3.1415; theta += 0.01 )
	{

		vx		=	cos( theta );
		vy		=	sin( theta );

		scale	=	scale_w > scale_l ? scale_w : scale_l;
		t		=	find_edge( vx, vy, subpx, subpy, scale );

		if( t < 0 )			
			continue;

		x		=	(int)(vx*t+subpx);
		y		=	(int)(vy*t+subpy);

		if( x<0 || y<0 || x>=curva_map.size1() || y>=curva_map.size2() )		
			continue;

		//if( x == 445 && y == 238 )
		//	cout << "???";

		if( curva_map( x, y ) > EDGE_THRESHOLD )
		{
			//image->pixel_channel( x, y, 0,255,0 );

			//tmp1		=	x;
			//tmp2		=	y;
			// �����Ӥj���I���L
			//if( !facet.edge(1.0*x+0.1, 1.0*y+0.1, image, tmp1, tmp2 ) )
			//	continue;
			facet.edge( 1.0*x + 0.1, 1.0*y + 0.1, image, tmp1, tmp2 );		// �o����L���覡 �令�� (1.5  2.5)  �o�˪��Ȩ��N

			// �p�G�w�g���o���I �N���[�J��� �קK�����I
			is			=	true;
			for( i=0; i<count; i++ )
			{
				if( tmp1 == X[i] && tmp2 == Y[i] )
					is	=	false;
			}

			if( is )
			{
				facet.edge( 1.0*x + 0.1, 1.0*y + 0.1, image, px, py );

				//X[count]	=	x;
				//Y[count]	=	y;
				X[count]	=	px;
				Y[count]	=	py;
				
				outxy << x << " " << y << endl;

				count++;
			}
		}

		else
			continue;


		//	return	false;
		//t1	=	tmax;

	}

	for( i=0; i<count; i++ )
		final.pixel_channel( X[i], Y[i], 0, 255, 0 );


	//cout << "c\n";

	// ��X
	out.open("BMP/x.txt");
	for( i=0; i<count; i++ )
		out << X[i] << endl;
	out.close();
	out.clear();
	
	out.open("BMP/y.txt");
	for( i=0; i<count; i++ )
		out << Y[i]  << endl;
	out.close();
	out.clear();

	ellipse_fit( subpx, subpy, scale_w, scale_l  );
	circle_fit( subpx, subpy, scale_w, scale_l );


	// taubin
	NChernov::reals	*rx		=	new	NChernov::reals[count];
	NChernov::reals	*ry		=	new	NChernov::reals[count];
	for( i = 0; i < count; i++ )
	{
		rx[i]	=	X[i];
		ry[i]	=	Y[i];
	}
	NChernov::Data		data( count, rx, ry );
	NChernov::Circle	circle;

Timer	TTT;
TTT.Start();
	NChernov::CircleFitByTaubin( data, circle );
TTT.Stop();
//ofstream	tout("circle_taubin_time.txt");
cout << "time " << TTT.Elapsed() << " ms" << endl;
//tout.close();


	circle.print();	



	outxy.close();



	// �I�s�~���{��
	QProcess	prc;
	prc.start("NChernov.exe");


}


/*=================================================================================
	�p��eigenvector 
===================================================================================*/ 
void		SUBPIX_BLOB::eigenvector( int x, int y, double *evx, double *evy, double rx, double ry, double rxx, double ryy, double rxy )
{	
	double		dtmp;
	double		eigen1,		eigen2;						// �s eigenvalue
	double		eigen;
	double		D;
	double		nx,			ny;							// normalized vector
	double		prxx,		pryy;

	// ���U�O�Deigen value������
	D		=	(rxx+ryy)*(rxx+ryy) - 4.*(rxx*ryy-rxy*rxy);
	//if( D < 0 )		error_msg("SUBPIX_BLOB::eigenvector D < 0");
	
	eigen1	=	0.5 * ((rxx+ryy) + sqrt( D )) ;
	eigen2	=	0.5 * ((rxx+ryy) - sqrt( D )) ;




	// �D����Ȥ���j��  (�u���V)
	eigen	=	fabs(eigen1) > fabs(eigen2) ? eigen1 : eigen2;

	prxx		=	rxx - eigen;
	pryy		=	ryy - eigen;
	
	// ���U�b�D eigen vector
	dtmp	=	rxy*rxy - (eigen-rxx)*(eigen-rxx);
	if( dtmp > 0 )
	{
		nx		=	rxy / sqrt(dtmp) ;
		ny		=	(eigen-rxx) / sqrt(dtmp) ;
	}
	else
	{
		dtmp	=	rxy*rxy - (eigen-ryy)*(eigen-ryy);
		if( dtmp <= 0 )
		{

			// �o��O�S�Ұ�	 �p�߳o�䪺�P�_  �����D���ɭԦ^���ˬd�o��O�_�y��bug
			/*
				case 1   
				 a -a
				-a  a
			*/
			if( fabs(prxx-pryy) < 0.0001 && fabs(prxx+rxy) < 0.0001 && fabs(prxx) > 0.0001 )
			{
				nx	=	1.0 / sqrt(2.0);
				ny	=	1.0 / sqrt(2.0);
			}
			/*
				case 2
				 a  0
				 0  0
			*/
			else if( fabs(rxy) < 0.0001 && fabs(prxx) >= 0.0001 && fabs(pryy) < 0.0001 )
			{
				nx	=	0;
				ny	=	1;
			}
			/*
				case 3
				 0  0
				 0  a
			*/
			else if( fabs(rxy) < 0.0001 && fabs(pryy) >= 0.0001 && fabs(prxx) < 0.0001 )
			{
				nx	=	1;
				ny	=	0;
			}
			/*
				case 4 
				 a a
				 a a
			*/
			else if( fabs(prxx-pryy) < 0.0001 && fabs(prxx-rxy) < 0.0001 && fabs(prxx) > 0.0001 )
			{
				nx	=	 1.0 / sqrt(2.0);
				ny	=	-1.0 / sqrt(2.0);
			}
			else
			{
				/*nx	=	0;
				ny	=	0;*/
				// ��ڤW ���ӭn��0,0   ���F�p���K����(1,0)����
				nx	=	1;
				ny	=	0;
			}
		}
		else
		{
			nx		=	(eigen-ryy) / sqrt( dtmp );
			ny		=	rxy / sqrt( dtmp );
		}
	}





	dtmp	=	nx*nx + ny*ny ;

	//if( fabs(dtmp) > 0.001 )
	if( dtmp != 0 )
	{
		nx	/=	sqrt(dtmp);
		ny	/=	sqrt(dtmp);
	}
	//else	error_msg("eigenvector = 0");

	*evx	=	nx;
	*evy	=	ny;

}


/*=================================================================================
	��Jmatrix �ন���� 
===================================================================================*/ 
void		SUBPIX_BLOB::output( boost::numeric::ublas::matrix<int> map )
{
	int			i,	j;
	int			mag;
	double		max,	min;
	QImage		img( map.size1(), map.size2(), QImage::Format_RGB888 );

	max		=	0;
	min		=	1234567;

	for( i=0; i<map.size1(); i++ )
		for( j=0; j<map.size2(); j++ )
		{
			if( max < map(i,j) )
				max	=	map(i,j);
			if( min > map(i,j) )
				min	=	map(i,j);
		}

	for( i=0; i<map.size1(); i++ )
		for( j=0; j<map.size2(); j++ )
		{
			mag		=	(int)( 255.0 * (1.0*map(i,j) - min)/(1.0*max - min) );
			if( map(i,j) == 1 )
				img.pixel_channel( i, j, 255, 0, 0 );
			else
				img.pixel_channel( i, j, 0, 0, 0 );
		}

	img.save( "output_test.bmp" );

}



/*=================================================================================
	��Jmatrix �ন���� 
===================================================================================*/ 
void		SUBPIX_BLOB::output( boost::numeric::ublas::matrix<double> map, char *str )
{
	int			i,	j;
	int			mag;
	double		max,	min;
	QImage		img( map.size1(), map.size2(), QImage::Format_RGB888 );

	max		=	0;
	min		=	1234567;

	for( i=0; i<map.size1(); i++ )
		for( j=0; j<map.size2(); j++ )
		{
			if( max < map(i,j) )
				max	=	map(i,j);
			if( min > map(i,j) )
				min	=	map(i,j);
		}

	for( i=0; i<map.size1(); i++ )
		for( j=0; j<map.size2(); j++ )
		{
			mag		=	(int)( 255.0 * (map(i,j) - min)/(max - min) );
			img.gray_channel( i, j, mag );
		}

	img.save( str );

}



/*=================================================================================
	�j�Mlocal max min  
	�ϥ� non maximal suppression
	3*3-1 = 8 ��neighbor������
===================================================================================*/ 
void		SUBPIX_BLOB::local_extreme( boost::numeric::ublas::matrix<double> map )
{
	int		i,	j,	c,	r,	tmp;

	int		w		=	map.size1();
	int		h		=	map.size2();
	int		cur		=	0;
	int		next	=	1;

	boost::numeric::ublas::matrix<int>	mask,	skip;

	mask.resize( w, h );
	for( i=0; i<w; i++ )
		for( j=0; j<h; j++ )
			mask(i,j)	=	0;

	skip.resize( h, 2 );
	for( i=0; i<h; i++ )
	{
		skip(i,0)	=	0;
		skip(i,1)	=	0;
	}

	for( c = 1;  c < w-1;  c++ )
	{
		r	=	1;
		while( r < h-1 )
		{
			if( skip( r, cur ) )
			{
				r	=	r+1;
				continue;
			}

			if( map(r,c) <= map(r+1,c) )
			{
				r	=	r+1;
				while( r < h-1 && map(r,c) <= map(r+1,c) )
					r	=	r+1;
				if( r == h-1 )
					break;
			}
			else if( map(r,c) <= map(r-1,c) )
			{
				r	=	r+1;
				continue;
			}

			skip( r+1, cur )	=	1;

			// �}�l��neighbors ���
			if( map(r,c) <= map( r-1,c+1 ) )	{ r = r+1; continue; }
			skip( r-1, next )	=	1;

			if( map(r,c) <= map(r,c+1 )	)		{ r = r+1; continue; }
			skip( r, next )		=	1;

			if( map( r,c ) <= map(r+1,c+1 ) )	{ r = r+1; continue;  }
			skip( r+1,next)		=	1;

			if( map(r,c) <= map(r-1,c-1) )		{ r = r+1; continue; }
			if( map(r,c) <= map(r, c-1 ) )		{ r = r+1; continue; }
			if( map(r,c) <= map(r+1,c-1) )		{ r = r+1; continue; }


			// ���s��local extreme
			mask( r, c )	=	1;	
			r				=	r+1;
		}

		tmp		=	cur;
		cur		=	next;
		next	=	tmp;
		
		for( i=0; i<h; i++ )
			skip( i, next )	=	0;
	}

	for( i=0; i<mask.size1(); i++ )
		for( j=0; j<mask.size2(); j++ )
		{
			if( mask(i,j) == 1 )
				cout << i << " " << j << " = " << map(i,j) << " is local extreme!!\n";
		}

}




/*=================================================================================
	�j�Mlocal max min  
	�ϥ� non maximal suppression
	9 + 9 + 8 = 26 �� neighbor������
===================================================================================*/ 
int			SUBPIX_BLOB::local_extreme( boost::numeric::ublas::matrix<double> map, boost::numeric::ublas::matrix<double> pmap, boost::numeric::ublas::matrix<double> nmap )
{
	int		i,	j,	c,	r,	tmp;
	int		count;

	int		w		=	map.size1();
	int		h		=	map.size2();
	int		cur		=	0;
	int		next	=	1;

	boost::numeric::ublas::matrix<int>	mask,	skip;

	mask.resize( w, h );
	for( i=0; i<w; i++ )
		for( j=0; j<h; j++ )
			mask(i,j)	=	0;

	skip.resize( h, 2 );
	for( i=0; i<h; i++ )
	{
		skip(i,0)	=	0;
		skip(i,1)	=	0;
	}

	for( c = 1;  c < w-1;  c++ )
	{
		r	=	1;
		while( r < h-1 )
		{
			if( skip( r, cur ) )
			{
				r	=	r+1;
				continue;
			}

			if( map(r,c) <= map(r+1,c) )
			{
				r	=	r+1;
				while( r < h-1 && map(r,c) <= map(r+1,c) )
					r	=	r+1;
				if( r == h-1 )
					break;
			}
			else if( map(r,c) <= map(r-1,c) )
			{
				r	=	r+1;
				continue;
			}

			skip( r+1, cur )	=	1;

			// �}�l��neighbors ���
			// �쥻�� 8 ��neighbor
			if( map(r,c) <= map( r-1,c+1 ) )	{ r = r+1; continue; }
			skip( r-1, next )	=	1;

			if( map(r,c) <= map(r,c+1 )	)		{ r = r+1; continue; }
			skip( r, next )		=	1;

			if( map( r,c ) <= map(r+1,c+1 ) )	{ r = r+1; continue;  }
			skip( r+1,next)		=	1;

			if( map(r,c) <= map(r-1,c-1) )		{ r = r+1; continue; }
			if( map(r,c) <= map(r, c-1 ) )		{ r = r+1; continue; }
			if( map(r,c) <= map(r+1,c-1) )		{ r = r+1; continue; }

			// ���P�ثת�neighbor
			// �W�@�Ӥث�
			if( map(r,c) <= pmap(r,c) )			{ r = r+1; continue; }
			if( map(r,c) <= pmap(r-1,c) )		{ r = r+1; continue; }
			if( map(r,c) <= pmap(r+1,c) )		{ r = r+1; continue; }
			if( map(r,c) <= pmap(r,c-1) )		{ r = r+1; continue; }
			if( map(r,c) <= pmap(r,c+1) )		{ r = r+1; continue; }
			if( map(r,c) <= pmap(r-1,c-1) )		{ r = r+1; continue; }
			if( map(r,c) <= pmap(r+1,c-1) )		{ r = r+1; continue; }
			if( map(r,c) <= pmap(r-1,c+1) )		{ r = r+1; continue; }
			if( map(r,c) <= pmap(r+1,c+1) )		{ r = r+1; continue; }
			// �U�@�Ӥث�
			if( map(r,c) <= nmap(r,c) )			{ r = r+1; continue; }
			if( map(r,c) <= nmap(r-1,c) )		{ r = r+1; continue; }
			if( map(r,c) <= nmap(r+1,c) )		{ r = r+1; continue; }
			if( map(r,c) <= nmap(r,c-1) )		{ r = r+1; continue; }
			if( map(r,c) <= nmap(r,c+1) )		{ r = r+1; continue; }
			if( map(r,c) <= nmap(r-1,c-1) )		{ r = r+1; continue; }
			if( map(r,c) <= nmap(r+1,c-1) )		{ r = r+1; continue; }
			if( map(r,c) <= nmap(r-1,c+1) )		{ r = r+1; continue; }
			if( map(r,c) <= nmap(r+1,c+1) )		{ r = r+1; continue; }


			// ���s��local extreme
			mask( r, c )	=	1;	
			r				=	r+1;
		}

		tmp		=	cur;
		cur		=	next;
		next	=	tmp;
		
		for( i=0; i<h; i++ )
			skip( i, next )	=	0;
	}

	// �p���`�@���X��extreme
	count	=	0;
	for( i=0; i<mask.size1(); i++ )
		for( j=0; j<mask.size2(); j++ )
		{
			if( mask(i,j) == 1 )
				count++;
				//cout << i << " " << j << " = " << map(i,j) << " is local extreme!!\n";
		}

	return	count;
}




/*=================================================================================
	dosomething 2�� ���ե�
===================================================================================*/ 
void		SUBPIX_BLOB::dosomething2( char *str, int index )
{
	PointInt		rd,		lu;
	lu		=	PointInt( 684, 0 );
	rd		=	PointInt( 1001, 140 );

	scale_w	=	48;
	scale_l	=	18;

	image	=	QImage(str);
	final	=	image;							// ����bfinal�W�����I����

	// ��filter���ʧ@  �åB���w�d��
	GaussFilter_2D		G_filter( scale_w, scale_l );
	filter	=	G_filter.do_filter( image, lu, rd );

	filter.save("filter.bmp");

	// �p��curvature
	curvature( lu, rd );

	// �p�� rx ry rxx ryy rxy �����L����
	diff( lu, rd );
	
	// ��M�����I ���䵥��  ��Mblob���ʧ@
	find_blob( lu, rd );

	final.save("output.bmp");

	// �h�p��sub pixel �� corner
	corner( index );

}


/*=================================================================================
	�p��eigenvector 
===================================================================================*/ 
void		SUBPIX_BLOB::eigenvector2( int x, int y, double *evx, double *evy, double rx, double ry, double rxx, double ryy, double rxy )
{	
	double		dtmp;
	double		eigen1,		eigen2;						// �s eigenvalue
	double		eigen;
	double		D;
	double		nx,			ny;							// normalized vector
	double		prxx,		pryy;

	// ���U�O�Deigen value������
	D		=	(rxx+ryy)*(rxx+ryy) - 4.*(rxx*ryy-rxy*rxy);
	//if( D < 0 )		error_msg("SUBPIX_BLOB::eigenvector D < 0");
	
	eigen1	=	0.5 * ((rxx+ryy) + sqrt( D )) ;
	eigen2	=	0.5 * ((rxx+ryy) - sqrt( D )) ;




	// �D����Ȥ���j��  (�u���V)
	eigen	=	fabs(eigen1) > fabs(eigen2) ? eigen1 : eigen2;

	prxx		=	rxx - eigen;
	pryy		=	ryy - eigen;
	
	// ���U�b�D eigen vector
	dtmp	=	rxy*rxy - (eigen-rxx)*(eigen-rxx);
	if( dtmp > 0 )
	{
		nx		=	rxy / sqrt(dtmp) ;
		ny		=	(eigen-rxx) / sqrt(dtmp) ;
	}
	else
	{
		dtmp	=	rxy*rxy - (eigen-ryy)*(eigen-ryy);
		if( dtmp <= 0 )
		{

			// �o��O�S�Ұ�	 �p�߳o�䪺�P�_  �����D���ɭԦ^���ˬd�o��O�_�y��bug
			/*
				case 1   
				 a -a
				-a  a
			*/
			if( fabs(prxx-pryy) < 0.0001 && fabs(prxx+rxy) < 0.0001 && fabs(prxx) > 0.0001 )
			{
				nx	=	1.0 / sqrt(2.0);
				ny	=	1.0 / sqrt(2.0);
			}
			/*
				case 2
				 a  0
				 0  0
			*/
			else if( fabs(rxy) < 0.0001 && fabs(prxx) >= 0.0001 && fabs(pryy) < 0.0001 )
			{
				nx	=	0;
				ny	=	1;
			}
			/*
				case 3
				 0  0
				 0  a
			*/
			else if( fabs(rxy) < 0.0001 && fabs(pryy) >= 0.0001 && fabs(prxx) < 0.0001 )
			{
				nx	=	1;
				ny	=	0;
			}
			/*
				case 4 
				 a a
				 a a
			*/
			else if( fabs(prxx-pryy) < 0.0001 && fabs(prxx-rxy) < 0.0001 && fabs(prxx) > 0.0001 )
			{
				nx	=	 1.0 / sqrt(2.0);
				ny	=	-1.0 / sqrt(2.0);
			}
			else
			{
				/*nx	=	0;
				ny	=	0;*/
				// ��ڤW ���ӭn��0,0   ���F�p���K����(1,0)����
				nx	=	1;
				ny	=	0;
			}
		}
		else
		{
			nx		=	(eigen-ryy) / sqrt( dtmp );
			ny		=	rxy / sqrt( dtmp );
		}
	}





	dtmp	=	nx*nx + ny*ny ;

	//if( fabs(dtmp) > 0.001 )
	if( dtmp != 0 )
	{
		nx	/=	sqrt(dtmp);
		ny	/=	sqrt(dtmp);
	}
	//else		error_msg("eigenvector = 0");


	*evx	=	nx;
	*evy	=	ny;


	//cout << "��l�覡 �I " << x << " " << y << endl;
	//cout << "eigenvalue = " << eigen1 << " " << eigen2 << "  Ratio = " << eigen1/eigen2 << endl;
	//cout << "vector = " << nx << " " << ny << "\n\n";

}








#ifdef _OPENPIV_
/*=================================================================================
	����{���X���a��
	openPIV ����subpixel blob detection
===================================================================================*/
void	SUBPIX_BLOB::dosomething()
{


	//circle_fit( 1, 1, 1, 1 );


	progress->debuger("Start subpixel blob detector...");

	image	=	filedata->get_image();
	final	=	image;							// ����bfinal�W�����I����

	progress->debuger("Start Gauss filter...");

	// ��filter���ʧ@
	GaussFilter_2D		G_filter( scale_w, scale_l );
	filter	=	G_filter.do_filter( image, progress );

	//filter.save("filter.bmp");
	progress->debuger("finish Gauss filter...");

	// �p��curvature
	curvature();

	// �p�� rx ry rxx ryy rxy �����L����
	diff();
	
	// ��M�����I ���䵥��  ��Mblob���ʧ@
	find_blob();
	final.save("output.bmp");

	// ��X��D�e��
	//emit( filedata->output(final) );
	emit( progress->outputImage( 0, final ) );
	progress->debuger("<font color=\"Red\"> finish subpixel blob detector... </font>");
}
#endif






#ifdef _OPENPIV_
/*=================================================================================
	����{���X���a��
	openPIV ����subpixel blob detection
===================================================================================*/
void	SUBPIX_BLOB::dosomething3( QImage graytone )
{
	//QProcess	prc;
	//prc.start("NChernov.exe");


	//circle_fit( 1, 1, 1, 1 );
	//progress->debuger("Start subpixel blob detector...");

	scale_w	=	settings->get_scale_w();
	scale_l	=	settings->get_scale_w();

	image	=	graytone;//filedata->get_image();
	final	=	image;							// ����bfinal�W�����I����

	progress->debuger("Start Gauss filter...");

	// ��filter���ʧ@
	GaussFilter_2D		G_filter( scale_w, scale_l );
	filter	=	G_filter.do_filter( image, progress );

	//filter.save("filter.bmp");
	progress->debuger("finish Gauss filter...");

	// �p��curvature
	curvature();

	// �p�� rx ry rxx ryy rxy �����L����
	diff();
	
	// ��M�����I ���䵥��  ��Mblob���ʧ@
	find_blob();
	final.save("output.bmp");

	// ��X��D�e��
	//emit( filedata->output(final) );
	progress->outputImage( 0, final );
	progress->debuger("<font color=\"Red\"> finish subpixel blob detector... </font>");
}
#endif
