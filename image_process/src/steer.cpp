// steer.cpp : �w�q�D���x���ε{�����i�J�I�C
//

#include "stdafx.h"


#include "steer.h"

#include <QImage>

using namespace std;



/*==============================================================================================
	exp ���
	�ΨӳB�z�ǤJ�ȬO��ƪ�case
/*==============================================================================================*/
double	Steer::exp( int x )
{
	return	exp( (double)x );
}




/*==============================================================================================
	�Ψӵepolar��
/*==============================================================================================*/
void	Steer::polar_draw()
{
	int		PolarSizeX	=	501;
	int		PolarSizeY	=	501;
	int		i,	j;
	int		ii,	jj;
	int		x,	y;
	int		x2,	y2;
	int		size	=	1000;
	double	theta,	value;
	double	theta2,	value2;

	polar	=	QImage( PolarSizeX, PolarSizeY, QImage::Format_RGB888 );
	polar.fill( 0 );

	for( i=0; i<MAX_THETA; i++ )
	{
#ifdef _OPENPIV_
		progress->setGress( 100.*i/MAX_THETA );
#endif
		for( j=0; j<size; j++ )
		{
			theta	=	theta_data(i) + pi;
			value	=	value_data(i) * PolarSizeX / 2;
			theta2	=	theta_data( (i+1)%MAX_THETA ) + pi;
			value2	=	value_data( (i+1)%MAX_THETA ) * PolarSizeX / 2;

			x		=	value * cos( theta );
			y		=	value * sin( theta );
			x2		=	value2 * cos( theta2 );
			y2		=	value2 * sin( theta2 );

			x		+=	PolarSizeX/2 + 1;
			y		+=	PolarSizeY/2 + 1;

			x2		+=	PolarSizeX/2 + 1;
			y2		+=	PolarSizeY/2 + 1;

			ii		=	(int)( x*(size-1-j) + x2*j )/size;
			jj		=	(int)( y*(size-1-j) + y2*j )/size;

			polar.pixel_channel( ii, jj, 255, 0, 0 );
		}
	}

}



/*==============================================================================================
	�Ѻc
/*==============================================================================================*/
Steer::~Steer() {}



#ifdef _OPENPIV_
/*=======================================================================================================
	�غc
	OPENPIV ���U�ϥ�
/*=======================================================================================================*/
Steer::Steer(Settings *settingsPass, DataContainer  *filedataPass)	
	: AndreaEngine( )
{	
	progress	=	settingsPass->getProgress();	
	settings	=	settingsPass;
	filedata	=	filedataPass;

    mutex.lock();

	int		i;

	// ��ݭnlock���ʧ@
	N		=	11;					// �M�w�X��
	g_size	=	4;					// ������ 1 �j.
	o_size	=	3;					// ��l size �Ѧҽפ�@�k �ҥH�� 31   ����63*63 �� map
	pi		=	3.141592653589793;

	X		=	settings->get_measure_X();
	Y		=	settings->get_measure_Y();

	w		=	new double[N];
	char		str[100];

	// Ū�� wight�����
	sprintf( str, "./Steer/w%d.txt", N );
	ifstream	in(str);

	for( i=0; i<N; i++ )
		in >> w[i] ;
		//w[i]	=	1.0 / 11.0;
	in.close();

    mutex.unlock();
}
#else
/*==============================================================================================
	�غc���
/*==============================================================================================*/
Steer::Steer()
{
	int		i;

	N		=	11;						// �M�w�X��
	g_size	=	4;
	o_size	=	3;						// ��l size �Ѧҽפ�@�k �ҥH�� 31   ����63*63 �� map
	pi		=	3.141592653589793;

	image	=	QImage("test.bmp");		// �w�]Ū���Ϥ�

	w		=	new double[N];

	char		str[100];

	// Ū�� wight�����
	sprintf( str, "./Steer/w%d.txt", N );
	ifstream	in(str);

	for( i=0; i<N; i++ )
		in >> w[i] ;
		//w[i]	=	1.0 / 11.0;
	in.close();

}
#endif


/*==============================================================================================
	radial ���
	�o��i�H�Ҽ{�λ�����
/*==============================================================================================*/
double		Steer::g_radial( double r )
{
	if( r < g_size )
		return	1;
		//return	(r<=1) ? 1 : 1 + (1.-r)/(g_size-1) ;
	else
		return	0;
}


/*==============================================================================================
	���� g_map
/*==============================================================================================*/
void		Steer::generate_map()
{
	int		i,	j,	n;
	int		ii,	jj;
	double	_he,	_ho;
	double	phi,	alpha;
	double	r;

	for( ii=0; ii<MAX_THETA; ii++ )
		draw_filter( ii );
}


