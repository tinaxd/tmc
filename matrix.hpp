#pragma once

#include <array>
#include <iostream>
#include <type_traits>

namespace tmc
{

  template <typename T>
  T zero()
  {
    T z;
    return z;
  }

#define ZERO(tyn_, exp_) template <> tyn_ zero() { return exp_; }
  ZERO(int, 0)
  ZERO(long, 0)
  ZERO(long long, 0)
  ZERO(float, 0.0f)
  ZERO(double, 0.0)
#undef ZERO

  template <typename T, size_t N>
  std::array<T, N> zero_array()
  {
    std::array<T, N> array;
    for (size_t i=0; i<N; i++)
      {
	array[i] = zero<T>();
      }
    return array;
  }
     
  template <typename T, size_t M>
  class Row;
  
  template <typename T, size_t M, size_t N>
  class Matrix
  {
  public:
    Matrix() {}
    
    Matrix(std::array<T, M*N> array)
    {
      for (size_t i=0; i<M*N; i++)
	{
	  auto row = i / N;
	  auto col = i % N;
	  rows[row][col] = array[i];
	}
    }
      
    Matrix(const Matrix& m)
      : rows(m.rows) {}

    Matrix(Matrix&& m)
      : rows(std::move(m.rows)) {}

    Matrix(Row<T, N> row)
      : rows({row})
    {
      static_assert(M == 1, "cannot convert to Row because N != 1");
    }

    Matrix& operator=(const Matrix& m)
    {
      rows = m.rows;
      return *this;
    }

    Matrix& operator=(Matrix&& m)
    {
      rows = std::move(m.rows);
      return *this;
    }

    bool operator==(const Matrix& o)
    {
      return rows == o.rows;
    }

    Matrix& operator+=(const Matrix& rhs)
    {
      for (size_t i=0; i<M*N; i++)
	{
	  rows[i] += rhs.rows[i];
	}
      return *this;
    }

    Matrix& operator-=(const Matrix& rhs)
    {
      for (size_t i=0; i<M*N; i++)
	{
	  rows[i] -= rhs.rows[i];
	}
      return *this;
    }

    Matrix& operator*=(const Matrix& rhs)
    {
      Matrix<T, M, N> new_mat;
      for (size_t i=0; i<M; i++)
	{
	  for (size_t j=0; j<N; j++)
	    {
	      auto sum = zero<T>();
	      for (size_t k=0; k<N; k++)
		{
		  sum += rows[i][k] * rhs[k][j];
		}
	      new_mat[i][j] = std::move(sum);
	    }
	}
      *this = std::move(new_mat);
      return *this;
    }
	      
    Row<T, N>& operator[](size_t index)
    {
      return rows[index];
    }

    const Row<T, N>& operator[](size_t index) const
    {
      return rows[index];
    }

  private:
    std::array<Row<T, N>, M> rows;
  };

  template <typename T, size_t M, size_t N>
  Matrix<T, M, N> operator+(Matrix<T, M, N> lhs, const Matrix<T, M, N>& rhs)
  {
    lhs += rhs;
    return lhs;
  }

  template <typename T, size_t M, size_t N>
  Matrix<T, M, N> operator-(Matrix<T, M, N> lhs, const Matrix<T, M, N>& rhs)
  {
    lhs -= rhs;
    return lhs;
  }

  template <typename T, size_t M>
  Matrix<T, M, M> operator*(Matrix<T, M, M> lhs, const Matrix<T, M, M>& rhs)
  {
    lhs *= rhs;
    return lhs;
  }

  template <typename T, size_t M, size_t N, size_t Q, size_t P>
  Matrix<T, M, P> operator*(const Matrix<T, M, N>& lhs, const Matrix<T, Q, P>& rhs)
  {
    static_assert(N == Q, "dimension mismatch");
    
    Matrix<T, M, P> new_mat;
    for (size_t i=0; i<M; i++)
      {
	for (size_t j=0; j<P; j++)
	  {
	    auto sum = zero<T>();
	    for (size_t k=0; k<N; k++)
	      {
		sum += lhs[i][k] * rhs[k][j];
	      }
	    new_mat[i][j] = sum;
	  }
      }
    return new_mat;
  }

  template <typename T, size_t M, size_t N>
  std::ostream& operator<<(std::ostream& os, const Matrix<T, M, N>& m)
  {
    for (size_t i=0; i<M; i++)
      {
	os << m[i];
	os << std::endl;
      }
    return os;
  }


  template <typename T, size_t M>
  class Row
  {
  public:
    Row()
      : array(zero_array<T, M>()) {}
    Row(std::array<T, M> array)
      : array(std::move(array)) {}
    Row(const Row& row)
      : array(row.array) {}
    Row(Row&& row)
      : array(std::move(row.array)) {}
    Row(Matrix<T, 1, M> mat)
      : Row(mat[0]) {}

    Row& operator=(const Row& row)
    {
      array = row.array;
    }
    
    Row& operator=(Row&& row)
    {
      array = std::move(row.array);
    }

    T& operator[](size_t index)
    {
      return array[index];
    }

    const T& operator[](size_t index) const
    {
      return array[index];
    }

    Row& operator+=(const Row& o)
    {
      for (size_t i=0; i<M; i++)
	{
	  array[i] += o.array[i];
	}
      return *this;
    }

    Row& operator-=(const Row& o)
    {
      for (size_t i=0; i<M; i++)
	{
	  array[i] -= o.array[i];
	}
      return *this;
    }

    bool operator==(const Row& o)
    {
      return array == o.array;
    }
      
  private:
    std::array<T, M> array;
  };

  template <typename T, size_t M>
  std::ostream& operator<<(std::ostream& os, const Row<T, M>& row)
  {
    for (size_t i=0; i<M; i++)
      {
	if (i != 0) os << ' ';
	os << row[i];
      }
    return os;
  }
}
