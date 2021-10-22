/*
 * @Author: zhou33
 * @Date: 2021-10-16 14:10:53
 * @LastEditors: zhou33
 * @LastEditTime: 2021-10-22 12:49:32
 */
//单例模式、智能指针、懒汉式
#include "Singleton.h"

shared_ptr<Singleton> Singleton::m_instance = nullptr;
mutex Singleton::m_mutex;

shared_ptr<Singleton> Singleton::getInstance()
{
    if (m_instance == nullptr)
    {
        lock_guard<mutex> lk(m_mutex);
        if (m_instance == nullptr)
            m_instance = shared_ptr<Singleton>(new Singleton, PtrDestroy);
    }
    return m_instance;
}

void Singleton::PtrDestroy(Singleton *instance)
{
    cout << "PtrDestroy()" << endl;
    delete instance;
}