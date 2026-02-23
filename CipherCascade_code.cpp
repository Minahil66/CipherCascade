//skeleton code
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
        ofstream outfile("EncryptedVer.enc", ios::binary);
        outfile.write(holder.data(), holder.size());
        outfile.close();
        cout<<"Encrypted file saved as EncryptedVer.enc"<<endl;
        }
    void saveDecrypted(){
        ofstream outfile("DecryptedVer", ios::binary);
        outfile.write(holder.data(), holder.size());
        outfile.close();
        cout<<"Decrypted file saved as DecryptedVer"<<endl;

    }
   // virtual void bruteforce()=0;
    Ciphers(string n):filename(n){ //enter name/path
        ifstream file(n, ios::binary);
        if(!file.is_open()){
            cout<<"Error: File failed to open"<<n<<endl;
            exit(1);
        }
            file.seekg(0, ios::end); //put the cursor in the end of the file and DO NOT MOV any positions
            size=file.tellg(); //snce file is already in binary, tell us the size
            file.seekg(0,ios::beg); //okay size done, now put the cursor back to the beginning
            holder.resize(size); //changing the size of our vector
            if(file.fail()){
            cout << "Error: Failed to read file " << n << endl;
            exit(1);
        }
            file.read(holder.data(), size); //data() is a raw pointer to the place/mem where vector is storing elements, so file.read(where to put data (expects a pointer), how many bytes to read(int));
            file.close(); //close the file
            cout<<"Successfully read "<<size<<" bytes into memory"<<endl;
    }
    string getFilename()const{ return filename;}
    int getsize()const{return size;}
    virtual ~Ciphers(){}
};

class SimpleBlockCipher:public Ciphers{
    private:
    vector <vector<unsigned char>> blocks; //vector of vectors
    int groups;
    int key;
    unsigned char invsBox[256];
    unsigned char sBox[256] = {
        0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76, //premade AES style substitution box available online, also called sbox, an important concept in crytography, 
        0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
        0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
        0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
        0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
        0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
        0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
        0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
        0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
        0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
        0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
        0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
        0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
        0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
        0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
        0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16
    };
    public:
    SimpleBlockCipher(string n, int k=1):Ciphers(n){
        key=k;
        geninv();
    }
    void setKey(int num){key=num;}
    void geninv(){
    for(int i=0;i<256;i++){
    invsBox[sBox[i]] = i;
    } }
    void blockformation(){
        int s=holder.size();
        int groups=(s+16-1)/16;
        for(int i=0;i<groups;i++){
            vector<unsigned char> block;
            int start=i*16;
            int end=min(16+start,(int)holder.size());
            for(int j=start;j<end;j++){
                block.push_back(holder[j]);
            }
            blocks.push_back(block); 
        }
    }
    void Xor_SBC(){
                for(int i=0;i<blocks.size();i++){
        for(int j=0;j<blocks[i].size();j++){
            blocks[i][j]^=key;
        }
        }
    }
    void encrypt() override{
        //block formation:
        blocks.clear();
        blockformation();
        //substitution
        for(int i=0;i<blocks.size();i++){
        for(int j=0;j<blocks[i].size();j++){
            blocks[i][j]=sBox[(unsigned char)blocks[i][j]];
            }}
        //shift the last to front 
        for(auto& block:blocks){
        if(block.size()==16){
            unsigned char last=block[15];
            block.pop_back();
            for(int i=15;i>0;i--){
                block[i]=block[i-1];
            }
            block[0]=last;
            }
        }
        //Xor with key
        Xor_SBC();
        //combine back in a file
        int p=0;
        for(auto& block:blocks){ //auto means figure out the data type yourself
        for(auto& i:block){
        holder[p++]=i; //overwriting holder
        }
        }
    }
        void decrypt() override{
        ifstream infile("EncryptedVer.enc", ios::binary);
        if(!infile.is_open()){
            cout<<"Error: cannot open encrypted file"<<endl;
        return;
    }
        infile.seekg(0, ios::end);
        size=infile.tellg();
        infile.seekg(0,ios::beg);
        holder.resize(size);
        infile.read(holder.data(), size);
        infile.close(); 
       //forming blocks again
        blocks.clear();
        blockformation();
        //xor back
        Xor_SBC();
        //shift back the last element that we moved to front
        for(auto& block:blocks){
        if(block.size()==16){
        unsigned char first=block[0];
        for(int i=0;i<15;i++){
        block[i]=block[i+1];
        }
        block[15]=first;
        }}
        //substitute back oms
        for(int i=0;i<blocks.size();i++){
        for(int j=0;j<blocks[i].size();j++){
        blocks[i][j]=invsBox[(unsigned char)blocks[i][j]];
        } }
        int po=0;
        for(auto& block:blocks){
        for(auto& i:block){
        holder[po++]=i;
        }} }
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
class Vigenere:public Ciphers{
string CipherText;
string OriginalText;
    public:
    Vigenere(string T, string K){
       text = T;
         key = K;
        for(int i=0;i<text.length();i++) //uppercase text
            text[i] = toupper(text[i]);
       
        for(int i=0;i<key.length();i++) //uppercase key
            key[i] = toupper(key[i]);
    }
    string encrypt(){
        CipherText = "";
    
        for(int i=0;i<text.length();i++){
        char t = text[i];
        char k = key[i % key.length()];    
        
    int tValue =  t - 'A';
    int kValue =  k - 'A';
    int mod = ((tValue + kValue)%26);
    int c = mod + 'A';
    CipherText += c;
     }
     return CipherText;
}