/*==============================================================================================
	��Jmap��� �L�X�ϧ� 
	(�۰�normalized)
/*==============================================================================================*/
void		Steer::output_map( boost::numeric::ublas::matrix<double> map )
{
	output_map( map, "output.bmp");
	/*int			i,	j;
	double		max,	min;
	double		dtmp;

	QImage	img( map.size1(), map.size2(), QImage::Format_RGB888 );
	//img.setwidth_height( map.size1(), map.size2() );

	max		=	0;
	min		=	1234567;

	for( i=0; i<map.size1(); i++ )
		for( j=0; j<map.size2(); j++ )
		{
			if( max < map(i,j) )
				max		=	map(i,j);
			if( min > map(i,j) )
				min		=	map(i,j);
		}
	
	// ��Ӯt���h���� ���B�z
	if( fabs( max - min ) < 0.0001 )
	{
		cout << "max is near min...!!\n";
		max		=	1;
		min		=	0;
	}

	for( i=0; i<map.size1(); i++ )
		for( j=0; j<map.size2(); j++ )
		{
			dtmp	=	255. * ( map(i,j) - min ) / ( max - min );
			img.gray_channel( i, j, (int)dtmp );
		}

	//img.save_image("output.bmp");
	img.save("output.bmp");*/
}



/*==============================================================================================
	��Jmap��� �L�X�ϧ� 
	(�۰�normalized)
	�åB���w�ɦW
/*==============================================================================================*/
void		Steer::output_map( boost::numeric::ublas::matrix<double> map, char *name )
{
	int			i,	j;
	double		max,	min;
	double		dtmp;

	QImage	img( map.size1(), map.size2(), QImage::Format_RGB888 );

	max		=	0;
	min		=	1234567;

	for( i=0; i<map.size1(); i++ )
		for( j=0; j<map.size2(); j++ )
		{
			if( max < map(i,j) )
				max		=	map(i,j);
			if( min > map(i,j) )
				min		=	map(i,j);
		}
	
	// ��Ӯt���h���� ���B�z
	if( fabs( max - min ) < 0.0001 )
	{
		cout << "max is near min...!!\n";
		max		=	1;
		min		=	0;
	}

	for( i=0; i<map.size1(); i++ )
		for( j=0; j<map.size2(); j++ )
		{
			dtmp	=	255. * ( map(i,j) - min ) / ( max - min );
			img.gray_channel( i, j, (int)dtmp );
		}

	img.save(name);
}




/*==============================================================================================
	down sample �ʧ@�b�o��
/*==============================================================================================*/
boost_matrix	Steer::downsample( boost_matrix map )
{
	if( map.size1() != o_size*2 + 1 )
		cout << "Steer::downsample error!!\n";

	int			i,	j;
	int			ii,	tmp;
	double		dtmp;

	boost_matrix	dmap1,	dmap2;					// �Ȧs�Ϊ� map ����Ψ� down sample
	boost_matrix	rmap;

	dmap1.resize(   o_size * 2 + 1,  o_size * 2 + 1 );
	
	for( i = -o_size;    i <= o_size;    i++   )
	{
		for(  j = -o_size;    j <= o_size;   j++  )
		{
			//dmap1( i + o_size,  j + o_size  )	=	i*i + j*j; //exp( -i*i - j*j );		// ���ե�  
			dmap1( i+o_size, j+o_size )		=	map( i+o_size, j+o_size );
		}
	}

	// �i�� downsample���ʧ@
	// ���ƬO�p��X�Ӫ�  �n��case�M�w
	for( ii=0; ii<56; ii++ )
	{
		tmp		=	dmap1.size1() - 1 ;
		dmap2.resize( tmp, tmp );

		// �i��down sample
		for(   i = 0;     i < dmap1.size1() - 1;     i++    )
			for(     j = 0;     j < dmap1.size2() - 1;    j++     )
			{
				dtmp	=	dmap1( i   , j   );
				dtmp	+=	dmap1( i+1 , j   );
				dtmp	+=	dmap1( i   , j+1 );
				dtmp	+=	dmap1( i+1 , j+1 );
				dtmp	*=	0.25;

				// �o��i�H�h���@�ӧP�_ �ݭn���n�O������ (0,0) �I���Ȥ���
				//if( i != tmp/2+1 && j != tmp/2+1 )
				dmap2( i, j )	=	dtmp;
			}

		// �N��Ʀ^�s��dmap1
		dmap1	=	dmap2;
	}

	rmap	=	dmap1;

	//output_map( g_map );

	return	rmap;
}



