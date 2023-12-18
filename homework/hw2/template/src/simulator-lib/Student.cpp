#include "Student.h"
#include <iostream>

Student::Student(const unsigned& factNum, const unsigned& minuteOfArriving,
	const char spec[], const unsigned& minutesInClub33)
{
	setFactNum(factNum);
	setMinuteOfArriving(minuteOfArriving);
	setSpec(spec);
	setMinuteInClub33(minutesInClub33);
}

void Student::setFactNum(const unsigned& factNum)
{
	this->factNum = factNum;
}
void Student::setMinuteOfArriving(const unsigned& minuteOfArriving)
{
	this->minuteOfArriving = minuteOfArriving;
}
void Student::setSpec(const char spec[])
{
	strcpy(this->spec, spec);
}
void Student::setMinuteInClub33(const unsigned& minuteInClub33)
{
	this->minutesInClub33 = minuteInClub33;
}

unsigned Student::getFactNum() const
{
	return factNum;
}
unsigned Student::getMinuteOfArriving() const
{
	return minuteOfArriving;
}
const char* Student::getSpec() const
{
	return spec;
}
unsigned Student::getMinuteInClub33() const
{
	return minutesInClub33;
}