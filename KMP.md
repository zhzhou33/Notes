+ KMP算法原理

  + 首先对匹配的模板pattern构造一个临时辅助数组

    + 例如 pattern = "abcdabce"
    + 临时数组为 [0,0,0,0,1,2,3,0];
      + 解释：之前没有出现过数组元素值为 0，如果出现过，则为前一个的 下标 index+1
    + 为什么这么构建？
      + 利用**前缀后缀**思想，即某个字符既是前缀又是后缀
      + 可以观察到，数组元素值不为 0 的位置，表示的是前一个相同字符的下一个字符位置
      + 也就是说，下一次匹配可以从这个位置进行，也就省去了前缀的匹配过程
      + 例如，以 第二个 'c' 为例，临时数组对应的值为 3 ，代表下一个字符可以从 pattern[3] = 'd' 开始，因为 ‘abc’ 既是前缀又是后缀
        + 既然匹配到了第二个 'c' 了，说明 string = \*abcdabcx\* ，这时匹配到 'x' ， 发现显然不匹配，则检查前一个数组下标，跳转到 'd' 的位置匹配
        + 跳到到 'd' ，因为此时 'abc' 已经匹配上了，既是前缀又是后缀，直接从 pattern[3] 开始匹配。
      + 需要注意的是，因为采用的是双指针 i j ， 所以对于 'e' 来说，需要检查前一个与 j 进行比较，具体执行情况文字表述不清
    + 了解了临时数组的作用，利用临时数组也就很容易构建出KMP的过程

  + ```c++
    /*
        KMP算法，总的时间复杂度 O(m+n)
                总的空间复杂度 O(n)
        n 是匹配的字符串
        m 是需要检查的字符串
    */
    
    //计算临时数组，时间复杂度 O(n),空间复杂度 O(n)
    vector<int> computeTemporaryArray(string &pattern)
    {
        vector<int> temp(pattern.length(), 0);
        int index = 0;
        for (int i = 1; i < pattern.length();)
        {
            if (pattern[i] == pattern[index])
            {
                temp[i] = index + 1;
                index++;
                i++;
            }
            else
            {
                if (index != 0)
                    index = temp[index - 1];
                else
                {
                    temp[i] = 0;
                    i++;
                }
            }
        }
        return temp;
    }
    
    //KMP,时间复杂度 O(str.length())
    bool KMP(string &str, string &pattern)
    {
        vector<int> temp = computeTemporaryArray(pattern);
        int i = 0;
        int j = 0;
        while (i < str.length() && j < pattern.length())
        {
            if (str[i] == pattern[j])
            {
                i++;
                j++;
            }
            else
            {
                if (j != 0)
                    j = temp[j - 1];
                else
                    i++;
            }
        }
        if (j == pattern.length())
            return true;
        return false;
    }
    ```