/*
Name of Program: EECS 348 Lab 10
Description: Program that reads a file containing 20 doubles in string format, 
prints if each double is valid, and the sum of each double + -123.456. Runs all operations in string format
Input: numbers.txt
Output: Sum with -123.456 and validity of each double in file
Collaborators: None
Sources: DeepSeek
Author: Oscar Ohly
Creation date: 04/15/2025
*/
#include <iostream> //gets standard C++ library
#include <fstream> //gets file reading operations
#include <string> //gets string manipulating operations
#include <vector> //gets vector library
#include <algorithm> //gets library with operations to sort, remove, and reverse elements
#include <stdexcept> //gets error handling library

using namespace std; //removes the need for std:: prefix for cout, cin, and more

bool is_valid_double(const string& num_str) { //func checks if string is a valid double
    if (num_str.empty()) return false; //returns false if string empty

    size_t start = 0; // (lines 24-27) initializes a set of variables used to determine if double is valid
    bool has_sign = false;
    bool has_dot = false;
    bool has_digits = false;

    if (num_str[0] == '+' || num_str[0] == '-') { //(lines 29-32) checks if string has valid sign
        start = 1;
        has_sign = true;
    }

    for (size_t i = start; i < num_str.size(); i++) { //runs for num of chars in string, loop as a whole checks if all chars are valid
        char c = num_str[i]; //sets current char to c

        if (c == '.') { //runs if c is a dot
            if (has_dot) return false;  // (lines 38-39) checks if there are multiple dots
            has_dot = true;
            if (i == num_str.size() - 1) return false;  //checks if there is a dot at the end of string
        } //ends if statement
        else if (isdigit(c)) { //runs if c is a digit
            has_digits = true; //sets has_digits to true
        } //ends else if block
        else { //(lines 45-47) checks if c is not a valid char
            return false;
        }
    } //ends for loop

    if (!has_digits) return false; //checks if string has at least one digit

    if (num_str.find('.') == start && has_sign) return false; //(lines 52-53) checks if string starts with decimal point or sign+decimal point, considers them invalid
    if (num_str.find('.') == 0) return false;

    return true; //if string passes all test, return true/valid
} //ends func

string remove_leading_zeros(string num) { //helper func removes leading zeros from string
    if (num.empty()) return num; //if string num is empty return num

    bool is_negative = (num[0] == '-'); //bool is_negative is set to if num is negative or not
    size_t start = (num[0] == '+' || num[0] == '-') ? 1 : 0; //if num has sign, sets start of num to index 1

    size_t first_non_zero = num.find_first_not_of('0', start); //finds first non-zero char
    if (first_non_zero == string::npos) { //(lines 65-67) if there are no non-zero values, return -0 or 0 based on is_negative
        return is_negative ? "-0" : "0";
    }

    size_t dot_pos = num.find('.'); //finds position of decimal point
    if (dot_pos != string::npos && dot_pos < first_non_zero) { //checks if a decimal point exists and is before the first non-zero char
        first_non_zero = dot_pos; //sets first_non_zero to dot_pos
        if (first_non_zero == start) { //(lines 72-74) handles cases such as 0.123 and -0.123
            first_non_zero--;
        }
    } //ends if statement

    string result; //initializes string result which will be returned
    if (is_negative) result += '-'; //adds - sign as first char in result if num is negative
    result += num.substr(first_non_zero); //adds all chars after the last leading zero to result

    if (result.empty() || result == "-") { //(lines 81-83) handles cases all digits removed before decimal
        return is_negative ? "-0" : "0";
    }

    return result; //returns final result without leading zeros
} //ends func

void split_number(const string& num, string& int_part, string& frac_part, bool& is_negative) { //func that splits a number into int and frac parts
    size_t dot_pos = num.find('.'); //finds position of decimal point
    is_negative = (!num.empty() && num[0] == '-'); //sets is_negative to bool value if num is negative

    size_t start = (num[0] == '+' || num[0] == '-') ? 1 : 0; //sets start to first index of num disregarding any sign

    if (dot_pos == string::npos) { //(lines 94-96) checks if there is no decimal point, sets int part to full num and frac part to 0
        int_part = num.substr(start);
        frac_part = "0";
    } else { //(97-100) sets int and frac parts to appropriate values considering position of decimal point
        int_part = num.substr(start, dot_pos - start);
        frac_part = num.substr(dot_pos + 1);
    }

    if (int_part.empty()) int_part = "0"; //(102-103) handles cases in which parts are empty
    if (frac_part.empty()) frac_part = "0";
} //ends func

