#include <cassert>
#include <cstdint>
#include <cstdio>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>
#include "bigint.h"


BigInt::BigInt()
  // TODO: initialize member variables
{
  sign = false;
  magnitude = new std::vector<uint64_t>();
  magnitude->push_back(uint64_t(0));
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
  return *this;
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

  return(BigInt(result));
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

  return(BigInt(result));
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

std::string BigInt::binary(uint64_t number) const
{
  std::string result;
  uint64_t temp = 1;
  for (int i = 0; i <64; i++)
  {
    if ((temp & number) != 0)
    {
      result.insert(0,1,'1');
    }
    else {
      result.insert(0,1,'0');
    }
    temp = temp<<1;
  }
  return result;
}

bool BigInt::is_bit_set(unsigned n) const
{
  // TODO: implement
  std::string result;
  std::stringstream temp;
  int size = this->magnitude->size();
  for(int i = (size-1); i>=0; i--)
  {
      temp<< binary(this->get_bits(i));
  }
  result = temp.str();
  int length = result.length();

  if( n > (length-1))
  {
    return false;
  }
  char check = result.at(length-n-1);

  if (check == '0')
  {
    return false;
  }
  else {
    return true;
  }
}

BigInt BigInt::operator<<(unsigned n) const
{
  // TODO: implement
  if (sign == true)
  {
    throw std::invalid_argument("The BigInt is negative"); 
  }
  
  int bit_push = n /64;
  int dig_push = n %64;
  std::vector<uint64_t>* result = new std::vector<uint64_t>();
  uint64_t pre = 0;
  uint64_t next = 0;
  uint64_t current = 0;

  for (int i =0; i<bit_push; i++)
  {
    result->push_back(uint64_t(0));
  }

  if (dig_push==0)
  {
    for (auto a = magnitude->begin(); a != magnitude->end(); a++)
    {
      result->push_back(*a);
    }
    return(BigInt(result));
  }

  int size = this->magnitude->size();
  for(int i = 0; i<size; i++)
  {
    current = get_bits(i);
    next = current & (((~0UL)<<(64-dig_push)));
    next = next >> (64-dig_push);
    current = current << dig_push;
    current = current ^ pre;
    result->push_back(current);
    pre = next;
  }

  if (next != 0)
  {
    result->push_back(next);
  }

  return(BigInt(result));
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
  if ((sign == false)&&(rhs.sign == true))
  {
    return 1;
  }
  else if ((sign == true)&&(rhs.sign == false))
  {
    return -1;
  }
  else if ((sign == false)&&(rhs.sign == false))
  {
    int check = compare_magnitudes(*this, rhs);
    if (check == 1)
    {
      return 1;
    }
    else if (check == 2)
    {
      return -1;
    }
  }
  else {
    int check = compare_magnitudes(*this, rhs);
    if (check == 2)
    {
      return 1;
    }
    else if (check == 1)
    {
      return -1;
    }
  }
  return 0;
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
  return result;
}

std::string BigInt::to_dec() const
{
  // TODO: implement
}

