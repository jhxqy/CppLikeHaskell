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
    template<typename T1,typename T2>
    using EQ=typename std::conditional<T1::value==T2::value, std::true_type, std::false_type>::type;
    template<typename T1,typename T2>
    using LT=typename std::conditional<T1::value<T2::value, std::true_type, std::false_type>::type;
    template<typename T1,typename T2>
    using LE=typename std::conditional<T1::value<=T2::value, std::true_type, std::false_type>::type;
    template<typename T1,typename T2>
    using GT=typename std::conditional<(T1::value>T2::value), std::true_type, std::false_type>::type;
    template<typename T1,typename T2>
    using GE=typename std::conditional<T1::value>=T2::value, std::true_type, std::false_type>::type;
};
template<typename T>
struct TypeOp;

template<int N>
struct TypeOp<Int<N>>{
    using type=IntOp;
};


template<typename ...Args>
struct TypeList{};
namespace imple{

template<typename List>
struct PrintListImple;

template<>
struct PrintListImple<TypeList<>>{
    static void Show(){
        std::cout<<"]"<<std::endl;
    }
};
template<typename H>
struct PrintListImple<TypeList<H>>{
    static void Show(){
        std::cout<<H::value;
        PrintListImple<TypeList<>>::Show();
    }
};
template<typename H,typename ...Args>
struct PrintListImple<TypeList<H,Args...>>{
    static void Show(){
        std::cout<<H::value<<",";
        PrintListImple<TypeList<Args...>>::Show();
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
struct Head<TypeList<>>;

template<typename H,typename...Args>
struct Head<TypeList<H,Args...>>{
    using type=H;
};

template<typename List>
struct Tail;

template<>
struct Tail<TypeList<>>;

template<typename H,typename...Args>
struct Tail<TypeList<H,Args...>>{
    using type=TypeList<Args...>;
};

template<typename List,typename Value>
struct PushFront;

template<typename...Args,typename Value>
struct PushFront<TypeList<Args...>, Value>{
    using type=TypeList<Value,Args...>;
};
template<typename List,typename Value>
struct PushBack;

template<typename...Args,typename Value>
struct PushBack<TypeList<Args...>, Value>{
    using type=TypeList<Args...,Value>;
};

template<typename List1,typename List2>
struct ListCnt;

template <typename...Args1,typename...Args2>
struct ListCnt<TypeList<Args1...>, TypeList<Args2...>>{
    using type=TypeList<Args1...,Args2...>;
};


template<typename List,template<typename>class Op>
struct Map;

template<template<typename>class Op>
struct Map<TypeList<>,Op>{
    using type=TypeList<>;
};
template<typename H,typename...Args,template<typename >class Op>
struct Map<TypeList<H,Args...>,Op>{
    using type=typename PushFront<typename Map<TypeList<Args...>,Op>::type, typename Op<H>::type>::type;
};
template<typename List,template<typename>class Op>
struct Filter;

template<template<typename>class Op>
struct Filter<TypeList<>,Op>{
    using type=TypeList<>;
};
template<typename H,typename...Args,template<typename >class Op>
struct Filter<TypeList<H,Args...>,Op>{
    using trueType=typename PushFront<typename Filter<TypeList<Args...>,Op>::type, H>::type;
    using falseType=typename Filter<TypeList<Args...>, Op>::type;
    using type=typename std::conditional<Op<H>::value, trueType, falseType>::type;
};

//foldl 参数为二元函数、列表、初始值

template<typename List,typename Acc,template<typename,typename>class Op>
struct Foldl;

template<typename Acc,template<typename,typename>class Op>
struct Foldl<TypeList<>,Acc,Op>{
    using type=Acc;
};

template<typename H,typename...Args,typename Acc,template<typename,typename>class Op>
struct Foldl<TypeList<H,Args...>,Acc,Op>{
    using type=typename Foldl<TypeList<Args...>, typename Op<Acc, H>::type, Op>::type;
};
template<typename List,typename Acc,template<typename,typename>class Op>
struct Foldr;

template<typename Acc,template<typename,typename>class Op>
struct Foldr<TypeList<>,Acc,Op>{
    using type=Acc;
};

template<typename H,typename...Args,typename Acc,template<typename,typename>class Op>
struct Foldr<TypeList<H,Args...>,Acc,Op>{
    using type=typename Op<Acc, typename Foldr<TypeList<Args...>,H,Op>::type>::type;
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
    using type=typename Foldl<L, TypeList<>,imple::ReverseParm>::type;
};

namespace imple{
template<typename T1,typename T2>
struct QuickImpleLE{
    using Op=typename TypeOp<T1>::type;
    using type=typename Op::template LE<T1,T2>::type;
    static constexpr bool  value=type::value;
};
template<typename T1,typename T2>
struct QuickImpleGT{
    using Op=typename TypeOp<T1>::type;
    using type=typename Op::template GT<T1,T2>::type;
    static constexpr bool  value=type::value;

};
}

template<typename List>
struct QuickSort;

template<>
struct QuickSort<TypeList<>>{
    using type=TypeList<>;
};

template<typename H,typename...Args>
struct QuickSort<TypeList<H,Args...>>{
    template<typename T>
    using LEH=imple::QuickImpleLE<T, H>;
    template<typename T>
    using GTH=imple::QuickImpleGT<T, H>;
    using smallSorted=typename  Filter<typename QuickSort<TypeList<Args...>>::type,LEH>::type;
    using bigSorted=typename  Filter<typename QuickSort<TypeList<Args...>>::type,GTH>::type;
    using type=typename ListCnt<typename ListCnt<smallSorted, TypeList<H>>::type, bigSorted>::type;
};


}


#endif /* rewrite_list_hpp */
