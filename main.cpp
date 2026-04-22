// Driver for local testing (not used by OJ if header submission). Kept minimal.
#include <bits/stdc++.h>
#include "skip_list.hpp"
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Simple interactive tester: commands: ins x, del x, qry x, exit
    SkipList<long long> sl;
    string op; long long x;
    while (cin >> op){
        if(op=="ins"){cin>>x; sl.insert(x);} 
        else if(op=="del"){cin>>x; sl.deleteItem(x);} 
        else if(op=="qry"){cin>>x; cout << (sl.search(x)?"Yes":"No") << '\n';}
        else if(op=="exit"){break;}
    }
    return 0;
}
