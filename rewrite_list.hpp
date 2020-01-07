//
//  rewrite_list.hpp
//  LikeHaskell
//
//  Created by 贾皓翔 on 2020/1/6.
//  Copyright © 2020 贾皓翔. All rights reserved.
//
//上几页代码发现自己很愚蠢的混淆了值和类型，只能被迫重写了

#ifndef rewrite_list_hpp
#define rewrite_list_hpp

#include <stdio.h>
#include <iostream>
#include <type_traits>
namespace typelist{

template<int N>
struct Int{
    static constexpr int value=N;
};
#define TypeOp(typeName,opName,op) \
template<typename T1,typename T2>\
struct typeName##opName{\
    using type=Int<T1::value op T2::value>;\
};


TypeOp(Int,Add,+)
TypeOp(Int,Sub,-)
TypeOp(Int,Mul,*)
TypeOp(Int,Div,/)

struct IntOp{
    template<typename T1,typename T2>
    using add=IntAdd<T1,T2>;
    template<typename T1,typename T2>
    using sub=IntSub<T1,T2>;
    template<typename T1,typename T2>
    using mul=IntMul<T1,T2>;
    template<typename T1,typename T2>
    using div=IntDiv<T1,T2>;
};
template<typename T>
struct TypeOp;

template<int N>
struct TypeOp<Int<N>>{
    using type=IntOp;
};


template<typename ...Args>
struct List{};
namespace imple{

template<typename List>
struct PrintListImple;

template<>
struct PrintListImple<List<>>{
    static void Show(){
        std::cout<<"]"<<std::endl;
    }
};
template<typename H>
struct PrintListImple<List<H>>{
    static void Show(){
        std::cout<<H::value;
        PrintListImple<List<>>::Show();
    }
};
template<typename H,typename ...Args>
struct PrintListImple<List<H,Args...>>{
    static void Show(){
        std::cout<<H::value<<",";
        PrintListImple<List<Args...>>::Show();
    }
};


}

template<typename List>
struct PrintList{
    static void Show(){
        std::cout<<"[";
        imple::PrintListImple<List>::Show();
    }
};

template<typename List>
struct Head;

template<>
struct Head<List<>>;

template<typename H,typename...Args>
struct Head<List<H,Args...>>{
    using type=H;
};

template<typename List>
struct Tail;

template<>
struct Tail<List<>>;

template<typename H,typename...Args>
struct Tail<List<H,Args...>>{
    using type=List<Args...>;
};

template<typename List,typename Value>
struct PushFront;

template<typename...Args,typename Value>
struct PushFront<List<Args...>, Value>{
    using type=List<Value,Args...>;
};
template<typename List,typename Value>
struct PushBack;

template<typename...Args,typename Value>
struct PushBack<List<Args...>, Value>{
    using type=List<Args...,Value>;
};

template<typename List1,typename List2>
struct ListCnt;

template <typename...Args1,typename...Args2>
struct ListCnt<List<Args1...>, List<Args2...>>{
    using type=List<Args1...,Args2...>;
};


template<typename List,template<typename>class Op>
struct Map;

template<template<typename>class Op>
struct Map<List<>,Op>{
    using type=List<>;
};
template<typename H,typename...Args,template<typename >class Op>
struct Map<List<H,Args...>,Op>{
    using type=typename PushFront<typename Map<List<Args...>,Op>::type, typename Op<H>::type>::type;
};
template<typename List,template<typename>class Op>
struct Filter;

template<template<typename>class Op>
struct Filter<List<>,Op>{
    using type=List<>;
};
template<typename H,typename...Args,template<typename >class Op>
struct Filter<List<H,Args...>,Op>{
    using trueType=typename PushFront<typename Filter<List<Args...>,Op>::type, H>::type;
    using falseType=typename Filter<List<Args...>, Op>::type;
    using type=typename std::conditional<Op<H>::value, trueType, falseType>::type;
};

//foldl 参数为二元函数、列表、初始值

template<typename List,typename Acc,template<typename,typename>class Op>
struct Foldl;

template<typename Acc,template<typename,typename>class Op>
struct Foldl<List<>,Acc,Op>{
    using type=Acc;
};

template<typename H,typename...Args,typename Acc,template<typename,typename>class Op>
struct Foldl<List<H,Args...>,Acc,Op>{
    using type=typename Foldl<List<Args...>, typename Op<Acc, H>::type, Op>::type;
};
template<typename List,typename Acc,template<typename,typename>class Op>
struct Foldr;

template<typename Acc,template<typename,typename>class Op>
struct Foldr<List<>,Acc,Op>{
    using type=Acc;
};

template<typename H,typename...Args,typename Acc,template<typename,typename>class Op>
struct Foldr<List<H,Args...>,Acc,Op>{
    using type=typename Op<Acc, typename Foldr<List<Args...>,H,Op>::type>::type;
};

namespace imple{
template<typename Acc,typename X>
struct SumAdd{
    using Op=typename TypeOp<X>::type;
    using type=typename Op::template add<Acc,X>::type;
};

template<typename Acc,typename X>
struct ReverseParm{
    using type=typename PushFront<Acc, X>::type;
};
 
}

template<typename List>
struct Sum{
    using type=typename Foldr<List, Int<0>, imple::SumAdd>::type;
};
template<typename L>
struct Reverse{
    using type=typename Foldl<L, List<>,imple::ReverseParm>::type;
};


}


#endif /* rewrite_list_hpp */
