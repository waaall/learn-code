


* 动态规划
* 贪心算法
* 搜索与回溯
* 分治思想

### 两数之和

* 给定一个整数数组 nums 和一个整数目标值 target，请你在该数组中找出 和为目标值 target的那两个整数，并返回它们的数组下标。你可以假设每种输入只会对应一个答案。但是，数组中同一个元素在答案里不能重复出现。你可以按任意顺序返回答案。

示例 ：
```shell
(1)
输入：nums = [2,7,11,15], target = 9
输出：[0,1]
解释：因为 nums[0] + nums[1] == 9 ，返回 [0, 1] 。

(2)
输入：nums = [3,2,4], target = 6
输出：[1,2]
```

* 解答：
很明显暴力的解法是两层for循环查找，时间复杂度是O(n^2)。

建议大家做这道题目之前，先做一下这两道

242. 有效的字母异位词
243. 两个数组的交集
244. 有效的字母异位词 这道题目是用数组作为哈希表来解决哈希问题，349. 两个数组的交集这道题目是通过set作为哈希表来解决哈希问题。

本题呢，则要使用map，那么来看一下使用数组和set来做哈希法的局限。

数组的大小是受限制的，而且如果元素很少，而哈希值太大会造成内存空间的浪费。
set是一个集合，里面放的元素只能是一个key，而两数之和这道题目，不仅要判断y是否存在而且还要记录y的下表位置，因为要返回x 和 y的下表。所以set 也不能用。
此时就要选择另一种数据结构：map ，map是一种key value的存储结构，可以用key保存数值，用value在保存数值所在的下表。

C++中map，有三种类型： 如图

std::unordered_map 底层实现为哈希表，std::map 和std::multimap 的底层实现是红黑树。

同理，std::map 和std::multimap 的key也是有序的（这个问题也经常作为面试题，考察对语言容器底层的理解）。 更多哈希表的理论知识请看关于哈希表，你该了解这些！。

**这道题目中并不需要key有序，选择std::unordered_map 效率更高！**

C++代码：
```c
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        std::unordered_map <int,int> map;
        for(int i = 0; i < nums.size(); i++) {
            auto iter = map.find(target - nums[i]);
            if(iter != map.end()) {
                return {iter->second, i};
            }
            map.insert(pair<int, int>(nums[i], i));
        }
        return {};
    }
};
```
依然不懂这个答案。为何可以直接初始化一个Hash表就可以找vector，它怎么对应的？



### 回文数

* 给你一个整数 x ，如果 x 是一个回文整数，返回 true ；否则，返回 false 。回文数是指正序（从左向右）和倒序（从右向左）读都是一样的整数。

```shell
例如，121 是回文，而 123 不是。
 
示例 1：

输入：x = 121
输出：true
示例 2：

输入：x = -121
输出：false
解释：从左向右读, 为 -121 。 从右向左读, 为 121- 。因此它不是一个回文数。
示例 3：

输入：x = 10
输出：false
解释：从右向左读, 为 01 。因此它不是一个回文数。
```

进阶：你能不将整数转为字符串来解决这个问题吗？

* 解答：
```c
bool isPalindrome(int x) {
    if(x<0) return false;
    if(x==0) return true;
    
    //如果不将其转化为unsigned，则1234567899这个数倒过来会溢出。
    unsigned int y = 0;
    int mid = x; 
    while(mid != 0){
        int d = mid%10;
        y = y*10 + d;
        mid = mid/10;
    }
    cout << y << endl;
    return y==x;
}

int main(int argc, char const *argv[])
{
    int x = 1234567899;
    cout << isPalindrome(x);

    return 0;
}
```
### 罗马数字转整数

```c
class Solution {
private:
    unordered_map<char, int> symbolValues = {
        {'I', 1},
        {'V', 5},
        {'X', 10},
        {'L', 50},
        {'C', 100},
        {'D', 500},
        {'M', 1000},
    };

public:
    int romanToInt(string s) {
        int ans = 0;
        int n = s.length();
        for (int i = 0; i < n; ++i) {
            int value = symbolValues[s[i]];
            if (i < n - 1 && value < symbolValues[s[i + 1]]) {
                ans -= value;
            } else {
                ans += value;
            }
        }
        return ans;
    }
};
```

### 字符串相加

给定两个字符串形式的非负整数 num1 和num2 ，计算它们的和并同样以字符串形式返回。你不能使用任何內建的用于处理大整数的库（比如 BigInteger）， 也不能直接将输入的字符串转换为整数形式。


示例 1：

