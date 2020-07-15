#include <iostream>
#include <string>

/**
 * @brief Adds two signed integers, using binary operators, using following way
 * 
 * bit a | bit b | Carry in | result | Carry out
 * ----------------------------------------------
 *    0  |   0   |    0     |    0   |    0   
 *    0  |   0   |    1     |    1   |    0   
 *    0  |   1   |    0     |    1   |    0   
 *    0  |   1   |    1     |    0   |    1   
 *    1  |   0   |    0     |    1   |    0   
 *    1  |   0   |    1     |    0   |    1   
 *    1  |   1   |    0     |    0   |    1 
 *    1  |   1   |    1     |    1   |    1     
 * 
 * @param a The first operand
 * @param b The second operand
 * @return int Result of the operation
 */
int add(const int a, const int b)
{
    int result = 0;
    
    // Size of operands in bits
    constexpr unsigned int size = sizeof(int) * 8;
    unsigned int i = 0, carry = 0;
    
    // Iterate through all bits and perform add operation
    for(; i < size; i++)
    {
        // Calculate result 
        result |= (a >> i & 1 ^ b >> i & 1 ^ carry) << i;
        // Calculate carry 
        carry = (a >> i & 1) & (b >> i & 1) | 
                (a >> i & 1) & carry | 
                (b >> i & 1) & carry; 
    }
    
    return result;
}

/*
---------------------------------------------------
Possible test cases for `add` function
--------------------------------------------------- 
01: a=0             b=0             result=0
02: a=100           b=0             result=100 
03: a=16            b=1             result=17
04: a=4294967295    b=4294967295    result=-2
04: a=4294967295    b=1             result=0
05: a=4294967295    b=2             result=1
06: a=4294967295    b=10            result=9
07: a=15            b=4294967295    result=14
08: a=16            b=128           result=144
09: a=1234567890    b=9876543210    result=-1773790788
10: a=1111          b=2222          result=3333

*/
