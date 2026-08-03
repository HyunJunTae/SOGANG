#include <iostream>
#include <string>
using namespace std;


string insert_space(string input_string)
{
  string new_string;

  for(int i=0; i<input_string.length(); i++) {
    new_string += input_string[i];
    if (input_string[i+1] >= 65 && input_string[i+1] <= 90) {
      new_string += ' ';
    }
  }

  return new_string;
}




int main() {

  string original_string;
  cout << "Enter the string (w/o space): ";
  getline(cin, original_string);
  cout << "Edited string: " << insert_space(original_string) << endl;

  return 0;

}