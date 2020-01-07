//
//  main.cpp
//  LikeHaskell
//
//  Created by 贾皓翔 on 2020/1/5.
//  Copyright © 2020 贾皓翔. All rights reserved.
//

#include <iostream>
#include "List.hpp"
#include "rewrite_list.hpp"
using namespace std;
template<typename T>
struct Show;

template<int N>
struct AddOne{
    static constexpr int value=N+1;
};
template<int T,int N>
struct MoreThan{
    static constexpr bool value=T>N;
};
template<int T>
using MoreThanFive=MoreThan<T, 20 >;

template<int init,int a>
struct ADD{
    static constexpr int value=init+a;
};
void Old(){
    using namespace List;
    using l0=List<int>;
    using l1=List<int,1>;
    using l2=List<int,1,2>;
    using l3=List<int,1,2,3>;
    using l4=List<int,1,2,3,4>;
    using l5=List<int, 13,4938,4,3,455,12,9,706>;
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
    PrintList<Take<l3, 0>::type>::Show();
    PrintList<Replicate<int, 0, 5>::type>::Show();
    PrintList<Reverse<l4>::type>::Show();
    PrintList<QuickSort<l5>::type>::Show();
    PrintList<Map<l4,AddOne>::type>::Show();
    PrintList<Filter<l5,MoreThanFive>::type>::Show();
    cout<<Foldl<l4, int, ADD, 0>::value<<endl;
}
void New(){
    
    using namespace typelist;
    
    using l1=TypeList<Int<1>,Int<2>,Int<3>,Int<4>>;
    using l2=Tail<l1>::type;
    using l3=TypeList<Int<43>,Int<25>,Int<33>,Int<11>,Int<98>,Int<54>>;
    PrintList<l1>::Show();
    PrintList<l2>::Show();
    cout<<Sum<l1>::type::value<<endl;
    PrintList<Reverse<l1>::type>::Show();
    PrintList<QuickSort<l3>::type>::Show();
}
    
int main(){
    //Old();
    New();

}
