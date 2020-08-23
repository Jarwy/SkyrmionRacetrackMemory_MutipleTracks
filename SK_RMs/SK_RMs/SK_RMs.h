#include<iostream>
#include<string>
#include<vector>
#include<bitset>
using namespace std;


class SK_RM {
private:
	int bits;				//��Ƥj�p(bit)
	int volumn;				//�O����e�q(bit)
	int NofHead;			//Ū�g�Y�ƶq
	int bufferSize;			//�첾�e�q(bit)
	int interval;			//Ū�g�Y�u�@�j�p(bit)
	int totalCapacity;		//�O����e�q+�첾�e�q
public:
	vector<int> headTable;	//Ū�g�Y��m
	vector<int> RM;			//<<error:vector���O�˪O>> SOL: �[�W"using namespace std;" 
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
	int volumn;				//�O����e�q(bit)
	int NofHead;			//Ū�g�Y�ƶq
	int bufferSize;			//�첾�e�q(bit)
	int trackNumber;		//�O�������
	
public:
	vector<int> headTable;	//Ū�g�Y��m
	vector<SK_RM> RMs;
	SK_RMs(int _volumn, int _headNumber, int _trackNumber);
	void LeftShift();							//�h�y��쥪��
	void RightShift();							//�h�y���k��
	void Shift_Multi(bool direction, int step);		//�h�y�h��첾
	void InsertL(int idx, int value);
	void InsertR(int idx, int value);
	void DeleteL(int idx);
	void DeleteR(int idx);
	void Update(int idx, int value);
	void CompleteRead();							//�Hinterleaved���覡Ū������G��Racetrack memory
	int HeadSelector(int idx);
	bool FindLocation(int headIdx, int bitIdx, int step);
};
