#include<iostream>
using namespace std;
int main(){
	int outputRate,bufferSize,bufferFilled=0;
	cout<<"Enter size of buffer";
	cin>>bufferSize;
	cout<<"Enter output rate:";
	cin>>outputRate;
	while(true){
		cout<<"Is there data to send 1<yes> 0<no>";
		int ch,data;
		cin>>ch;
		if(ch){
			cout<<"Enter amout of data to send:";
			cin>>data;
			if(data+bufferFilled>bufferSize){
				cout<<"Buffer filled, Paccket dropped\n";
			}else{
				bufferFilled+=data;
				cout<<"added to buffer\n";
			}
		}
		if(bufferFilled>0){
			if(bufferFilled<outputRate){
				cout<<"Sent "<<bufferFilled<<" no. of packets\n";
				bufferFilled=0;
			}else{
				cout<<"Send "<<outputRate<<" no. of packets\n";
				bufferFilled-=outputRate;
			}
		}else{
			cout<<"no data to send\n";
		}
		

	}
}