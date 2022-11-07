#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#define FAILURE_CODE 1
#define NIBBLE 4


enum { CHAR, UCHAR, INT, UINT, LONG, DOUBLE };
typedef int type;

//Defines properties for double precision.
//Expression range , Double precision,
//Expoenent, Significand precision, Biased integer
enum {EXPRESS = 8, DP = 15};
enum {DBEXP = 11, DBSP = 52, DBBIASED = 1023};

//Replaces assert() in <assert.h>
void assert(bool b) {
  if (b) return;
  fprintf(stderr,"Input error.\n");
  exit(FAILURE_CODE);
}

//Replaces pow() in <math.h for double
double power(int x, int y) {
  double result = 1;
  if (y >= 0 ) {
    for (int i = 0; i < y; i++) result *= x;
  }
  else {
    y = -y;
    for (int i = 0; i < y; i++)
    result /= x;
  }
  return result;
}

//Replaces pow() in <math.h>
long lPow(int x, int y) {
  long result = 1;
  for(int i = 0; i < y; i++) result *=x;
  return result;
}

//Replaces round() in <math.h>
double round(double n) {
  double result = n;
  result -= n - (int)n;
  return result;
}

//Replaces remainder operator(%) for type double
double remain(double n, int m) {
  double result = (int)n % m;
  return result;
}

//Checks the type
char *identifyType(int n, char *args[n]) {
  if (strcmp(args[1] , "unsigned") == 0) {
    int size = strlen(args[1]) + strlen(args[2]) + 2;
    char *name = (char*)malloc(sizeof(char)*size);
    strcpy(name, args[1]);
    strcat(name, " ");
    strcat(name, args[2]);
    for (int i = 2; i < n - 1 ; i++) {
      args[i] = args[i + 1];
    }
    args[n-1] = NULL;
    return name;
  }
  else {
    int size = strlen(args[1]) + 1;
    char *name = (char*)malloc(sizeof(char)*size);
    strcpy(name, args[1]);
    return name;
  }
}

//Checks whether the input is in Nibble or not.
//Since the number always starts from args[2],
//Only the comparsion with n and args[3] is required.
bool isBinary(int n, char *args[n]) {
  if (n >= 4) {
    if (args[3] != NULL) return true;
    else return false;
  }
  return false;
}

//Checks if the type is identified
type checkType(char* s[]) {
  type type = 0;
  if (strcmp(*s, "char") == 0) type = CHAR;
  else if (strcmp(*s, "unsigned char") == 0) type = UCHAR;
  else if (strcmp(*s, "int") == 0) type = INT;
  else if (strcmp(*s, "unsigned int") == 0) type = UINT;
  else if (strcmp(*s, "long") == 0) type = LONG;
  else if (strcmp(*s, "double") == 0) type = DOUBLE;
  else assert(false);
  return type;
}

double callMin(type t) {
  double min;
  switch(t) {
    case CHAR: min = (-1)*power(2,7); break;
    case UCHAR: min = 0; break;
    case INT: min = (-1)*power(2,31); break;
    case UINT: min = 0; break;
    case LONG: min = (-1)*power(2,31); break;
    case DOUBLE: min = (-1)*power(2,63); break;
  }
  return min;
}

double callMax(type t) {
  double max;
  switch(t) {
    case CHAR: max = (power(2,7))-1; break;
    case UCHAR: max = (power(2,8))-1; break;
    case INT: max = (power(2,31))-1; break;
    case UINT: max = (power(2,32))-1; break;
    case LONG: max = (power(2,63))-1; break;
    case DOUBLE: max = (power(2,63))-1; break;
  }
  return max;
}

int callBit(type t) {
  int bit;
  switch(t) {
    case CHAR: bit = 8; break;
    case UCHAR: bit = 8; break;
    case INT: bit = 32; break;
    case UINT: bit = 32; break;
    case LONG: bit = 32; break;
    case DOUBLE: bit = 64; break;
    case DBEXP: bit = DBEXP; break;
    case DBSP: bit = DBSP; break;
    default: bit = t;
  }
  return bit;
}

