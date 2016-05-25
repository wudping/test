#include "c11_sample.h"
#include <iostream>
#include <vector>
#include <list>
#include <deque>

using namespace std;


void c11_sample_1()
{
    int     i;
    
    // Uniform initialization
    int a1 = {-5};
    cout << "a1 = " << a1 << endl;
    
    //
    float a2{5.44};
    cout << "a2 = " << a2 << endl;
    
    //
    unsigned int a3(5);  // c99
    cout << "a3 = " << a3 << endl;
    
    //
    double a4[5]{1.1, 2.2, 3.3, 4.4, 5.5};
    for( i = 0; i < 5; i++ )
        cout << a4[i] << " ";
    cout << endl;
    
    //
    char a5[5] = { 'a', 'b', 'c', 'd', 'e' };   // c99
    for( i = 0; i < 5; i++ )
        cout << a5[i] << " ";
    cout << endl;
    
    //
    int *ptr = new int[3]{ 5, 9, 3};
    for( i = 0; i < 3; i++ )
        cout << ptr[i] << " ";
    cout << endl;
    
    //
    //int a6{2.34};   // compile error. can't convert double to int.
    int a7 = 2.34;    // ok. a7 = 2   convert double to int
    cout << a7 << endl;
}



// initializer list
template<typename T>
std::deque<T> sort_alpha( std::initializer_list<T> list )
{
    std::deque<T>    deq{list};
    
    if( list.size() > 0 )
    {
        for( int i = 0; i < deq.size(); i++ )
            for( int j = 0; j < deq.size()-1; j++ )
            {
                if( deq[j] > deq[j+1] )
                    deq[j] ^= deq[j+1] ^= deq[j] ^= deq[j+1];
            }
    }
    else
    {
        deq.clear();
        deq.push_back( T() );
        deq.push_back( T{} );
        deq.push_back( T()+1 ); // 並不是一個好的寫法,僅為測試用
    }
    
    return  deq;
}




template<typename T>
void print_deque( T a )
{
    cout << "print " << a << " ";
}


void c11_sample_2()
{
    //
    std::deque<char> deq = sort_alpha<char>( {'a', 'z', 'd', 'e', 'h', 'i', 'x', 'y', 'b', 'u', 'v'} );
    std::for_each( deq.begin(), deq.end(), []( char& c )    // it can use char& or char
                  {
                      cout << c << " ";
                  });
    cout << endl;
    
    //
    std::deque<int> deq2 = sort_alpha<int>( {} );
    std::for_each( deq2.begin(), deq2.end(), print_deque<int> );
    cout << endl;
    
    //
    std::vector<double> v = { 1.1, 2, 3, 4.4 };
    for( int i = 0; i < v.size(); i++ )
        cout << v[i] << " ";
    cout << endl;
    
    //
    std::list<int> list{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    for( auto itr = list.begin(); itr != list.end(); ++itr )
        cout << *itr << " ";
    cout << endl;
    
    // note: for迴圈有更多應用方式,這邊不多作介紹.
    // search Range-based for loop, see http://en.cppreference.com/w/cpp/language/range-for
    for ( int x : {-1, -2, -3} )
        cout << x << " ";
    cout << endl;
    
    //
    auto ptr = new int[3]{100, 99, 98};
    cout << ptr[0] << " " << ptr[1] << " " << ptr[2] << endl;
    delete [] ptr;

}



class B
{
public:
    B( int _a ) : a(_a)
    {}
    
    B( int _a, double _d ) : a(_a), d(_d)
    {}
    
    int a = 4;
    //int b(3);  // compile fail.
    int c{4};
    double d;
};



class A
{
public:
    A() : ptr( new int[5]{1,2,3,4,5} )
    {
        cout << "A()" << endl;
    }
    
    A( int _d1, int _d2 ) : data1(_d1), data2(_d2)
    {
        cout << "A(int,int)" << endl;
    }
    
    // initializer list
    A( std::initializer_list<int> list )
    {
        cout << "A(initializer_list<int>" << endl;
        
        int count = 0;
        ptr = new int[ list.size() ];
        for( auto itr = list.begin(); itr != list.end(); ++itr )
        // for( int i = 0, auto itr = list.begin(); ... ) will compile error
            ptr[count++]  =   *itr;
    }

    // it will run initializer_list<int>, not run initializer_list<short>
    A( std::initializer_list<short> list )
    {
        cout << "A(initializer_list<short>" << endl;
        
        int count = 0;
        ptr = new int[ list.size() ];
        for( auto itr = list.begin(); itr != list.end(); ++itr )
            // for( int i = 0, auto itr = list.begin(); ... ) will compile error
            ptr[count++]  =   *itr;
    }

    
    ~A()
    {
        delete [] ptr;
    }
    
    int data1 = 11;
    int data2{99};
    int *ptr = nullptr;
    
    //B b(99); // compile fail.
    B b1{99};
    B b2 = {99};
    B b3 = {-4, 4.5442};
    
};


void c11_sample_3()
{
    int i;
    
    A a1;
    for( i = 0; i < 5; i++ )
        cout << a1.ptr[i] << " ";
    cout << a1.data1 << " " << a1.data2 << endl;
    
    A a2(3,4);   // c99
    cout << a2.data1 << " " << a2.data2 << " " << a2.b3.d << endl;
    //cout << a2.ptr[0] << endl;  // crash
    
    // if use initialer_list in constructor, a3 and a4 will run A(initializer_list), not A(int,int)
    A a3{ 2, 3 };
    cout << a3.data1 << " " << a3.data2 << endl;
    
    A a4 = { 36, -53 };
    cout << a4.data1 << " " << a4.data2 << endl;
    
    A a5( {-1,2,0,4} );
    //A a5{ {-1,2,0,4} }; the same as above.v
    for( i = 0; i < 4; i++ )
        cout << a5.ptr[i] << " ";
    cout << endl;
    
}



void c11_sample_4()
{
    // left value, right value, move sema
    
    int &&a = 13;
    printf( "%d\n", a );
    a = 4;
    printf( "%p\n", &a );
    
}



