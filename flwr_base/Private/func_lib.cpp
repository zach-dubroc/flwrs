// Fill out your copyright notice in the Description page of Project Settings.


#include "func_lib.h"

bool Ufunc_lib::test_function(const int& input)
{
	if (input % 2 == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
