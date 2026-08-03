#include <iostream>
#include <string>
using namespace std;


string my_remove_chars(string original_string, string remove)
{ 
  int count = 0;
  string new_string;

  for(int i=0; i<original_string.length(); i++) {
    for(int j=0; j<remove.length(); j++) {
      if (original_string[i] != remove[j]) 
        count += 1;
    }
    if (count == remove.length())
      new_string += original_string[i];
    count = 0;
  }

  return new_string;
}



// string my_remove_chars(string original_string, string remove)
// {
//   int i, j;

//   for(i=0; i<remove.length(); i++) {
//     j = 0;

//     while(1) {
//       if (remove[i] == original_string[j]) {
//         original_string.erase(j, 1);
//         continue;
//       }
//       if (original_string.length() <= j) {
//         break;
//       }
//       j++;
//     }
//   }

//   return original_string;
// }


// string my_remove_chars(string original_string, string remove)
// {
//   int i, j;

//   for(i=0; i<remove.length(); i++) {
//     while(1) {
//       j = original_string.find(remove[i]);
//       if (j == -1) 
//         break;
//       original_string.erase(j, 1);
//     }
//   }

//   return original_string;
// }


int main() {

  string original_string;
  string remove;
  string result_string;

  cout << "Enter the string: ";
  getline(cin, original_string);
  cout << "Enter the characters that you want to remove: ";
  getline(cin, remove);

  result_string = my_remove_chars(original_string, remove);
  cout << "Edited string: " << result_string << endl;

  return 0;

}