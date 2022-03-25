/*
 * @Author: zhou33
 * @Date: 2021-10-03 09:15:24
 * @LastEditors: zhou33
 * @LastEditTime: 2021-10-03 10:55:49
 */
#include "hello.h"
#include <iostream>
using std::cout;
using std::endl;
void Hello::show()
{
    cout << "Hello show()" << endl;
}
void Hello::show() const
{
    cout << "hello show() const" << endl;
}