int convertS(int n, char num[]) {
  int result = 0;
  for (int i = 0; i < n; i++) {
    result += (num[i]) * power(10, n-i-1);
  }
  return result;
}

double convertDecimal(int num[]) {
  int n = sizeof(*num);
  double result = 0;
  for(int i = 0; i < n; i++) {
    result += (num[i] * power(10, -(i + 1)));
  }
  return result;
}

//Converts input into double.
double convert(type t, char num[]) {
  if (t == DOUBLE) {
    double n = atof(num);
    return n;
  }
  else {
    double n = strlen(num);
    double result = 0;
    if (num[0] == '-' ) {
      for (int i = 1; i < n; i++) {
        result += ((num[i]-'0') * power(10, n-i-1));
      }
      result *= -1;
    }
    else {
      for (int i = 0; i < n; i++) {
        result += ((num[i]-'0') * power(10, n-i-1));
      }
    }
    return result;
  }
}

double decToBin(double n, int i, char arr[]) {
  if (n == 0) arr[i] = 0;
  else if (n < 2) {
    arr[i] = n;
    n = 0;
  }
  else {
    arr[i] = remain(n ,2);
    n = round(n / 2);
  }
  return n;
}

//Overall calculation of transformign decimal to binary.
char *calculate(type t, double result) {
  int bit = callBit(t);
  char *res = (char*)malloc(sizeof(char)*bit);
  double n = result;
  if (n < 0) {
    n += power(2,(bit-1));
    res[0] = 1;
    for (int i = (bit-1); i > 0 ; i--) {
      n = decToBin(n, i, res);
    }
  }
  else {
    for (int i = (bit-1); i >= 0 ; i--) {
      n = decToBin(n, i, res);
    }
  }
  return res;
}

//Overall callculation for double to binary.
char *calForDouble(type t, int result) {
  int bit = t;
  char *res = (char*)malloc(sizeof(char)*(bit+1));
  int n = result;
  for (int i = (bit-1); i >= 0 ; i--) {
    if (n == 0) res[i] = 0;
    else if (n < 2) {
      res[i] = 1;
      n = 0;
    }
    else {
      res[i] = n % 2;
      n /= 2;
    }
  }
  res[bit] = '\0';
  return res;
}

//Distinguishes double to whole and floating points.
double parseDecimal(double result) {
  double decimal = 0;
  if (result < 0) decimal = (int)result - result;
  else decimal = result - (int)result;
  return decimal;
}

//Calculates binary for floating point.
double pointToBin(double n) {
  int *p = (int*)malloc(sizeof(int)*DBEXP);
  double current = n;
  for(int i = 0; i < DBEXP; i++) {
    current *= 2;
    if (current >= 1) {
    p[i] = 1;
    current -= 1;
    }
    else p[i] = 0;
  }
  double result = convertDecimal(p);
  free(p);
  return result;
}


char *resultForDb(int sign, int *exp, double *res) {
  int bit = callBit(DOUBLE);
  char *num = (char*)malloc(sizeof(char)*bit);
  int n = *exp;
  double e = n;
  double r = *res;
  char man[DP];
  snprintf(man, 15, "%.15f" , r);
  if (sign == 1) num[0] = 1;
  else num[0] = 0;
  char *ex = calculate(DBEXP, e);
  for(int i = 0; i < DBEXP; i++) {
    num[i+1] = ex[i];
  }
  for (int i = 0; i < DP - 2 ; i++) {
    num[i+DBEXP+1] = man[i+2]-'0';
  }
  for (int i = DBEXP + DP -2; i < 64 ; i++) num[i] = 0;
  free(ex);
  return num;
}

//Express exponent, mantissa
char *dbPrecision(int sign, double n) {
  double res = n;
  int exp = DBBIASED;
  while((res >= 2) || (res < 1)) {
    if (res >= 2) {
      res /= 10;
      exp++;
    }
    else if (res < 1) {
      res *= 10;
      exp--;
    }
  }
  res -= 1;
  int *ex = &exp;
  double *re = &res;
  char *arr = resultForDb(sign, ex, re);
  return arr;
}

