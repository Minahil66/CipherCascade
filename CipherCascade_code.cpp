//code
#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<cstdlib>
#include<algorithm>
using namespace std;
class Ciphers{
    private:
    string filename;
    protected:
    vector<char> holder;
    int size;
    public:
    virtual void encrypt()=0;
    virtual void decrypt()=0;
    void saveEncrypted(){
        string name;
        cout<<"Save file as:(Enter name) "<<endl;
        cin>>name;
         ofstream outfile(name, ios::binary);
        outfile.write(holder.data(), holder.size());
        outfile.close();
        cout<<"File saved as: "<<name<<endl<<endl;
        }
    void saveDecrypted(){
        string name;
        cout<<"Save file as:(Enter name) "<<endl;
        cin>>name;
        ofstream outfile("DecryptedVer", ios::binary);
        outfile.write(holder.data(), holder.size());
        outfile.close();
        cout<<"File saved as: "<<name<<endl<<endl;
    }
    Ciphers(string n):filename(n){ //enter name/path
        ifstream file(n, ios::binary);
        if(!file.is_open()){
        cout<<"Error: File failed to open"<<n<<endl;
        return;
        }
        file.seekg(0, ios::end); //put the cursor in the end of the file and DO NOT MOV any positions
        size=file.tellg(); //snce file is already in binary, tell us the size
        file.seekg(0,ios::beg); //okay size done, now put the cursor back to the beginning
        holder.resize(size); //changing the size of our vector
        if(file.fail()){
        cout << "Error: Failed to read file " << n << endl;
        return;
        }
        file.read(holder.data(), size); //data() is a raw pointer to the place/mem where vector is storing elements, so file.read(where to put data (expects a pointer), how many bytes to read(int));
        file.close(); //close the file
        cout<<"Successfully read "<<size<<" bytes into memory"<<endl;
    }
    string getFilename()const{ return filename;}
    int getsize()const{return size;}
    virtual ~Ciphers(){}
};
class AtbashCipher:public Ciphers{
    public:
    AtbashCipher(string n):Ciphers(n){}
    void encrypt()override{
    for(int i=0;i<holder.size();i++){
    holder[i]=255-(unsigned char)holder[i];
    }
    }
    void decrypt()override{
    for(int i=0;i<holder.size();i++){
    holder[i]=255-(unsigned char)holder[i];
    }
    }
};
class ByteReversalCipher:public Ciphers{
public:
    ByteReversalCipher(string n) : Ciphers(n) {}
    
    void encrypt() override {
    reverse(holder.begin(), holder.end());
    }
    void decrypt() override{    
    reverse(holder.begin(), holder.end());  // Reverse back
    }
};
class VigenereCipher : public Ciphers {
    string key;
public:
    VigenereCipher(string n, string k):Ciphers(n) {
    key = k; }
    void encrypt() override {
        int keyIndex = 0;
        
    for (int i = 0; i < holder.size(); i++) {
    char c = holder[i];
    if (isalpha(c)) {
    char k = key[keyIndex % key.length()];
    
    int kValue = toupper(k) - 'A';
    if (isupper(c)) {
    int tValue = c - 'A';
    holder[i] = ((tValue + kValue) % 26) + 'A'; //tottal alphabets = 26
    }
    else
    { //lowercase
    int tValue = c - 'a';
    holder[i] = ((tValue + kValue) % 26) + 'a';
    } 
    keyIndex++;  //increment key index
    }
}
}
    void decrypt() override 
    {
        int keyIndex = 0;
    for (int i = 0; i < holder.size(); i++) {
    char c = holder[i];
    
    if (isalpha(c)) {

    char k = key[keyIndex % key.length()]; //to repeat key until length matches as of text
 
    int kValue = toupper(k) - 'A';
    if (isupper(c)) {
        int cValue = c - 'A';
        holder[i] = ((cValue - kValue + 26) % 26) + 'A';
     }
     else { // lowercase
        int cValue = c - 'a';
        holder[i] = ((cValue - kValue + 26) % 26) + 'a';
                }
        keyIndex++;        
    } } } };
    
