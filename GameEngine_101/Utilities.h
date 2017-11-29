#ifndef _Utilities_HG_
#define _Utilities_HG_

// Inspired by: https://stackoverflow.com/questions/686353/c-random-float-number-generation

template <class T>

T getRandInRange( T min, T max )
{
	double value = min + static_cast <double> ( rand() ) 
		           / ( static_cast <double> ( RAND_MAX/(static_cast<double>(max-min) ) ) );

	return static_cast<T>(value);

}

#endif
