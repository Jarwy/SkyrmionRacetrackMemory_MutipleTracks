#include "SK_RMs.h"
#include<numeric>

//SK_RM Consructor
SK_RM::SK_RM(int _volumn, int _headNumber) {
	bits = 0;
	volumn = _volumn;
	NofHead = _headNumber;
	interval = volumn / NofHead;
	bufferSize = interval - 1;
	totalCapacity = volumn + bufferSize;
	//vector<int> vec(totalCapacity);
	//RM = vec;
	RM.resize(totalCapacity);

	headTable.resize(NofHead);				//headTable用來存放讀寫頭們所在位置(headNumber為讀寫頭數量)	
	int headIndex = bufferSize;					//預設第一支讀寫頭在vector尾部
	for (int i = 0; i < NofHead; i++) {		//以第一支讀寫頭為準，算出剩下的讀寫頭位置並存入vector
		headTable[i] = headIndex;
		headIndex = headIndex + interval;
	}
	/*印出所有讀寫頭位置(偵錯用)*/
	cout << "Head indexes: ";
	for (int i = 0; i < NofHead; i++) {
	cout <<headTable[i] << " ";
	}cout<< endl;
	
}

void SK_RM::PrintRM(string name) {
	cout << name << ":";
	for (int i = 0; i < volumn; i++) {
		cout << RM[i] << " ";
	}
	cout << endl;
}

//void SK_RM::PrintRM() {
//	for (vector<int>::iterator it = RM.begin(); it!= RM.end(); it++) {
//		cout << *it << " ";
//	}
//	cout << endl;
//}

//
void SK_RM::LeftShift() {	//assume only one side has spare space 
	RM.erase(RM.begin());					//刪除最左邊bit
	RM.resize(totalCapacity);				//resize確保空間不變(相對等於資料往左移)
}

//RightShift整條記憶體一個bit
void SK_RM::RightShift() {
	RM.insert(RM.begin(), 0);				//左邊補零(資料往右推)
	RM.resize(totalCapacity);				//確保空間不變
}

//Delete在指定位置上的bit，左邊的資料往右補
void SK_RM::DeleteL(int index) {
	RM.erase(RM.begin() + index);			//刪除
	RM.insert(RM.begin(), 0);				//左邊補零(資料往右推)
	//RM.assign(RM.begin()+start, RM.end());
	RM.resize(totalCapacity);				//確保空間不變
}

//Delete在指定位置上的bit，右邊的資料往左補
void SK_RM::DeleteR(int index) {
	RM.erase(RM.begin() + index);			//刪除
	RM.insert(RM.end(), 0);					//右邊補零(資料往左推)
	RM.resize(totalCapacity);				//確保空間不變
}

//Insert單一bit在指定位置，位置以左的bit往右移
void SK_RM::InsertL(int index, int value) {
	RM.erase(RM.begin());					//刪除第一位以確保插入後空間不變
	RM.insert(RM.begin() + index, value);	//插入
	//RM.resize(totalCapacity);
}

//Insert單一bit在指定位置，位置以右的bit往右移
void SK_RM::InsertR(int index, int value) {
	RM.insert(RM.begin() + index, value);	//插入
	RM.resize(totalCapacity);				//確保空間不變，資料可能會從右邊被推出去
	RM.swap(RM);							//省空間？待研究
}

//Update指定bit的值
void SK_RM::Update(int index, int value) {
	RM[index] = value;
}

//Write
void SK_RM::Write(int value) {
	int newValue = 0;
	bits = 0;
	while (value != newValue && !(value == 0 && newValue == 1)) {		//進制轉換迴圈
		bits++;															//紀錄寫入bit數
		//cout << value % 2 << " ";										//Used to check if the converter correct
		SK_RM::InsertL(headTable[headTable.size() - 1], value % 2);		//自最尾端的頭寫入(從LSB寫起)
		newValue = value;													
		value = value / 2;
	}
	//cout << endl;
	for (int i = 0; i < volumn - bits; i++) {							//將寫入的資料左移到vector底部
		LeftShift();//若空間為8bit，只寫入{[ ][ ][0][1][0][1][0][1]}，則將資料左移到底，不足補零(左移)
	}
}

