#include<bits\stdc++.h>
using namespace std;

int p10[10]		={3,5,2,7,4,10,1,9,8,6};
int p8[8]  		={6,3,7,4,8,5,10,9};
int ip[8]  		={2,6,3,1,4,8,5,7};
int ip_inv[8]	={4,1,3,5,7,2,8,6};
int ep[8]		={4,1,2,3,2,3,4,1};
int p4[4]		={2,4,3,1};
int s0[4][4]	={1,0,3,2,
				  3,2,1,0,
				  0,2,1,3,
				  3,1,3,2};
int s1[4][4]	={0,1,2,3,
				  2,0,1,3,
				  3,0,1,0,
				  2,1,0,3};
				 	
void apply_p8(int key[])
{
	int x[8];
	for (int i = 0; i < 8; ++i)
	{
		x[i]=key[p8[i]-1];
	}
	std::copy(x,x+8,key);
}

int shift(int a[],int n)
{
	int tmp=a[0];
	for(int i=0;i<n-1;i++)
	{
		a[i]=a[i+1];
	}
	a[n-1]=tmp;
}
void apply_p10(int key[])
{
	int x[10];
	for (int i = 0; i < 10; ++i)
	{
		x[i]=key[p10[i]-1];
	}
	std::copy(x,x+10,key);
}

void apply_ip(int key[])
{
	int x[8];
	for (int i = 0; i < 8; ++i)
	{
		x[i]=key[ip[i]-1];
	}
	std::copy(x,x+8,key);
}

void apply_ipinv(int key[])
{
	int x[8];
	for (int i = 0; i < 8; ++i)
	{
		x[i]=key[ip_inv[i]-1];
	}
	std::copy(x,x+8,key);
}

void apply_ep(int key[],int x[])
{
	for (int i = 0; i < 8; ++i)
	{
		x[i]=key[ep[i]-1];
	}
}

void apply_p4(int key[])
{
	int x[4];
	for (int i = 0; i < 4; ++i)
	{
		x[i]=key[p4[i]-1];
	}
	std::copy(x,x+4,key);
}


void split(int x[],int a[],int b[],int n)
{
	for (int i = 0; i < n; ++i)
	{
		a[i]=x[i];
		b[i]=x[n+i];
	}
}
void merger(int k[],int k1[],int k2[],int n)
{
	for(int i=0;i<n;i++)
	{
		k[i]=k1[i];
		k[n+i]=k2[i];
	}
}
void key_maker(int key[],int key1[],int key2[])
{
	apply_p10(key);
	int k1[5],k2[5];
	split(key,k1,k2,5);
	shift(k1,5);
	shift(k2,5);
	merger(key,k1,k2,5);										
	std::copy(key,key+10,key1);			///////////////////     KEY 1
	apply_p8(key1);
	split(key,k1,k2,5);
	shift(k1,5);
	shift(k2,5);
	shift(k1,5);
	shift(k2,5);
	merger(key,k1,k2,5);
	std::copy(key,key+10,key2);			///////////////////      KEY 2
	apply_p8(key2);
}
void exor(int a[],int b[],int n)
{
	for(int i=0;i<n;i++)
	{
		if(a[i]!=b[i])
		{
			a[i]=1;
		}
		else
		{
			a[i]=0;
		}
	}
}
int bin(int x[])
{
	return 2*x[0]+x[1];
}