//Calculates overall decimal to binary for type double.
char *calDouble(type t, double result) {
  int sign = 0;
  if (result < 0) {
    result = -result;
    sign = 1;
  }
  double decimal = parseDecimal(result);
  int whole = result - decimal;
  char *wh = calForDouble(EXPRESS, whole);
  int who = convertS(EXPRESS, wh);
  decimal = pointToBin(decimal);
  double r = who + decimal;
  char *res = dbPrecision(sign, r);
  free(wh);
  return res;
}

//Calculation for binary to decimal
long binToDec(int start, int bit, char num[]) {
  long result = 0;
  for(int i = start; i < bit ; i++) {
    result += (num[i]-'0') * lPow(2, bit - i -1);
  }
  return result;
}

long changeToDec(type t, char num[]) {
  long r = 0;
  int bit = callBit(t);
  if ((t == UINT) || (t == UCHAR)) {
    r = binToDec(0, bit, num);
  }
  //Didn't make the conversion of double.
  else if (t == DOUBLE) assert(false);
  else {
    if ((num[0]-'0') == 1) {
      r = binToDec(1, bit, num);
      r -= lPow(2, bit-1);
    }
    else r = binToDec(0, bit, num);
  }
  return r;
}

bool checkBinary (char num[]) {
  int size = strlen(num);
  for (int i = 0; i < size; i++) {
    if((num[i] != '0') && (num[i] != '1')) return false;
  }
  return true;
}

//Checks whether nibbles are in four.
bool checkStruct(int fin, int n, char *args[n]) {
  for (int i = 2; i < fin; i++) {
    int n = strlen(args[i]);
    if (n != 4) return false;
  }
  return true;
}

//Checks the bit size according to type.
bool checkNibble(type t, int fin) {
  int size = fin - 2;
  int bit = callBit(t);
  int nib = bit / NIBBLE;
  if (nib != size) return false;
  else return true;
}

//Puts nibbled input into single number.
char *mergeNumbers(int fin, int n, char *args[n]) {
  int size = 0;
  for(int i = 2; i < fin; i++) {
    size += strlen(args[i]);
    size++;
  }
  char *num = (char*)malloc(sizeof(char)*size);
  strcpy(num, args[2]);
  for(int i = 3; i < fin; i++) {
    strcat(num, args[i]);
  }
  assert(checkBinary(num));
  return num;
}

long handleBinary(type t, int n, char *args[n]) {
  int fin = n;
  if ((t == UCHAR) || (t == UINT)) fin -= 1;
  assert(checkStruct(fin, n, args));
  assert(checkNibble(t, fin));
  char *num = mergeNumbers(fin, n, args);
  assert(checkBinary(num));
  long result = changeToDec(t, num);
  free(num);
  return result;
}

void resultForBin(long n) {
  printf("%ld\n", n);
}

void result(type t, double num) {
  char *target;
  if (t == DOUBLE) target = calDouble(DOUBLE, num);
  else target = calculate(t, num);
  int bit = callBit(t);
  int count = 0;
  for (int i = 0 ; i < bit ; i++) {
    if (count == NIBBLE) {
      printf(" ");
      count = 0;
    }
    printf("%d", target[i]);
    count++;
  }
  printf("\n");
  free(target);
}

//--------------------------------------------------------
//Checks whether the input is valid.
//--------------------------------------------------------

bool digitOnly(int start, int n, char num[]) {
  for (int i = start; i < n; i++) {
    if (('0' > num[i]) || (num[i] > '9')) return false;
  }
  return true;
}

int findDot(int n, char num[]) {
  int point = 0;
  for (int j = 0 ; j < n; j++) {
    if (num[j] == '.') point = j;
  }
  if ((point == 0) || (point == n-1)) assert(false);
  return point;
}

