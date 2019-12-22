#include<iostream>
#include<string>
using namespace std;

string compute_xor(string a,string b){
    string remainder="";
    for(int i=1;b[i]!='\0';i++){
        if(a[i]==b[i]) remainder+='0';
        else remainder+='1';
    }
    return remainder;
}

int main(){
    string divisor="10001000000100001";
    cout<<"Enter DATA:";
    string input;
    cin>>input;
    int len_input=input.length();
    string modified_input=input;
    for(int i=1;i<divisor.length();i++) modified_input+="0";
    int len=modified_input.length();
    cout<<"DIVIDEND TO CALCULATE REMAINDER:"<<modified_input<<endl;
    string temp=modified_input.substr(0,17);
    int i=0;

    cout<<divisor<<"|"<<modified_input<<endl;
    for(int i=0;i<len_input;i++){
        for(int j=0;j<18;j++) cout<<" ";
        //cout<<"CHECKING "<<temp[0]<<endl;
        if(temp[0]=='1'){
            temp=compute_xor(temp,divisor);
            for(int j=0;j<i;j++) cout<<" ";
            cout<<divisor<<endl;            
        }else{
            temp=compute_xor(temp,"00000000000000000");
            for(int j=0;j<i;j++) cout<<" ";
            cout<<"00000000000000000"<<endl;  
        }
        for(int j=0;j<18;j++) cout<<" ";
        for(int j=0;j<i;j++) cout<<" ";
        for(int j=0;j<18;j++) cout<<"-";cout<<endl;

        temp+=modified_input[divisor.length()+i];
	
        for(int j=0;j<18;j++) cout<<" ";
        for(int j=0;j<i+1;j++) cout<<" ";
        cout<<temp<<endl;
    }
    string codeword=input+temp;
    cout<<"CODEWORD:"<<codeword<<endl;
    cout<<"Do you wnat to create error 1<yes> 0<no>:";
    int choice,pos;
    cin>>choice;
    if(choice){
        cout<<"Enter position to create error at:";
        cin>>pos;
        codeword[pos-1]=(codeword[pos-1]=='1')?'0':'1';
        cout<<"Error data:"<<codeword<<endl;
    }else{
        cout<<"No Error data:"<<codeword<<endl;
    }
    temp=codeword.substr(0,17);
    cout<<divisor<<"|"<<codeword<<endl;
    for(int i=0;i<codeword.length()-divisor.length();i++){
         for(int j=0;j<18;j++) cout<<" ";
        //cout<<"CHECKING "<<temp[0]<<endl;
        if(temp[0]=='1'){
            temp=compute_xor(temp,divisor);
            for(int j=0;j<i;j++) cout<<" ";
            cout<<divisor<<endl;            
        }else{
            temp=compute_xor(temp,"00000000000000000");
            for(int j=0;j<i;j++) cout<<" ";
            cout<<"00000000000000000"<<endl;  
        }
        for(int j=0;j<18;j++) cout<<" ";
        for(int j=0;j<i;j++) cout<<" ";
        for(int j=0;j<18;j++) cout<<"-";cout<<endl;
        temp+=codeword[divisor.length()+i];
        for(int j=0;j<18;j++) cout<<" ";
        for(int j=0;j<i+1;j++) cout<<" ";
        cout<<temp<<endl;
    }
    int flag=0;
    for(int i=0;temp[i]!='\0';i++){
        if(temp[i]!='0'){
            flag=1;
            break;
        }
    }
    if(flag){
        cout<<"Error detected"<<endl;
    }else{
        cout<<"No error Detected"<<endl;
    }
}