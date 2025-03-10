#include <iostream>
#include <thread>
#include <mutex>

class Single2 {
private:
    Single2() {}
    Single2(const Single2&) = delete;
    Single2& operator=(const Single2&) = delete;
    
public:
    static Single2 &GetInst(){
        static Single2 single;
        return single;
    }
};

class Single2Hungry{
private:    
    Single2Hungry(){}
    Single2Hungry(const Single2Hungry&) = delete;
    Single2Hungry& operator=(const Single2Hungry&) = delete;
public:
    static Single2Hungry &GetInst(){
        if(single == nullptr){
            single = new Single2Hungry();
        }
        return *single;
    }
private:
    static Single2Hungry *single;

};


Single2Hungry* Single2Hungry::single = nullptr;


class SinglePointer{
    private:
        SinglePointer(){}
        SinglePointer(const SinglePointer&) = delete;
        SinglePointer& operator=(const SinglePointer&) = delete;
    public: 
        static SinglePointer* GetInst(){
            if(single != nullptr){
                return single;
            }
            s_mutex.lock();
            if(single != nullptr){
                s_mutex.unlock();
                return single;
            }
            single = new SinglePointer();
            s_mutex.unlock();
            return single;
        }
    private:
        static SinglePointer *single;
        static std::mutex s_mutex;
};

SinglePointer* SinglePointer::single = nullptr;
std::mutex SinglePointer::s_mutex;

void thread_func_lazy(int i){
    std::cout << "thread func lazy" << i << std::endl;
    std::cout<<"single addr is"<<SinglePointer::GetInst()<<std::endl;
}
void test_singlelazy(){
    for(int i=0;i<3;i++){
        std::thread t(thread_func_lazy,i);
        t.join();
    }
}
int main() {
    using std::cout;
    using std::endl;
    
    cout << "Hello, World!" << endl;
    cout << "s1 addr is" << &Single2::GetInst() << endl;
    cout << "s2 addr is" << &Single2::GetInst() << endl;
    cout << "--------------------" << endl; 


    cout << "s1 addr is" << &Single2Hungry::GetInst() << endl;
    cout << "s2 addr is" << &Single2Hungry::GetInst() << endl;
    cout << "--------------------" << endl; 
    test_singlelazy();

    return 0;
}