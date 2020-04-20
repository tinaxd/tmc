#include "matrix.hpp"

using namespace tmc;

int main()
{
  Matrix<int, 2, 2> c({1, 3, 5, 7});
  Matrix<int, 2, 3> a({1, 2, 3, 4, 5, 6});
  Matrix<int, 2, 2> b({-4, -3, 1, 0});
  Matrix<int, 1, 2> row_mat({1, 2});
  Row<int, 2> row_row = row_mat; // Matrix<T, 1, N> and Row<T, N> are same.
  Matrix<int, 1, 2> row_mat_again = row_row;
  Matrix<int, 2, 2> row_mat_error = row_row; // compile-time error!
  auto d = b * a; // type inference works correctly.
  auto e = a * b; // compile-time error!
  std::cout << d << std::endl;
  return 0;
}
