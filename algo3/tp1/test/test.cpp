#include <iostream>
#include <stdlib.h>
using namespace std;

#define forn(i,n) for(unsigned long long int i=0;i<n;i++)

int main(){
	unsigned long long int n, m;
	cin >> n;
	cin >> m;
	for(unsigned long long int i=0;i<n;i+=m){
		cout << i << " ";
		forn(j,i){
			cout << rand()%10000 << " ";
		}
		cout << endl;
	}
	cout << -1;
	return 0;
}
