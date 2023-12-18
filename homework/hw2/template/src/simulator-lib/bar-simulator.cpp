#include "bar-simulator.h"
#include "Student.h"
#include "List.h"

Student loadStudent(std::istream& input);
void putStudentsInTheList(std::istream& input, List& list, const unsigned& G);
char* loadSpec(std::istream& input);
bool isDigit(const char& symbol) { return '0' <= symbol && symbol <= '9'; }
size_t findStudentFriend(const Student& st, List& ls, const unsigned& G);
size_t findStudentPos(const Student& st, List& ls);
void manageTheClub(const unsigned& K, const unsigned& G, std::ostream& output, List& students);
unsigned findMinTimeInTheClub(List& inTheClub);
unsigned countGroup(List::iterator it, const unsigned& G, const unsigned& minute);
void deleteStudentsFromQueue(List& students, List& inTheClub);
void studentsExitFromClub(const unsigned& minute, unsigned& students, List& inTheClub, std::ostream& output);
bool checkIfStudentsIsInTwoLists(const Student& st, List& inTheClub);


///
/// @brief Simulate the student bar problem
///
/// @param input
///   The stream, which contains the simulation commands
/// @param output
///   The stream, where the steps of simulation have to be printed.
///
void simulate_bar(std::istream& input, std::ostream& output)
{
	unsigned K, G, N;
	input >> K >> G >> N;
	if (K == 0 && G != 0)
		throw std::runtime_error("No space in the club!\n");
	else if (K == 0  || N == 0)
	{
		output << "";
		return;
	}			
	List list;
	putStudentsInTheList(input, list, G);
	manageTheClub(K, G, output, list);
}


void putStudentsInTheList(std::istream& input, List& list, const unsigned& G)
{
	while (input.peek() != EOF)
	{
		if (list.isEmpty())
			list.pushBack(loadStudent(input));
		else
		{
			Student tmp = loadStudent(input);
			if (input.peek() == EOF)
				break;
			size_t posPush = findStudentFriend(tmp, list, G);
			if (posPush + 1 >= list.getSize())
			{
				posPush = list.getSize();
				list.pushAt(posPush, tmp);
			}
			else
				list.pushAt(posPush + 1, tmp);
		}
	}
}
Student loadStudent(std::istream& input)
{
	Student st = Student();
	unsigned helper;
	for (short i = 0; i < 4 && input.peek() != EOF; ++i)
	{
		switch (i)
		{
		case 0:
			input >> helper;
			if (helper == '\n')
				break;
			st.setFactNum(helper);
			break;
		case 1:
			input >> helper;
			st.setMinuteOfArriving(helper);
			break;
		case 2:
			st.setSpec(loadSpec(input));
			break;
		case 3:
			input >> helper;
			st.setMinuteInClub33(helper);
			break;
		}
	}
	return st;
}
char* loadSpec(std::istream& input)
{
	char result[25];
	char helper;
	unsigned i = 0;
	while (!isDigit(input.peek()))
	{
		input.get(helper);
		result[i++] = helper;
	}
	result[i] = '\0';
	return result;
}

size_t findStudentFriend(const Student& st, List& ls, const unsigned& G)
{
	if (ls.getSize() == 1)
		return 1;
	unsigned groupCount = 0;
	List::iterator it1 = ls.begin();
	List::iterator it2 = ++ls.begin();
	while (it1 && it2)
	{
		if (!strcmp(it1.operator*().getSpec(), st.getSpec()))
			++groupCount;
		if (!strcmp(it1.operator*().getSpec(), st.getSpec()) &&
			strcmp(it2.operator*().getSpec(), st.getSpec()))
		{
			if (groupCount < G)
			{
				return findStudentPos(it1.operator*(), ls);
			}
			else
				groupCount = 0;
		}
		++it1;
		++it2;
	}
	return ls.getSize();
}
size_t findStudentPos(const Student& st, List& ls)
{
	size_t pos = 0;
	for (const Student& val : ls)
	{
		if (st.getFactNum() == val.getFactNum())
			return pos;
		++pos;
	}
	return -1;
}

void manageTheClub(const unsigned& K, const unsigned& G, std::ostream& output, List& students)
{
	List inTheClub;
	unsigned studentsInTheClub = 0;
	unsigned minute = 0;
	List::iterator it = students.begin();
	while (!students.isEmpty()) {
		for (unsigned i = 0; i < students.getSize(); ++i) {
			if (!it.valid())
				break;
			if (it.operator*().getMinuteOfArriving() <= minute
				&& countGroup(it, G, minute) <= (K - studentsInTheClub)) {
				unsigned sizeGr = countGroup(it, G, minute);
				while (sizeGr != 0) {
					output << minute << ' ' << it.operator*().getFactNum() << " enter\n";
					it.operator*().setMinuteInClub33(it.operator*().getMinuteInClub33() + minute);
					inTheClub.pushBack(it.operator*());
					++studentsInTheClub;
					--sizeGr;
					++it;
					if (!it.valid())
						break;
				}
			}
			else {
				unsigned sizeGr = countGroup(it, G, minute);
				for (unsigned i = 0; i < sizeGr; ++i, ++it);
			}
			if (studentsInTheClub == K)
				break;
		}
		deleteStudentsFromQueue(students, inTheClub);
		if (studentsInTheClub == K)
			minute = findMinTimeInTheClub(inTheClub);
		else
			++minute;
		studentsExitFromClub(minute, studentsInTheClub, inTheClub, output);
		if (students.isEmpty())
			break;
		it.startAgain();
	}
	
	if (studentsInTheClub != 0) {
		do {
			minute = findMinTimeInTheClub(inTheClub);
			studentsExitFromClub(minute, studentsInTheClub, inTheClub, output);
		} while (studentsInTheClub != 0);
	}
}
unsigned findMinTimeInTheClub(List& inTheClub)
{
	unsigned res = inTheClub.front().getMinuteInClub33();
	for (const Student& val : inTheClub)
	{
		if (val.getMinuteInClub33() < res)
			res = val.getMinuteInClub33();
	}
	return res;
}

unsigned countGroup(List::iterator it, const unsigned& G, const unsigned& minute)
{
	size_t count = 1;
	List::iterator tmp = it;
	++it;
	if (!it.valid())
		return count;
	while (!strcmp(tmp.operator*().getSpec(), it.operator*().getSpec()) 
		&& it.operator*().getMinuteOfArriving() <= minute)
	{
		++count;
		++it;
		if (count > G)
			return 1;
		if (!it.valid())
			return count;
	}
	return count;
}


void deleteStudentsFromQueue(List& students, List& inTheClub)
{
	for (const Student& it : inTheClub) {
		if (checkIfStudentsIsInTwoLists(it, inTheClub)
			&& checkIfStudentsIsInTwoLists(it, students)) {
			students.popAt(findStudentPos(it, students));
		}
	}
}
void studentsExitFromClub(const unsigned& minute, unsigned& students, List& inTheClub, std::ostream& output)
{
	List tmp;
	for (const Student& it : inTheClub) {
		if (it.getMinuteInClub33() <= minute) {
			tmp.pushBack(it);
			output << minute << ' ' << it.getFactNum() << " exit\n";
		}
	}
	for (const Student& it : tmp) {
		inTheClub.popAt(findStudentPos(it, inTheClub));
		--students;
	}
}
bool checkIfStudentsIsInTwoLists(const Student& st, List& inTheClub)
{
	for (const Student& val : inTheClub) {
		if (val.getFactNum() == st.getFactNum())
			return true;
	}
	return false;
}
