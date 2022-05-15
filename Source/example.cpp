#include <iostream>
#include "Trainer.h"

int main() {
	Trainer a("xyz.exe");	// Attach the Trainer to a process. The process must be running
	a.AddEntry("ammo", 0x10f4f4, { 0x374, 0x14, 0x0 }, 0x0); // Add an entry you can refer to with "ammo". Add the static address with its corresponding offsets. 
	// The last parameter describes a pure offset.
	a.Write("ammo", int(1000))	// Set the ammo to 1000
	return a.Read<int>("ammo"); // This should return 1000
}
