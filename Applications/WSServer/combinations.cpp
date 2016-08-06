//#include <algorithm>
//#include <cassert>
//#include <iostream>
//#include <vector>

//using std::copy;
//using std::cout;
//using std::endl;
//using std::vector;

//void combinations_r_recursive(const vector<char> &elems, unsigned long req_len,
//                vector<unsigned long> &pos, unsigned long depth,
//                unsigned long margin)
//{
//    // Have we selected the number of required elements?
//    if (depth >= req_len) {
//        cout << "{\"" ;
//        for (unsigned long ii = 0; ii < pos.size(); ++ii)
//            cout << elems[pos[ii]];
//        cout << "\"}," << endl;
//        return;
//    }

//    // Try to select new elements to the right of the last selected one.
//    for (unsigned long ii = margin; ii < elems.size(); ++ii) {
//        pos[depth] = ii;
//        combinations_r_recursive(elems, req_len, pos, depth + 1, ii);
//    }
//    return;
//}

//void combinations_r(const vector<char> &elems, unsigned long req_len)
//{
//    assert(req_len > 0 && req_len <= elems.size());
//    vector<unsigned long> positions(req_len, 0);
//    combinations_r_recursive(elems, req_len, positions, 0, 0);
//}

//const unsigned long num_elements = 17;
//const unsigned long comb_len = 2;

//int main()
//{
//	vector<char> elements(num_elements);
//	char elements_str[num_elements + 1] = "ACEFGHIJKNOPSTUXY";
//	copy(elements_str, elements_str + num_elements, elements.begin());

//	combinations_r(elements, comb_len);
//	return 0;
//}


// An optimized version that uses reverse instead of sort for
// finding the next permutation

// Program to print all permutations of a string in sorted order.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Following function is needed for library function qsort(). Refer
   http://www.cplusplus.com/reference/clibrary/cstdlib/qsort/ */
int compare (const void *a, const void * b)
{  return ( *(char *)a - *(char *)b ); }

// A utility function two swap two characters a and b
void swap (char* a, char* b)
{
    char t = *a;
    *a = *b;
    *b = t;
}

// This function finds the index of the smallest character
// which is greater than 'first' and is present in str[l..h]
int findCeil (char str[], char first, int l, int h)
{
    // initialize index of ceiling element
    int ceilIndex = l;

    // Now iterate through rest of the elements and find
    // the smallest character greater than 'first'
    for (int i = l+1; i <= h; i++)
      if (str[i] > first && str[i] < str[ceilIndex])
            ceilIndex = i;

    return ceilIndex;
}
// A utility function to reverse a string str[l..h]
void reverse(char str[], int l, int h)
{
while (l < h)
{
    swap(&str[l], &str[h]);
    l++;
    h--;
}
}

// Print all permutations of str in sorted order
void sortedPermutations ( char str[] )
{
    // Get size of string
    int size = strlen(str);

    // Sort the string in increasing order
    qsort( str, size, sizeof( str[0] ), compare );

    // Print permutations one by one
    bool isFinished = false;
    while ( ! isFinished )
    {
        // print this permutation
        printf ("%s \n", str);

        // Find the rightmost character which is smaller than its next
        // character. Let us call it 'first char'
        int i;
        for ( i = size - 2; i >= 0; --i )
        if (str[i] < str[i+1])
            break;

        // If there is no such chracter, all are sorted in decreasing order,
        // means we just printed the last permutation and we are done.
        if ( i == -1 )
            isFinished = true;
        else
        {
            // Find the ceil of 'first char' in right of first character.
            // Ceil of a character is the smallest character greater than it
            int ceilIndex = findCeil( str, str[i], i + 1, size - 1 );

            // Swap first and second characters
            swap( &str[i], &str[ceilIndex] );

            // reverse the string on right of 'first char'
            reverse( str, i + 1, size - 1 );
        }
    }
}
