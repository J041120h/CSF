#include <cassert>
#include <cstdio>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "bigint.h"


BigInt::BigInt()
  // TODO: initialize member variables
{
  sign = false;
  magnitude = new std::vector<uint64_t>();
}

BigInt::BigInt(uint64_t val, bool negative)
  // TODO: initialize member variables
{
  sign = negative;
  magnitude = new std::vector<uint64_t>();
  magnitude->push_back(val);
}

BigInt::BigInt(std::vector<uint64_t>* val, bool negative)
  // TODO: initialize member variables
{
  sign = negative;
  magnitude = new std::vector<uint64_t>();
  magnitude = val;
}

BigInt::BigInt(std::initializer_list<uint64_t> vals, bool negative)
  // TODO: initialize member variables
{
  sign = negative;
  magnitude = new std::vector<uint64_t>(vals);
}

BigInt::BigInt(const BigInt &other)
  // TODO: initialize member variables
{
  this->sign = other.sign;
  magnitude = new std::vector<uint64_t>();
  for(auto i = other.magnitude->begin(); i != other.magnitude->end(); ++i)
  {
    this->magnitude->push_back(*i);
  }
}

BigInt::~BigInt()
{
  delete magnitude;
}

BigInt &BigInt::operator=(const BigInt &rhs)
{
  // TODO: implement
  this->sign = rhs.sign;
  for(auto i = rhs.magnitude->begin(); i != rhs.magnitude->end(); ++i)
  {
    this->magnitude->push_back(*i);
  }
}

bool BigInt::is_negative() const
{
  // TODO: implement
  return sign;
}

const std::vector<uint64_t> &BigInt::get_bit_vector() const {
  // TODO: implement
  return *magnitude;
}

uint64_t BigInt::get_bits(unsigned index) const
{
  // TODO: implement
  int size = magnitude->size();
  if (size < (index+1))
  {
    return uint64_t(0);
  }
  else {
    return magnitude->at(index);
  }
}

BigInt BigInt::add_magnitudes(const BigInt &lhs, const BigInt &rhs)
{
  uint64_t carrier = 0;
  int count;
  std::vector<uint64_t>* result = new std::vector<uint64_t>();
  uint64_t sum;
  if (lhs.get_bit_vector().size()>= rhs.get_bit_vector().size())
  {
    count = lhs.get_bit_vector().size();
  }
  else 
  {
    count = rhs.get_bit_vector().size();
  }
  for (int i=0; i<count; i++)
  {
    sum = rhs.get_bits(i)+lhs.get_bits(i)+carrier;

    if((rhs.get_bits(i)==0xFFFFFFFFFFFFFFFF)||(lhs.get_bits(i)==0xFFFFFFFFFFFFFFFF))
    {
      if (carrier == 1)
      {
        carrier = 1;
        result->push_back(sum);
        continue;
      }
    }

    if((sum < rhs.get_bits(i))||(sum < lhs.get_bits(i)))
    {
      carrier = 1;
    }
    else {
    carrier =0;
    }
    result->push_back(sum);
  }

  if (carrier == 1)
  {
    result->push_back(1);
  }

  return BigInt(result);
}

int BigInt::compare_magnitudes(const BigInt &lhs, const BigInt &rhs)
{
  if (lhs.get_bit_vector().size() > rhs.get_bit_vector().size())
  {
    return 1;
  }
  else if (lhs.get_bit_vector().size() < rhs.get_bit_vector().size())
  {
    return 2;
  }
  else 
  {
    int count = lhs.get_bit_vector().size();
    for (int i=count; i>=0; i--)
    {
      if (lhs.get_bits(i) == rhs.get_bits(i))
      {
        continue;
      }
      else if (lhs.get_bits(i)>rhs.get_bits(i))
      {
        return 1;
      }
      else {
        return 2;
      }
    }
    return 0;
  }
}

BigInt BigInt::subtract_magnitudes(const BigInt &lhs, const BigInt &rhs)
{
  uint64_t carrier = 0;
  int count = lhs.get_bit_vector().size();;
  std::vector<uint64_t>* result = new std::vector<uint64_t>();
  uint64_t sum;

  for (int i=0; i<count; i++)
  {
    sum = lhs.get_bits(i)-rhs.get_bits(i)-carrier;

    if(sum > lhs.get_bits(i))
    {
      carrier = 1;
    }
    else {
      carrier =0;
    }
    result->push_back(sum);
  }
  return BigInt(result);
}

void BigInt::sign_set(bool status)
{
  this->sign = status; 
}

BigInt BigInt::operator+(const BigInt &rhs) const
{
  // TODO: implement
  int check = compare_magnitudes(*this, rhs);
  if (rhs.sign == this->sign)
  {
    BigInt result = add_magnitudes(*this, rhs);
    result.sign_set(rhs.sign);
    return result;
  }
  else 
  {
    if (check == 0)
    {
      return BigInt(uint64_t(0));
    }
    else if (check ==1)
    {
      BigInt result = subtract_magnitudes(*this, rhs);
      result.sign_set(this->sign);
      return result;
      
    }
    else {
      BigInt result = subtract_magnitudes(rhs,*this); 
      result.sign_set(rhs.sign);
      return result; 
    }
  }
}

BigInt BigInt::operator-(const BigInt &rhs) const
{
  // TODO: implement
  // Hint: a - b could be computed as a + -b
  BigInt temp = rhs;
  temp.sign_set(!rhs.sign);
  return(*this+temp);
}

BigInt BigInt::operator-() const
{
  // TODO: implement
  BigInt result = *this;
  result.sign_set(!(this->sign));
  return(result);
}

bool BigInt::is_bit_set(unsigned n) const
{
  // TODO: implement
}

BigInt BigInt::operator<<(unsigned n) const
{
  // TODO: implement
}

BigInt BigInt::operator*(const BigInt &rhs) const
{
  // TODO: implement
}

BigInt BigInt::operator/(const BigInt &rhs) const
{
  // TODO: implement
}

int BigInt::compare(const BigInt &rhs) const
{
  // TODO: implement
}

std::string BigInt::to_hex() const
{
  // TODO: implement
  std::string result;
  std::stringstream temp;
  int size = this->magnitude->size();
  if (this->sign == true)
  {
    temp<<'-';
  }
  for(int i = (size-1); i>=0; i--)
  {
    if (i ==size-1)
    {
      temp << std::hex << this->get_bits(i);
    }
    else
    {
      temp<< std::setfill('0') << std::setw(16) << this->get_bits(i);
    }
  }
  result = temp.str();
  if (result.empty()) {
    result = '0';
  }
  return result;
}

std::string BigInt::to_dec() const
{
  // TODO: implement
}