输入：num1 = "11", num2 = "123"
输出："134"
示例 2：

输入：num1 = "456", num2 = "77"
输出："533"
示例 3：

输入：num1 = "0", num2 = "0"
输出："0"

* 解答思路：
（1）如果不把字符串转为整数，则需要用“ASCII”码来做文章。0-9的ASCII码是挨着的，所以他们与'0'的差，在二进制上就是真正的该数字，比如'5'-'0'，得到4的二进制数。
（2）另一方面，加法运算的逻辑就是CPU加法器了类似，用额外一位来存放进位信息。
（3）可以用几次强制类型转换，让ASCII码和int类型来回转换，不要弄错其转换的对应关系就好（转换时，int的二进制=ASCII码的二进制），下面答案中是隐式的转换。

```c
class Solution {
public:
    string addStrings(string num1, string num2) {
        string ans = "";
        int i = num1.length() -1, j = num2.length() -1, add = 0;
        while (i>=0||j>=0||add!=0){
            int x = i>=0 ? num1[i]-'0' : 0;
            int y = j>=0 ? num2[j]-'0' : 0;
            int result = x+y+add;
            ans.push_back('0' + result%10);
            add=result/10;
            --j;
            --i;
        }
        reverse(ans.begin(), ans.end());
        return ans;
    }
};
```


### 括号匹配问题

给定一个只包括 '('，')'，'{'，'}'，'['，']' 的字符串 s ，判断字符串是否有效。有效字符串需满足：左括号必须用相同类型的右括号闭合。左括号必须以正确的顺序闭合。



* 解答：
```c
class Solution {
public:
    bool isValid(string s) {
        int n = s.size();
        if (n % 2 == 1) {
            return false;
        }

        unordered_map<char, char> pairs = {
            {')', '('},
            {']', '['},
            {'}', '{'}
        };
        stack<char> stk;
        for (char ch: s) {
            if (pairs.count(ch)) {
                if (stk.empty() || stk.top() != pairs[ch]) {
                    return false;
                }
                stk.pop();
            }
            else {
                stk.push(ch);
            }
        }
        return stk.empty();
    }
};
```

### 最大相同前缀

```c
class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        string ans = "";
        for(int i =0; i<strs[0].length();i++){
            char c = strs[0][i];
            for (string s : strs){
                if (s[i] != c)
                    return ans;
            }
            ans += c;
        }
        return ans;
    }
};
```

### 至少是其他两倍大数
给你一个整数数组 nums，其中总是存在唯一的一个最大整数。请你找出数组中的最大元素并检查它是否 至少是数组中每个其他数字的两倍 。如果是，则返回 最大元素的下标 ，否则返回 -1 。

示例 1：

输入：nums = [3,6,1,0]
输出：1
解释：6 是最大的整数，对于数组中的其他整数，6 至少是数组中其他元素的两倍。6 的下标是 1 ，所以返回 1 。
示例 2：

输入：nums = [1,2,3,4]
输出：-1
解释：4 没有超过 3 的两倍大，所以返回 -1 。
示例 3：

输入：nums = [1]
输出：0
解释：因为不存在其他数字，所以认为现有数字 1 至少是其他数字的两倍。

```c
class Solution {
public:
    int dominantIndex(vector<int>& nums) {
        int m1 = -1, m2 = -1;
        int index = -1;
        for (int i = 0; i < nums.size(); i++) {
            if (nums[i] > m1) {
                m2 = m1;
                m1 = nums[i];
                index = i;
            } else if (nums[i] > m2) {
                m2 = nums[i];
            }
        }
        return m1 >= m2 * 2 ? index : -1;
    }
};
```


### 最大子数组和
给你一个整数数组 nums ，请你找出一个具有最大和的连续子数组（子数组最少包含一个元素），返回其最大和。子数组 是数组中的一个连续部分。
 
示例 1：

输入：nums = [-2,1,-3,4,-1,2,1,-5,4]
输出：6
解释：连续子数组 [4,-1,2,1] 的和最大，为 6 。
示例 2：

输入：nums = [1]
输出：1
示例 3：

输入：nums = [5,4,-1,7,8]
输出：23

* 解答：

```c
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int ans = nums[0],temp=nums[0];
        
        for (int i =1;i<=nums.size()-1;i++){
            if (temp+nums[i] <= nums[i])
                temp = nums[i];
            else 
                temp += nums[i];
            if(ans < temp)
                ans = temp;
        }
        return ans;
    }
};
```


### 删除有序数组中的重复项