string add_positive_integers(const string& a, const string& b) { //func that adds two positive integers
    string result; //initializes string result which will be returned
    int carry = 0; //initializes carry int to handle carrying
    int i = a.length() - 1; //int i set to last index of a
    int j = b.length() - 1; //int j set to last index of b

    while (i >= 0 || j >= 0 || carry > 0) { //runs for as long as it takes to iterate through both strings and carry
        int digit_a = (i >= 0) ? (a[i--] - '0') : 0; //sets digit_a to value at current index of a, or 0 if index is out of range 
        int digit_b = (j >= 0) ? (b[j--] - '0') : 0; //sets digit_b to value at current index of b, or 0 if index is out of range 
        int sum = digit_a + digit_b + carry; //initializes int sum that adds digit_a, digit_b, and carry
        carry = sum / 10; //sets carry to 0 if sum is less than 10, to 1 if sum is more than or equal to 10
        result.push_back((sum % 10) + '0'); //appends sum%10 to result
    } //ends loop

    reverse(result.begin(), result.end()); //reverses order of elements in result to represent actual result of addition
    return result.empty() ? "0" : result; //returns 0 if result is empty or returns result if it isn't empty
} //ends func

int compare_positive_integers(const string& a, const string& b) { //compares two strings lexographically
    if (a.length() != b.length()) { //runs if lengths of strings are unequal
        return a.length() > b.length() ? 1 : -1; //returns 1 if a is longer than b, and -1 if b is longer than a
    } //ends if statement
    return a.compare(b); // returns 1 if a>b, -1 if a<b, or 0 if a=b
} //ends func

string subtract_positive_integers(string a, string b) {//func that substracts two positive integers
    string result; //initializes string result that will be returned
    int borrow = 0; //intializes borrow variable
    int i = a.length() - 1; //sets int i to last index of a
    int j = b.length() - 1; //sets int j to last index of b

    while (i >= 0 || j >= 0) { //runs as long as indexes of strings are >= 0
        int digit_a = (i >= 0) ? (a[i--] - '0') : 0; //sets digit_a to current index of a, or 0 if index is out of bounds
        int digit_b = (j >= 0) ? (b[j--] - '0') : 0; //sets digit_b to current index of b, or 0 if index is out of bounds
        
        digit_a -= borrow; //substracts borrow from digit_a
        borrow = 0; //sets borrow to 0
        
        if (digit_a < digit_b) { //runs if digit_a is less than digit_b, simulating borrowing
            digit_a += 10; //adds 10 to digit_a
            borrow = 1; //sets borrow to 1
        } //ends if statement
        
        result.push_back((digit_a - digit_b) + '0'); //appends subtraction operation of current index to result
    } //ends loop

    while (result.size() > 1 && result.back() == '0') { //(lines 152-154) removes leading zeros from result
        result.pop_back();
    }

    reverse(result.begin(), result.end()); //reverses result, to get final outcome of operation
    return result.empty() ? "0" : result; //returns result or if result is empty, returns 0
} //ends func

string add_fractional_parts(string a, string b, int& carry) { //func that adds fractional parts of two strings
    int max_len = max(a.length(), b.length()); //(161-163) pads a and b with 0s if not same length
    a.append(max_len - a.length(), '0');
    b.append(max_len - b.length(), '0');

    string result; //initializes string result which will be returned
    carry = 0; //sets carry to 0

    for (int i = max_len - 1; i >= 0; i--) { //iterates backwards through the length of both parts
        int sum = (a[i] - '0') + (b[i] - '0') + carry; //sets sum to the sum of the current index of a and b plus the carry
        carry = sum / 10; //sets carry equal to 0 if sum is less than 10, or 1 if sum >= 10
        result.push_back((sum % 10) + '0'); //appends sum to result
    } //ends loop

    reverse(result.begin(), result.end()); //reverses result
    return result; //returns result
} //ends func

