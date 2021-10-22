/*
 * @Author: zhou33
 * @Date: 2021-10-22 12:35:35
 * @LastEditors: zhou33
 * @LastEditTime: 2021-10-22 13:15:42
 */
#pragma once
#include <mutex>
#include <memory>
#include <iostream>
using namespace std;

class Singleton
{
public:
    static shared_ptr<Singleton> getInstance();

private:
    //Singleton() = default;
    //~Singleton() = default;
    Singleton() { cout << "Singleton()" << endl; }
    ~Singleton() { cout << "~Singleton()" << endl; }
    Singleton(const Singleton &) {}
    Singleton &operator=(const Singleton &) {}
    static void PtrDestroy(Singleton *instance);

private:
    static shared_ptr<Singleton> m_instance;
    static mutex m_mutex;
};
