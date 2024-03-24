int main() {
  volatile int *LEDR_ptr = 0xFF200000;
  volatile int *KEYs = 0xff200050;
  int value;
  int edge_cap;

  while (1) {
    edge_cap = *(KEYs + 3);
    if (edge_cap == 1) {
      *LEDR_ptr = 0x3FF;
      *(KEYs + 3) = 0x1;
    }
    if (edge_cap == 2) {
      *LEDR_ptr = 0x0;
      *(KEYs + 3) = 0x2;
    }
  }
}