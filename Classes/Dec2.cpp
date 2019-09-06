#include "Dec2.h"

#include <cmath>

Dec2::Dec2(int _x, int _y){
	x = _x;
	y = _y;
}

Dec2::Dec2():Dec2(0, 0){

}

Dec2::Dec2(Dec2 *dec2):Dec2(dec2->x, dec2->y){

}

bool Dec2::operator<(const Dec2& d) const{
	if (x == d.x){
		return y < d.y;
	}

	return x < d.x;
}

const Dec2 Dec2::operator+(const Dec2 &d) const{
	Dec2 result(*this);
	result.x += d.x;
	result.y += d.y;

	return result;
}

const Dec2 Dec2::operator-(const Dec2 &d) const{
	Dec2 result(*this);
	result.x -= d.x;
	result.y -= d.y;

	return result;
}

const Dec2 Dec2::operator*(const Dec2 &d) const{
	Dec2 result(*this);
	result.x *= d.x;
	result.y *= d.y;

	return result;
}

const Dec2 Dec2::operator*(const int &d) const{
	Dec2 result(*this);
	result.x *= d;
	result.y *= d;

	return result;
}

bool Dec2::operator!=(const Dec2& d) const{
	return this->x != d.x || this->y != d.y;
}

bool Dec2::operator==(const Dec2& d) const {
	return this->x == d.x && this->y == d.y;
}

Dec2& Dec2::operator+=(const Dec2& d){
	this->x += d.x;
	this->y += d.y;

	return *this;
}

Dec2 Dec2::operator/(const int &d) const{
	Dec2 result(*this);
	result.x /= d;
	result.y /= d;

	return result;
}

int Dec2::getMachDistance(const Dec2 &d1, const Dec2 &d2){
	int ret = std::abs(d1.x - d2.x) + std::abs(d1.y - d2.y);

	return ret;
}