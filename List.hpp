//
//  List.hpp
//  LikeHaskell
//
//  Created by 贾皓翔 on 2020/1/5.
//  Copyright © 2020 贾皓翔. All rights reserved.
//

#ifndef List_hpp
#define List_hpp

#include <stdio.h>
#include <iostream>
#include <type_traits>
namespace List{


template<typename Type,Type...element>
struct List{
    using type=Type;
};

template<typename List>
struct Head;

template<typename T>
struct Head<List<T>>;

template<typename T,T H,T ...Args>
struct Head<List<T,H,Args...>>{
    static constexpr T value=H;
};

namespace Imple{
template<typename T,T a,T b>
struct Greater{
    static constexpr T value=a>b?a:b;
};
template<typename T,T a,T b>
struct Litter{
    static constexpr T value=a<b?a:b;
};


template<template<typename T,T,T> class Trait,typename List>
struct FindOneBetter;

template<template<typename T,T,T> class Trait,typename T>
struct FindOneBetter<Trait,List<T>>;

template<template<typename T,T,T> class Trait,typename T,T H>
struct FindOneBetter<Trait,List<T,H>>{
    static constexpr T value =H;
};

template <template<typename T,T,T> class Trait,typename T,T H,T...Args>
struct FindOneBetter<Trait,List<T,H,Args...>>{
    static constexpr T nextMax=FindOneBetter<Trait, List<T, Args...>>::value;
    static constexpr T value=Trait<T,nextMax,H>::value;
};

}

template<typename List>
using Maximum=Imple::FindOneBetter<Imple::Greater,List>;
template<typename List>
using Minimum=Imple::FindOneBetter<Imple::Litter, List>;

template<typename List>
struct Tail;
template<typename T>
struct Tail<List<T>>;
template<typename T, T H>
struct Tail<List<T,H>>{
    using type=List<T>;
};
template<typename T, T H, T...Args>
struct Tail<List<T,H,Args...>>{
    using type=List<T,Args...>;
};

template<typename List>
struct Null:public std::false_type{
};

template<typename T>
struct Null<List<T>>:public std::true_type{
};

template<typename List>
struct Length;

template<typename T>
struct Length<List<T>>{
    static constexpr size_t value=0;
};

template<typename T,T H>
struct Length<List<T,H>>{
    static constexpr size_t value=1;
};

template<typename T,T H,T...Args>
struct Length<List<T,H,Args...>>{
    static constexpr size_t value=1+Length<List<T,Args...>>::value;
};

namespace Imple{
template <typename T,T a,T b>
struct AddValue{
    static constexpr T value=a+b;
};
template <typename T,T a,T b>
struct MulValue{
    static constexpr T value=a*b;
};
template<template<typename T,T,T>class Op,typename List>
struct OpListValue;

template<template<typename T,T,T>class Op,typename T>
struct OpListValue<Op, List<T>>{
    static constexpr T value=0;
};

template<template<typename T,T,T>class Op,typename T,T H>
struct OpListValue<Op, List<T,H>>{
    static constexpr T value=H;
};

template<template<typename T,T,T>class Op,typename T,T H,T...Args>
struct OpListValue<Op, List<T,H,Args...>>{
    static constexpr T value=Op<T,OpListValue<Op, List<T,Args...>>::value,H>::value;
};


}

template<typename List>
using Sum=Imple::OpListValue<Imple::AddValue, List>;
template<typename List>
using Product=Imple::OpListValue<Imple::MulValue, List>;


template<typename List,typename List::type Value>
struct PushFront;

template<typename T,T...Args,T Value>
struct PushFront<List<T,Args...>,Value>{
    using type=List<T,Value,Args...>;
};

template<typename List,typename List::type Value>
struct PushBack;

template<typename T,T...Args,T Value>
struct PushBack<List<T,Args...>,Value>{
    using type=List<T,Args...,Value>;
};

template<typename List1,typename List2>
struct ListCnt;
template<typename T,T...Args1,T...Args2>
struct ListCnt<List<T,Args1...>, List<T,Args2...>>{
    using type=List<T,Args1...,Args2...>;
};

template<typename List,typename List::type Value>
struct Elem;

template<typename T,T Value>
struct Elem<List<T>,Value>{
    static constexpr bool value=false;
};

template<typename T,T H,T ...Args,T Value>
struct Elem<List<T,H,Args...>,Value>{
    static constexpr bool value=(H==Value)||Elem<List<T,Args...>,Value>::value;
};

namespace Imple{
template<typename List>
struct PrintListImple;

template<typename T>
struct PrintListImple<List<T>>{
    static void Show(){
    //    std::cout<<"]"<<std::endl;
    }
};
template<typename T,T H>
struct PrintListImple<List<T,H>>{
    static void Show(){
        std::cout<<H;
        PrintListImple<List<T>>::Show();
    }
};

template<typename T,T H,T...Args>
struct PrintListImple<List<T,H,Args...>>{
    static void Show(){
        std::cout<<H<<",";
        PrintListImple<List<T, Args...>>::Show();
    }
};

}

template<typename List>
struct PrintList{
    static void Show(){
        std::cout<<"[";
        Imple::PrintListImple<List>::Show();
        std::cout<<"]"<<std::endl;
    }
};


template<typename List>
struct Last;
template<typename T>
struct Last<List<T>>;

template<typename T,T H>
struct Last<List<T,H>>{
    static constexpr T value=H;
};
template<typename T,T H,T...Args>
struct Last<List<T,H,Args...>>{
    static constexpr T value=Last<List<T,Args...>>::value;
};

template<typename List>
struct Init;

template<typename List,size_t N>
struct Take;
template<typename T,T...Args>
struct Take<List<T,Args...>,0>{
    using type=List<T>;
};
template<typename T,T H,T...Args>
struct Take<List<T,H,Args...>,0>{
    using type=List<T>;
};
template<typename T,T H,T...Args,size_t N>
struct Take<List<T,H,Args...>,N>{
    using type=typename PushFront<typename Take<List<T, Args...>,N-1>::type, H>::type;
};

template<typename T,int N,size_t X>
struct Replicate{
    using type=typename PushFront<typename Replicate<T,N-1,X>::type, X>::type;
};

template<typename T,size_t X>
struct Replicate<T,0,X>{
    using type=List<T>;
};

template<typename List>
struct Reverse;

template<typename T>
struct Reverse<List<T>>{
    using type=List<T>;
};

template<typename T,T H,T...Args>
struct Reverse<List<T,H,Args...>>{
    using type=typename PushBack<typename Reverse<List<T,Args...>>::type, H>::type;
};



namespace Imple{
template<typename T,T a,T b>
struct ValueLE{
    static constexpr bool value=a<=b;
};
template<typename T,T a,T b>
struct ValueGT{
    static constexpr bool value=a>b;
};

template<template<typename T,T a,T b>class Op,typename List,typename List::type Value>
struct Select;
template<template<typename T,T a,T b>class Op,typename T,T Value>
struct Select<Op,List<T>,Value>{
    using type=List<T>;
};
template<template<typename T,T a,T b>class Op,typename T,T H,T...Args,T Value>
struct Select<Op,List<T,H,Args...>,Value>{
    static constexpr bool nowOk=Op<T,H,Value>::value;
    using type=typename std::conditional<nowOk,typename PushFront<typename Select<Op,List<T, Args...>,Value>::type, H>::type ,typename Select<Op,List<T, Args...>,Value>::type>::type;

};

}
template<typename List>
struct QuickSort;

template<typename T>
struct QuickSort<List<T>>{
    using type=List<T>;
};
template<typename T,T H,T...Args>
struct QuickSort<List<T, H,Args...>>{
    using smallerSorted=typename QuickSort<typename Imple::Select<Imple::ValueLE, List<T, Args...>, H>::type>::type;
    using biggerSorted=typename QuickSort<typename Imple::Select<Imple::ValueGT, List<T, Args...>, H>::type>::type;
    using type=typename ListCnt<typename PushBack<smallerSorted, H>::type, biggerSorted>::type;
};

template<typename List,template<typename List::type Value> class Op>
struct Map;
template<typename T,template<T Value> class Op>
struct Map<List<T>,Op>{
    using type=List<T>;
};
template<typename T,T H,T...Args,template<T Value> class Op>
struct Map<List<T,H,Args...>,Op>{
    using type=typename PushFront<typename Map<List<T, Args...>, Op>::type,Op<H>::value>::type;
};

template<typename List,template<typename List::type Value> class Op>
struct Filter;
template<typename T,template<T Value> class Op>
struct Filter<List<T>,Op>{
    using type=List<T>;
};
template<typename T,T H,T...Args,template<T Value> class Op>
struct Filter<List<T,H,Args...>,Op>{
    using trueType=typename PushFront<typename Filter<List<T,Args...>, Op>::type,H>::type;
    using falseType=typename Filter<List<T,Args...>, Op>::type;
    using type=typename std::conditional<Op<H>::value, trueType, falseType>::type;
};
//foldl 参数为二元函数、列表、初始值

template<typename List,typename InitType,template<InitType,typename List::type>class Op,InitType init>
struct Foldl;

template<typename T,typename InitType,template<InitType,T>class Op,InitType init>
struct Foldl<List<T>,InitType,Op,init>{
    static constexpr InitType value=init;
};
template<typename T,T H,T...Args,typename InitType,template<InitType,T>class Op,InitType init>
struct Foldl<List<T,H,Args...>,InitType,Op,init>{
    static constexpr InitType value=Foldl<List<T, Args...>,InitType,Op,Op<init, H>::value>::value;
};


}
//template<typename List,template<typename,typename>class Op,typename Init>
//struct Foldl;
//


#endif /* List_hpp */
