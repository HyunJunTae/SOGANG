/*
 * bitMask(x): Return a mask that has (32 - x) zeros followed by x ones.
 * - Assume 0 <= x <= 31
 * - Ex) bitMask(0) = 000...000 in binary = 0
 * - Ex) bitMask(31) = 011...111 in binary = 0x7fffffff
 */
int bitMask(int x) {

  int bits = 0xffffffff;
  int bits_moved = bits<<x;
  bits_moved = ~bits_moved;
  
  return bits_moved;
}