class XORCipher : public Ciphers {
private:
    int key;
public:
    XORCipher(string n, int k) : Ciphers(n) {
    key = k;
    }
    void encrypt() override {
    for (int i = 0; i < holder.size(); i++) {
    holder[i] = holder[i] ^ key;
    }
    }
    void decrypt() override {
    for (int i = 0; i < holder.size(); i++) {
    holder[i] = holder[i] ^ key;   // XOR reverses itself
    } } };
    class Stegonography{ //for the time being works only for .bmp files
        string filename;
        vector<char>imgdata;
        public:
        Stegonography(string n): filename(n){
            ifstream infile(filename, ios::binary);
            if(!infile.is_open()){
            cout<<"File failed to open"<<endl;
            return;
            }
            infile.seekg(0, ios::end);
            imgdata.resize(infile.tellg());
            infile.seekg(0, ios::beg);
            if(infile.fail()){
            cout<<"Failed to read data into memory"<<endl;
            return;
            }
            infile.read(imgdata.data(), infile.tellg());
            infile.close();
            cout<<"Successfully read file data into memory"<<endl;
        }
        void hideMessage(){
            string msg;
            cout<<"Enter the message to hide: ";
            cin.ignore();
            getline(cin,msg);

            vector<char> message(msg.begin(), msg.end()); 

            if(message.size()>imgdata.size()-0x36){
                cout<<"Message too long, exceeds image size"<<endl;
                return;
            }
            for(int i=0;i<message.size(); i++){
                imgdata[0x36+i]=message[i];
            }
            ofstream outfile("stego_" + filename, ios::binary);
            outfile.write(imgdata.data(), imgdata.size());
            outfile.close();
            cout<<"Hidden in stego_"<<filename<<endl;
        }
        };         
int main(){

int n=1;
    int choice;
    string file, key;
    int s;
    cout<<"Enter the path/file: "<<endl;
    cin>>file;
    cout<<"SELECT:"<<endl;
    cout<<"1. Encrypt"<<endl<<"2. Decrypt"<<endl<<"3.Stegonograph a BMP image"<<"4. Meta Data Manager"<<endl;
    cin>>s;
    if (s==1 || s==2){
    while(n<=3){
    cout<<"Select "<<n<<" cipher:"<<endl;
    cout<<"1.Vigenere Cipher"<<endl<<"2.Xor Cipher"<<endl<<"3.Byte Reversal Cipher"<<endl<<"4.Atbash Cipher"<<endl;
    cin>>choice;

    switch(choice){
    case 1:{
    cout<<"Enter string key: "<<endl;
    cin>>key;
    {
    VigenereCipher v1(file, key);
    if(s==1){
    cout<<"Encrypting file through Vignere Cipher..."<<endl;
    v1.encrypt();
    v1.saveEncrypted();
    file="EncryptedVer";
    }
    else{
    cout<<"Decrypting file through Vignere Cipher..."<<endl;
    v1.decrypt();
    v1.saveDecrypted();
    file="DecryptedVer";
    }
    }
    break;
    }
    case 2:{
    int k;
    cout<<"Enter integer key: "<<endl;
    cin>>k;
    {
    XORCipher x1(file,k);
    if(s==1){
    cout<<"Encrypting file through XOR Cipher..."<<endl;
    x1.encrypt();
    x1.saveEncrypted();
    file="EncryptedVer";
    }
    else{
    cout<<"Decrypting file through XOR Cipher..."<<endl;
    x1.decrypt();
    x1.saveDecrypted();
    file="DecryptedVer";
    }
    }
    break;
    }
    case 3:{
    {
    ByteReversalCipher b1(file);
    if(s==1){
    cout<<"Encrypting file through Byte Reversal Cipher..."<<endl;
    b1.encrypt();
    b1.saveEncrypted();
    file="EncryptedVer";
    }
    else{
    cout<<"Decrypting file through Byte Reversal Cipher..."<<endl; 
    b1.decrypt();
    b1.saveDecrypted();
    file="DecryptedVer";
      }
      }
    break;
     }
    case 4: {
    {
    AtbashCipher a1(file);
    if(s==1){
    cout<<"Encrypting file through Atbash Cipher..."<<endl;
    a1.encrypt();
    a1.saveEncrypted();
    file="EncryptedVer";
    }
    else{
    cout<<"Decrypting file through Atbash Cipher..."<<endl;
    a1.decrypt();
    a1.saveDecrypted();
    file="DecryptedVer";
    } }
    break;
   }
    default: {
    cout<<"Error: incorrect selection;"<<endl;
    break;
    } } n++; } } 
    else if(s==3){
        
    }
}
