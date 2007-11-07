#include "bounded_float.hpp"
#include <iostream>
#include <assert.h>

using namespace std;
using namespace Radakan;
using namespace Radakan :: Mathematics;


Bounded_Float::Bounded_Float(float new_max)
{
    this->val = 0;
    this->min = 0;
    this->set_max(new_max);
}//default constructor, behaviour: asserts val is an element of [0;new_max] with a starting value of 0

Bounded_Float::Bounded_Float(float new_val,float new_min,float new_max)
{
	
	this->max = 10.0; // a temporary assignment, so that set_min below passes
	this->set_min(new_min);
	
	this->set_max(new_max);
    this->set_val(new_val);
    
}

Bounded_Float::~Bounded_Float()
{}

float Bounded_Float::get_min() const
{
    return this->min;
}

float Bounded_Float::get_max() const
{
    return this->max;
}

float Bounded_Float::get_val() const
{
    return this->val;
}

void Bounded_Float::set_max(float new_max)
{
	assert(new_max > this->min);	//so that we can't have an illegal situation where min is larger then max
    this->max = new_max;
}

void Bounded_Float::set_min(float new_min)
{
	assert(new_min < this->max); //so that we can't have an illegal situation where min is larger then max
    this->min = new_min;
}

void Bounded_Float::set_val(float new_val)
{
    assert(new_val <= this->max);
    assert(new_val >= this->min);
    this->val = new_val;
}

//=
Bounded_Float Bounded_Float::operator=(const Bounded_Float & operand)
{
    this->set_val(operand.get_val());
    return (*this);
}
Bounded_Float Bounded_Float::operator=(float operand)
{
    this->set_val(operand);
    return (*this);
}


//+
Bounded_Float Bounded_Float::operator+(const Bounded_Float & operand)
{
    return Bounded_Float(this->get_val()+operand.get_val(),this->get_min(),this->get_max());
}
Bounded_Float Bounded_Float::operator+(float operand)
{
    return Bounded_Float(this->get_val()+operand,this->get_min(),this->get_max());
}


//-
Bounded_Float Bounded_Float::operator-(const Bounded_Float & operand)
{
    return Bounded_Float(this->get_val()-operand.get_val(),this->get_min(),this->get_max());
}
Bounded_Float Bounded_Float::operator-(float operand)
{
    return Bounded_Float(this->get_val()-operand,this->get_min(),this->get_max());
}

//*
Bounded_Float Bounded_Float::operator*(const Bounded_Float & operand)
{
    return Bounded_Float(this->get_val()*operand.get_val(),this->get_min(),this->get_max());
}
Bounded_Float Bounded_Float::operator*(float operand)
{
    return Bounded_Float(this->get_val()*operand,this->get_min(),this->get_max());
}

///
Bounded_Float Bounded_Float::operator/(const Bounded_Float & operand)
{
     return Bounded_Float(this->get_val()/operand.get_val(),this->get_min(),this->get_max());
}
Bounded_Float Bounded_Float::operator/(float operand)
{
    return Bounded_Float(this->get_val()/operand,this->get_min(),this->get_max());
}

