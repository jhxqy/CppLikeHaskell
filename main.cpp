//
//  main.cpp
//  LikeHaskell
//
//  Created by 贾皓翔 on 2020/1/5.
//  Copyright © 2020 贾皓翔. All rights reserved.
//

#include <iostream>
#include "List.hpp"
using namespace std;
template<typename T>
struct Show;


int main(){
    using namespace List;
    using l0=List<int>;
    using l1=List<int,1>;
    using l2=List<int,1,2>;
    using l3=List<int,1,2,3>;
    using l4=List<int,1,2,3,4>;
    cout<<Maximum<l3>::value<<endl;
    cout<<Minimum<l3>::value<<endl;
    cout<<Length<l3>::value<<endl;
    cout<<Length<l0>::value<<endl;
    PrintList<ListCnt<l2, l3>::type>::Show();
    PrintList<Tail<l3>::type>::Show();
    cout<<Null<l0>::value<<endl;
    cout<<Null<l1>::value<<endl;
    cout<<Sum<l4>::value<<endl;
    cout<<Product<l4>::value<<endl;
    cout<<Elem<l4, 4>::value<<endl;
    cout<<Elem<l4, 5>::value<<endl;
    PrintList<l3>::Show();
    cout<<Last<l4>::value<<endl;
}