bool isInputLegit(type t, char num[]) {
  int n = strlen(num);
  if (t == DOUBLE) {
    if (num[0] == '0') {
      if(num[1] != '.') return false;
      if(!digitOnly(2, n, num)) return false;
    }
    else if(num[0] == '-') {
      if (num[1] == '0') {
        if(num[2] != '.') return false;
        if(!digitOnly(3, n, num)) return false;
      }
    }
    else if (('0' < num[0]) && (num[0] <= '9')) {
      int point = findDot(n ,num);
      if(!digitOnly(1, point ,num)) return false;
      if(!digitOnly(point+1, n, num)) return false;
    }
    else return false;
  }
  else {
    if (num[0] < '1' || num[0] > '9') {
      if (num[0] != '-') return false;
    }
    if(!digitOnly(1, n, num)) return false;
  }
  return true;
}

bool overflown(type t, double num) {
  double min = callMin(t);
  double max = callMax(t);
  if (( min > num) || (num > max)) return false;
  else return true;
}

void checkInput(type t, char num[]) {
  double n = convert(t, num);
  assert(isInputLegit(t, num));
  assert(overflown(t, n));
}


//--------------------------------------------------------
//Test functions
//--------------------------------------------------------

enum {IsInputLegit , Overflown};
typedef int func;

bool call(func f, type t, double number, char num[]) {
  bool result = false;
  switch(f) {
    case IsInputLegit: result = isInputLegit(t, num); break;
    case Overflown: result = overflown(t, number); break;
    default: assert(false);
  }
  return result;
}

bool check(func f, type t, char num[], bool b) {
  double n = convert(t, num);
  bool answer = call(f, t, n, num);
  bool ok = b;
  if (answer == ok) return true;
  else return false;
}

bool checkBin(func f, type t, char num[], bool b) {
  double n = convert(t, num);
  bool answer = call(f, t, n, num);
  bool ok = b;
  if (answer == ok) return true;
  else return false;
}

void testIsInputLegit() {
  assert(check(IsInputLegit, CHAR, "7", true));
  assert(check(IsInputLegit, CHAR, "-128", true));
  assert(check(IsInputLegit, CHAR, "255", true));
  assert(check(IsInputLegit, CHAR, "08", false));
  assert(check(IsInputLegit, UCHAR, "255", true));
  assert(check(IsInputLegit, INT, "255", true));
  assert(check(IsInputLegit, INT, "12.32", false));
  assert(check(IsInputLegit, DOUBLE, "0.16", true));
  assert(check(IsInputLegit, DOUBLE, "100.12", true));
  assert(check(IsInputLegit, DOUBLE, "12.3.2", false));
  assert(check(IsInputLegit, DOUBLE, ".123", false));
  assert(check(IsInputLegit, DOUBLE, "00.123", false));
}

void testOverflown() {
  assert(check(Overflown, CHAR, "7", true));
  assert(check(Overflown, CHAR, "-128", true));
  assert(check(Overflown, CHAR, "255", false));
  assert(check(Overflown, CHAR, "08", true));
  assert(check(Overflown, UCHAR, "255", true));
  assert(check(Overflown, INT, "2,147,483,648", false));
  assert(check(Overflown, UINT, "2,147,483,648", false));
  assert(check(Overflown, LONG, "2,147,483,648", true));
  assert(check(Overflown, DOUBLE, "2,147,483,648", true));
}

void test() {
  testIsInputLegit();
  testOverflown();
  printf("All tests pass.\n");
}

int main(int n, char *args[n]) {
  if (n==1) test();
  else if (n==2) assert(false);
  else {
    char* name = identifyType(n, args);
    type t = checkType(&name);
    if (isBinary(n, args)) {
      long decimal = handleBinary(t, n, args);
      resultForBin(decimal);
    }
    else {
      char* target = args[2];
      checkInput(t, target);
      double num = convert(t ,args[2]);
      result(t, num);
    }
    free(name);
  }
  return 0;
}
