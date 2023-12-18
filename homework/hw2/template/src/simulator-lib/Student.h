#pragma once


class Student
{
private:
	unsigned factNum;
	unsigned minuteOfArriving;
	char spec[25];
	unsigned minutesInClub33;
public:
	Student() = default;
	Student(const unsigned& factNum, const unsigned& minuteOfArriving,
		const char spec[], const unsigned& minutesInClub33);
	Student(const Student& other) = default;
	Student& operator=(const Student& other) = default;
	~Student() = default;

	void setFactNum(const unsigned& factNum);
	void setMinuteOfArriving(const unsigned& minuteOfArriving);
	void setSpec(const char spec[]);
	void setMinuteInClub33(const unsigned& minuteInClub33);

	unsigned getFactNum() const;
	unsigned getMinuteOfArriving() const;
	const char* getSpec() const;
	unsigned getMinuteInClub33() const;
};