/*==============================================================================================
	���ե� �efilter
/*==============================================================================================*/
void	Steer::draw_filter( int ii )
{
	boost::numeric::ublas::matrix<int>		count_map;				// �p���map�̭�  �C��pixel ���h�֭��I
	boost::numeric::ublas::matrix<double>	&filter_map_even	=	he[ii];
	boost::numeric::ublas::matrix<double>	&filter_map_odd		=	ho[ii];

	count_map.resize( 2*g_size+1, 2*g_size+1 );
	filter_map_even.resize( 2*g_size+1, 2*g_size+1 );
	filter_map_odd.resize( 2*g_size+1, 2*g_size+1 );
	count_map.clear();
	filter_map_even.clear();
	filter_map_odd.clear();

	int			i,	j,	n;
	double		x,	y,	xx,	yy;
	double		r,	theta;
	double		value,	radial;
	double		_he,	_ho;
	double		dsum;
	double		alpha;

	
	alpha	=	2.0 * ii * pi / MAX_THETA;

	// �η��y�ШӲ���filter
	for( r = 0.01; r <= g_size; r += 0.01 )
		for( theta = 0; theta < 2*pi; theta += 0.01 )
		{
			x		=	r * cos(theta) ;
			y		=	r * sin(theta) ;
			xx		=	fabs(x);			// �Υ��ȥh���|�ˤ��J�A��^�쥻��sign
			yy		=	fabs(y);
			i		=	(int) (xx + 0.5);
			j		=	(int) (yy + 0.5);

			i		*=	( x>=0 ) ? 1 : -1;
			j		*=	( y>=0 ) ? 1 : -1;

			i		+=	g_size;
			j		+=	g_size;
			_he		=	0;
			_ho		=	0;
			// �p�� sum cos( n*theta ) sin(n*theta) ���ʧ@
			for( n = 1; n <= N; n++ )
			{
				_he	+=	w[n-1] * cos( n*(theta-alpha) );
				_ho	+=	w[n-1] * sin( n*(theta-alpha) );
			}
			radial	=	g_radial( r );

			count_map(i, j)++;
			filter_map_even(i,j)	+=	_he * radial;
			filter_map_odd(i,j)		+=	_ho * radial;
		}

	for( i=0; i<filter_map_even.size1(); i++ )
		for( j=0; j<filter_map_even.size2(); j++ )
		{
			if( count_map(i,j) != 0 )
			{
				filter_map_even(i,j)	/=	count_map(i,j);	
				filter_map_odd(i,j)		/=	count_map(i,j);
			}
		}

/*	char	str[100];
	sprintf(str,"even_%d.bmp",ii);
	output_map( filter_map_even, str );

		//char	str[100];
	sprintf(str,"odd_%d.bmp",ii);
	output_map( filter_map_odd, str );*/

}



/*==============================================================================================
	�M��local max
/*==============================================================================================*/
void	Steer::local_max( double &L, double &T, double &X )
{
	int		i,	j;
	int		tmax_i;			// �s��index
	int		max_i_1,	max_i_2,	max_i_3,	max_i_4;
	double	tmax,	max1,	max2;

	std::vector<int>		max_index;

	i	=	value_data.size()-1;
	if( value_data(0) > value_data(i) && value_data(0) > value_data(1) )
	{
		progress->debuger(  QString("local max = %1, v=%2").arg(0).arg( value_data(0)) );
		max_index.push_back(0);
	}

	if( value_data(i) > value_data(0) && value_data(i) > value_data(i-1) )
	{
		progress->debuger(  QString("local max = %1, v=%2").arg(i).arg( value_data(i)) );
		max_index.push_back(i);
	}

	for( i=1; i<value_data.size()-1; i++ )
	{
		if( value_data(i) > value_data(i+1) && value_data(i) > value_data(i-1) )
		{
			progress->debuger(  QString("local max = %1, v=%2").arg(i).arg( value_data(i)) );
			max_index.push_back(i);
		}
	}

	// local max 1
	tmax		=	0;
	tmax_i		=	-1;
	for( i=0; i<max_index.size(); i++ )
	{
		j	=	max_index[i];
		if( tmax < value_data(j) )
		{
			tmax	=	value_data(j);
			tmax_i	=	j;
		}
	}
	max_i_1	=	tmax_i;

	// local max 2
	tmax		=	0;
	tmax_i		=	-1;
	for( i=0; i<max_index.size(); i++ )
	{
		j	=	max_index[i];

		if( j == max_i_1 )	continue;

		if( tmax < value_data(j) )
		{
			tmax	=	value_data(j);
			tmax_i	=	j;
		}
	}
	max_i_2	=	tmax_i;

	// local max 3
	tmax		=	0;
	tmax_i		=	-1;
	for( i=0; i<max_index.size(); i++ )
	{
		j	=	max_index[i];

		if( j == max_i_1 || j == max_i_2 )	continue;

		if( tmax < value_data(j) )
		{
			tmax	=	value_data(j);
			tmax_i	=	j;
		}
	}
	max_i_3	=	tmax_i;

	// local max 4
	tmax		=	0;
	tmax_i		=	-1;
	for( i=0; i<max_index.size(); i++ )
	{
		j	=	max_index[i];
		
		if( j == max_i_1 || j == max_i_2 || j == max_i_3 )	continue;

		if( tmax < value_data(j) )
		{
			tmax	=	value_data(j);
			tmax_i	=	j;
		}
	}
	max_i_4	=	tmax_i;

	// max_i_1  max_i_2 ...  �Olocal max��index


	// �p��L T X junction
	double		m1	=	max_i_1 != -1 ? value_data( max_i_1 ) : 0;
	double		m2	=	max_i_2 != -1 ? value_data( max_i_2 ) : 0;
	double		m3	=	max_i_3 != -1 ? value_data( max_i_3 ) : 0;
	double		m4	=	max_i_4 != -1 ? value_data( max_i_4 ) : 0;

	m2	/=	m1;
	m3	/=	m1;
	m4	/=	m1;
	m1	=	1.0;

	L	=	m1 * m2 * ( 1 - m3 ) * (1 - m4 );
	T	=	m1 * m2 * m3 * (1 - m4 );
	X	=	m1 * m2 * m3 * m4;

}


