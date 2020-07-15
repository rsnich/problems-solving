#include <vector>
#include <algorithm>

/**
 * @brief   The function finds a subset that the sum of subset elements is the equal or the 
 *          closest one to the provided target. This function is implemented using dynamic 
 *          programming and has complexity O(nT) or if T is a small enough then complexity is 
 *          O(n log n) (where n - number of elements in the array and T - specified sum target) 
 *          because of sorting elements in the provided array. In order to save calculated data 
 *          the algorith uses 2D array with columns 0...T and rows 0...n. It is better to sort
 *          data in the array what significuntly simplifies using the algoritm.
 *          
 *          Example:  
 * 
 *          I = [1 3 5 6]
 *          T = 8
 * 
 *          Create following 2D array to save result of solved sub problems:
 * 
 *            
 *          0   0 1 2 3 4 5 6 7 8 (T)
 *          1   0 0 1 2 3 4 5 6 7
 *          3   0 0 1 0 0 1 2 3 4 
 *          5   0 0 1 2 0 0 0 1 0  
 *          6   0 0 1 2 0 0 0 0 1
 *         (I)         
 * 
 *          
 *          For column '0' (T=0) the result is always '0' that is because the column is initialized with '0'.
 *          For other cells, if I[i-1] < memorize[0][j], where j current memorize[0][j] value, it just copies 
 *          value from the above cell as until I[i-1] < memorize[0][j] it doen't make any effect on the result. 
 *          (i-1 because the 2D array always contain the first '0' row). 
 *
 *          This part from the algorithm below
 *          ...
 *          if(V[i-1] > memorize[0][j])
 *           {
 *               // Just copy value from the cell above
 *               memorize[i][j] = memorize[i-1][j];
 *           }
 *          ... 
 * 
 *          If the above condition is false then it calculates delta of current T column (memorize[0][j]) and 
 *          I[i-1] then goes to the above row and column with index delta in order to get already calculated result 
 *          for delta and saves the result to current cell.
 *          
 *          This part from the algorithm below
 *          
 *          {
 *               uint32_t rest = memorize[0][j] - V[i-1];
 *               ...
 *               memorize[i][j] = memorize[i-1][rest];
 *           }
 * 
 *          The function will exit if there is the first '0' value in the last column, it means it has found the fist 
 *          subset where S==T. Otherwise the algorithm will complete filling the 2D array and then will take the minimum 
 *          value from the last column as should be the cosest S value to T.
 *          
 *          Having indexes of the final result the algorithms collects necessary elements from the array I into the subset M,
 *          using following algorithm:
 *          ************************** 
 *          1. Saves I[i-1] value to the subset M
 *          2. Calculates a new coumn index like memorize[0][j] - V[i-1]
 *          3. Goes to the above row and the calculated columb, takes the value of the cell and goes up untile the cell value the same.
 *          4. Goes to the step 1 while j > memorize[i_res][T].      
 **         ************************** 
 * @param T target for the subset sum
 * @param I array of numbers
 * @param M contains elements of the result subset
 * @param S sum of subset elements
 */

void find_largest_sum(uint32_t T, 
                      const std::vector<uint32_t> &I, 
                      std::vector<uint32_t>& M, 
                      uint32_t& S)
{
    S = 0;
    
    if(I.empty())
        return;

    // Handle a special case when T=0 and the set might contain 0
    if(T==0)
    {
        // Add '0' to the subset if the array contains it
        auto found = std::find(I.begin(), I.end(), 0);
        if(found != I.end())
        {
            M.push_back(0);
        }
    }

    // Allocate 2D array in order to save calculating results
    using memorize_t = std::vector<std::vector<uint32_t>>;
    memorize_t memorize(I.size()+1, std::vector<uint32_t>(T+1, 0));
    
    auto V = I;
    int i, j, min_indx=0;
    std::sort(V.begin(), V.end());


    // Saves neccessary elements to the result subset
    auto collect_result = [&](int i_res) {
        i = i_res;
        j = T;

        while(j > memorize[i_res][T])
        {
            M.push_back(V[i-1]);
            S += V[i-1];
            j-= V[i-1];
            --i;
            if(i < 0 && j <= memorize[i_res][T])
                break;
            
            int cell_val = memorize[i][j];
            for(;i-1 >= 0 && memorize[i-1][j]==cell_val; --i);
        }
    };
    
    // Initialize the first row for '0' value
    for(i=1; i<=T; i++)
        memorize[0][i] = i;

    // Fill the 2D aray and calculate value for every cell
    for(i=1; i<memorize.size(); i++)
    {
        for(j=0; j<=T; j++)
        {
            if(V[i-1] > memorize[0][j])
            {
                // Just copy the value from the cell above
                memorize[i][j] = memorize[i-1][j];
            }
            else
            {
                // Calculate diff between T and V[i-1]
                uint32_t rest = memorize[0][j] - V[i-1];
                if(j>=T)
                {
                    if(rest==0)
                    {
                        // Found the necessary subset, collect result and exit
                        collect_result(i);
                        return;
                    }
                    // Watch for the minimum difference S and T
                    if(memorize[i][T] < memorize[min_indx][T])
                        min_indx = i;
                }
                // Save result by taking result from the already calculated cell
                memorize[i][j] = memorize[i-1][rest];
            }
        }    
    }
    // Collect result for the clothest sum
    collect_result(min_indx);
}

/*
---------------------------------------------------
Possible test cases for `find_largest_sum` function
--------------------------------------------------- 

01: T=11,   I=[1 2 3 4 5 6 7],                  S=11, M=[5 3 2 1] 
02: T=15,   I=[1 2 3 4 5 6 7],                  S=15, M=[5 4 3 2 1] 
03: T=19,   I=[4 3 6 5 4 3],                    S=19, M=[5 4 4 3 3] 
04: T=21,   I=[4 3 6 5 4 3],                    S=19, M=[5 4 4 3 3] 
05: T=25,   I=[4 3 6 5 4 3],                    S=19, M=[6 5 4 4 3 3] 
06: T=25,   I=[4 4 4 4 4 4 4 4 4],              S=24, M=[4 4 4 4 4 4] 
07: T=12,   I=[1 2 2 3 3 3 4 4 4 4 5 5 5 5 5],  S=12, M=[3 3 3 2 1] 
08: T=3,    I=[4 4 4 4 4 4 4 4 4],              S=0,  M=[] 
09: T=3,    I=[0 0 0],                          S=0,  M=[0] 
10: T=1,    I=[1 2 3 4 5 6 7],                  S=1,  M=[1]
11: T=4,    I=[1 2 3 4 5 6 7],                  S=4,  M=[4]
12: T=7,    I=[1 2 3 4 5 6 7],                  S=7,  M=[7]
13: T=700,  I=[1 2 3 4 5 6 7],                  S=28, M=[7]
14: T=700,  I=[],                               S=0,  M=[]
15: T=0,    I=[1 2 3 4 5 6 7],                  S=0,  M=[]
16: T=0,    I=[0 1 2 3],                        S=0,  M=[0] 

*/
