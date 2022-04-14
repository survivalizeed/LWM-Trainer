#include <iostream>
#include "Trainer.h"

int main() {
	Trainer a("ac_client.exe");
	a.AddEntry("ammo", 0x10f4f4, { 0x374, 0x14, 0x0 });
	a.WriteDynAddress(0x00475E8A, {0x0, 0x1, 0x2, 0x3});
	std::cout << a.ReadDynAddress<long>(0x00475E8A);
	a.Write("ammo", long long(123));
}