我用的特别笨的办法，还是失败了。因为尝试用“删除”函数。而没有想到，删除函数是一个时间复杂度很高的函数(因为每次删除都需要重排，o(n)的时间复杂度)。

而这种情况下，直接覆盖(赋值)会好的多！（还有下面一个问题《移除指定元素》也是类似的思路）

```c
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        int n = nums.size();
        if (n == 0) {
            return 0;
        }
        int fast = 1, slow = 1;
        while (fast < n) {
            if (nums[fast] != nums[fast - 1]) {
                nums[slow] = nums[fast];
                ++slow;
            }
            ++fast;
        }
        return slow;
    }
};
```

* [一个问题](https://stackoverflow.com/questions/3673684/peeking-the-next-element-in-stl-container)

### 移除指定元素
给你一个数组 nums 和一个值 val，你需要 原地 移除所有数值等于 val 的元素，并返回移除后数组的新长度。
不要使用额外的数组空间，你必须仅使用 O(1) 额外空间并 原地 修改输入数组。
元素的顺序可以改变。你不需要考虑数组中超出新长度后面的元素。
 
说明:
为什么返回数值是整数，但输出的答案是数组呢?
请注意，输入数组是以「引用」方式传递的，这意味着在函数里修改输入数组对于调用者是可见的。

```c
#include <string.h>
#include <vector>
#include <map>
#include <stack>
#include <unordered_map>
#include <iostream>
using namespace std;

int removeElement(vector<int>& nums, int val) {
    int j=0;
    for (int i=0; i<nums.size();++i){
        if (nums[i]!=val){
            nums[j]=nums[i];
            j++;
        }
    }
    return j;
}

int main(int argc, char const *argv[])
{
    vector<int> s = {0,1,2,2,3,0,4,2};
    int val = 2;
    removeElement(s,val);
    for (int i = 0; i < 4; ++i)
    {
        cout << endl << s[i];
    }

    return 0;
}
```
### 删除字符串中的所有相邻重复项
给出由小写字母组成的字符串 S，重复项删除操作会选择两个相邻且相同的字母，并删除它们。在 S 上反复执行重复项删除操作，直到无法继续删除。在完成所有重复项删除操作后返回最终的字符串。答案保证唯一。

示例：
输入："abbaca"
输出："ca"
解释：
例如，在 "abbaca" 中，我们可以删除 "bb" 由于两字母相邻且相同，这是此时唯一可以执行删除操作的重复项。之后我们得到字符串 "aaca"，其中又只有 "aa" 可以执行重复项删除操作，所以最后的字符串为 "ca"。

* 解答：

```c

```

### 合并两个有序链表

```shell
示例1: 
输入：l1 = [1,2,4], l2 = [1,3,4]
输出：[1,1,2,3,4,4]

示例2：
输入：l1 = [], l2 = []
输出：[]

示例3：
输入：l1 = [], l2 = [0]
输出：[0]
```

* 解答：
设置两个指针，互。。。

```c
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode* preHead = new ListNode(-1);

        ListNode* prev = preHead;
        while (l1 != nullptr && l2 != nullptr) {
            if (l1->val < l2->val) {
                prev->next = l1;
                l1 = l1->next;
            } else {
                prev->next = l2;
                l2 = l2->next;
            }
            prev = prev->next;
        }

        // 合并后 l1 和 l2 最多只有一个还未被合并完，我们直接将链表末尾指向未合并完的链表即可
        prev->next = l1 == nullptr ? l2 : l1;

        return preHead->next;
    }
};
```

### 爬楼梯
假设你正在[爬楼梯](https://leetcode-cn.com/problems/climbing-stairs)。需要 n 阶你才能到达楼顶。

每次你可以爬 1 或 2 个台阶。你有多少种不同的方法可以爬到楼顶呢？


示例 1：

输入：n = 2
输出：2
解释：有两种方法可以爬到楼顶。
1. 1 阶 + 1 阶
2. 2 阶
示例 2：

输入：n = 3
输出：3
解释：有三种方法可以爬到楼顶。
1. 1 阶 + 1 阶 + 1 阶
2. 1 阶 + 2 阶
3. 2 阶 + 1 阶

* 解答

```c
class Solution {
public:
    int climbStairs(int n) {
        int p = 0, q = 0, r = 1;
        for (int i = 1; i <= n; ++i) {
            p = q; 
            q = r; 
            r = p + q;
        }
        return r;
    }
};
```

#### 动态规划
[动态规划-知乎](https://www.zhihu.com/question/23995189/answer/613096905)



