#include<iostream>
using namespace std;
void generate(int a[4],int * b){
	b[0]=(a[0]+a[1]+a[3])%2;
	b[1]=(a[0]+a[2]+a[3])%2;
	b[3]=(a[1]+a[2]+a[3])%2;
	b[2]=a[0];
	b[4]=a[1];
	b[5]=a[2];
	b[6]=a[3];
}
int check(int* b){
	int r[3];
	r[0]=(b[0]+b[2]+b[4]+b[6])%2;
	r[1]=(b[1]+b[2]+b[5]+b[6])%2;
	r[2]=(b[3]+b[4]+b[5]+b[6])%2;
	return 4*r[2]+2*r[1]+r[0];
}
int main(){
	int a[4],*b,n,pos;
	b=new int[4];
	cout<<"Enter data to send in digits:\n";
	for(int i=0;i<4;i++){
		cin>>a[3-i];
	}
	generate(a,b);
	cout<<"Codeword:"<<b[0]<<b[1]<<b[2]<<b[3]<<b[4]<<b[5]<<b[6]<<endl;
	cout<<"DO you want to insert error,enter position to insert error if not enter 0:";
	cin>>pos;
	if(pos>0){
		b[pos-1]=b[pos-1]?0:1;
	}
	n=check(b);
	if(n==0){
		cout<<"DATA:"<<b[6]<<b[5]<<b[4]<<b[2]<<" No error was detected\n";
	}else{
		cout<<"Error at "<<n<<" Corrected DATA:";
		b[n-1]=b[n-1]?0:1;
		cout<<b[6]<<b[5]<<b[4]<<b[2];
	}

}