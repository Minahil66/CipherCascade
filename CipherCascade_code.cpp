#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<cstdlib>
#include<algorithm>
using namespace std;

// =============================================================
// FACADE PATTERN: This base class hides ALL complex file I/O
// Child classes only need to write encrypt() and decrypt()
// =============================================================

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
            cout<<"Encrypted file saved as EncryptedVer.enc"<<endl<<endl;
        }
        
        void saveDecrypted(){
            ofstream outfile("DecryptedVer", ios::binary);
            outfile.write(holder.data(), holder.size());
            outfile.close();
            cout<<"Decrypted file saved as DecryptedVer"<<endl<<endl;
        }
        
        Ciphers(string n):filename(n){
            ifstream file(n, ios::binary);
            if(!file.is_open()){
                cout<<"Error: File failed to open"<<n<<endl;
                exit(1);
            }
            file.seekg(0, ios::end);
            size=file.tellg();
            file.seekg(0,ios::beg);
            holder.resize(size);
            if(file.fail()){
                cout << "Error: Failed to read file " << n << endl;
                exit(1);
            }
            file.read(holder.data(), size);
            file.close();
            cout<<"Successfully read "<<size<<" bytes into memory"<<endl;
        }
        
        string getFilename()const{ return filename;}
        int getsize()const{ return size;}
        virtual ~Ciphers(){}
};

// =============================================================
// STRATEGY PATTERN: Concrete Strategy 1 of 4
// All ciphers share the same encrypt/decrypt interface
// =============================================================

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

// STRATEGY PATTERN: Concrete Strategy 2 of 4
class ByteReversalCipher:public Ciphers{
public:
    ByteReversalCipher(string n) : Ciphers(n) {}
    void encrypt() override {
        reverse(holder.begin(), holder.end());
    }
    void decrypt() override{    
        reverse(holder.begin(), holder.end());
    }
};

// STRATEGY PATTERN: Concrete Strategy 3 of 4
class VigenereCipher : public Ciphers {
    string key;
public:
    VigenereCipher(string n, string k):Ciphers(n) {
        key = k;
    }
    void encrypt() override {
        int keyIndex = 0;
        for (int i = 0; i < holder.size(); i++) {
            char c = holder[i];
            if (isalpha(c)) {
                char k = key[keyIndex % key.length()];
                int kValue = toupper(k) - 'A';
                if (isupper(c)) {
                    int tValue = c - 'A';
                    holder[i] = ((tValue + kValue) % 26) + 'A';
                }
                else {
                    int tValue = c - 'a';
                    holder[i] = ((tValue + kValue) % 26) + 'a';
                } 
                keyIndex++;
            }
        }
    }
    void decrypt() override {
        int keyIndex = 0;
        for (int i = 0; i < holder.size(); i++) {
            char c = holder[i];
            if (isalpha(c)) {
                char k = key[keyIndex % key.length()];
                int kValue = toupper(k) - 'A';
                if (isupper(c)) {
                    int cValue = c - 'A';
                    holder[i] = ((cValue - kValue + 26) % 26) + 'A';
                }
                else {
                    int cValue = c - 'a';
                    holder[i] = ((cValue - kValue + 26) % 26) + 'a';
                }
                keyIndex++;        
            }
        }
    }
};

// STRATEGY PATTERN: Concrete Strategy 4 of 4
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
            holder[i] = holder[i] ^ key;
        }
    }
};

// =============================================================
// MAIN: CHAIN OF RESPONSIBILITY + STRATEGY + FACADE all work here
// CHAIN: while loop passes "file" between 3 ciphers
// STRATEGY: User picks cipher 1,2,3,4 at runtime
// FACADE: encrypt() and saveEncrypted() hide complexity
// =============================================================

int main(){
    cout << "\n";
    cout << "==================================\n";
    cout << "     C I P H E R   C A S C A D E    \n";
    cout << "==================================\n";
    cout << "   Vigenere    XOR    Byte Rev    \n";
    cout << "              Atbash              \n";
    cout << "           3-Step Chain              \n";
    cout << "==================================\n\n";
    
    int n=1;
    int choice;
    string file, key;
    int s;
    
    cout<<"Enter the path/file you want to encrypt/decrypt: "<<endl;
    cin>>file;
    cout<<"Do you want to encrypt the file or decrypt it?"<<endl;
    cout<<"1. Encrypt"<<endl<<"2. Decrypt"<<endl;
    cin>>s;
    
    // =============================================================
    // CHAIN OF RESPONSIBILITY: Loop runs 3 times
    // Each cipher saves to "EncryptedVer.enc" and passes filename
    // Next cipher loads that file and processes it
    // =============================================================
    
    while(n <= 3){
        
        cout<<"Select "<<n<<" cipher:"<<endl;
        cout<<"1.Vigenere Cipher"<<endl<<"2.Xor Cipher"<<endl<<"3.Byte Reversal Cipher"<<endl<<"4.Atbash Cipher"<<endl;
        cin>>choice;
        
        // =============================================================
        // STRATEGY PATTERN: Switch chooses which algorithm to run
        // FACADE PATTERN: Each cipher just calls encrypt() and save
        // =============================================================
        
        switch(choice){
            case 1:{
                cout<<"Enter string key: "<<endl;
                cin>>key;
                {
                    VigenereCipher v1(file, key);
                    if(s==1){
                        cout<<"Encrypting file through Vigenere Cipher..."<<endl;
                        v1.encrypt();
                        v1.saveEncrypted();
                        file="EncryptedVer.enc";  // CHAIN: pass to next
                    }
                    else{
                        cout<<"Decrypting file through Vigenere Cipher..."<<endl;
                        v1.decrypt();
                        v1.saveDecrypted();
                        file="DecryptedVer";      // CHAIN: pass to next
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
                        file="EncryptedVer.enc";
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
                        file="EncryptedVer.enc";
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
                        file="EncryptedVer.enc";
                    }
                    else{
                        cout<<"Decrypting file through Atbash Cipher..."<<endl;
                        a1.decrypt();
                        a1.saveDecrypted();
                        file="DecryptedVer";
                    }
                }
                break;
            }
            default: {
                cout<<"Error: incorrect selection;"<<endl;
                break;
            }
        }
        n++;  // CHAIN: move to next link
    }
}
