#include<stdlib.h>
#include "myDES.h"

int bin2dec(int A[], int n){
	int pos = n-1;
	int m = 0;
	int num = 0;
	while(pos>=0){
		num+=(A[pos]<<m);
		pos--;
		m++;
	}
	return num;
}
void dec2bin(int n,int *size,int A[]){
	int i = 0;

	while(n!=0){
		A[i++] = n%2;
		n = n>>1;
	}

	int begin = 0, end = i-1;
	*size = end+1;

	int temp;
	while(begin<end){
		temp = A[begin];
		A[begin++] = A[end];
		A[end--] = temp;
	}

}
void f(int A[],int J[],int Res[]){

	int E[] = {32,1,2,3,4,5,4,5,6,7,8,9,8,9,10,11,12,13,12,13,14,15,16,17,16,17,18,19,20,21,20,21,22,23,24,25,24,25,26,27,28,29,28,29,30,31,32,1};
	int S[8][4][16] = {{{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},{0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},{4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},{15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}},{{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},{3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},{0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},{13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}},{{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},{13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},{13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}},{{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},{13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},{10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}},{{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},{14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},{4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}},{{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},{10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},{9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}},{{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},{13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},{1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}},{{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},{1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},{7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}}}; 
	int EA[48];

	//expanding A from 32 bits to 48 bits using E --> EA
	for(int i=0;i<48;i++){
		EA[i] = A[E[i]-1];
	}

	int ind;
	int EAxorJ[8][6];
	for(int i=0;i<8;i++){
		for(int j=0;j<6;j++){
			ind = (6*i+j);
			EAxorJ[i][j] = EA[ind]^J[ind];
		}
	}


	//getting array C of 32 bits from ExorJ of 48 bits using S
	int C[8][4];

	for(int i=0;i<8;i++){
		//use S[i] to get C[i] from EAxorJ[i]
		int temp[2];
		temp[0] = EAxorJ[i][0];
		temp[1] = EAxorJ[i][5];
		int row = bin2dec(temp,2);
		int temp2[4];
		for(int j=1;j<=4;j++){
			temp2[j-1] = EAxorJ[i][j];
		}

		int col = bin2dec(temp2,4);
		int val = S[i][row][col];

		int size;
		int tmp[10];
		dec2bin(val,&size,tmp);

		int k = 0;
		if(size<4){
			int t = 4 - size;
			for(;k<t;k++){
				C[i][k] = 0;
			}
		}

		for(int l = 0;l<size;k++,l++){
			C[i][k] = tmp[l]; 
		}

	}

	//permuting the 32 bit array C to obtain res

	int P[] = {16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10,2,8,24,14,32,27,3,9,19,13,30,6,22,11,4,25};
	int index;
	for(int i=0;i<32;i++){
		index = P[i]-1;
		Res[i] = C[index/4][index%4];
	}
}
void IP(int P[]){
	int IP[] = {58,50,42,34,26,18,10,2,60,52,44,36,28,20,12,4,62,54,46,38,30,22,14,6,64,56,48,40,32,24,16,8,57,49,41,33,25,17,9,1,59,51,43,35,27,19,11,3,61,53,45,37,29,21,13,5,63,55,47,39,31,23,15,7};
	
	int temp[64];
	for(int i=0;i<64;i++){
		temp[i] = P[IP[i]-1];
	} 
	for(int i=0;i<64;i++){
		P[i] = temp[i];
	}
}

void IPinv(int C[]){
	int Ipinv[] = {40,8,48,16,56,24,64,32,39,7,47,15,55,23,63,31,38,6,46,14,54,22,62,30,37,5,45,13,53,21,61,29,36,4,44,12,52,20,60,28,35,3,43,11,51,19,59,27,34,2,42,10,50,18,58,26,33,1,41,9,49,17,57,25};

	int temp[64];
	for(int i=0;i<64;i++){
		temp[i] = C[Ipinv[i]-1];
	} 
	for(int i=0;i<64;i++){
		C[i] = temp[i];
	}
}