        string decrypt(){
        string OriginalText = "";
    
        for(int i=0;i<CipherText.length();i++){
        char c = CipherText[i];
        char k = key[i % key.length()];    
        
    int cValue =  c - 'A';
    int kValue =  k - 'A';
    int mod = ((cValue - kValue + 26)%26);
    int r = mod + 'A';
    OriginalText += r;
     }
     return OriginalText;
}    
};
class XORCipher : public Ciphers{
    string CipherText;
    string OriginalText;
     int key;
public:
 XORCipher(string T, int K)
{
       text = T;
       key = K;
    }
string encrypt() {
         CipherText = "";

        for (int i = 0; i < text.length(); i++) {
            char c = text[i]^key;
            CipherText +=c;
        }
        return CipherText;
    } 
    string decrypt() {
 OriginalText = ""; 
        for (int i = 0; i < CipherText.length(); i++) {
            char c = CipherText[i] ^ key;
            OriginalText += c;
        }
        return OriginalText;
}
    };
int main(){
        int choice, c;
    cout << "Choose any 3 Ciphers :\n";
    cout << "1. Simple Block Cipher ";
    cout << "2. Byte Reversal";
    cout << "3. Vigenere Cipher \n";
    cout << "4. XOR Cipher \n";
    cout << "Enter choice: ";
    cin >> choice;
      switch(choice) {
        case 3: {
            string text, key;
            cout << "Enter text:\n ";
            getline(cin, text);
            cout << "\nEnter key: ";
            getline(cin, key);

            Vigenere v(text, key);

            cout << "\nChoose : \n1. Encrypt\n2. Decrypt\n ";
            cin >> c;

            if(c == 1) {
                cout << "Encrypted version : " << v.encrypt() << endl;
            } else if(c == 2) {
                cout << "Decrypted version : " << v.decrypt()<< endl;
            } else
                cout << "Invalid input" << endl;
            break;
        }

        case 4: {
            string text;
            int key;
            cout << "Enter text: ";
            getline(cin, text);
            cout << "\nEnter key : ";
            cin >> key;
            XORCipher xor(text, key);
            cout << "\nChoose :\n1. Encrypt\n2. Decrypt\n ";
            cin >>c;
            if(c == 1) {
                cout << "Encrypted version : " << xor.encrypt() << endl;
            } else if(c == 2) {
                cout << "Decrypted version : " << xor.decrypt() << endl;
            } else
                cout << "Invalid input" << endl;
            break;
        } }
    return 0;
}
