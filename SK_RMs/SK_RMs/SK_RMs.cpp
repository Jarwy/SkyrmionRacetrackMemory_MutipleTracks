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

	headTable.resize(NofHead);				//headTable�ΨӦs��Ū�g�Y�̩Ҧb��m(headNumber��Ū�g�Y�ƶq)	
	int headIndex = bufferSize;					//�w�]�Ĥ@��Ū�g�Y�bvector����
	for (int i = 0; i < NofHead; i++) {		//�H�Ĥ@��Ū�g�Y���ǡA��X�ѤU��Ū�g�Y��m�æs�Jvector
		headTable[i] = headIndex;
		headIndex = headIndex + interval;
	}
	/*�L�X�Ҧ�Ū�g�Y��m(������)*/
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
	RM.erase(RM.begin());					//�R���̥���bit
	RM.resize(totalCapacity);				//resize�T�O�Ŷ�����(�۹ﵥ���Ʃ�����)
}

//RightShift����O����@��bit
void SK_RM::RightShift() {
	RM.insert(RM.begin(), 0);				//����ɹs(��Ʃ��k��)
	RM.resize(totalCapacity);				//�T�O�Ŷ�����
}

//Delete�b���w��m�W��bit�A���䪺��Ʃ��k��
void SK_RM::DeleteL(int index) {
	RM.erase(RM.begin() + index);			//�R��
	RM.insert(RM.begin(), 0);				//����ɹs(��Ʃ��k��)
	//RM.assign(RM.begin()+start, RM.end());
	RM.resize(totalCapacity);				//�T�O�Ŷ�����
}

//Delete�b���w��m�W��bit�A�k�䪺��Ʃ�����
void SK_RM::DeleteR(int index) {
	RM.erase(RM.begin() + index);			//�R��
	RM.insert(RM.end(), 0);					//�k��ɹs(��Ʃ�����)
	RM.resize(totalCapacity);				//�T�O�Ŷ�����
}

//Insert��@bit�b���w��m�A��m�H����bit���k��
void SK_RM::InsertL(int index, int value) {
	RM.erase(RM.begin());					//�R���Ĥ@��H�T�O���J��Ŷ�����
	RM.insert(RM.begin() + index, value);	//���J
	//RM.resize(totalCapacity);
}

//Insert��@bit�b���w��m�A��m�H�k��bit���k��
void SK_RM::InsertR(int index, int value) {
	RM.insert(RM.begin() + index, value);	//���J
	RM.resize(totalCapacity);				//�T�O�Ŷ����ܡA��ƥi��|�q�k��Q���X�h
	RM.swap(RM);							//�٪Ŷ��H�ݬ�s
}

//Update���wbit����
void SK_RM::Update(int index, int value) {
	RM[index] = value;
}

//Write
void SK_RM::Write(int value) {
	int newValue = 0;
	bits = 0;
	while (value != newValue && !(value == 0 && newValue == 1)) {		//�i���ഫ�j��
		bits++;															//�����g�Jbit��
		//cout << value % 2 << " ";										//Used to check if the converter correct
		SK_RM::InsertL(headTable[headTable.size() - 1], value % 2);		//�۳̧��ݪ��Y�g�J(�qLSB�g�_)
		newValue = value;													
		value = value / 2;
	}
	//cout << endl;
	for (int i = 0; i < volumn - bits; i++) {							//�N�g�J����ƥ�����vector����
		LeftShift();//�Y�Ŷ���8bit�A�u�g�J{[ ][ ][0][1][0][1][0][1]}�A�h�N��ƥ����쩳�A�����ɹs(����)
	}
}

//BitWrite
void SK_RM::BitWrite(int bitvalue) {
	SK_RM::InsertL(headTable[NofHead - 1], bitvalue);		//�q���ݪ�Ū�g�Y�g�J��@bit
}

//BitRead
int SK_RM::BitReturn(int idx) {
	return RM[headTable[idx]];					//�q���wŪ�g�YŪ����@bit
}

//SK_RMs Consructor
SK_RMs::SK_RMs(int _volumn, int _headNumber, int _trackNumber) {
	volumn = _volumn;
	NofHead = _headNumber;
	trackNumber = _trackNumber;
	bufferSize = (volumn / NofHead) - 1;
	vector<SK_RM> vecs(trackNumber,SK_RM(volumn, NofHead));		//�ϥΪ̿�J�O����W��α���
	RMs = vecs;

	headTable.resize(NofHead);
	int headIndex = bufferSize;
	for (int i = 0; i < NofHead; i++) {					//�H�Ĥ@��Ū�g�Y���ǡA��X�ѤU��Ū�g�Y��m�æs�Jvector
		headTable[i] = headIndex;
		headIndex = headIndex + (volumn / NofHead);			//Ū�g�Y�|�b�C�ӪA�Ȱϧ���
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
	//�g�J����
	//track0 => trackN
	//  LSB  =>  MSB
	int j = 0;
	vector<int> headValue;														//�ΨӸ�Ū�g�Y�U���֭p��
	headValue.resize(NofHead, 0);
	for (int i = 0; i < bufferSize; i++) {										//�첾�j��
		for (j = 0; j < NofHead; j++) {										//�M��Ū�g�Y�j��
			headValue[j] = 0; 
			for (int k = 0; k < trackNumber; k++) {
				headValue[j] = headValue[j] + pow(2, k)*RMs[k].BitReturn(j);	//�G�i����Q�i��
			}
		}
		for (j = 0; j < NofHead; j++) {
			cout << headValue[j] << " ";										//�L�X��e�Ҧ�Ū�g�Y�U���Q�i��֭p��
		}
		cout << endl;
		RMsRightShift();														//����k��
	}

	for (j = 0; j < NofHead; j++) {											//Ū��Ū�g�d�򤺪��̫�@bit��L�ݦA�첾
		headValue[j] = 0;
		for (int k = 0; k < trackNumber; k++) {
			headValue[j] = headValue[j] + pow(2, k)*RMs[k].BitReturn(j);
		}
	}
	for (j = 0; j < NofHead; j++) {
		cout << headValue[j] << " ";
	}
	cout << endl;

	for (int i = 0; i < bufferSize; i++) {										//�_��
		RMsLeftShift();
	}
	cout << endl;
}

int SK_RMs::HeadSelector(int idx) {
	int interval = volumn / NofHead;
	return (idx / interval);
}

bool SK_RMs::FindLocation(int headIdx, int bitIdx, int step) {	//��ܲ��ʤ�V�A�æ^�Ǵ_���V
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
	for (int i = 0; i < trackNumber; i++) {	//���M�C�@���O����g�J
		RMs[i].BitWrite(b[i]);
	}
}