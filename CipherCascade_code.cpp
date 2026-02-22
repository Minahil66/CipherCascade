//skeleton code
#include<iostream>
#include<vector>
#include<fstream>
#include<string>
using namespace std;
class Ciphers{
    private:
    string filename;
    vector<char> holder;
    public:
    int size;
    virtual void encrypt()=0;
    virtual void decrypt()=0;
    virtual void bruteforce()=0;
    Ciphers(string name):filename(name){ //enter name/path
        ifstream file(name, ios::binary);
        if(file.fail()){
            cout<<"Error: file operation failed"<<name<<endl;
            file.seekg(0, ios::end); //put the cursor in the end of the file and DO NOT MOV
            size=file.tellg(); //snce file is already in binary, tell us the size
            file.seekg(0,ios::beg); //okay size done, now put the cursor back to the beginning
            holder.resize(size); //changing the size of our vector
            file.read(holder.data(), size); //data() is a raw pointer to the place/mem where vector is storing elements, so file.read(where to put data (expects a pointer), how many bytes to read(int));
            file.close(); //close the file
            cout<<"Successfully read "<<size<<" bytes into memory"<<endl;
        }
    }
};