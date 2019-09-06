#ifndef _DEC2_H_
#define _DEC2_H_

class Dec2 {
public:
	int x;
	int y;

	Dec2();
	Dec2(int _x, int _y);
	Dec2(Dec2 *dec2);


	bool operator<(const Dec2& d) const;
	bool operator!=(const Dec2& d) const;
	bool operator==(const Dec2& d) const;

	const Dec2 operator+(const Dec2 &d) const;
	const Dec2 operator-(const Dec2 &d) const;
	const Dec2 operator*(const Dec2 &d) const;

	Dec2 operator/(const int &d) const;

	Dec2& operator+=(const Dec2& d);

	const Dec2 operator*(const int &d) const;

	static int getMachDistance(const Dec2 &d1, const Dec2 &d2);

};


#endif //_DEC2_H_