/*==============================================================================================
	�^�ǳ̤j��
/*==============================================================================================*/
double		Steer::SMAX( double a, double b )
{
	return	a>b ? a : b;
}



/*==============================================================================================
	���o���I�� energy
/*==============================================================================================*/
double		Steer::get_Energy( int ii, int x, int y )
{
	int		i,	j;
	double	dtmp,	dtmp1,	dtmp2;

	dtmp	=	0;
	dtmp1	=	0;
	dtmp2	=	0;
	for( i= -g_size; i<=g_size; i++ )
		for( j= -g_size; j<=g_size; j++ )
		{
			if( i == 0 && j == 0 )
				continue;

			// �o�䪺index  ���F����  �ҥH�~��� x+i  y+j
			if( x+i >= 0 && x+i < image.width() && y+j >= 0 && y+j < image.height() )
			{
				dtmp1	+=	0.0 + he[ii]( i+g_size, j+g_size ) * image.gray_channel( x+i, y+j );
				dtmp2	+=	0.0 + ho[ii]( i+g_size, j+g_size ) * image.gray_channel( x+i, y+j );
	//			dtmp	+=	E[ii]( i+g_size, j+g_size ) * image.gray_channel( x-i, y-j );
			}
		}

	dtmp	=	dtmp1*dtmp1 + dtmp2*dtmp2;
	//dtmp	=	fabs(dtmp1) + fabs(dtmp2);

	return	dtmp;
}







#ifdef _OPENPIV_
/*=======================================================================
	openPIV
	�{�Ǫ��i�J�I
/*=======================================================================*/
void	Steer::dosomething()
{
	// ��l�ʧ@
	image	=	filedata->get_image();

	// ²�檺���b.
	if( X < 0 || X >= image.width() || Y < 0 || Y >= image.height() )
	{
		X	=	0;
		Y	=	0;
	}

	int		i;
	double	theta,	value;
	double	max;

	for( i=0; i<100; i++ )
	{
		progress->setGress( i );
		draw_filter( i );
	}

	theta_data.resize( 100 );
	value_data.resize( 100 );

	for( i=0; i<MAX_THETA; i++ )
	{
		theta	=	2.0 * i * pi * 0.01 ;
		value	=	get_Energy( i, X, Y );
		theta_data( i )		=	theta;
		value_data( i )		=	value;
	}

	
	// �M��local max
	double	L,	T,	X;
	local_max( L, T, X );
	progress->debuger( QString("L = %1, T = %2, X = %3").arg(L).arg(T).arg(X), QColor(0,136,0) );

	// normalized value_data
	max	=	0;
	for( i=0; i<MAX_THETA; i++ )
	{
		progress->setGress( 100.*i/MAX_THETA );
		if( max < value_data(i) )
			max	=	value_data(i);
	}
	progress->debuger( QString("The maximal is %1").arg(max) );
	for( i=0; i<MAX_THETA; i++ )
		value_data(i)	/=	max;

	polar_draw();

	//emit( filedata->output(polar) );
	progress->outputImage( 0, polar );
	progress->debuger("<font color=\"Red\"> finish Steer Junction detector... </font>");

}
#endif

