// ParallelForLoopBenchmark.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <ctime>
#include "StopWatch.h"
#include <omp.h>
#include <ppl.h>

const size_t size=50000000;
BYTE* a = new BYTE[size];
BYTE* b = new BYTE[size];
BYTE* c = new BYTE[size];

using namespace std;

void Init();

void Destroy();

void Serial();

void OpenMP();

void PPL();

void AutoParallelizer();


int _tmain(int argc, _TCHAR* argv[])
{
	Init();

	Serial();

	OpenMP();

	PPL();

	AutoParallelizer();

	Destroy();

	return 0;
}

void Init()
{
	srand( (unsigned int)(time(NULL)) );

	#pragma loop(hint_parallel(8))
	for(int i=0; i<size; ++i)
	{
		a[i] = (BYTE)(rand()%127);
		b[i] = (BYTE)(rand()%127);
	}
}

void Destroy()
{
	delete [] a;
	delete [] b;
	delete [] c;
}

void Serial()
{
	StopWatch watch;
	watch.Start();
	for(size_t i=0; i<size; ++i)
	{
		c[i] = a[i] * b[i];
	}
	UINT time = watch.Stop();

	cout<<"Serial loop timing:"<<time<<endl;
}

void OpenMP()
{
	StopWatch watch;
	watch.Start();
	//omp_set_dynamic( 0 );
	//omp_set_num_threads( 8 );
	#pragma omp parallel for
	for(int i=0; i<size; ++i)
	{
		c[i] = a[i] * b[i];
	}
	UINT time = watch.Stop();

	cout<<"OpenMP loop timing:"<<time<<endl;

}
void PPL()
{
	StopWatch watch;
	watch.Start();
	Concurrency::parallel_for(size_t(0), size, [&](size_t i)
	{
		c[i] = a[i] * b[i];
	}
	);
	UINT time = watch.Stop();

	cout<<"PPL loop timing:"<<time<<endl;
}

void AutoParallelizer()
{
	StopWatch watch;
	watch.Start();
	#pragma loop(hint_parallel(8))
	for(size_t i=0; i<size; ++i)
	{
		c[i] = a[i] * b[i];
	}
	UINT time = watch.Stop();

	cout<<"Auto parallelizer loop timing:"<<time<<endl;
}