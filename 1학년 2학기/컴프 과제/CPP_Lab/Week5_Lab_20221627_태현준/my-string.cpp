#include <cstdio>
#include "my-string.hpp"

// TODO: Fill in these functions.

// Destructor.
MyString::~MyString() {
	free(this->arr);
}

// Get the length of the string.
size_t MyString::GetLength() {
	char a[this->length+1];
	
	for(int i=0; i<this->length; i++) {
		a[i] = this->arr[i];
	}
	a[this->length] = '\0';
	
	// return strlen(this->arr); // ??????????????????????????????????????????????????????????????????????????????? 왜 안됨????????
	// 안되는 이유 : hpp에서 입력받을 때는, ABC\0 이렇게 입력받은채로 바로 strlen하니까 3으로 잘 나오는데,
	// malloc해서 저장받은 다음에 할 때는, malloc 길이가 널 문자 포함 안되어있으니까 오류남.
	return strlen(a);
}

// Make the string into an empty string.
void MyString::Clear() {

	// char a[1];
	// strcpy(this->arr, a);

	int k = GetLength();
        for(int i=0; i<k; i++) {
            this->arr[i] = '\0';
        }

	this->length = 0;

	return;
}

// Append 's' at the end of this string.
void MyString::Append(MyString *s) {

	int i = this->length;
	int j = s->length;

	char a[i+j+1];

	for(int k=0; k<i; k++)
		a[k] = this->arr[k];
	for(int k=0; k<j; k++)
		a[i+k] = s->arr[k];
	a[i+j] = '\0';
	
	realloc(this->arr, i+j);
	strncpy(this->arr, a, i+j);

	this->length += j;

	return;
}

// Insert 's' at position 'idx' of this string. idx=0 means the start.
void MyString::Insert(MyString *s, int idx) {

	int i = this->length;
	int j = s->length;   // ABCDEF

	if (idx > i+1)
		return;

	char a[i+j+1];

	for (int k=0; k<idx; k++)  // ABCDEF abc       1
		a[k] = this->arr[k];
	for (int k=0; k<j; k++)
		a[idx+k] = s->arr[k];
	for (int k=idx; k<i; k++)
		a[k+j] = this->arr[k];
	a[i+j] = '\0';

	realloc(this->arr, i+j);
	strcpy(this->arr, a);
	
	this->length += j;

	return;
}

// Starting from the position 'idx', remove 'n' characters.
void MyString::Remove(int idx, int n) {

	int i = this->length;

	if (n > i-idx)          // abcdefgh	ABCDEF
		return;

	char a[i-n+1];

	for (int j=0; j<idx; j++)
		a[j] = this->arr[j];
	for (int j=0; j<i-n-1; j++) 
		a[idx+j] = this->arr[idx+n+j];
	a[i-n] = '\0';

	realloc(this->arr, i-n);
	strcpy(this->arr, a);

	this->length -= n;

	return;
}