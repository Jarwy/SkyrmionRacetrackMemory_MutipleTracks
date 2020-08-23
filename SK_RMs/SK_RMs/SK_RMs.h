#include<iostream>
#include<string>
#include<vector>
#include<bitset>
using namespace std;


class SK_RM {
private:
	int bits;				//資料大小(bit)
	int volumn;				//記憶體容量(bit)
	int NofHead;			//讀寫頭數量
	int bufferSize;			//位移容量(bit)
	int interval;			//讀寫頭工作大小(bit)
	int totalCapacity;		//記憶體容量+位移容量
public:
	vector<int> headTable;	//讀寫頭位置
	vector<int> RM;			//<<error:vector不是樣板>> SOL: 加上"using namespace std;" 
	SK_RM(int _volumn, int _headNumber);
	void PrintRM(string _name);
	void LeftShift();
	void RightShift();
	void DeleteL(int index);
	void DeleteR(int index);
	void InsertL(int index, int value);
	void InsertR(int index, int value);
	void Update(int index, int value);
	void Write(int value);
	void Read();
	void BitWrite(int bitvalue);
	int BitReturn(int idx);
	friend class SK_RMs;
};

class SK_RMs {
private:
	int volumn;				//記憶體容量(bit)
	int NofHead;			//讀寫頭數量
	int bufferSize;			//位移容量(bit)
	int trackNumber;		//記憶體條數
	
public:
	vector<int> headTable;	//讀寫頭位置
	vector<SK_RM> RMs;
	SK_RMs(int _volumn, int _headNumber, int _trackNumber);
	void LeftShift();							//多軌單位左移
	void RightShift();							//多軌單位右移
	void Shift_Multi(bool direction, int step);		//多軌多位位移
	void InsertL(int idx, int value);
	void InsertR(int idx, int value);
	void DeleteL(int idx);
	void DeleteR(int idx);
	void Update(int idx, int value);
	void CompleteRead();							//以interleaved的方式讀完整塊二維Racetrack memory
	int HeadSelector(int idx);
	bool FindLocation(int headIdx, int bitIdx, int step);
};