//BitWrite
void SK_RM::BitWrite(int bitvalue) {
	SK_RM::InsertL(headTable[NofHead - 1], bitvalue);		//從尾端的讀寫頭寫入單一bit
}

//BitRead
int SK_RM::BitReturn(int idx) {
	return RM[headTable[idx]];					//從指定讀寫頭讀取單一bit
}

//SK_RMs Consructor
SK_RMs::SK_RMs(int _volumn, int _headNumber, int _trackNumber) {
	volumn = _volumn;
	NofHead = _headNumber;
	trackNumber = _trackNumber;
	bufferSize = (volumn / NofHead) - 1;
	vector<SK_RM> vecs(trackNumber,SK_RM(volumn, NofHead));		//使用者輸入記憶體規格及條數
	RMs = vecs;

	headTable.resize(NofHead);
	int headIndex = bufferSize;
	for (int i = 0; i < NofHead; i++) {					//以第一支讀寫頭為準，算出剩下的讀寫頭位置並存入vector
		headTable[i] = headIndex;
		headIndex = headIndex + (volumn / NofHead);			//讀寫頭會在每個服務區尾部
	}
}

void SK_RMs::RMsLeftShift() {
	for (int i = 0; i < trackNumber; i++) {
		RMs[i].LeftShift();
	}
}

void SK_RMs::RMsRightShift() {
	for (int i = 0; i < trackNumber; i++) {
		RMs[i].RightShift();
	}
}

void SK_RMs::Shift_Multi(bool direction, int step) {			//1=>leftshift, 0=>rightShift
	if (direction) {
		for (int i = 0; i < step; i++) { RMsLeftShift(); }
	}
	else {
		for (int i = 0; i < step; i++) { RMsRightShift(); }
	}
}

void SK_RMs::CompleteRead() {
	//寫入順序
	//track0 => trackN
	//  LSB  =>  MSB
	int j = 0;
	vector<int> headValue;														//用來裝讀寫頭下的累計值
	headValue.resize(NofHead, 0);
	for (int i = 0; i < bufferSize; i++) {										//位移迴圈
		for (j = 0; j < NofHead; j++) {										//遍歷讀寫頭迴圈
			headValue[j] = 0; 
			for (int k = 0; k < trackNumber; k++) {
				headValue[j] = headValue[j] + pow(2, k)*RMs[k].BitReturn(j);	//二進制轉十進制
			}
		}
		for (j = 0; j < NofHead; j++) {
			cout << headValue[j] << " ";										//印出當前所有讀寫頭下的十進制累計值
		}
		cout << endl;
		RMsRightShift();														//全體右移
	}

	for (j = 0; j < NofHead; j++) {											//讀取讀寫範圍內的最後一bit後無需再位移
		headValue[j] = 0;
		for (int k = 0; k < trackNumber; k++) {
			headValue[j] = headValue[j] + pow(2, k)*RMs[k].BitReturn(j);
		}
	}
	for (j = 0; j < NofHead; j++) {
		cout << headValue[j] << " ";
	}
	cout << endl;

	for (int i = 0; i < bufferSize; i++) {										//復位
		RMsLeftShift();
	}
	cout << endl;
}

int SK_RMs::HeadSelector(int idx) {
	int interval = volumn / NofHead;
	return (idx / interval);
}

bool SK_RMs::FindLocation(int headIdx, int bitIdx, int step) {	//選擇移動方向，並回傳復位方向
	if (headIdx > bitIdx) {
		Shift_Multi(false, step);
		return true;
	}
	else {
		Shift_Multi(true, step);
		return false;
	}
}

void SK_RMs::InsertL(int value, int idx) {
	bitset<4> b(value);
	int headNumber = HeadSelector(idx);
	int step = abs(headTable[headNumber] - idx);
	bool dir = FindLocation(headTable[headNumber], idx, step);
	for (int i = 0; i < trackNumber; i++) {	//輪遍每一條記憶體寫入
		RMs[i].BitWrite(b[i]);
	}
}