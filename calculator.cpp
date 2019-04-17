// Brittany Kraemer
// CS301 - 03
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

struct digit {
  int data;
  digit *next;
};

digit* loadNumber(ifstream &file);
int digcmp(digit * left, digit *right);
char getOperator(ifstream & file);
void writeNumber(digit * num, ofstream & file);
digit* addNumbers(digit * left, digit * right);
digit* subNumbers(digit * left, digit * right);
void printNum(digit * num);
void subCarry(digit * head, digit * prev);

int main() {
  digit *left, *right, *result;
  char op;
  ifstream inFile ("largeNumbers.txt");
  ofstream outFile ("output.txt");

  char character;
  left = loadNumber(inFile);
  while (left) {
    right = loadNumber(inFile);
    op = getOperator(inFile);
    if (op == '+') {
      result = addNumbers(left, right);
    }
    else {
      result = subNumbers(left, right);
    }
    writeNumber(result, outFile);
    left = loadNumber(inFile);
  }
  outFile.close();
  inFile.close();
  return 0;
}

/*
*  returns a number <0 || 0 || >0
*  <0 right number is larger
*  0  the numbers are equal
*  >0 left number is larger
*  only works if the number is stored in reverse order
*/

int digcmp(digit * left, digit * right) {
    int result = 0;
    do
    {
        if (left->data - right->data == 0){
        } // take no action
        else {
            result = left->data - right->data;
        }
        left = left->next;
        right = right->next;

    } while (left != nullptr && right != nullptr);

    if (left == nullptr && right != nullptr) {
        // right number is larger
        result = -1;
    }

    if (left != nullptr && right == nullptr) {
        // left number is larger
        result = 1;
    }
    return result;
}

// loads the number from a text file into a linked list of individual digits
digit * loadNumber(ifstream &file){
  digit *head = nullptr;
  char value;
  string nline;
  getline(file, nline);
  for (int i = 0; i < nline.size(); i++) {
    head = new digit{(int)(nline[i] -48), head};
  }
  return head;
  delete head;
}

// takes in the operator from the text file in which we perform on the two numbers
char getOperator(ifstream & file){
  string oline;
  getline(file, oline);
  return oline[0]; 
}

// prints the number as output
void printNum(digit * num){
  digit *temp = num;
  while(temp != nullptr) {
    cout << temp->data;
    temp = temp->next;
  }
  cout << endl;
}

// writes the number in a file
void writeNumber(digit * num, ofstream & file){
  digit *temp = num;
  while(temp != nullptr) {
    file << temp->data;
    temp = temp->next;
  }
  file << endl;
}

// adds two linked lists and returns the result in a separate linked list
digit * addNumbers(digit * left, digit * right) {
  digit *temp1 = left;
  digit *temp2 = right;
  digit *newList = nullptr;
  int sum = 0;
  int carry = 0;
  while(temp1 != nullptr) {
    sum = temp1->data + temp2->data + carry;
    if (sum < 10) {
      // no carry over
      carry = 0;
    }
    else {
      // with carry over
      sum = sum%10;
      carry = 1;
    }
    newList = new digit {sum, newList};
    if (temp1->next == nullptr && carry == 1) {
      // adds an extra digit at the beginning of the list for carry over
      newList = new digit{carry, newList};
    }
    temp1 = temp1->next;
    temp2 = temp2->next;
  }
  return newList;
  delete newList, temp1, temp2;
}

// a helper function for dealing with carry digits in subtraction
void subCarry(digit * head, digit * prev){
    head->data = head->data + 10;
    prev->data = prev->data - 1;
}

// subtracts two linked lists and returns their result in a separate linked list
digit * subNumbers(digit * left, digit * right){
    bool negative = false;
    digit *newList = nullptr;
    digit *temp1 = left;
    digit *temp2 = right;
    digit *prev = temp1->next;
    if (digcmp(left, right) == -1) {
      // deals with subtraction resulting in negative
      temp1 = right;
      temp2 = left;
      negative = true;
    }
    int subtract = 0;
    while(temp1 != nullptr) {
      subtract = temp1->data - temp2->data;
      if (subtract < 0 && prev != nullptr) {
        // if right digit > left digit, carry over & perform subtraction again 
        subCarry(temp1, prev);
        subtract = temp1->data - temp2->data;
      }
      if (negative == true && temp1->next == nullptr) {
        // the first digit becomes negative when subtraction results in a negative number
        subtract = -subtract;
      }
      newList = new digit{subtract, newList};
      temp1 = temp1->next;
      temp2 = temp2->next;
      if (prev != nullptr){
        // ensures prev doesnt go out of bounds
        prev = prev->next;
      }
    }
    return newList;
    delete temp1, temp2, prev, newList;
}

