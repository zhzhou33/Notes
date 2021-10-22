/*
 * @Author: zhou33
 * @Date: 2021-10-16 14:19:11
 * @LastEditors: zhou33
 * @LastEditTime: 2021-10-22 12:51:12
 */
#include "Singleton.h"

int main()
{
    shared_ptr<Singleton> instance1 = Singleton::getInstance();
    shared_ptr<Singleton> instance2 = Singleton::getInstance();
    cout << "instance1=" << instance1 << endl;
    cout << "instance2=" << instance2 << endl;
}