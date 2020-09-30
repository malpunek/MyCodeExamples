#include <cstdio>
#include <vector>

#include "libalgo/set.hh"

int main(){
	int n;
	scanf(" %d", &n);
	MySet<int> S;
	for( int i=0; i<n; i++){
		int typ, x;
		scanf(" %d %d", &typ, &x);
		if (typ == 5){
			if (S.find(x))
				printf("JEST\n");
			else printf("NIEMA\n");
			}
		else if (typ>0)
			S.insert(x);
		else if (typ<0)
			S.erase(x);
		else{
			int val;
			scanf(" %d", &val);
			S.shift(x, val);
			}
		std::vector<int> VVV = S.sortedValues();
		for(int i=0; i< VVV.size(); i++)
			printf("%d ", VVV[i]);
		printf("\n");
		}
	return 0;
	}