string add_string_numbers(const string& a, const string& b) { //func that adds two doubles in string format
    if (a == "0") return b; //(lines 179-180) handles cases where one string is 0
    if (b == "0") return a;

    string a_int, a_frac, b_int, b_frac; //initialzes four strings representing parts of both doubles
    bool a_neg, b_neg; //initializes two bools representing if a or b are negative
    split_number(a, a_int, a_frac, a_neg); //calls split_number on a to get parts of a
    split_number(b, b_int, b_frac, b_neg); //calls split_number on b to get parts of b

    if (!a_neg && !b_neg) { //runs if both a and b are positive
        int frac_carry = 0; //(lines 188-189) adds frac parts of a and b, could produce carry
        string frac_sum = add_fractional_parts(a_frac, b_frac, frac_carry);
        
        string int_sum = add_positive_integers(a_int, b_int); //(lines 191-194) adds int parts of a and b, taking into account frac_carry
        if (frac_carry > 0) {
            int_sum = add_positive_integers(int_sum, to_string(frac_carry));
        }
        
        string result = int_sum; //(lines 196-200) combines frac part and int part, removing any leading zeros
        if (frac_sum != "0") {
            result += "." + frac_sum;
        }
        return remove_leading_zeros(result);
    } //ends if block
    else if (a_neg && b_neg) { //runs if both a and b are negative
        string abs_sum = add_string_numbers(a.substr(1), b.substr(1)); //recurses back to case 1 but without negative signs
        return "-" + abs_sum; //adds negative sign to abs_sum and returns
    } //ends else if block
    else { //runs if a and b have different signs, performs subtraction
        string a_abs = a_neg ? a.substr(1) : a; //sets a_abs to absolute value of a
        string b_abs = b_neg ? b.substr(1) : b; //sets b_abs to absolute value of b
        
        int cmp = compare_positive_integers(a_int, b_int); //sets cmp to whichever int part of a and b is larger
        if (cmp == 0) { //(lines 211-213) runs if int parts are equal, sets cmp to which frac part is larger
            cmp = a_frac.compare(b_frac);
        }
        
        if (cmp == 0) { //(lines 215-217) runs if frac parts are equal, returns 0 since both a and b are equal
            return "0";
        }
        
        string larger_int, larger_frac, smaller_int, smaller_frac; //initializes four strings representing larger and smaller parts of a and b
        bool result_negative; //initializes bool representing if final result is negative or not
        
        if (cmp > 0) { //(lines 222-227) runs if a is larger than b, sets four strings from earlier to appropriate values
            larger_int = a_int;
            larger_frac = a_frac;
            smaller_int = b_int;
            smaller_frac = b_frac;
            result_negative = a_neg;
        } else { //(lines 228-234) runs if b is larger than or equal to b, sets four strings from earlier to appropriate values
            larger_int = b_int;
            larger_frac = b_frac;
            smaller_int = a_int;
            smaller_frac = a_frac;
            result_negative = b_neg;
        }
        
        int max_frac_len = max(larger_frac.length(), smaller_frac.length()); //(lines 236-239) pads frac parts of numbers to same length with zeros
        larger_frac.append(max_frac_len - larger_frac.length(), '0');
        smaller_frac.append(max_frac_len - smaller_frac.length(), '0');
        
        string frac_result; //initializes frac_result which will be final frac part from subtraction
        int frac_borrow = 0; //initializes frac_borrow equal to 0
        
        for (int i = max_frac_len - 1; i >= 0; i--) { //loop iterates backwards from last index of length of frac parts
            int digit_l = (larger_frac[i] - '0') - frac_borrow; //sets digit_l to value of current index of larger frac part
            frac_borrow = 0; //sets frac_borrow to 0
            int digit_s = (smaller_frac[i] - '0'); //sets digit_s to value of current index of smaller frac part
            
            if (digit_l < digit_s) { //(lines 248-251) sets up borrow for next iteration
                digit_l += 10;
                frac_borrow = 1;
            }
            
            frac_result.push_back((digit_l - digit_s) + '0'); //appends value to frac_result based on subtraction operation
        } //ends loop
        
        reverse(frac_result.begin(), frac_result.end()); //reverses frac_result
        
        string int_result = subtract_positive_integers(larger_int, smaller_int); //(lines 258-261) subtracts int parts considering frac_borrow if necessary
        if (frac_borrow > 0) {
            int_result = subtract_positive_integers(int_result, "1");
        }
        
        string result = (result_negative ? "-" : "") + remove_leading_zeros(int_result); //creates result with first index set to - if negative
        if (frac_result != string(max_frac_len, '0')) { //runs if there are trailing zeros in frac part
            size_t last_non_zero = frac_result.find_last_not_of('0'); //(lines 265-270) removes trailing zeros from frac_part if needed
            if (last_non_zero != string::npos) {
                frac_result = frac_result.substr(0, last_non_zero + 1);
                result += "." + frac_result;
            }
        }
        
        return result; //returns final result from subtraction
    }
}

void process_file(const string& filename) { //func that processes doubles in string format from file
    ifstream file(filename); //opens file
    if (!file.is_open()) { //(lines 278-281) checks if file was opened, prints error message if not
        cerr << "Error: Could not open file '" << filename << "'" << endl;
        return;
    }

    string line; //creates new string line
    while (getline(file, line)) { //runs for length of file, sets line equal to current line of file
        line.erase(remove(line.begin(), line.end(), ' '), line.end()); // removes spaces from line
        line.erase(remove(line.begin(), line.end(), '\t'), line.end()); // removes tabs from line
        line.erase(remove(line.begin(), line.end(), '\r'), line.end()); // removes carriage returns from line
        line.erase(remove(line.begin(), line.end(), '\n'), line.end()); // removes newlines from line

        if (line.empty()) continue; //runs if line is empty, continues to next line

        if (is_valid_double(line)) { //runs if line contains a valid double
            string normalized = remove_leading_zeros(line); //creates new string normalized which represents current line without leading zeros
            string result = add_string_numbers(normalized, "-123.456"); //string result which represents the current line + -123.456
            cout << "Valid number: " << normalized << ", result after addition: " << result << endl; //prints current line and sum with -123.456
        } else { //runs if not valid double
            cout << "Invalid number: " << line << endl; //prints error message
        } //ends if statement
    } //ends while loop

    file.close(); //closes file
} //ends func

int main(int argc, char* argv[]) { //func main that runs when program is executed
    if (argc != 2) { //checks if argument count is exactly 2
        cerr << "Usage: " << argv[0] << " <filename>" << endl; //if incorrect number of args, error
        return 1; //default return value
    } //ends if statement

    string filename = argv[1]; //sets string filename to arg 2
    process_file(filename); //calls process_file to run operations

    return 0; //default return value
} //ends main