void PC1(int K[],int LKey[],int RKey[]){
	int left[] = {57,49,41,33,25,17,9,1,58,50,42,34,26,18,10,2,59,51,43,35,27,19,11,3,60,52,44,36};
	int right[] = {63,55,47,39,31,23,15,7,62,54,46,38,30,22,14,6,61,53,45,37,29,21,13,5,28,20,12,4};
	for(int i=0;i<28;i++){
		LKey[i] = K[left[i]-1];
	}
	for(int i=0;i<28;i++){
		RKey[i] = K[right[i]-1];
	}
}

void PC2(int K[], int roundKey[]){
	int pc2[] = {14,17,11,24,1,5,3,28,15,6,21,10,23,19,12,4,26,8,16,7,27,20,13,2,41,52,31,37,47,55,30,40,51,45,33,48,44,49,39,56,34,53,46,42,50,36,29,32};
	
	for(int i=0;i<48;i++){
		roundKey[i] = K[pc2[i]-1];
	}
}

void leftRotate(int A[],int Siz, int r){
	int* temp;
	temp = (int*)malloc(r*sizeof(int));
	int i;
	for(i=0;i<r;i++){
		temp[i] = A[i];
	}
	for(;i<Siz;i++){
		A[i-r]=A[i];
	}
	i=Siz-r;
	for(;i<Siz;i++){
		A[i] = temp[i-Siz+r];
	}
}

void encrypt(int P[],int K[],int C[]){
	//P and K are 64 bit long

	IP(P);
	int LKey[28];
	int RKey[28];
	PC1(K,LKey,RKey);
	int Rounds[] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};
	int temp[56],roundKey[48],leftPart[32],rightPart[32];
	
	for(int i=0;i<32;i++){
		leftPart[i] = P[i];
		rightPart[i] = P[i+32];
	}

	int fRes[32],temporary;
	for(int i=0;i<16;i++){
		leftRotate(LKey,28,Rounds[i]);
		leftRotate(RKey,28,Rounds[i]);

		for(int j=0;j<28;j++){
			temp[j] = LKey[j];
			temp[j+28] = RKey[j];
		}
		PC2(temp,roundKey);

		f(rightPart,roundKey,fRes);

		for(int j=0;j<32;j++){
			temporary = leftPart[j];
			leftPart[j] = rightPart[j];
			rightPart[j] = temporary ^ (fRes[j]);
		}			
	}
	
	for(int j=0;j<32;j++){
		C[j] = rightPart[j];
		C[j+32] = leftPart[j];
	}	
	IPinv(C);
}

void decrypt(int C[],int K[],int P[]){
	//C and K are 64 bit long

	int LKey[28];
	int RKey[28];
	PC1(K,LKey,RKey);
	int Rounds[] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};
	int roundKey[16][48],temp[56];
	
	for(int i=0;i<16;i++){

		leftRotate(LKey,28,Rounds[i]);
		leftRotate(RKey,28,Rounds[i]);

		for(int j=0;j<28;j++){
			temp[j] = LKey[j];
			temp[j+28] = RKey[j];
		}
		PC2(temp,roundKey[i]);
	}
	
	IP(C);
	int leftPart[32], rightPart[32], temporary, fRes[32];

	for(int j=0;j<32;j++){
		rightPart[j] = C[j];
		leftPart[j] = C[j+32];
	}	

	for (int i = 15; i >=0; --i)
	{	

		f(leftPart,roundKey[i],fRes);

		for(int j=0;j<32;j++){
			temporary = rightPart[j];
			rightPart[j] = leftPart[j];
			leftPart[j] = temporary ^ fRes[j]; 
		}	
	}
	for(int i=0;i<32;i++){
		P[i] = leftPart[i];
		P[i+32] = rightPart[i];
	}
	IPinv(P);

}

void ascii2bin(char c,int res[]){
	int i = 0, temp[8];
	int n = (int)(c);

	while(n){
		temp[i] = n%2;
		n = n>>1;
		if(n)i++;
	}

	while(i<8){
		temp[++i] = 0;
	}

	int k = 7;
	i = 0;
	while(k>=0){
		res[k--] = temp[i++];
	}

	
}
void ascii2binINT(int c,int res[]){
	int i = 0, temp[8];
	int n = c;

	while(n){
		temp[i] = n%2;
		n = n>>1;
		if(n)i++;
	}

	while(i<8){
		temp[++i] = 0;
	}

	int k = 7;
	i = 0;
	while(k>=0){
		res[k--] = temp[i++];
	}

	
}
