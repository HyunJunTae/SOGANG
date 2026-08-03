/*
 * absVal(x): return the absolute value of x.
 * - Assume that INT_MIN (-2^31) is not provided as an input.
 * - Ex) absVal(2) = 2, absVal(-1) = 1
 */
int absVal(int x)
{

  int sign = x >> 31; // 부호
  int zero_or_one = (~sign) + 1;
  int x_after_change = (x ^ sign) + zero_or_one;
  return x_after_change;
}
