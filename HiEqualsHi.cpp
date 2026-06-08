#include <iostream>
#include <string>
#include <cctype>
using namespace std;

int strcmp_case_insensitive(const string& s1, const string& s2)
{
    size_t i = 0;

    while (i < s1.length() && i < s2.length())
    {
        char c1 = tolower(s1[i]);
        char c2 = tolower(s2[i]);

        if (c1 != c2)
        {
            return c1 - c2;  
        }

        i++;
    }

    return (int)s1.length() - (int)s2.length();
}


int main()
{
    cout << "Testing strcmp_case_insensitive:\n\n";

    string a1 = "Hello";
    string a2 = "hello";
    cout << a1 << " vs " << a2 << " → " 
         << strcmp_case_insensitive(a1, a2) << endl;

    string b1 = "Apple";
    string b2 = "Banana";
    cout << b1 << " vs " << b2 << " → " 
         << strcmp_case_insensitive(b1, b2) << endl;

    string c1 = "Zebra";
    string c2 = "Lion";
    cout << c1 << " vs " << c2 << " → " 
         << strcmp_case_insensitive(c1, c2) << endl;

    string d1 = "String One";
    string d2 = "string one";
    cout << d1 << " vs " << d2 << " → " 
         << strcmp_case_insensitive(d1, d2) << endl;

    string e1 = "String One";
    string e2 = "string Two";
    cout << e1 << " vs " << e2 << " → " 
         << strcmp_case_insensitive(e1, e2) << endl;

    return 0;
}