void sboxextractor(int a[],int b[],int c[],int d[])
{
	int n=4;
	int x[2];
	x[0]=a[0];
	x[1]=a[3];
	int y[2];
	y[0]=a[1];
	y[1]=a[2];
	int p=s0[bin(x)][bin(y)];
	if(p==0)
	{
		c[0]=0;
		c[1]=0;
	}
	else if(p==1)
	{
		c[0]=0;
		c[1]=1;
	}
	else if(p==2)
	{
		c[0]=1;
		c[1]=0;
	}
	else if(p==3)
	{
		c[0]=1;
		c[1]=1;
	}
	x[0]=b[0];
	x[1]=b[3];
	y[0]=b[1];
	y[1]=b[2];
	p=s1[bin(x)][bin(y)];
	if(p==0)
	{
		d[0]=0;
		d[1]=0;
	}
	else if(p==1)
	{
		d[0]=0;
		d[1]=1;
	}
	else if(p==2)
	{
		d[0]=1;
		d[1]=0;
	}
	else if(p==3)
	{
		d[0]=1;
		d[1]=1;
	}
}
void encrypt(int plain_text[],int key1[],int key2[],int cipher_text[])
{
	int p1[4],p2[4];
	apply_ip(plain_text);
	
	split(plain_text,p1,p2,4);
	int tmp[4];
	copy(p2,p2+4,tmp);
	int tmp_p2[8];
	apply_ep(p2,tmp_p2);
	exor(tmp_p2,key1,8);
	int xor_1[4],xor_2[4];
	split(tmp_p2,xor_1,xor_2,4);
	
	int c[2],d[2];
	sboxextractor(xor_1,xor_2,c,d);
	merger(xor_1,c,d,2);
	apply_p4(xor_1);
	exor(xor_1,p1,4);
	copy(xor_1,xor_1+4,p2);						//////  Swapping
	copy(tmp,tmp+4,p1);							////// Swapping
	
	///////////////////   	AFTER SWAPPING //////////////////////

	copy(p2,p2+4,tmp);
	apply_ep(p2,tmp_p2);
	exor(tmp_p2,key2,8);
	split(tmp_p2,xor_1,xor_2,4);
	sboxextractor(xor_1,xor_2,c,d);
	merger(xor_1,c,d,2);
	apply_p4(xor_1);
	exor(xor_1,p1,4);
	merger(tmp_p2,xor_1,tmp,4);
	apply_ipinv(tmp_p2);
	copy(tmp_p2,tmp_p2+8,cipher_text);
}

void decrypt(int plain_text[],int key2[],int key1[],int cipher_text[])
{
	
	int p1[4],p2[4];
	apply_ip(plain_text);
	
	split(plain_text,p1,p2,4);
	int tmp[4];
	copy(p2,p2+4,tmp);
	int tmp_p2[8];
	apply_ep(p2,tmp_p2);
	exor(tmp_p2,key1,8);
	int xor_1[4],xor_2[4];
	split(tmp_p2,xor_1,xor_2,4);
	
	int c[2],d[2];
	sboxextractor(xor_1,xor_2,c,d);
	merger(xor_1,c,d,2);
	apply_p4(xor_1);
	exor(xor_1,p1,4);
	copy(xor_1,xor_1+4,p2);						//////  Swapping
	copy(tmp,tmp+4,p1);							////// Swapping
	
	///////////////////   	AFTER SWAPPING //////////////////////

	copy(p2,p2+4,tmp);
	apply_ep(p2,tmp_p2);
	exor(tmp_p2,key2,8);
	split(tmp_p2,xor_1,xor_2,4);
	sboxextractor(xor_1,xor_2,c,d);
	merger(xor_1,c,d,2);
	apply_p4(xor_1);
	exor(xor_1,p1,4);
	merger(tmp_p2,xor_1,tmp,4);
	apply_ipinv(tmp_p2);
	copy(tmp_p2,tmp_p2+8,cipher_text);	
}
main()
{
	int key[10],plain_text[8],key1[10],key2[10];
	int cipher_text[8],decrypted_text[8];
	///////////////////////			input   	/////////////


	cout<<"Enter 10 bit key : ";
	for (int i = 0; i < 10; ++i)
	{
		cin>>key[i];
	}	
	cout<<"Enter 8 bit plain text : ";
	for (int i = 0; i < 8; ++i)
	{
		cin>>plain_text[i];
	}

		/////////////////////	Key Making	/////////////////
		
		

	key_maker(key,key1,key2);
	int new_key1[8],new_key2[8];
	copy(key1,key1+8,new_key1);				//////////// 	new_key1 is key 1
	copy(key2,key2+8,new_key2);				///////////		new_key2 is key 2


	///////////////////////////////		Encryption 		//////////////////




	encrypt(plain_text,new_key1,new_key2,cipher_text);
	cout<<endl<<"Cipher_text : ";
	for (int i = 0; i < 8; ++i)
	{
		cout<<cipher_text[i];
	}
	
	
	
	/////////////////////// 		Decryption		//////////////////////
	decrypt(cipher_text,new_key1,new_key2,decrypted_text);
	cout<<endl<<"Decrypted_text : ";
	for (int i = 0; i < 8; ++i)
	{
		cout<<decrypted_text[i];
	}
}
