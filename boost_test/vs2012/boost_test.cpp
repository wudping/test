// bosot_test.cpp : Defines the entry point for the console application.
//


#include <iostream>
#include <string>
using namespace std;

#include <boost/program_options.hpp>
namespace BPO = boost::program_options;

int main( int argc, char** argv )
{
  int iValInt;

  // setup program options description
  BPO::options_description bOptions( "Test Options" );
  bOptions.add_options()
      ( "help", "Produce help message" )
      ( "vint",    BPO::value<int>(), "int value" );
  
  argc = 2;
  argv[0] = "git";
  argv[1] = "--version";

  // parse program options
  BPO::variables_map mVMap;
  BPO::store( BPO::parse_command_line( argc, argv, bOptions ), mVMap );
  BPO::notify( mVMap );

  // output help message if required
  if( mVMap.count( "help" ) )
  {
    cout << bOptions << endl;
    return 1;
  }

  // process other option
  if( mVMap.count( "vint" ) )
  {
    iValInt = mVMap["vint"].as<int>();
    cout << "Int: " << iValInt << endl;
  }
}