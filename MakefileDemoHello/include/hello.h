/*
 * @Author: zhou33
 * @Date: 2021-10-03 09:15:15
 * @LastEditors: zhou33
 * @LastEditTime: 2021-10-03 11:06:50
 */
#pragma once
#include <string>
using std::string;

class Hello
{
public:
    Hello(){};
    Hello(int a, string s) : i(a), str(s) {}
    void show();
    void show() const;

private:
    int i;
    string str;
};