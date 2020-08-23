#include "SK_RMs.h"

int main() {

	SK_RM R1(8, 2);
	//cout << "RM's capacity is " << R1.RM.capacity() << endl;
	
	R1.PrintRM("R1");
	R1.Write(127);
	R1.PrintRM("R1");
	R1.Read();

	/*
	R1.Write(64);
	R1.Read();
	SK_RM R2(8, 2);
	R2.SingleWrite(1);
	R2.SingleWrite(1);
	R2.PrintRM("R2");
	*/
	SK_RMs node1(8, 2, 4);
	node1.InsertL(7, 1);
	node1.InsertL(7, 2);
	node1.InsertL(7, 3);
	node1.InsertL(7, 4);
	node1.InsertL(7, 5);
	node1.InsertL(7, 6);
	node1.InsertL(7, 7);
	node1.InsertL(7, 8);

	node1.RMs[0].PrintRM("RMs[0]");
	node1.RMs[1].PrintRM("RMs[1]");
	node1.RMs[2].PrintRM("RMs[2]");
	node1.RMs[3].PrintRM("RMs[3]");
	cout << endl;
	node1.CompleteRead();
	node1.Shift_Multi(0, 2);
	cout << endl;
	
	node1.Shift_Multi(2, 2);
	node1.Update(4, 15);
	node1.RMs[0].PrintRM("RMs[0]");
	node1.RMs[1].PrintRM("RMs[1]");
	node1.RMs[2].PrintRM("RMs[2]");
	node1.RMs[3].PrintRM("RMs[3]");
	node1.CompleteRead();

	system("pause");
	return 0;
}